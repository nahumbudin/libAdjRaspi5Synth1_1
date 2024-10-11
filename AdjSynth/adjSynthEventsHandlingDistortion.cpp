/**
*	@file		adjSynthEventsHandlingDistortion.cpp
*	@author		Nahum Budin
*	@date		11-Oct-2024
*	@version	1.1
*					1. Code refactoring and notaion.
*	
*	@brief		AdjHeart Synthesizer Distortion Events Handling
*
*	History:\n
*	
*	version 1.0		15_Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"

/**
*   @brief  Initiates a distortion effect related event with integer value (affects all voices).
*			All available parameters values are defined in LibAPI/synthesizer.h
*   @param  int distid	target distortion: _DISTORTION_1_EVENT, _DISTORTION_2_EVENT
*	@param	int eventid	specific event code:\n
*				_DISTORTION_DRIVE, _DISTORTION_RANGE, _DISTORTION_BLEND\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_DISTORTION_DRIVE: 0-100\n
*				_DISTORTION_RANGE: 0-100\n
*				_DISTORTION_BLEND: 0-100\n
*	@param	int program	program number
*
*   @return 0
*/
int AdjSynth::distortion_event(int distid, int eventid, int val, _settings_params_t *params, int program)
{
	int voice;
	float value;

	switch (eventid)
	{
	case _DISTORTION_DRIVE:
		if (distid == _DISTORTION_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.distortion_1.drive",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);	

		}
		else if (distid == _DISTORTION_2_EVENT)
		{
					
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.distortion_2.drive",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}

		break;

	case _DISTORTION_RANGE:
		if (distid == _DISTORTION_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.distortion_1.range",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (distid == _DISTORTION_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.distortion_2.range",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}

		break;

	case _DISTORTION_BLEND:
		if (distid == _DISTORTION_1_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.distortion_1.blend",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		else if (distid == _DISTORTION_2_EVENT)
		{
			adj_synth_settings_manager->set_int_param_value
				(params,
				"adjsynth.distortion_2.blend",
				val,
				_EXEC_BLOCK_CALLBACK,
				program);
		}
		
		break;
	}
	
	return 0;
}

/**
*   @brief  Initiates a distortion effect related event with boolean value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int distid	target distortion: _DISTORTION_1_EVENT, _DISTORTION_2_EVENT
*	@param	int eventid	specific event code:\n
*				_DISTORTION_ENABLE, _DISTORTION_AUTO_GAIN\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_DISTORTION_ENABLE, _DISTORTION_AUTO_GAIN: true - enabled; false - disabled\n
*	@param int program	program number
*   @return 0
*/
int AdjSynth::distortion_event_bool(int distid, int eventid, bool val, _settings_params_t *params, int program)
{
	switch (eventid)
	{
	case _ENABLE_DISTORTION:
		adj_synth_settings_manager->set_bool_param_value
			(params,
			"adjsynth.distortion.enabled",
			val,
			_EXEC_BLOCK_CALLBACK,
			program);
		
		break;

	case _DISTORTION_AUTO_GAIN:
		adj_synth_settings_manager->set_bool_param_value
			(params,
			"adjsynth.distortion.auto_gain_enabled",
			val,
			_EXEC_BLOCK_CALLBACK,
			program);

		break;	
	}
	
	return 0;
}
