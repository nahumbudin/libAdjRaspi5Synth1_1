/**
*	@file		adjSynthSettingsCallbacksVoiceNoise.cpp
*	@author		Nahum Budin
*	@date		15_Nov-2019
*	@date		5-Oct-2024
*	@version	1.1
*					1. Code refactoring and notaion.
*	
*	@brief		Callback to handle voice block Noise Generator Settings
*	settings
*
*	History:\n
*	
*	version 1.0		15_Nov-2019	First version
*		
*/

#include "adjSynth.h"

int set_voice_block_noise_enabled_cb(bool enable, int voice, int prog)
{
	if (enable)
	{
		AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
						enable_noise_1();
	}
	else
	{
		AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
						disable_noise_1();
	}
	return 0;
}

int set_voice_block_noise_color_cb(int typ, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->noise_1->
					set_noise_type(typ);	
	return 0;
}

int set_voice_block_noise_send_filter_1_cb(int snd, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					set_noise_1_send_filter_1_level(snd);
	return 0;
}

int set_voice_block_noise_send_filter_2_cb(int snd, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->
					dsp_voice->set_noise_1_send_filter_2_level(snd);
	return 0;
}

int set_voice_block_noise_amp_modulation_lfo_num_cb(int lfon, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					set_noise_1_amp_mod_lfo(lfon);
	return 0;
}

int set_voice_block_noise_amp_modulation_lfo_level_cb(int lfolev, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					set_noise_1_amp_mod_lfo_level(lfolev);
	return 0;
}

int set_voice_block_noise_amp_modulation_env_num_cb(int envn, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					set_noise_1_amp_mod_env(envn);
	return 0;
}

int set_voice_block_noise_amp_modulation_env_level_cb(int envlev, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					set_noise_1_amp_mod_env_level(envlev);
	return 0;
}

