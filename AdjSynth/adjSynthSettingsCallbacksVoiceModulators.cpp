/**
*	@file		adjSynthSettingsCallbacksVoiceModulators.cpp
*	@author		Nahum Budin
*	@date		5-Oct-2024
*	@version	1.2
*					1. Code refactoring and notaion.
*	
*	@brief		Callback to handle voice block Modulators Settings
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

int set_voice_block_lfo_1_waveform_cb(int wavf, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->lfo_1->
					set_waveform(wavf);
	return 0;
}

int set_voice_block_lfo_1_rate_cb(int rate, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					set_lfo_1_frequency(rate);
	return 0;
}

int set_voice_block_lfo_1_symmetry_cb(int sym, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->lfo_1->
					set_pwm_dcycle(sym);
	return 0;
}


int set_voice_block_lfo_2_waveform_cb(int wavf, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->lfo_2->
					set_waveform(wavf);
	return 0;
}

int set_voice_block_lfo_2_rate_cb(int rate, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					set_lfo_2_frequency(rate);
	return 0;
}

int set_voice_block_lfo_2_symmetry_cb(int sym, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->lfo_2->
					set_pwm_dcycle(sym);
	return 0;
}


int set_voice_block_lfo_3_waveform_cb(int wavf, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->lfo_3->
					set_waveform(wavf);
	return 0;
}

int set_voice_block_lfo_3_rate_cb(int rate, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					set_lfo_3_frequency(rate);
	//	AdjSynth::get_instance()->audioPolyMixer->setLfo3Frequency(rate);
	return 0;
}

int set_voice_block_lfo_3_symmetry_cb(int sym, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					lfo_3->set_pwm_dcycle(sym);
	return 0;
}


int set_voice_block_lfo_4_waveform_cb(int wavf, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->lfo_4->
					set_waveform(wavf);
	return 0;
}

int set_voice_block_lfo_4_rate_cb(int rate, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					set_lfo_4_frequency(rate);
	//	AdjSynth::get_instance()->audioPolyMixer->setLfo4Frequency(rate);
	return 0;
}

int set_voice_block_lfo_4_symmetry_cb(int sym, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->lfo_4->
					set_pwm_dcycle(sym);
	return 0;
}


int set_voice_block_lfo_5_waveform_cb(int wavf, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->lfo_5->
					set_waveform(wavf);
	return 0;
}

int set_voice_block_lfo_5_rate_cb(int rate, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					set_lfo_5_frequency(rate);
	//	AdjSynth::get_instance()->audioPolyMixer->setLfo5Frequency(rate);
	return 0;
}

int set_voice_block_lfo_5_symmetry_cb(int sym, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->lfo_5->
					set_pwm_dcycle(sym);
	return 0;
}


int set_voice_block_env_1_attack_cb(int attck, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->adsr_1->
					set_attack_time_sec(attck);
	return 0;
}

int set_voice_block_env_1_decay_cb(int dec, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->adsr_1->
					set_decay_time_sec(dec);
	return 0;
}

int set_voice_block_env_1_sustain_cb(int sus, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->adsr_1->
					set_send_level_1(sus);
	return 0;
}

int set_voice_block_env_1_release_cb(int rel, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->adsr_1->
					set_release_time_sec(rel);
	return 0;
}


int set_voice_block_env_2_attack_cb(int attck, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->adsr_2->
					set_attack_time_sec(attck);
	return 0;
}

int set_voice_block_env_2_decay_cb(int dec, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->adsr_2->
					set_decay_time_sec(dec);
	return 0;
}

int set_voice_block_env_2_sustain_cb(int sus, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->adsr_2->
					set_sustain_level(sus);
	return 0;
}

int set_voice_block_env_2_release_cb(int rel, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->adsr_2->
					set_release_time_sec(rel);
	return 0;
}


int set_voice_block_env_3_attack_cb(int attck, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->adsr_3->
					set_attack_time_sec(attck);
	return 0;
}

int set_voice_block_env_3_decay_cb(int dec, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->adsr_3->
					set_decay_time_sec(dec);
	return 0;
}

int set_voice_block_env_3_sustain_cb(int sus, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->adsr_3->
					set_sustain_level(sus);
	return 0;
}

int set_voice_block_env_3_release_cb(int rel, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->adsr_3->
					set_release_time_sec(rel);
	return 0;
}


int set_voice_block_env_4_attack_cb(int attck, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					adsr_4->set_attack_time_sec(attck);
	return 0;
}

int set_voice_block_env_4_decay_cb(int dec, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					adsr_4->set_decay_time_sec(dec);
	return 0;
}

int set_voice_block_env_4_sustain_cb(int sus, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					adsr_4->set_sustain_level(sus);
	return 0;
}

int set_voice_block_env_4_release_cb(int rel, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					adsr_4->set_release_time_sec(rel);
	return 0;
}


int set_voice_block_env_5_attack_cb(int attck, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->
					adsr_5->set_attack_time_sec(attck);
	return 0;
}

int set_voice_block_env_5_decay_cb(int dec, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->adsr_5->
					set_decay_time_sec(dec);
	return 0;
}

int set_voice_block_env_5_sustain_cb(int sus, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->adsr_5->
					set_sustain_level(sus);
	return 0;
}

int set_voice_block_env_5_release_cb(int rel, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->adsr_5->
					set_release_time_sec(rel);
	return 0;
}
