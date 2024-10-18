/**
*	@file		adjSynthSettingsCallbacksVoiceDistortion.cpp
*	@author		Nahum Budin
*	@date		5-Oct-2024
*	@version	1.2
*					1. Code refactoring and notaion.
*	
*	@brief		Callback to handle voice block Distortion Settings
*	settings
*
*	History:\n
*	
*	version	1.1	5-Feb_2021
*					1. Code refactoring and notaion.
*	version 1.0	15_Nov-2019	First version
*		
*/

#include "adjSynth.h"

int set_voice_block_distortion_enabled_cb(bool enable, int voice, int prog)
{
	if (enable)
	{
		AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->enable_distortion_1();
		AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->enable_distortion_2();
	}
	else
	{
		AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->disable_distortion_1();
		AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->enable_distortion_2();
	}

	return 0;
}

int set_voice_block_distortion_auto_gain_enabled_cb(bool enable, int voice, int prog)
{
	if (enable)
	{
		AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->
						dsp_voice->enable_distortion_1_auto_gain();
		AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->
						dsp_voice->enable_distortion_2_auto_gain();
	}
	else
	{
		AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->
						dsp_voice->disable_distortion_1_auto_gain();
		AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->
						dsp_voice->disable_distortion_2_auto_gain();
	}

	return 0;
}


int set_voice_block_distortion_1_drive_cb(int drv, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->
					dsp_voice->distortion_1->set_drive((float)drv / 100.f);
	return 0;
}

int set_voice_block_distortion_1_range_cb(int rng, int voice, int prog)
{
	float value = ((float)rng / 100.f) * (_DISTORTION_MAX_RANGE - _DISTORTION_MIN_RANGE) + _DISTORTION_MIN_RANGE;

	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->
					dsp_voice->distortion_1->set_range(value);
	return 0;
}

int set_voice_block_distortion_1_blend_cb(int blnd, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->
					dsp_voice->distortion_1->set_blend((float)blnd / 100.f);
	return 0;
}


int set_voice_block_distortion_2_drive_cb(int drv, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->
					dsp_voice->distortion_2->set_drive((float)drv / 100.f);
	return 0;
}

int set_voice_block_distortion_2_range_cb(int rng, int voice, int prog)
{
	float value = ((float)rng / 100.f) * (_DISTORTION_MAX_RANGE - _DISTORTION_MIN_RANGE) + _DISTORTION_MIN_RANGE;

	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->
					dsp_voice->distortion_2->set_range(value);
	return 0;
}

int set_voice_block_distortion_2_blend_cb(int blnd, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->
					dsp_voice->distortion_2->set_blend((float)blnd / 100.f);
	return 0;
}
