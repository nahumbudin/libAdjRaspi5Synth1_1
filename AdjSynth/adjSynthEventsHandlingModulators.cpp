/**
*	@file		adjSynthEventsHandlingModulators.cpp
*	@author		Nahum Budin
*	@date		11-Oct-2024
*	@version	1.1
*					1. Code refactoring and notaion.
*	
*	@brief		AdjHeart Synthesizer Modulators Events Handling
*
*	History:\n
*	
*	version 1.0		15_Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"	

/**
*   @brief  Initiates a modulator (LFO/ADSR) related event with integer value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int beqid	target modulator:
\verbatim
	_LFO_1_EVENT, _LFO_2_EVENT, _LFO_3_EVENT, _LFO_4_EVENT, _LFO_5_EVENT, _LFO_6_EVENT
	_ENV_1_EVENT, _ENV_2_EVENT, _ENV_3_EVENT, _ENV_4_EVENT, _ENV_5_EVENT, _ENV_6_EVENT
\endverbatim
*	@param	int modid	specific event code:\n
*				_MOD_ADSR_ATTACK, _MOD_ADSR_DECAY, _MOD_ADSR_SUSTAIN, _MOD_ADSR_RELEASE\n
*				_MOD_LFO_WAVEFORM, _MOD_LFO_RATE, _MOD_LFO_SYMMETRY\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_MOD_ADSR_ATTACK: 0-100; 0 -> 0   100 -> getAdsrMaxAttackTimeSec()\n
*				_MOD_ADSR_DECAY: 0-100; 0 -> 0   100 -> getAdsrMaxDecayTimeSec()\n
*				_MOD_ADSR_SUSTAIN: 0-100; 0 -> 0   100 -> getAdsrMaxSustainTimeSec()\n
*				_MOD_ADSR_RELEASE: 0-100; 0 -> 0   100 -> getAdsrMaxReleaseTimeSec()\n
*				_MOD_LFO_WAVEFORM:\n
\verbatim
	_OSC_WAVEFORM_SINE, _OSC_WAVEFORM_SQUARE, _OSC_WAVEFORM_PULSE
	_OSC_WAVEFORM_TRIANGLE, _OSC_WAVEFORM_SAMPHOLD
\endverbatim
*				_MOD_LFO_RATE: 0-100; 0 -> getLFOminFrequency(); 100 -> getLFOmaxFrequency()\n
*				_MOD_LFO_SYMMETRY: 5-95
*	@param	int program	program number
*
*   @return 0
*/
int AdjSynth::modulator_event_int(int modid, int eventid, int val, _settings_params_t *params, int program)
{
	switch (eventid)
	{
	case _MOD_ADSR_ATTACK:
		if (modid == _ENV_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.env_1.attack",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);	
		}
		else if (modid == _ENV_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.env_2.attack",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _ENV_3_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.env_3.attack",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _ENV_4_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.env_4.attack",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _ENV_5_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.env_5.attack",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _ENV_6_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
			(params,
				"adjsynth.env_6.attack",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		break;

	case _MOD_ADSR_DECAY:
		if (modid == _ENV_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.env_1.decay",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _ENV_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.env_2.decay",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _ENV_3_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.env_3.decay",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _ENV_4_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.env_4.decay",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _ENV_5_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.env_5.decay",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _ENV_6_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
			(params,
				"adjsynth.env_6.decay",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		break;

	case _MOD_ADSR_SUSTAIN:
		if (modid == _ENV_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.env_1.sustain",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _ENV_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.env_2.sustain",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _ENV_3_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.env_3.sustain",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _ENV_4_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.env_4.sustain",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _ENV_5_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.env_5.sustain",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _ENV_6_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
			(params,
				"adjsynth.env_6.sustain",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		break;

	case _MOD_ADSR_RELEASE:
		if (modid == _ENV_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.env_1.release",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _ENV_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.env_2.release",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _ENV_3_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.env_3.release",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _ENV_4_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.env_4.release",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _ENV_5_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.env_5.release",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _ENV_6_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
			(params,
				"adjsynth.env_6.release",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		break;

	case _MOD_LFO_WAVEFORM:
		if (modid == _LFO_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.lfo_1.waveform",
				val,
				_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _LFO_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.lfo_2.waveform",
				val,
				_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _LFO_3_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.lfo_3.waveform",
				val,
				_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _LFO_4_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.lfo_4.waveform",
				val,
				_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _LFO_5_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.lfo_5.waveform",
				val,
				_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _LFO_6_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
			(params,
				"adjsynth.lfo_6.waveform",
				val,
				_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
				program);
		}
		break;

	case _MOD_LFO_RATE:
		if (modid == _LFO_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.lfo_1.rate",
				val,
				_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
				program);
					
		}
		else if (modid == _LFO_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.lfo_2.rate",
				val,
				_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _LFO_3_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.lfo_3.rate",
				val,
				_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _LFO_4_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.lfo_4.rate",
				val,
				_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _LFO_5_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.lfo_5.rate",
				val,
				_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _LFO_6_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
			(params,
				"adjsynth.lfo_6.rate",
				val,
				_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
				program);
		}
		break;

	case _MOD_LFO_SYMMETRY:
		if (modid == _LFO_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.lfo_1.symmetry",
				val,
				_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _LFO_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.lfo_2.symmetry",
				val,
				_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _LFO_3_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.lfo_3.symmetry",
				val,
				_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _LFO_4_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.lfo_4.symmetry",
				val,
				_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _LFO_5_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.lfo_5.symmetry",
				val,
				_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (modid == _LFO_6_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
			(params,
				"adjsynth.lfo_6.symmetry",
				val,
				_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
				program);
		}
		break;
	}
	
	return 0;
}
