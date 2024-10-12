/**
*	@file		adjSynth.h
*	@author		Nahum Budin
*	@date		4-Oct-2024
*	@version	1.3 
*					1. Code refactoring and notaion.
*					
*	@brief		A collection of 4 synthesizers: Additive, Karplus String, PAD and Morphed Sine Oscilator (MSO)
*	
*	History:\n
*	
*	version 1.2		4-Feb--2021:
*			1. Code refactoring and notaion.
*			2. Adding sample-rate and bloc-size settings
*			3. Adding start/stop audio
*	version	1.1		18-Jan-2021: Adding audio parameters settings
*	version 1.0		15_Nov-2019: First version
*/

#pragma once

#include "adjSynthPADcreator.h"
#include "adjSynthVoice.h"
#include "adjSynthPolyphony.h"
#include "adjSynthProgram.h"
#include "adjSynthPolyphonyManager.h"
#include "synthKeyboard.h"

#include "../Audio/audioManager.h"
#include "../Audio/audioBandEqualizer.h"
#include "../Audio/audioReverb.h"
#include "../Audio/audioPolyphonyMixer.h"

class DSP_AdjSynthVoice;
class AudioManager;
class SynthVoice;
class AudioOutputFloat;
class AudioPolyMixerFloat;

void callback_audio_voice_update(int voice_num);
void callback_audio_update_cycle_end_tasks(int param);
//void callback_voice_end(int voice);

int set_patch_settings_default_params_callback_wrapper(_settings_params_t *params, int prog);

class AdjSynth
{
public:
	
	
	~AdjSynth();
	
	static AdjSynth *get_instance();
	
	int set_sample_rate(int samp_rate);
	int set_audio_block_size(int size);
	int set_audio_driver_type(int drive);
	
	int get_sample_rate();	
	int get_audio_block_size();	
	int get_audio_driver_type();
	
	int start_audio(int driver, int samp_rate, int block_size);
	int stop_audio();
	
	DSP_AdjSynthVoice *get_original_main_dsp_voices(int voice);
	
	int init_synth_settings_params(_settings_params_t *settings_params);
	
	void init_synth_voices();

	void init_synth_programs();	

	void set_master_volume(int vol);
	int get_master_volume();
	
	int set_settings_params(Settings *settings,  
		_settings_params_t *settings_params);

	std::string get_program_patch_name(int prog);
	
	_settings_params_t *get_active_patch_params();
	_settings_params_t *get_active_settings_params();
	//_setting_params_t *get_active_general_settings_params();
	
	int get_num_of_voices();
	int get_num_of_programs();
	void set_active_sketch(int ask);
	int get_active_sketch();
	int copy_sketch(int src_sk, int dest_sk);

	void init_poly();
	void init_jack();
	//void start_audio();
	
	void set_midi_mapping_mode(int mod);
	int get_midi_mapping_mode();
	
	int set_default_patch_parameters(_settings_params_t *params, int prog);
	int set_default_patch_parameters_vco(_settings_params_t *params, int prog);
	int set_default_patch_parameters_noise(_settings_params_t *params, int prog);
	int set_default_patch_parameters_kps(_settings_params_t *params, int prog);
	int set_default_patch_parameters_mso(_settings_params_t *params, int prog);
	int set_default_patch_parameters_pad(_settings_params_t *params, int prog);
	int set_default_patch_parameters_filter(_settings_params_t *params, int prog);
	int set_default_patch_parameters_amp(_settings_params_t *params, int prog);
	int set_default_patch_parameters_distortion(_settings_params_t *params, int prog);
	int set_default_patch_parameters_modulators(_settings_params_t *params, int prog);		
	
	int set_default_settings_parameters(_settings_params_t *params);
	int set_default_settings_parameters_equalizer(_settings_params_t *params);
	int set_default_settings_parameters_reverb(_settings_params_t *params);
	int set_default_settings_parameters_mixer(_settings_params_t *params);
	int set_default_settings_parameters_keyboard(_settings_params_t *params);

	//int set_default_general_settings_parameters(_setting_params_t *params);
	//int set_default_settings_parameters_audio(_setting_params_t *params);
	
	void set_hammond_percusion_mode(int mod, _settings_params_t *params, int program);
		
	int vco_event_int(int vcoid, int eventid, int val, _settings_params_t *params, int program);
	int vco_event_bool(int vcoid, int eventid, bool val, _settings_params_t *params, int program);
	int noise_event_int(int noiseid, int eventid, int val, _settings_params_t *params, int program);
	int noise_event_bool(int noiseid, int eventid, bool val, _settings_params_t *params, int program);
	int karplus_event_int(int karlplusid, int eventid, int val, _settings_params_t *params, int program);
	int karplus_event_bool(int karlplusid, int eventid, bool val, _settings_params_t *params, int program);
	int mso_event_int(int msoid, int eventid, int val, _settings_params_t *params, int program);
	int mso_event_bool(int msoid, int eventid, bool val, _settings_params_t *params, int program);
	int pad_event_int(int padid, int eventid, int val, _settings_params_t *params, int program);
	int pad_event_bool(int padid, int eventid, bool val, _settings_params_t *params, int program);
	int filter_event(int filtid, int eventid, int val, _settings_params_t *params, int program);
	int audio_event_int(int audid, int eventid, int val, _settings_params_t *params, int program);
	int audio_event_bool(int audid, int eventid, bool val, _settings_params_t *params, int program);
	int amp_event(int ampid, int eventid, int val, _settings_params_t *params, int program);
	int distortion_event_int(int distid, int eventid, int val, _settings_params_t *params, int program);
	int distortion_event_bool(int distid, int eventid, bool val, _settings_params_t *params, int program);
	int modulator_event_int(int modid, int eventid, int val, _settings_params_t *params, int program);
	
	int reverb_event_int(int revid, int eventid, int val, _settings_params_t *params);
	int reverb_event_bool(int revid, int eventid, bool val, _settings_params_t *params);
	int band_equilizer_event(int beqid, int eventid, int val, _settings_params_t *params);
	int band_equilizer_event_bool(int beqid, int eventid, bool val, _settings_params_t *params);	
	int midi_mixer_event(int mixid, int eventid, int val, _settings_params_t *params);	
	int kbd_event_int(int kbid, int eventid, int val, _settings_params_t *params);
	int kbd_event_bool(int kbid, int eventid, bool val, _settings_params_t *params);
	int midi_mode_event(int midmodid, int eventid, int val, _settings_params_t *params);
	int play_mode_event_bool(int pmodid, int eventid, bool val, _settings_params_t *params);
	
	void  midi_play_note_on(uint8_t channel, uint8_t byte2, uint8_t byte3, int voc = 0);
	void  midi_play_note_off(uint8_t channel, uint8_t byte2, uint8_t byte3, int voc = 0);
	
	// UI callbacks intiations
	void set_num_of_poly_disp_callback(int numv);
	void mark_voice_not_busy_callback(int vnum);
	void mark_voice_busy_callback(int vnum);
	void osc1_set_unison_mode_callback(int unimode);
	//	void setEventPrevDestCallback(int src, int evntId, int dest);
	void set_utilization_callback(int util);	
	int get_utilization_callback();
	void update_ui_callback();

	AdjPolyphonyManager *polypony_manager;
	
	SynthKeyboard *kbd1 = NULL;

	AudioManager *audio_manager = NULL;
	AudioPolyMixerFloat *audio_poly_mixer = NULL;

	AudioReverb *audio_reverb = NULL;
	AudioBandEqualizer *audio_equalizer = NULL;
	AudioOutputFloat *audio_out = NULL;
	
	

	
	/** This pool of SynthVoice objects is run and controlled by the synthesizer.
		Each of them is only a pointer to a SynthVoice object. */
	static SynthVoice *synth_voice[_SYNTH_MAX_NUM_OF_VOICES];

	SynthProgram *synth_program[_SYNTH_MAX_NUM_OF_PROGRAMS];
	static AdjPolyphonyManager *synth_polyphony_manager;

	SynthPADcreator *synth_pad_creator = NULL;
	DSP_MorphingSinusOscWTAB *mso_wtab = NULL;
	Wavetable *program_wavetable = NULL;
	
	
	static int num_of_core_voices;
	
	int poly_mode;
	int play_mode;
	int midi_mapping_mode;
	
	Settings *adj_synth_settings_manager = NULL;
	//ModSynthSettings *adj_synth_general_settings_manager;

	
private:
	
	AdjSynth();
	
	static AdjSynth *adj_synth;

	/* Holds the AdjSynth patch parameters */	
	_settings_params_t active_adj_synth_patch_params;
	/* Holds the AdjSynth settings parameters */
	_settings_params_t active_adj_synth_settings_params;
	/* Holds the ModSynth general settings parameters */
	//_setting_params_t active_adj_synth_general_settings_params;
	
	/* Hammond percussion mode settings */
	bool hammond_percussion_on, hammond_percussion_slow, hammond_percussion_soft, hammond_ercussion_3_rd;
	
	// Used to hold original voice settings to be reproduced when a dsp voice is disallocated by a voice program.
	DSP_AdjSynthVoice *original_main_dsp_voices[_SYNTH_MAX_NUM_OF_VOICES] = { NULL };
	
	int sample_rate, audio_block_size, audio_driver_type;
	
	int num_of_voices;
	int num_of_programs;
	int active_sketch;

	int master_volume;
	
	int utilization;

	// Audio connections
	AudioConnectionFloat *connection_mixer_out_L = NULL;
	AudioConnectionFloat *connection_mixer_out_R = NULL;

	AudioConnectionFloat *connection_mixer_send_L = NULL;
	AudioConnectionFloat *connection_mixer_send_R = NULL;

	AudioConnectionFloat *connection_equilizer_out_L = NULL;
	AudioConnectionFloat *connection_equilizer_out_R = NULL;

	AudioConnectionFloat *connection_reverb_out_L = NULL;
	AudioConnectionFloat *connection_reverb_out_R = NULL;
	
	// for update_all of output common path- holds the 1st object in the propagate chain
	AudioBlockFloat *audio_common_first_update = NULL;
	
};

//Callbacks to set general settings

int set_audio_jack_mode_cb(int mode, int prog);
int set_audio_jack_auto_start_state_cb(bool state, int prog);
int set_audio_jack_auto_connect_state_cb(bool state, int prog);

int set_amp_ch_1_send_cb(int lev, int prog);
int set_amp_ch_2_send_cb(int lev, int prog);

int set_reverb3m_preset_cb(int pset, int prog);
int set_reverb_room_size_cb(int rs, int prog);
int set_reverb_damp_cb(int dmp, int prog);
int set_reverb_wet_cb(int wt, int prog);
int set_reverb_dry_cb(int dry, int prog);
int set_reverb_width_cb(int wd, int prog);
int set_reverb_mode_cb(int md, int prog);
int set_reverb3m_enable_state_cb(bool en, int prog);
int set_reverb_enable_state_cb(bool en, int prog);

int set_band_equalizer_band_31_level_cb(int lvl, int prog);
int set_band_equalizer_band_62_level_cb(int lvl, int prog);
int set_band_equalizer_band_125_level_cb(int lvl, int prog);
int set_band_equalizer_band_250_level_cb(int lvl, int prog);
int set_band_equalizer_band_500_level_cb(int lvl, int prog);
int set_band_equalizer_band_1K_level_cb(int lvl, int prog);
int set_band_equalizer_band_2K_level_cb(int lvl, int prog);
int set_band_equalizer_band_4K_level_cb(int lvl, int prog);
int set_band_equalizer_band_8K_level_cb(int lvl, int prog);
int set_band_equalizer_band_16K_level_cb(int lvl, int prog);
int set_band_equalizer_preset_cb(int pres, int prog);

int set_mixer_lfo_1_waveform_cb(int wavf, int prog);
int set_mixer_lfo_2_waveform_cb(int wavf, int prog);
int set_mixer_lfo_3_waveform_cb(int wavf, int prog);
int set_mixer_lfo_4_waveform_cb(int wavf, int prog);
int set_mixer_lfo_5_waveform_cb(int wavf, int prog);

int set_mixer_lfo_1_rate_cb(int rate, int prog);
int set_mixer_lfo_2_rate_cb(int rate, int prog);
int set_mixer_lfo_3_rate_cb(int rate, int prog);
int set_mixer_lfo_4_rate_cb(int rate, int prog);
int set_mixer_lfo_5_rate_cb(int rate, int prog);


int setmixer_lfo_1_symmetry_cb(int sym, int prog);
int setmixer_lfo_2_symmetry_cb(int sym, int prog);
int setmixer_lfo_3_symmetry_cb(int sym, int prog);
int setmixer_lfo_4_symmetry_cb(int sym, int prog);
int setmixer_lfo_5_symmetry_cb(int sym, int prog);

int set_keyboard_portamento_cb(int port, int prog);
int set_keyboard_sensetivity_cb(int sens, int prog);
int set_keyboard_sensetivity_low_cb(int sens, int prog);
int set_keyboard_split_point_cb(int splt, int prog);
int set_keyboard_polyphonic_mode_cb(int mod, int prog);
int set_keyboard_portamento_enable_state_cb(bool en, int prog);

int set_mixer_channel_level_cb(int lev, int chan);
int set_mixer_channel_pan_cb(int pan, int chan);
int set_mixer_channel_send_cb(int snd, int chan);


int set_play_mode_cb(int pmod, int prog);

// Callbacs to set a voice settings
int set_voice_block_osc_1_enabled_cb(bool enable, int voice, int prog);
int set_voice_block_osc_1_waveform_cb(int wvf, int voice, int prog);
int set_voice_block_osc_1_pwm_symmetry_cb(int sym, int voice, int prog);
int set_voice_block_osc_1_send_filter_1_cb(int snd, int voice, int prog);
int set_voice_block_osc_1_send_filter_2_cb(int snd, int voice, int prog);
int set_voice_block_osc_1_tune_offset_oct_cb(int oct, int voice, int prog);
int set_voice_block_osc_1_tune_offset_semitones_cb(int smt, int voice, int prog);
int set_voice_block_osc_1_tune_offset_cents_cb(int cnt, int voice, int prog);
int set_voice_block_osc_1_freq_modulation_lfo_num_cb(int lfon, int voice, int prog);
int set_voice_block_osc_1_freq_modulation_lfo_level_cb(int lfolev, int voice, int prog);
int set_voice_block_osc_1_freq_modulation_env_num_cb(int envn, int voice, int prog);
int set_voice_block_osc_1_freq_modulation_env_level_cb(int envlev, int voice, int prog);
int set_voice_block_osc_1_pwm_modulation_lfo_num_cb(int lfon, int voice, int prog);
int set_voice_block_osc_1_pwm_modulation_lfo_level_cb(int lfolev, int voice, int prog);
int set_voice_block_osc_1_pwm_modulation_env_num_cb(int envn, int voice, int prog);
int set_voice_block_osc_1_pwm_modulation_env_level_cb(int envlev, int voice, int prog);
int set_voice_block_osc_1_amp_modulation_lfo_num_cb(int lfon, int voice, int prog);
int set_voice_block_osc_1_amp_modulation_lfo_level_cb(int lfolev, int voice, int prog);
int set_voice_block_osc_1_amp_modulation_env_num_cb(int envn, int voice, int prog);
int set_voice_block_osc_1_amp_modulation_env_level_cb(int envlev, int voice, int prog);
int set_voice_block_osc_1_unison_mod_cb(int unimod, int voice, int prog);
int set_voice_block_osc_1_hammond_percussion_mode_cb(int pmode, int voice, int prog);
int set_voice_block_osc_1_unison_level_1_cb(int level, int voice, int prog);
int set_voice_block_osc_1_unison_level_2_cb(int level, int voice, int prog);
int set_voice_block_osc_1_unison_level_3_cb(int level, int voice, int prog);
int set_voice_block_osc_1_unison_level_4_cb(int level, int voice, int prog);
int set_voice_block_osc_1_unison_level_5_cb(int level, int voice, int prog);
int set_voice_block_osc_1_unison_level_6_cb(int level, int voice, int prog);
int set_voice_block_osc_1_unison_level_7_cb(int level, int voice, int prog);
int set_voice_block_osc_1_unison_level_8_cb(int level, int voice, int prog);
int set_voice_block_osc_1_unison_level_9_cb(int level, int voice, int prog);
int set_voice_block_osc_1_unison_distortion_cb(int dist, int voice, int prog);
int set_voice_block_osc_1_unison_detune_cb(int det, int voice, int prog);
int set_voice_block_osc_1_unison_set_square_cb(bool sqr, int voice, int prog);

int set_voice_block_osc_2_enabled_cb(bool enable, int voice, int prog);
int set_voice_block_osc_2_waveform_cb(int wvf, int voice, int prog);
int set_voice_block_osc_2_pwm_symmetry_cb(int sym, int voice, int prog);
int set_voice_block_osc_2_send_filter_1_cb(int snd, int voice, int prog);
int set_voice_block_osc_2_send_filter_2_cb(int snd, int voice, int prog);
int set_voice_block_osc_2_tune_offset_oct_cb(int oct, int voice, int prog);
int set_voice_block_osc_2_tune_offset_semitones_cb(int smt, int voice, int prog);
int set_voice_block_osc_2_tune_offset_cents_cb(int cnt, int voice, int prog);
int set_voice_block_osc_2_freq_modulation_lfo_num_cb(int lfon, int voice, int prog);
int set_voice_block_osc_2_freq_modulation_lfo_level_cb(int lfolev, int voice, int prog);
int set_voice_block_osc_2_freq_modulation_env_num_cb(int envn, int voice, int prog);
int set_voice_block_osc_2_freq_modulation_env_level_cb(int envlev, int voice, int prog);
int set_voice_block_osc_2_pwm_modulation_lfo_num_cb(int lfon, int voice, int prog);
int set_voice_block_osc_2_pwm_modulation_lfo_level_cb(int lfolev, int voice, int prog);
int set_voice_block_osc_2_pwm_modulation_env_num_cb(int envn, int voice, int prog);
int set_voice_block_osc_2_pwm_modulation_env_level_cb(int envlev, int voice, int prog);
int set_voice_block_osc_2_amp_modulation_lfo_num_cb(int lfon, int voice, int prog);
int set_voice_block_osc_2_amp_modulation_lfo_level_cb(int lfolev, int voice, int prog);
int set_voice_block_osc_2_amp_modulation_env_num_cb(int envn, int voice, int prog);
int set_voice_block_osc_2_amp_modulation_env_level_cb(int envlev, int voice, int prog);
int set_voice_block_osc_2_sync_on_osc_1_state_cb(bool sync, int voice, int prog);

int set_voice_block_noise_enabled_cb(bool enable, int voice, int prog);
int set_voice_block_noise_color_cb(int col, int voice, int prog);
int set_voice_block_noise_send_filter_1_cb(int snd, int voice, int prog);
int set_voice_block_noise_send_filter_2_cb(int snd, int voice, int prog);
int set_voice_block_noise_amp_modulation_lfo_num_cb(int lfon, int voice, int prog);
int set_voice_block_noise_amp_modulation_lfo_level_cb(int lfolev, int voice, int prog);
int set_voice_block_noise_amp_modulation_env_num_cb(int envn, int voice, int prog);
int set_voice_block_noise_amp_modulation_env_level_cb(int envlev, int voice, int prog);

int set_voice_block_karplus_synth_enabled_cb(bool enable, int voice, int prog);
int set_voice_block_karplus_synth_excitation_waveform_type_cb(int type, int voice, int prog);
int set_voice_block_karplus_synth_excitation_waveform_variations_cb(int var, int voice, int prog);
int set_voice_block_karplus_synth_decay_cb(int dec, int voice, int prog);
int set_voice_block_karplus_synth_pluck_damping_cb(int dump, int voice, int prog);
int set_voice_block_karplus_synth_pluck_damping_variations_cb(int dump, int voice, int prog);
int set_voice_block_karplus_synth_string_damping_cb(int dump, int voice, int prog);
int set_voice_block_karplus_synth_string_damping_variations_cb(int dump, int voice, int prog);
int set_voice_block_karplus_synth_string_damping_calculation_mode_cb(int mode, int voice, int prog);
int set_voice_block_karplus_synth_send_filter_1_cb(int snd, int voice, int prog);
int set_voice_block_karplus_synth_send_filter_2_cb(int snd, int voice, int prog);
int set_voice_block_karplus_synth_on_decay_cb(int dec, int voice, int prog);
int set_voice_block_karplus_synth_off_decay_cb(int dec, int voice, int prog);

int set_voice_block_mso_synth_enabled_cb(bool enable, int voice, int prog);
int set_voice_block_mso_synth_tune_offset_oct_cb(int oct, int voice, int prog);
int set_voice_block_mso_synth_tune_offset_semitones_cb(int semi, int voice, int prog);
int set_voice_block_mso_synth_tune_offset_cents_cb(int cnt, int voice, int prog);
int set_voice_block_mso_synth_send_filter_1_cb(int send, int voice, int prog);
int set_voice_block_mso_synth_send_filter_2_cb(int send, int voice, int prog);
int set_voice_block_mso_synth_freq_modulation_lfo_num_cb(int lfon, int voice, int prog);
int set_voice_block_mso_synth_freq_modulation_lfo_level_cb(int lfolev, int voice, int prog);
int set_voice_block_mso_synth_freq_modulation_env_num_cb(int envn, int voice, int prog);
int set_voice_block_mso_synth_freq_modulation_env_level_cb(int envlev, int voice, int prog);
int set_voice_block_mso_synth_pwm_modulation_lfo_num_cb(int lfon, int voice, int prog);
int set_voice_block_mso_synth_pwm_modulation_lfo_level_cb(int lfolev, int voice, int prog);
int set_voice_block_mso_synth_pwm_modulation_env_num_cb(int envn, int voice, int prog);
int set_voice_block_mso_synth_pwm_modulation_env_level_cb(int envlev, int voice, int prog);
int set_voice_block_mso_synth_amp_modulation_lfo_num_cb(int lfon, int voice, int prog);
int set_voice_block_mso_synth_amp_modulation_lfo_level_cb(int lfolev, int voice, int prog);
int set_voice_block_mso_synth_amp_modulation_env_num_cb(int envn, int voice, int prog);
int set_voice_block_mso_synth_amp_modulation_env_level_cb(int envlev, int voice, int prog);

int set_mso_synth_symmetry_cb(int sym, int prog);
int set_mso_synth_segment_position_a_cb(int pos, int prog);
int set_mso_synth_segment_position_b_cb(int pos, int prog);
int set_mso_synth_segment_position_c_cb(int pos, int prog);
int set_mso_synth_segment_position_d_cb(int pos, int prog);
int set_mso_synth_segment_position_e_cb(int pos, int prog);
int set_mso_synth_segment_position_f_cb(int pos, int prog);

int set_voice_block_pad_synth_enabled_cb(bool enable, int voice, int prog);
int set_voice_block_pad_synth_detune_octave_cb(int oct, int voice, int prog);
int set_voice_block_pad_synth_detune_semitones_cb(int semt, int voice, int prog);
int set_voice_block_pad_synth_detune_cents_cb(int cnts, int voice, int prog);
int set_voice_block_pad_synth_send_filter_1_cb(int snd, int voice, int prog);
int set_voice_block_pad_synth_send_filter_2_cb(int snd, int voice, int prog);
int set_voice_block_pad_synth_freq_modulation_lfo_num_cb(int lfo, int voice, int prog);
int set_voice_block_pad_synth_freq_modulation_lfo_level_cb(int lev, int voice, int prog);
int set_voice_block_pad_synth_freq_modulation_env_num_cb(int env, int voice, int prog);
int set_voice_block_pad_synth_freq_modulation_env_level_cb(int lev, int voice, int prog);
int set_voice_block_pad_synth_amp_modulation_lfo_num_cb(int lfo, int voice, int prog);
int set_voice_block_pad_synth_amp_modulation_lfo_level_cb(int lev, int voice, int prog);
int set_voice_block_pad_synth_amp_modulation_env_num_cb(int env, int voice, int prog);
int set_voice_block_pad_synth_amp_modulation_env_level_cb(int lev, int voice, int prog);
// Wavetable is common to all voices of a program
int set_voice_block_pad_synth_quality_cb(int qlt, int prog);
int set_voice_block_pad_synth_base_note_cb(int bnot, int prog);
int set_voice_block_pad_synth_base_width_cb(int bwd, int prog);
int set_voice_block_pad_synth_shape_cb(int shp, int prog);
int set_voice_block_pad_synth_shape_cutoff_cb(int shcut, int prog);
int set_voice_block_pad_synth_harmonies_level_0_cb(int lev, int prog);
int set_voice_block_pad_synth_harmonies_level_1_cb(int lev, int prog);
int set_voice_block_pad_synth_harmonies_level_2_cb(int lev, int prog);
int set_voice_block_pad_synth_harmonies_level_3_cb(int lev, int prog);
int set_voice_block_pad_synth_harmonies_level_4_cb(int lev, int prog);
int set_voice_block_pad_synth_harmonies_level_5_cb(int lev, int prog);
int set_voice_block_pad_synth_harmonies_level_6_cb(int lev, int prog);
int set_voice_block_pad_synth_harmonies_level_7_cb(int lev, int prog);
int set_voice_block_pad_synth_harmonies_level_8_cb(int lev, int prog);
int set_voice_block_pad_synth_harmonies_level_9_cb(int lev, int prog);
int set_voice_block_pad_synth_harmonies_detune_cb(int hdet, int prog);


int set_voice_block_filter_1_frequency_cb(int freq, int voice, int prog);
int set_voice_block_filter_1_octave_cb(int oct, int voice, int prog);
int set_voice_block_filter_1_q_cb(int q, int voice, int prog);
int set_voice_block_filter_1_kbd_track_cb(int kbdt, int voice, int prog);
int set_voice_block_filter_1_band_cb(int bnd, int voice, int prog);
int set_voice_block_filter_1_freq_modulation_lfo_num_cb(int lfo, int voice, int prog);
int set_voice_block_filter_1_freq_modulation_lfo_level_cb(int lev, int voice, int prog);
int set_voice_block_filter_1_freq_modulation_env_num_cb(int env, int voice, int prog);
int set_voice_block_filter_1_freq_modulation_env_level_cb(int lev, int voice, int prog);

int set_voice_block_filter_2_frequency_cb(int freq, int voice, int prog);
int set_voice_block_filter_2_octave_cb(int oct, int voice, int prog);
int set_voice_block_filter_2_q_cb(int q, int voice, int prog);
int set_voice_block_filter_2_kbd_track_cb(int kbdt, int voice, int prog);
int set_voice_block_filter_2_band_cb(int bnd, int voice, int prog);
int set_voice_block_filter_2_freq_modulation_lfo_num_cb(int lfo, int voice, int prog);
int set_voice_block_filter_2_freq_modulation_lfo_level_cb(int lev, int voice, int prog);
int set_voice_block_filter_2_freq_modulation_env_num_cb(int env, int voice, int prog);
int set_voice_block_filter_2_freq_modulation_env_level_cb(int lev, int voice, int prog);

int set_voice_block_amp_ch_1_level_cb(int lev, int voice, int prog);
int set_voice_block_amp_ch_1_pan_cb(int pan, int voice, int prog);
int set_voice_block_amp_ch_1_pan_modulation_lfo_num_cb(int num, int voice, int prog);
int set_voice_block_amp_ch_1_pan_modulation_lfo_level_cb(int lev, int voice, int prog);
int set_voice_block_amp_fixed_levels_state_cb(bool en, int voice, int prog);

int set_voice_block_amp_ch_2_level_cb(int lev, int voice, int prog);
int set_voice_block_amp_ch_2_pan_cb(int pan, int voice, int prog);
int set_voice_block_amp_ch_2_pan_modulation_lfo_num_cb(int num, int voice, int prog);
int set_voice_block_amp_ch_2_pan_modulation_lfo_level_cb(int lev, int voice, int prog);


int set_voice_block_distortion_enabled_cb(bool enable, int voice, int prog);
int set_voice_block_distortion_auto_gain_enabled_cb(bool enable, int voice, int prog);

int set_voice_block_distortion_1_drive_cb(int drv, int voice, int prog);
int set_voice_block_distortion_1_range_cb(int rng, int voice, int prog);
int set_voice_block_distortion_1_blend_cb(int blnd, int voice, int prog);

int set_voice_block_distortion_2_drive_cb(int drv, int voice, int prog);
int set_voice_block_distortion_2_range_cb(int rng, int voice, int prog);
int set_voice_block_distortion_2_blend_cb(int blnd, int voice, int prog);

int set_voice_block_lfo_1_waveform_cb(int wavf, int voice, int prog);
int set_voice_block_lfo_1_rate_cb(int rate, int voice, int prog);
int set_voice_block_lfo_1_symmetry_cb(int sym, int voice, int prog);

int set_voice_block_lfo_2_waveform_cb(int wavf, int voice, int prog);
int set_voice_block_lfo_2_rate_cb(int rate, int voice, int prog);
int set_voice_block_lfo_2_symmetry_cb(int sym, int voice, int prog);

int set_voice_block_lfo_3_waveform_cb(int wavf, int voice, int prog);
int set_voice_block_lfo_3_rate_cb(int rate, int voice, int prog);
int set_voice_block_lfo_3_symmetry_cb(int sym, int voice, int prog);

int set_voice_block_lfo_4_waveform_cb(int wavf, int voice, int prog);
int set_voice_block_lfo_4_rate_cb(int rate, int voice, int prog);
int set_voice_block_lfo_4_symmetry_cb(int sym, int voice, int prog);

int set_voice_block_lfo_5_waveform_cb(int wavf, int voice, int prog);
int set_voice_block_lfo_5_rate_cb(int rate, int voice, int prog);
int set_voice_block_lfo_5_symmetry_cb(int sym, int voice, int prog);

int set_voice_block_env_1_attack_cb(int attck, int voice, int prog);
int set_voice_block_env_1_decay_cb(int dec, int voice, int prog);
int set_voice_block_env_1_sustain_cb(int sus, int voice, int prog);
int set_voice_block_env_1_release_cb(int rel, int voice, int prog);

int set_voice_block_env_2_attack_cb(int attck, int voice, int prog);
int set_voice_block_env_2_decay_cb(int dec, int voice, int prog);
int set_voice_block_env_2_sustain_cb(int sus, int voice, int prog);
int set_voice_block_env_2_release_cb(int rel, int voice, int prog);

int set_voice_block_env_3_attack_cb(int attck, int voice, int prog);
int set_voice_block_env_3_decay_cb(int dec, int voice, int prog);
int set_voice_block_env_3_sustain_cb(int sus, int voice, int prog);
int set_voice_block_env_3_release_cb(int rel, int voice, int prog);

int set_voice_block_env_4_attack_cb(int attck, int voice, int prog);
int set_voice_block_env_4_decay_cb(int dec, int voice, int prog);
int set_voice_block_env_4_sustain_cb(int sus, int voice, int prog);
int set_voice_block_env_4_release_cb(int rel, int voice, int prog);

int set_voice_block_env_5_attack_cb(int attck, int voice, int prog);
int set_voice_block_env_5_decay_cb(int dec, int voice, int prog);
int set_voice_block_env_5_sustain_cb(int sus, int voice, int prog);
int set_voice_block_env_5_release_cb(int rel, int voice, int prog);
