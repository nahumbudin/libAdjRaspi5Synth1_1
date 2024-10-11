/**
*	@file		adjSynthPolyphonyManager.cpp
*	@author		Nahum Budin
*	@date		9-Oct-2024
*	@version	1.0 1st version	
*	
*	Based on adjSynthPolyphony.cpp version 1.1 3-Feb-2021
*
*	@brief		Handle AdjSynth polyphony
*/

#include <stddef.h>
#include <thread>

#include "adjSynthPolyphonyManager.h"
#include "adjSynth.h"
#include "../LibAPI/synthesizer.h"

AdjPolyphonyManager *AdjPolyphonyManager::poly_manager_instance = NULL;

AdjPolyphonyManager::AdjPolyphonyManager(int num_of_voic)
{
	max_number_of_voices = num_of_voic;
	
	if (max_number_of_voices > _SYNTH_MAX_NUM_OF_VOICES)
	{
		number_of_cores = _SYNTH_MAX_NUM_OF_VOICES;
	}
	
	number_of_cores = std::thread::hardware_concurrency();
	
	if (number_of_cores > _SYNTH_MAX_NUM_OF_CORES)
	{
		number_of_cores = _SYNTH_MAX_NUM_OF_CORES;
	}
	
	max_num_of_voices_per_core = max_number_of_voices / number_of_cores;
	
	for (int i = 0; i < _SYNTH_MAX_NUM_OF_CORES; i++)
	{
		cores_load[i] = 0;
	}
	
}

AdjPolyphonyManager *AdjPolyphonyManager::get_poly_manger_instance(int num_of_voic)
{
	if (poly_manager_instance == NULL)
	{
		poly_manager_instance = new AdjPolyphonyManager(num_of_voic);
	}
	
	return poly_manager_instance;
}

int AdjPolyphonyManager::get_number_of_cores()
{
	return number_of_cores;
}

int AdjPolyphonyManager::increase_core_processing_load_weight(int core, int wght)
{
	if ((core < number_of_cores) && (wght > 0))
	{
		cores_load[core] += wght;
	}
	else
	{
		return -1;
	}
	
	return cores_load[core];
}

int AdjPolyphonyManager::decrease_core_processing_load_weight(int core, int wght)
{
	if ((core < number_of_cores) && (wght > 0))
	{
		cores_load[core] -= wght;
		if (cores_load[core] < 0)
		{
			/* This should never happend! */
			cores_load[core] = 0;
		}
	}
	else
	{
		return -1;
	}
	
	return cores_load[core];
}

int AdjPolyphonyManager::get_core_processing_load_weight(int core)
{
	if (core < number_of_cores)
	{
		return cores_load[core];
	}
	else
	{
		return -1;
	}
}

int AdjPolyphonyManager::clear_core_processing_load_weight(int core)
{
	if (core < number_of_cores)
	{
		cores_load[core] = 0;
		return cores_load[core];
	}
	else
	{
		return -1;
	}
}

int AdjPolyphonyManager::get_less_busy_core()
{
	int i, min_load = 1000, min_core, result = 1000;
	
	// Look for the less busy core
	min_load = 1000;
	min_core = 1000;
	for (i = 0; i < number_of_cores; i++)
	{
		if (cores_load[i] < min_load)
		{
			min_load = cores_load[i];
			min_core = i;
		}
	}

	if (min_core >= number_of_cores) 
	{
		// No min core found - error
		return -1;
	}
	else 
	{
		return min_core;
	}
}

/**
*   @brief  Returns the number of the 1st found non busy voice on a selected core
*   @param  core selected core
*   @return the number of the 1st found non busy voice on a selected core 
*/
int AdjPolyphonyManager::get_a_free_voice(int core)
{
	int result, voice_num;
	int min_voice = -1, min_core = -1;
	bool reused = false;
	
	if (core < number_of_cores)
	{
		// Look for a free voice on selected core
		voice_num = core * max_num_of_voices_per_core;
		for (voice_num; voice_num < (core + 1) * max_num_of_voices_per_core; voice_num++)
		{
			if (AdjSynth::get_instance()->synth_voice[voice_num] != NULL)
			{
				if (!(AdjSynth::get_instance()->synth_voice[voice_num]->audio_voice->is_voice_active() ||
					  AdjSynth::get_instance()->synth_voice[voice_num]->audio_voice->is_voice_wait_for_not_active()))
				{
					// A free voice
					break;
				}
			}
			else
			{
				break;
			}
		}

		if (voice_num < max_number_of_voices)
		{
			return voice_num;
			//	printf("Min Core %i Free voice %i", core, voice);
		}
		else
		{
			return -1;
		}
	}
	else
	{
		{
			return -1;
		}
	}
}

/**
*   @brief  Returns the voice number of the voice which is active for the longest time
*   @param  none
*   @return the voice number of the voice which is active for the longest time
*/
int AdjPolyphonyManager::get_oldest_voice()
{			
	int i;
	int minvoice = -1;
	uint64_t mintime = UINT64_MAX;

	// Alocate first to be allocated in the past (oldest)
	for (i = 0; i < max_number_of_voices; i++)
	{
		if (AdjSynth::get_instance()->synth_voice[i] != NULL)
		{
			if (AdjSynth::get_instance()->synth_voice[i]->audio_voice->get_timestamp() < mintime)
			{
				mintime = AdjSynth::get_instance()->synth_voice[i]->audio_voice->get_timestamp();
				minvoice = i;
			}
		}
	}

	return minvoice;
}

/**
*   @brief  Returns a voice num that is already assigned to this note and program.
*			Used for reactivating a program note that is already playing.
*   @param  note	requested note
*   @param	program	requested program
*   @return a voice num that is already assigned to this note and program;
*			-2 if CPU is too loaded; -3 if params are out of range
*/
int AdjPolyphonyManager::get_reused_note(int note, int program)
{
	int i, result = -1;
	
	if (mod_synth_get_cpu_utilization() > 90)
		// CPU is too loaded
	{	
		return -2;
	}

	if ((note < 0) || (note > 127) || (program < 0) ||
		(program >= AdjSynth::get_instance()->get_num_of_programs()))
		// Illegal parameters range
	{	
		return -3;
	}
	
	// Look if any active voice already produces this note and allocated to this program
	for (i = 0; i < AdjSynth::get_instance()->get_num_of_voices(); i++)
	{
		if (AdjSynth::get_instance()->synth_voice[i] != NULL)
		{
			if ((AdjSynth::get_instance()->synth_voice[i]->audio_voice->is_voice_active() ||
				AdjSynth::get_instance()->synth_voice[i]->audio_voice->is_voice_wait_for_not_active()) &&
				AdjSynth::get_instance()->synth_voice[i]->audio_voice->get_note() == note &&
				AdjSynth::get_instance()->synth_voice[i]->get_allocated_program() == program)
			{
				result = i;
				break;
			}
		}
	}

	return result;
}

