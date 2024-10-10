/**
*	@file		adjSynthVoice.h
*	@author		Nahum Budin
*	@date		3-Oct-2024
*	@version	1.2 
*					1. Code refactoring and notaion.
*					
*	@version	2-Feb--2021	1.1
*					1. Code refactoring and notaion.
*					2. Adding sample-rate and bloc-size settings
*					3. Adding audio manager object ref.
*				9-Nov-2019	1.0 revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 June 13, 2018
*
*	@brief		Synthesizer voice object
*/

#pragma once

#include "../Settings/settings.h"
#include "../commonDefs.h"
#include "../LibAPI/synthesizer.h"
#include "../commonDefs.h"
#include "../Audio/audioVoice.h"
#include "../Audio/audioOutput.h"
#include "../Audio/audioManager.h"

#include "../DSP/dspMorphedSineOsc.h"




class SynthVoice
{
public:
	SynthVoice(int vnum = 0,
		int prg = 0, 
		int samp_rate = _DEFAULT_SAMPLE_RATE,
		int block_size = _DEFAULT_BLOCK_SIZE,
		_settings_params_t *params = NULL,
		DSP_MorphingSinusOscWTAB *mso_tab = NULL,
		Wavetable *synth_pad_wavetable = NULL,
		AudioManager *aud_mng = NULL);
	
	SynthVoice* get_instance();
	
	int set_sample_rate(int samp_rate);
	int get_sample_rate();
	
	int set_audio_block_size(int size);
	int get_audio_block_size();

	void set_voice_params(_settings_params_t *params);
	_settings_params_t *get_voice_params();

	void assign_dsp_voice(DSP_AdjSynthVoice *dspv = NULL);

	void set_allocated_program(int prg);
	int get_allocated_program();
	
	void update_all();

	AudioVoiceFloat *audio_voice = NULL;

	DSP_AdjSynthVoice *dsp_voice = NULL;

	int voice_num;
	int allocated_to_program_num;
	int allocated_to_program_voice_num;

	DSP_MorphingSinusOscWTAB *mso_wtab = NULL;
	Wavetable *pad_wavetable = NULL;
	
private:	
	
	// Play Mode: Poly, Solo, MIDI
	int play_mode;	
	bool update_in_progress;
	bool update_enable;

	int amp_1_pan_mod_lfo, amp_2_pan_mod_lfo;
	int amp_1_pan_mod_lfo_level, amp_2_pan_mod_lfo_level;
	
	int sample_rate, audio_block_size;
	
	AudioManager *audio_manager = NULL;
	
	AudioOutputFloat *audio_out = NULL;
	
	AudioConnectionFloat *connection_voice_out_ch1 = NULL;
	AudioConnectionFloat *connection_voice_out_ch2 = NULL;
	
	// for update_all - holds the 1st object in the propagate chain
	AudioBlockFloat *audio_first_update[_MAX_STAGE_NUM] = { NULL };
	
	_settings_params_t active_params, prev_active_params;
	Settings *settings_manager = NULL;
};
