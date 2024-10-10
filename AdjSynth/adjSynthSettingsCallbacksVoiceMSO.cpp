/**
*	@file		adjSynthSettingsCallbacksVoiceMSO.cpp
*	@author		Nahum Budin
*	@date		5-Oct-2024
*	@version	1.2
*					1. Code refactoring and notaion.
*	
*	@brief		Callback to handle voice block Morphed Sine Oscilator Settings
*	settings
*
*	History:\n
*	
*	version	1.1	5-Feb-2021
*					1. Code refactoring and notaion.
*	version 1.0	15_Nov-2019	First version
*		
*/

#include "adjSynth.h"

int set_voice_block_mso_synth_enabled_cb(bool enable, int voice, int prog)
{
	if (enable)
	{
		AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
						enable_morphed_sin_1();
	}
	else
	{
		AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
						disable_morphed_sin_1();
	}
	return 0;
}

int set_voice_block_mso_synth_tune_offset_oct_cb(int oct, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->mso_1->
					set_freq_detune_oct(oct);
	return 0;
}

int set_voice_block_mso_synth_tune_offset_semitones_cb(int semi, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->mso_1->
					set_freq_detune_semitones(semi);
	return 0;
}

int set_voice_block_mso_synth_tune_offset_cents_cb(int cnt, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->mso_1->
					set_freq_detune_cents(cnt);
	return 0;
}

int set_voice_block_mso_synth_send_filter_1_cb(int send, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					set_mso_1_send_filter_1_level(send);
	return 0;
}

int set_voice_block_mso_synth_send_filter_2_cb(int send, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					set_mso_1_send_filter_2_level(send);
	return 0;
}

int set_voice_block_mso_synth_freq_modulation_lfo_num_cb(int lfon, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
		set_mso_1_freq_mod_lfo(lfon);
	return 0;
}

int set_voice_block_mso_synth_freq_modulation_lfo_level_cb(int lfolev, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					set_mso_1_freq_mod_lfo_level(lfolev);
	return 0;
}

int set_voice_block_mso_synth_freq_modulation_env_num_cb(int envn, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					set_mso_1_freq_mod_env(envn);
	return 0;
}

int set_voice_block_mso_synth_freq_modulation_env_level_cb(int envlev, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					set_mso_1_freq_mod_env_level(envlev);
	return 0;
}

int set_voice_block_mso_synth_pwm_modulation_lfo_num_cb(int lfon, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					set_mso_1_pwm_mod_lfo(lfon);
	return 0;
}

int set_voice_block_mso_synth_pwm_modulation_lfo_level_cb(int lfolev, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					set_mso_1_pwm_mod_lfo_level(lfolev);
	return 0;
}

int set_voice_block_mso_synth_pwm_modulation_env_num_cb(int envn, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					set_mso_1_pwm_mod_env(envn);
	return 0;
}

int set_voice_block_mso_synth_pwm_modulation_env_level_cb(int envlev, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					set_mso_1_pwm_mod_env_level(envlev);
	return 0;
}

int set_voice_block_mso_synth_amp_modulation_lfo_num_cb(int lfon, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					set_mso_1_amp_mod_lfo(lfon);
	return 0;
}

int set_voice_block_mso_synth_amp_modulation_lfo_level_cb(int lfolev, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					set_mso_1_amp_mod_lfo_level(lfolev);
	return 0;
}

int set_voice_block_mso_synth_amp_modulation_env_num_cb(int envn, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					set_mso_1_amp_mod_env(envn);
	return 0;
}

int set_voice_block_mso_synth_amp_modulation_env_level_cb(int envlev, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					set_mso_1_amp_mod_env_level(envlev);
	return 0;
}

int set_mso_synth_symmetry_cb(int sym, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->mso_wtab->set_morphing_symetry(sym);
	return 0;
}

int set_mso_synth_segment_position_a_cb(int pos, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->mso_wtab->set_segment_position(
		pos_a, 
		pos, 
		&AdjSynth::get_instance()->synth_program[prog]->mso_wtab->base_segment_positions);
	
	return 0;
}

int set_mso_synth_segment_position_b_cb(int pos, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->mso_wtab->set_segment_position(
		pos_b, 
		pos, 
		&AdjSynth::get_instance()->synth_program[prog]->mso_wtab->base_segment_positions);
	
	return 0;
}

int set_mso_synth_segment_position_c_cb(int pos, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->mso_wtab->set_segment_position(
		pos_c, 
		pos, 
		&AdjSynth::get_instance()->synth_program[prog]->mso_wtab->base_segment_positions);
	
	return 0;
}

int set_mso_synth_segment_position_d_cb(int pos, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->mso_wtab->set_segment_position(
		pos_d, 
		pos, 
		&AdjSynth::get_instance()->synth_program[prog]->mso_wtab->base_segment_positions);
	
	return 0;
}

int set_mso_synth_segment_position_e_cb(int pos, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->mso_wtab->set_segment_position(
		pos_e, 
		pos, 
		&AdjSynth::get_instance()->synth_program[prog]->mso_wtab->base_segment_positions);
	
	return 0;
}

int set_mso_synth_segment_position_f_cb(int pos, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->mso_wtab->set_segment_position(
		pos_f, 
		pos, 
		&AdjSynth::get_instance()->synth_program[prog]->mso_wtab->base_segment_positions);
	
	return 0;
}

