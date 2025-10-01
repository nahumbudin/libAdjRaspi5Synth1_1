/**
*	@file		adjSynthPolyphonyManagerThreads.h
*	@author		Nahum Budin
*	@date		6-Feb-2025
*	@version	1.0 1st version	
*	
*	Based on adjSynthPolyphony.cpp version 1.1 3-Feb-2021
*
*	@brief		Handle AdjSynth polyphony using threads.
*	
*/

#include <stddef.h>
#include <thread>

#include "adjSynthPolyphonyManager.h"

AdjPolyphonyManager *AdjPolyphonyManager::poly_manager_instance = NULL;

AdjPolyphonyManager::AdjPolyphonyManager(int num_of_voices)
{
	max_number_of_voices = num_of_voices;

	if (max_number_of_voices > _SYNTH_MAX_NUM_OF_VOICES)
	{
		number_of_cores = _SYNTH_MAX_NUM_OF_VOICES;
	}

	number_of_cores = std::thread::hardware_concurrency();

	if (number_of_cores > _SYNTH_MAX_NUM_OF_CORES)
	{
		number_of_cores = _SYNTH_MAX_NUM_OF_CORES;
	}

	if (number_of_cores > 1)
	{
		max_num_of_voices_per_core = max_number_of_voices / (number_of_cores - 1);
	}
	else
	{
		max_num_of_voices_per_core = max_number_of_voices;
	}

	for (int i = 0; i < _SYNTH_MAX_NUM_OF_CORES; i++)
	{
		cores_load[i] = 0;
	}

	gettimeofday(&start_time, NULL);
}

AdjPolyphonyManager *AdjPolyphonyManager::get_poly_manger_instance(int num_of_voices)
{
	if (poly_manager_instance == NULL)
	{
		poly_manager_instance = new AdjPolyphonyManager(num_of_voices);
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
			/* This should never happen! */
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

	return -1;
}

/**
*   @brief  Returns the voice number of the voice which is active for the longest time
*   @param  none
*   @return the voice number of the voice which is active for the longest time
*/
int AdjPolyphonyManager::get_oldest_voice()
{

	return -1;
}

/**
*   @brief  Returns a voice num that is already assigned to this note and program.
*			If more than 1 found, look for the 1st to become used (oldest). 
*			Used for reactivating a program note that is already playing.
*   @param  note	requested note
*   @param	program	requested program
*   @return a voice num that is already assigned to this note and program;
*			-2 if CPU is too loaded; -3 if params are out of range
*/
int AdjPolyphonyManager::get_reused_note(int note, int program)
{

	return -1;
}

/**
*   @brief  Activate a voice resource..
*   @param  res	voice number
*   @param	note	specified note
*   @param	program	specified program
*   @return 0 if done.
*/
int AdjPolyphonyManager::activate_resource(int res_num, int note, int program)
{

	return -1;
}

/**
*   @brief  Free a voice.
*   @param  voice	voice number
*   @param	pending if set true, wait untill envelope is zero
*   @return tvoid
*/
void AdjPolyphonyManager::free_voice(int voice, bool pend)
{

}
