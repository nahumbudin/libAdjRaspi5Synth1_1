/**
 *	@file		audioPolyMixer.cpp
 *	@author		Nahum Budin
 *	@date		29-Sep-2025
 *	@version	1.0
 *
 *
 *	Mix audio ch1 and ch2 of all voices into a master stereo Left and Right output signals.
 *
 *	Unlike the previous version (audioPolyphonyMixer.h), this version does not handle the voice
 *   allocations to programs but only directlly the audio mixing of all the voices.
 *	The voice allocation to programs is handled by AdjSynthPrograms object.
 *
 */

#include "audioPolyMixer.h"

/* Mutex to controll audio memory blocks allocation (adjSynyh.h) */
extern pthread_mutex_t voice_mem_blocks_allocation_control_mutex;

/* Global used to set individual output level/pan/send for non program operation */
float master_level_1, master_level_2, master_pan_1, master_pan_2, master_send_1, master_send_2;

AudioManager *poly_mixer_manager;

AudioPolyMixer *AudioPolyMixer::audio_poly_mixer_instance = NULL;

int AudioPolyMixer::num_of_inputs = _SYNTH_MAX_NUM_OF_VOICES;

AudioPolyMixer::AudioPolyMixer(
	int stage,
	int num_of_voices,
	int block_size,
	int num_of_programs, // TODO: currently not used
	int mapping_mode,
	AudioBlockFloat **audio_first_update_ptr)
	: AudioBlockFloat(
		  0,	// no audio input - input data comes from AudioManager shared memory
		  NULL, // no audio input queue
		  audio_first_update_ptr,
		  stage)
{
	set_audio_block_size(block_size);
	set_midi_mapping_mode(mapping_mode);

	poly_mixer_manager = AudioManager::get_instance();

	num_of_inputs = num_of_voices;
	if (num_of_inputs > _SYNTH_MAX_NUM_OF_VOICES)
	{
		num_of_inputs = _SYNTH_MAX_NUM_OF_VOICES;
	}
	
	for (int i = 0; i < num_of_inputs; i++)
	{
		gain1[i] = 0.5f;
		gain2[i] = 0.5f;
		pan1[i] = 0.0f;
		pan2[i] = 0.0f;
		send1[i] = 0.0f;
		send2[i] = 0.0f;
	}

	master_level_1 = 0.5f;
	master_level_2 = 0.5f;
	master_pan_1 = 0.0f;
	master_pan_2 = 0.0f;
	master_send_1 = 0.0f;
	master_send_2 = 0.0f;

	/*
	programs = num_of_programs;
	if (programs > _SYNTH_MAX_NUM_OF_PROGRAMS)
	{
		programs = _SYNTH_MAX_NUM_OF_PROGRAMS;
	}
	*/

	/* LFOs for pan modulations */
	lfo1 = new DSP_Osc(5000,
					   _OSC_WAVEFORM_SINE,
					   50,
					   0,
					   0,
					   0,
					   false,
					   false,
					   false,
					   0.0f,
					   0.0f,
					   _OSC_UNISON_MODE_12345678);
	set_lfo_1_frequency((float)10);

	lfo2 = new DSP_Osc(5001,
					   _OSC_WAVEFORM_SINE,
					   50,
					   0,
					   0,
					   0,
					   false,
					   false,
					   false,
					   0.0f,
					   0.0f,
					   _OSC_UNISON_MODE_12345678);
	set_lfo_2_frequency((float)10);

	lfo3 = new DSP_Osc(5002,
					   _OSC_WAVEFORM_SINE,
					   50,
					   0,
					   0,
					   0,
					   false,
					   false,
					   false,
					   0.0f,
					   0.0f,
					   _OSC_UNISON_MODE_12345678);
	set_lfo_3_frequency((float)10);

	lfo4 = new DSP_Osc(5003,
					   _OSC_WAVEFORM_SINE,
					   50,
					   0,
					   0,
					   0,
					   false,
					   false,
					   false,
					   0.0f,
					   0.0f,
					   _OSC_UNISON_MODE_12345678);
	set_lfo_4_frequency((float)10);

	lfo5 = new DSP_Osc(5004,
					   _OSC_WAVEFORM_SINE,
					   50,
					   0,
					   0,
					   0,
					   false,
					   false,
					   false,
					   0.0f,
					   0.0f,
					   _OSC_UNISON_MODE_12345678);
	set_lfo_5_frequency((float)10);

	lfo6 = new DSP_Osc(5005,
					   _OSC_WAVEFORM_SINE,
					   50,
					   0,
					   0,
					   0,
					   false,
					   false,
					   false,
					   0.0f,
					   0.0f,
					   _OSC_UNISON_MODE_12345678);
	set_lfo_6_frequency((float)10);
}

AudioPolyMixer *AudioPolyMixer::get_instance(
	int stage,
	int num_of_voices,
	int num_of_programs,
	int block_size,
	int mapping_mode,
	AudioBlockFloat **audio_first_update_ptr)
{
	if (audio_poly_mixer_instance == NULL)
	{
		audio_poly_mixer_instance = new AudioPolyMixer(
			stage,
			num_of_voices,
			num_of_programs,
			block_size,
			mapping_mode,
			audio_first_update_ptr);
	}
	
	return audio_poly_mixer_instance;
}



AudioPolyMixer::~AudioPolyMixer()
{
	
}

/**
 *   @brief  Set mixer active state
 *   @param  active	when true mixer will become active and not active when set to false
 *   @return void
 */
void AudioPolyMixer::set_active()
{
	active = true;
}

/**
 *   @brief  Set mixer Master level 1
 *   @param  lev	level 0-100
 *   @return void
 */

/**
 *   @brief  sets the midi mapping mode
 *   @param  int mode: _MIDI_MAPPING_MODE_SKETCH, _MIDI_MAPPING_MODE_MAPPING
 *   @return mode if OK; -1 param out of range
 */
int AudioPolyMixer::set_midi_mapping_mode(int mode)
{
	int res = 0;

	if (is_valid_midi_mapping_mode(mode))
	{
		midi_mapping_mode = mode;
		res = midi_mapping_mode;
	}
	else
	{
		res = -1;
	}

	return res;
}

/**
 *   @brief  retruns the audio block size
 *   @param  none
 *   @return buffer size
 */
int AudioPolyMixer::get_midi_mapping_mode()
{
	return midi_mapping_mode;
}

void AudioPolyMixer::set_master_level_1(int lev)
{
	master_level_1 = (float)lev / 100.f;

	if (master_level_1 < 0)
	{
		master_level_1 = 0.f;
	}
	else if (master_level_1 > 1.f)
	{
		master_level_1 = 1.f;
	}
}

/**
 *   @brief  Set mixer Master level 2
 *   @param  lev	level 0-100
 *   @return void
 */
void AudioPolyMixer::set_master_level_2(int lev)
{
	master_level_2 = (float)lev / 100.f;
	if (master_level_2 < 0)
	{
		master_level_2 = 0.f;
	}
	else if (master_level_2 > 1.f)
	{
		master_level_2 = 1.f;
	}
}

/**
 *   @brief  Set mixer Master pan 1
 *   @param  pan	level 0-100 (50 - center)
 *   @return void
 */
void AudioPolyMixer::set_master_pan_1(int pan)
{
	master_pan_1 = (float)(pan - 50) / 50.f;
	if (master_pan_1 < -1.f)
	{
		master_pan_1 = -1.f;
	}
	else if (master_pan_1 > 1.f)
	{
		master_pan_1 = 1.f;
	}
}

/**
 *   @brief  Set mixer Master pan 2
 *   @param  pan	level 0-100 (50 - center)
 *   @return void
 */
void AudioPolyMixer::set_master_pan_2(int pan)
{
	master_pan_2 = (float)(pan - 50) / 50.f;
	if (master_pan_2 < -1.f)
	{
		master_pan_2 = -1.f;
	}
	else if (master_pan_2 > 1.f)
	{
		master_pan_2 = 1.f;
	}
}

/**
 *   @brief  Set mixer Master send 1
 *   @param  pan	level 0-100 (50 - center)
 *   @return void
 */
void AudioPolyMixer::set_master_send_1(int lev)
{
	master_send_1 = (float)lev / 100.f;
	if (master_send_1 < 0)
	{
		master_send_1 = 0.f;
	}
	else if (master_send_1 > 1.f)
	{
		master_send_1 = 1.f;
	}
}

/**
 *   @brief  Set mixer Master send 2
 *   @param  pan	level 0-100 (50 - center)
 *   @return void
 */
void AudioPolyMixer::set_master_send_2(int lev)
{
	master_send_2 = (float)lev / 100.f;
	if (master_send_2 < 0)
	{
		master_send_2 = 0.f;
	}
	else if (master_send_2 > 1.f)
	{
		master_send_2 = 1.f;
	}
}

/**
 *   @brief  sets the audio block size
 *   @param  int size: _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
 *   @return bloc size OK; -1 param out of range
 */
int AudioPolyMixer::set_audio_block_size(int size)
{
	int res = 0;

	if (is_valid_audio_block_size(size))
	{
		audio_block_size = size;
		res = audio_block_size;
	}
	else
	{
		res = -1;
	}

	return res;
}

/**
 *   @brief  retruns the audio block size
 *   @param  none
 *   @return buffer size
 */
int AudioPolyMixer::get_audio_block_size()
{
	return audio_block_size;
}

/* Set a voice gain1 level value.*/
int AudioPolyMixer::set_voice_gain1_level(int voice, float level)
{
	if ((voice >= 0) && (voice < num_of_inputs))
	{
		if (level < 0.f)
		{
			level = 0.f;
		}
		if (level > 1.f)
		{
			level = 1.f;
		}
		
		gain1[voice] = level;
		
		return 0;
	}
	
	return -1;
}

/* Set a voice gain2 level value.*/
int AudioPolyMixer::set_voice_gain2_level(int voice, float level)
{
	if ((voice >= 0) && (voice < num_of_inputs))
	{
		if (level < 0.f)
		{
			level = 0.f;
		}
		if (level > 1.f)
		{
			level = 1.f;
		}
		
		gain2[voice] = level;
		
		return 0;
	}

	return -1;
}

/* Set a voice send1 level value.*/
int AudioPolyMixer::set_voice_send1_level(int voice, float level)
{
	if ((voice >= 0) && (voice < num_of_inputs))
	{
		if (level < 0.f)
		{
			level = 0.f;
		}
		if (level > 1.f)
		{
			level = 1.f;
		}
		
		send1[voice] = level;
		
		return 0;
	}
	return -1;
}

/* Set a voice send2 level value.*/
int AudioPolyMixer::set_voice_send2_level(int voice, float level)
{
	if ((voice >= 0) && (voice < num_of_inputs))
	{
		if (level < 0.f)
		{
			level = 0.f;
		}
		if (level > 1.f)
		{
			level = 1.f;
		}
		
		send2[voice] = level;
		
		return 0;
	}
	return -1;
}

/* Set a voice pan1 value.*/
int AudioPolyMixer::set_voice_pan1(int voice, float pan)
{
	if ((voice >= 0) && (voice < num_of_inputs))
	{
		if (pan < -1.f)
		{
			pan = -1.f;
		}
		if (pan > 1.f)
		{
			pan = 1.f;
		}
		
		pan1[voice] = pan;
		
		return 0;
	}
	return -1;
}

/* Set a voice pan2 value.*/
int AudioPolyMixer::set_voice_pan2(int voice, float pan)
{
	if ((voice >= 0) && (voice < num_of_inputs))
	{
		if (pan < -1.f)
		{
			pan = -1.f;
		}
		if (pan > 1.f)
		{
			pan = 1.f;
		}
		
		pan2[voice] = pan;
		
		return 0;
	}
	return -1;
}

/**
 *   @brief  Set LFO 1 frequency 0-100
 *			(will be set to _MOD_LFO_MIN_FREQ to _MOD_LFO_MX_FREQ in a log10 scale)
 *	@param	freq	frequency 0-100
 *   @return void
 */
void AudioPolyMixer::set_lfo_1_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));

	// Set lfo frequencies due to subsampling rate
	lfo_1_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (lfo_1_actual_freq > (float)_OSC_MAX_FREQUENCY)
	{
		lfo_1_actual_freq = (float)_OSC_MAX_FREQUENCY;
	}
}

/**
 *   @brief  Set LFO 2 frequency 0-100
 *			(will be set to _MOD_LFO_MIN_FREQ to _MOD_LFO_MX_FREQ in a log10 scale)
 *	@param	freq	frequency 0-100
 *   @return void
 */
void AudioPolyMixer::set_lfo_2_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));

	// Set lfo frequencies due to subsampling rate
	lfo_2_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (lfo_2_actual_freq > (float)_OSC_MAX_FREQUENCY)
	{
		lfo_2_actual_freq = (float)_OSC_MAX_FREQUENCY;
	}
}

/**
 *   @brief  Set LFO 3 frequency 0-100
 *			(will be set to _MOD_LFO_MIN_FREQ to _MOD_LFO_MX_FREQ in a log10 scale)
 *	@param	freq	frequency 0-100
 *   @return void
 */
void AudioPolyMixer::set_lfo_3_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));

	// Set lfo frequencies due to subsampling rate
	lfo_3_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (lfo_3_actual_freq > (float)_OSC_MAX_FREQUENCY)
		lfo_3_actual_freq = (float)_OSC_MAX_FREQUENCY;
}

/**
 *   @brief  Set LFO 4 frequency 0-100
 *			(will be set to _MOD_LFO_MIN_FREQ to _MOD_LFO_MX_FREQ in a log10 scale)
 *	@param	freq	frequency 0-100
 *   @return void
 */
void AudioPolyMixer::set_lfo_4_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));

	// Set lfo frequencies due to subsampling rate
	lfo_4_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (lfo_4_actual_freq > (float)_OSC_MAX_FREQUENCY)
	{
		lfo_4_actual_freq = (float)_OSC_MAX_FREQUENCY;
	}
}

/**
 *   @brief  Set LFO 5 frequency 0-100
 *			(will be set to _MOD_LFO_MIN_FREQ to _MOD_LFO_MX_FREQ in a log10 scale)
 *	@param	freq	frequency 0-100
 *   @return void
 */
void AudioPolyMixer::set_lfo_5_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));

	// Set lfo frequencies due to subsampling rate
	lfo_5_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (lfo_5_actual_freq > (float)_OSC_MAX_FREQUENCY)
	{
		lfo_5_actual_freq = (float)_OSC_MAX_FREQUENCY;
	}
}

/**
 *   @brief  Set LFO 6 frequency 0-100
 *			(will be set to _MOD_LFO_MIN_FREQ to _MOD_LFO_MX_FREQ in a log10 scale)
 *	@param	freq	frequency 0-100
 *   @return void
 */
void AudioPolyMixer::set_lfo_6_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));

	// Set lfo frequencies due to subsampling rate
	lfo_6_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (lfo_6_actual_freq > (float)_OSC_MAX_FREQUENCY)
	{
		lfo_6_actual_freq = (float)_OSC_MAX_FREQUENCY;
	}
}

/**
 *	@brief	Set LFO 1 waveform
 *	@param wform waveform - _OSC_WAVEFORM_SINE, _OSC_WAVEFORM_TRIANGLE, _OSC_WAVEFORM_SAW, 
 *							_OSC_WAVEFORM_SQUARE, _OSC_WAVEFORM_RAMP, _OSC_WAVEFORM_SAMPHOLD
 *	@return void
 */
void AudioPolyMixer::set_lfo_1_waveform(float wf)
{
	if ((wf >= _OSC_WAVEFORM_SINE) && (wf <= _OSC_WAVEFORM_SAMPHOLD))
	{
		lfo1->set_waveform(wf);
	}
}

/**
 *	@brief	Set LFO 2 waveform
 *	@param wform waveform
 *	@return void
 */
void AudioPolyMixer::set_lfo_2_waveform(float wf)
{
	if ((wf >= _OSC_WAVEFORM_SINE) && (wf <= _OSC_WAVEFORM_SAMPHOLD))
	{
		lfo2->set_waveform(wf);
	}
}

/**
 *	@brief	Set LFO 3 waveform
 *	@param wform waveform
 *	@return void
 */
void AudioPolyMixer::set_lfo_3_waveform(float wf)
{
	if ((wf >= _OSC_WAVEFORM_SINE) && (wf <= _OSC_WAVEFORM_SAMPHOLD))
	{
		lfo3->set_waveform(wf);
	}
}

/**
 *	@brief	Set LFO 4 waveform
 *	@param wform waveform
 *	@return void
 */
void AudioPolyMixer::set_lfo_4_waveform(float wf)
{
	if ((wf >= _OSC_WAVEFORM_SINE) && (wf <= _OSC_WAVEFORM_SAMPHOLD))
	{
		lfo4->set_waveform(wf);
	}
}

/**
 *	@brief	Set LFO 5 waveform
 *	@param wform waveform
 *	@return void
 */
void AudioPolyMixer::set_lfo_5_waveform(float wf)
{
	if ((wf >= _OSC_WAVEFORM_SINE) && (wf <= _OSC_WAVEFORM_SAMPHOLD))
	{
		lfo5->set_waveform(wf);
	}
}

/**
 *	@brief	Set LFO 6 waveform
 *	@param wform waveform
 *	@return void
 */
void AudioPolyMixer::set_lfo_6_waveform(float wf)
{
	if ((wf >= _OSC_WAVEFORM_SINE) && (wf <= _OSC_WAVEFORM_SAMPHOLD))
	{
		lfo6->set_waveform(wf);
	}
}

/**
 *	@brief	Set LFO 1 symmetry
 *	@param sym	symmetry
 *	@return void
 */
void AudioPolyMixer::set_lfo_1_stmmetry(float sym)
{
	int symmetry = sym;

	if (symmetry < 5)
	{
		symmetry = 5;
	}
	else if (symmetry > 95)
	{
		symmetry = 95;
	}

	lfo1->set_pwm_dcycle(symmetry);
}

/**
 *	@brief	Set LFO 2 symmetry
 *	@param sym	symmetry
 *	@return void
 */
void AudioPolyMixer::set_lfo_2_stmmetry(float sym)
{
	int symmetry = sym;

	if (symmetry < 5)
	{
		symmetry = 5;
	}
	else if (symmetry > 95)
	{
		symmetry = 95;
	}

	lfo2->set_pwm_dcycle(symmetry);
}

/**
 *	@brief	Set LFO 3 symmetry
 *	@param sym	symmetry
 *	@return void
 */
void AudioPolyMixer::set_lfo_3_stmmetry(float sym)
{
	int symmetry = sym;

	if (symmetry < 5)
	{
		symmetry = 5;
	}
	else if (symmetry > 95)
	{
		symmetry = 95;
	}

	lfo3->set_pwm_dcycle(symmetry);
}

/**
 *	@brief	Set LFO 4 symmetry
 *	@param sym	symmetry
 *	@return void
 */
void AudioPolyMixer::set_lfo_4_stmmetry(float sym)
{
	int symmetry = sym;

	if (symmetry < 5)
	{
		symmetry = 5;
	}
	else if (symmetry > 95)
	{
		symmetry = 95;
	}

	lfo4->set_pwm_dcycle(symmetry);
}

/**
 *	@brief	Set LFO 5 symmetry
 *	@param sym	symmetry
 *	@return void
 */
void AudioPolyMixer::set_lfo_5_stmmetry(float sym)
{
	int symmetry = sym;

	if (symmetry < 5)
	{
		symmetry = 5;
	}
	else if (symmetry > 95)
	{
		symmetry = 95;
	}

	lfo5->set_pwm_dcycle(symmetry);
}
/**
 *	@brief	Set LFO 6 symmetry
 *	@param sym	symmetry
 *	@return void
 */
void AudioPolyMixer::set_lfo_6_stmmetry(float sym)
{
	int symmetry = sym;

	if (symmetry < 5)
	{
		symmetry = 5;
	}
	else if (symmetry > 95)
	{
		symmetry = 95;
	}

	lfo6->set_pwm_dcycle(symmetry);
}

/**
 *   @brief  Set amp 1 modulation LFO number.
 *	@param	Lfo		LFO number _LFO_NONE to  _LFO_6
 *   @return void
 */
void AudioPolyMixer::set_amp_1_pan_mod_lfo(int lfo)
{
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_6))
	{
		amp_1_pan_mod_lfo = lfo;
	}
}

/**
 *   @brief  Set amp 1 modulation LFO level.
 *	@param	Lev		Modulation level 0 to 100
 *   @return void
 */
void AudioPolyMixer::set_amp_1_pan_mod_lfo_level(int lev)
{
	if ((lev >= 0) && (lev <= 100))
	{
		amp_1_pan_mod_lfo_level = (float)lev / 100.0;
	}
}

/**
 *   @brief  Set amp 2 modulation LFO number.
 *	@param	Lfo		LFO number _LFO_NONE to  _LFO_5
 *   @return void
 */
void AudioPolyMixer::set_amp_2_pan_mod_lfo(int lfo)
{
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_3))
	{
		amp_2_pan_mod_lfo = lfo;
	}
}

/**
 *   @brief  Set amp 2 modulation LFO level.
 *	@param	Lev		Modulation level 0 to 100
 *   @return void
 */
void AudioPolyMixer::set_amp_2_pan_mod_lfo_level(int lev)
{
	if ((lev >= 0) && (lev <= 100))
	{
		amp_2_pan_mod_lfo_level = (float)lev / 100.0;
	}
}

/**
 *  @brief  Set amp 1 pan lfo modulation value.
 *	@param	modFactor	modulation factor (depth)
 *	@param	modVal		modulation signal value
 *	@param	value		pointer to a float variable to hold new modulation value
 *  @return void
 */
void AudioPolyMixer::set_amp_1_pan_lfo_modulation_value(float mod_factor, float mod_val, float *value)
{
	*(value) = mod_factor * mod_val + master_pan_1;
	if (*(value) < -1.0f)
	{
		*(value) = -1.0f; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	}
	else if (*(value) > 1.0f)
	{
		*(value) = 1.0f;
	}
}

/**
 *   @brief  Set amp 2 pan lfo modulation value.
 *	@param	modFactor	modulation factor (depth)
 *	@param	modVal		modulation signal value
 *	@param	value		pointer to a float variable to hold modulation valu
 *   @return void
 */
void AudioPolyMixer::set_amp_2_pan_lfo_modulation_value(float mod_factor, float mod_val, float *value)
{
	*(value) = mod_factor * mod_val + master_pan_1;
	if (*(value) < -1.0f)
	{
		*(value) = -1.0f; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	}
	else if (*(value) > 1.0f)
	{
		*(value) = 1.0f;
	}
}

/**
 *  @brief  Calculate next LFO modulation values.
 *	@param	valueCh1	pointer to a float variable to hold ch1 modulation value
 *	@param	valueCh2	pointer to a float variable to hold ch2 modulation value
 *	@param	none
 *   @return void
 */
void AudioPolyMixer::calc_next_modulation_values(float *mod_value_ch1, float *mod_value_ch2)
{
	lfo_out[0] = lfo1->get_next_output_val(lfo_1_actual_freq);
	lfo_out[1] = lfo2->get_next_output_val(lfo_2_actual_freq);
	lfo_out[2] = lfo3->get_next_output_val(lfo_3_actual_freq);
	lfo_out[3] = lfo4->get_next_output_val(lfo_4_actual_freq);
	lfo_out[4] = lfo5->get_next_output_val(lfo_5_actual_freq);
	lfo_out[5] = lfo6->get_next_output_val(lfo_6_actual_freq);

	if (amp_1_pan_mod_lfo > _LFO_NONE)
	{
		set_amp_1_pan_lfo_modulation_value(amp_1_pan_mod_lfo_level, 
										   lfo_out[amp_1_pan_mod_lfo - 1], 
										   mod_value_ch1);
	}
	else
	{
		set_amp_1_pan_lfo_modulation_value(0.0f, 0.0f, mod_value_ch1);
	}

	if (amp_2_pan_mod_lfo > _LFO_NONE)
	{
		set_amp_2_pan_lfo_modulation_value(amp_2_pan_mod_lfo_level, 
										   lfo_out[amp_2_pan_mod_lfo - 1], 
										   mod_value_ch2);
	}
	else
	{
		set_amp_2_pan_lfo_modulation_value(0.0f, 0.0f, mod_value_ch2);
	}
}

/**
 *   @brief  Execute an update cycle - get input samples, process and send to
 *				next audio block stage..
 *   @param  none
 *   @return void
 */
void AudioPolyMixer::update()
{
	audio_block_float_mono_t *block_out_L, *block_out_R, *block_send_L, *block_send_R;
	int voice, samp;

	/* Holds pan modulation values at sub sampling rate */
	float amp_1_pan_mod_samp[_AUDIO_MAX_BUF_SIZE / _CONTROL_SUB_SAMPLING + 1];
	float amp_2_pan_mod_samp[_AUDIO_MAX_BUF_SIZE / _CONTROL_SUB_SAMPLING + 1];

	/* Holds intermidiate values */
	float left_gain_1, left_gain_2, right_gain_1, right_gain_2;
	float left_send_1, left_send_2, right_send_1, right_send_2;

	/* Counts modulation subsample index. */
	int subsamp;

	// Fill PAN modulation arrays with subsampled modulation values.
	for (subsamp = 0; subsamp < audio_block_size / _CONTROL_SUB_SAMPLING + 1; subsamp++)
	{
		calc_next_modulation_values(&amp_1_pan_mod_samp[subsamp], &amp_2_pan_mod_samp[subsamp]);
	}

	if (active)
	{
		// Allocate output blocks
		pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);
		block_out_L = allocate_audio_block();
		block_out_R = allocate_audio_block();
		block_send_L = allocate_audio_block();
		block_send_R = allocate_audio_block();
		pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);

		if (!block_out_L || !block_out_R || !block_send_L || !block_send_R)
		{
			// unable to allocate any memory block, so we'll release what we could get, send nothing, and return.
			printf("Audio Poly Mixer - Unable to Allocate Memory/n");
			pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);

			if (block_out_L)
			{
				release_audio_block(block_out_L);
			}

			if (block_out_R)
			{
				release_audio_block(block_out_R);
			}

			if (block_send_L)
			{
				release_audio_block(block_send_L);
			}

			if (block_send_R)
			{
				release_audio_block(block_send_R);
			}
			pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);

			return;
		}

		subsamp = 0;

		// voice 0 - it is the 1st voice, so its samples will be used as base for all other voices.
		if (AdjSynth::get_instance()->synth_voice[0]->audio_voice->is_voice_active() ||
			AdjSynth::get_instance()->synth_voice[0]->audio_voice->is_voice_wait_for_not_active())
		{
			for (samp = 0; samp < audio_block_size; samp++)
			{
				if ((samp % _CONTROL_SUB_SAMPLING) == 0)
				{
					// Update modulation factors at sub sampling rate.
					left_gain_1 = gain1[0] * (1 - pan1[0]) * (1 - amp_1_pan_mod_samp[subsamp]) * master_level_1 * 0.1f;
					left_gain_2 = gain2[0] * (1 - pan2[0]) * (1 - amp_2_pan_mod_samp[subsamp]) * master_level_2 * 0.1f;
					right_gain_1 = gain1[0] * (1 + pan1[0]) * (1 + amp_1_pan_mod_samp[subsamp]) * master_level_1 * 0.1f;
					right_gain_2 = gain2[0] * (1 + pan2[0]) * (1 + amp_2_pan_mod_samp[subsamp]) * master_level_2 * 0.1f;

					if (midi_mapping_mode == _MIDI_MAPPING_MODE_MAPPING)
					{
						// In mapping mode use each voice send value for send calculation.
						left_send_1 = send1[0] * (1 - pan1[0]) * (1 - amp_1_pan_mod) * master_level_1 * 0.1f;
						left_send_2 = send2[0] * (1 - pan2[0]) * (1 - amp_2_pan_mod) * master_level_2 * 0.1f;
						right_send_1 = send1[0] * (1 + pan1[0]) * (1 + amp_1_pan_mod) * master_send_1 * 0.1f;
						right_send_2 = send2[0] * (1 + pan2[0]) * (1 + amp_2_pan_mod) * master_send_2 * 0.1f;
					}
					else
					{
						// In non-mapping mode use master send value for send calculation.
						left_send_1 = master_send_1 * (1 - master_pan_1) * (1 - amp_1_pan_mod) * 0.1f;
						left_send_2 = master_send_2 * (1 - master_pan_2) * (1 - amp_2_pan_mod) * 0.1f;
						right_send_1 = master_send_1 * (1 + master_pan_1) * (1 + amp_1_pan_mod) * 0.1f;
						right_send_2 = master_send_2 * (1 + master_pan_2) * (1 + amp_2_pan_mod) * 0.1f;
					}

					subsamp++;
				}

				// Calculate all block output samples for voice 0
				block_out_L->data[samp] =
					poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[0]->data[_LEFT][samp] *
						left_gain_1 +
					poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[0]->data[_RIGHT][samp] *
						left_gain_2;

				block_out_R->data[samp] =
					poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[0]->data[_LEFT][samp] *
						right_gain_1 +
					poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[0]->data[_RIGHT][samp] *
						right_gain_2;

				block_send_L->data[samp] =
					poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[0]->data[_LEFT][samp] *
						left_send_1 +
					poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[0]->data[_RIGHT][samp] *
						left_send_2;

				block_send_R->data[samp] =
					poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[0]->data[_LEFT][samp] *
						right_send_1 +
					poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[0]->data[_RIGHT][samp] *
						right_send_2;
			}
		}
		else
		{
			// voice not active - zero values output
			for (samp = 0; samp < audio_block_size; samp++)
			{
				block_out_L->data[samp] = 0;
				block_out_R->data[samp] = 0;
				block_send_L->data[samp] = 0;
				block_send_R->data[samp] = 0;
			}
		} // if (voice 0 active

		// All other voices: accumulate samples values to voice[0] samples or to 0 if voice[0] was not active.
		for (voice = 1; voice < num_of_inputs; voice++)
		{
			subsamp = 0;

			if (AdjSynth::get_instance()->synth_voice[voice]->audio_voice->is_voice_active() ||
				AdjSynth::get_instance()->synth_voice[voice]->audio_voice->is_voice_wait_for_not_active())
			{
				for (samp = 0; samp < audio_block_size; samp++)
				{
					if ((samp % _CONTROL_SUB_SAMPLING) == 0)
					{
						// Update modulation factors at sub sampling rate.
						left_gain_1 = gain1[voice] * (1 - pan1[voice]) * (1 - amp_1_pan_mod_samp[subsamp]) * master_level_1 * 0.1f;
						left_gain_2 = gain2[voice] * (1 - pan2[voice]) * (1 - amp_2_pan_mod_samp[subsamp]) * master_level_2 * 0.1f;
						right_gain_1 = gain1[voice] * (1 + pan1[voice]) * (1 + amp_1_pan_mod_samp[subsamp]) * master_level_1 * 0.1f;
						right_gain_2 = gain2[voice] * (1 + pan2[voice]) * (1 + amp_2_pan_mod_samp[subsamp]) * master_level_2 * 0.1f;

						if (midi_mapping_mode == _MIDI_MAPPING_MODE_MAPPING)
						{
							// In mapping mode use each voice send value for send calculation.
							left_send_1 = send1[voice] * (1 - pan1[voice]) * (1 - amp_1_pan_mod) * master_level_1 * 0.1f;
							left_send_2 = send2[voice] * (1 - pan2[voice]) * (1 - amp_2_pan_mod) * master_level_2 * 0.1f;
							right_send_1 = send1[voice] * (1 + pan1[voice]) * (1 + amp_1_pan_mod) * master_send_1 * 0.1f;
							right_send_2 = send2[voice] * (1 + pan2[voice]) * (1 + amp_2_pan_mod) * master_send_2 * 0.1f;
						}
						else
						{
							// In non-mapping mode use master send value for send calculation.
							left_send_1 = master_send_1 * (1 - master_pan_1) * (1 - amp_1_pan_mod) * 0.1f;
							left_send_2 = master_send_2 * (1 - master_pan_2) * (1 - amp_2_pan_mod) * 0.1f;
							right_send_1 = master_send_1 * (1 + master_pan_1) * (1 + amp_1_pan_mod) * 0.1f;
							right_send_2 = master_send_2 * (1 + master_pan_2) * (1 + amp_2_pan_mod) * 0.1f;
						}

						subsamp++;
					}

					// Accumulate all block output samples for all other voices
					block_out_L->data[samp] +=
						poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[voice]->data[_LEFT][samp] *
							left_gain_1 +
						poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[voice]->data[_RIGHT][samp] *
							left_gain_2;

					block_out_R->data[samp] +=
						poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[voice]->data[_LEFT][samp] *
							right_gain_1 +
						poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[voice]->data[_RIGHT][samp] *
							right_gain_2;

					block_send_L->data[samp] +=
						poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[voice]->data[_LEFT][samp] *
							left_send_1 +
						poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[voice]->data[_RIGHT][samp] *
							left_send_2;

					block_send_R->data[samp] +=
						poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[voice]->data[_LEFT][samp] *
							right_send_1 +
						poly_mixer_manager->audio_block_stereo_float_shared_memory_voices_output[voice]->data[_RIGHT][samp] *
							right_send_2;
				} // for (samp
			} // if (voice active
		} // for (voice

		// Recording TODO:
		/*
		if (RiffWave::getInstance()->isRecording())
		{
			for (i = 0; i < _PERIOD_SIZE; i++)
			{
				RiffWave::getInstance()->addLeftSample((int16_t)(blockOutL->data[i] * 32767.f));
				RiffWave::getInstance()->addRightSample((int16_t)(blockOutR->data[i] * 32767.f));
			}

			RiffWave::getInstance()->incBlockCount();
			callbackUpdateRecordingTimeDisplay(RiffWave::getInstance()->getBlockCount() * (uint32_t)_PERIOD_TIME_USEC / (uint32_t)1000);
		}

			//	if (RiffWave::getInstance()->getRecordingLength() >= 441000)
			//		RiffWave::getInstance()->stopRecording();
		*/

		// Send output blocks to next stage.
		transmit_audio_block(block_out_L, _LEFT);
		transmit_audio_block(block_out_R, _RIGHT);
		transmit_audio_block(block_send_L, _SEND_LEFT);
		transmit_audio_block(block_send_R, _SEND_RIGHT);

		// Release all blocks.
		pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);
		release_audio_block(block_out_L);
		release_audio_block(block_out_R);
		release_audio_block(block_send_L);
		release_audio_block(block_send_R);
		pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);
		
	} // if (active
}
