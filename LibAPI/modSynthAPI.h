/**
* @file			types.h
*	@author		Nahum Budin
*	@date		6-Jul-2024
*	@version	1.0
*	
*	@brief		Provides the synthesizer high level API
*	
*	Based on libAdjHeartModSynth_1.h Ver 1.3 9-Jan-2021
*/

#pragma once

#include <string>
#include <list>

#include "../LibAPI/types.h"
#include "../Settings/settings.h"

using namespace std;

class Instrument;

/**
*   @brief  Initializes the synthesizer engine.
*			Must be called first before any other call.
*   @param  none
*   @return 0 if done
*/
int mod_synth_init();

/**
*   @brief  Initializes the Bluetooth services.
*   @param  none
*   @return 0 if done
*/
int mod_synth_init_bt_services();

/**
*   @brief  Deinitializes the Bluetooth services.
*			terminate al BT related threads
*   @param  none
*   @return 0 if done
*/
int mod_synth_deinit_bt_services();

/**
*   @brief  Initializes the external MIDI interface service.
*   @param  int port_num  serial port number
*   @return 0 if done
*/
int mod_synth_init_ext_midi_services(int port_num);

/**
*   @brief  Deinitializes the external MIDI interface service.
*   @param  none
*   @return 0 if done
*/
int mod_synth_deinit_ext_midi_services();

/**
*   @brief  Initializes the MIDI streaming service.
*   @param  none
*   @return 0 if done
*/
int mod_synth_init_midi_services();

/**
*   @brief  Deinitializes the MIDI streaming service.
*   @param  none
*   @return 0 if done
*/
int mod_synth_deinit_midi_services();

/**
*   @brief  Closing and cleanup when application goes down.
*			Must be called whenever terminating the application.
*   @param  none
*   @return void
*/
void mod_synth_on_exit();	

/**
*   @brief  Returns the total (all cores) CPU utilization.
*   @param  none
*   @return int	the total (all cores) CPU utilization in precetages (0 to 100).
*/
int mod_synth_get_cpu_utilization();

/**
*   @brief  Adds an active instrument.
*   @param  string	instrument name string
*   @param	pointer to the instrument instance
*   @return 0 if done
*/
int mod_synth_add_module(string ins_name, Instrument *instrument=NULL);

/**
*   @brief  Removes an active instrument.
*   @param  string	instrument name string
*   @return 0 if done
*/
int mod_synth_remove_module(string ins_name);


/**
*   @brief  Initiates an amplifier-mixer related event with integer value (affects all voices).
*			All available parameters values are defined in LibAPI/synthesizer.h 
*   @param  int ampid	target amp-mixer: _AMP_CH1_EVENT, _AMP_CH2_EVENT
*	@param	int eventid	specific event code:\n
*				_AMP_LEVEL, _AMP_PAN, _AMP_PAN_MOD_LFO, _AMP_PAN_MOD_LFO_LEVEL\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_AMP_LEVEL: 0-100;\n
*				_AMP_PAN: 0-100; 0 -> Left   50 -> Mid   100- -> Right\n	
*				_AMP_PAN_MOD_LFO:\n
\verbatim
	_LFO_NONE, _LFO_1, _LFO_2, _LFO_3, _LFO_4, _LFO_5,
	_LFO_1_DELAYED_500MS, _LFO_2_DELAYED_500MS, _LFO_3_DELAYED_500MS, 
	_LFO_4_DELAYED_500MS, _LFO_5_DELAYED_500MS,_LFO_1_DELAYED_1000MS, 
	_LFO_2_DELAYED_1000MS, LFO_3_DELAYED_1000MS, _LFO_3_DELAYED_1000MS, 
	_LFO_4_DELAYED_1000MS, _LFO_5_DELAYED_1000MS, _LFO_1_DELAYED_1500MS, 
	_LFO_2_DELAYED_1500MS, _LFO_3_DELAYED_1500MS, _LFO_4_DELAYED_1500MS,
	_LFO_5_DELAYED_1500MS, _LFO_1_DELAYED_2000MS, _LFO_2_DELAYED_2000MS,
	_LFO_3_DELAYED_2000MS, _LFO_4_DELAYED_2000MS, _LFO_5_DELAYED_2000MS
\endverbatim
*				_AMP_PAN_MOD_LFO_LEVEL: 0-100\n
*
*   @return 0
*/
int mod_synth_amp_event(int ampid, int eventid, int val);

/**
*   @brief Initiates an audio related event with integer value.
*   All available parameters values are defined in LibAPI/synthesizer.h 
*   @param int aouid target audio : _AUDIO_EVENT_1 
*   @param int eventid specific event code :\n 
*		_AUDIO_JACK_MODE, _AUDIO_SAMPLE_RATE, _AUDIO_BLOCK_SIZE\n 
*	@param int val event parameter value(must be used with the relevant event id) :\n 
*		_AUDIO_JACK_MODE : _JACK_MODE_MANUAL, _JACK_MODE_AUTO\n
*		_AUDIO_JACK_SAMPLE_RATE : _JACK_SAMPLE_RATE_44, _JACK_SAMPLE_RATE_48\n
*		_AUDIO_JACK_BLOCK_SIZE : _JACK_BLOCK_SIZE_256, _JACK_BLOCK_SIZE_512, _JACK_BLOCK_SIZE_1024\n
*/

int mod_synth_audio_event_int(int audid, int eventid, int val);

/**
*   @brief Initiates an audio related event with boolean value.
*   All available parameters values are defined in defs.h 
*   @param int aouid target audio : _AUDIO_EVENT_1 
*   @param int eventid specific event code :\n 
*		_AUDIO_JACK_AUTO_START, _AUDIO_JACK_AUTO_CONNECT\n 
*	@param int val event parameter value(must be used with the relevant event id) :\n 
*		_AUDIO_JACK_AUTO_START : _JACK_AUTO_START_DIS, _JACK_AUTO_START_EN\n
*		_AUDIO_JACK_AUTO_CONNECTT : _JACK_AUTO_CONNECT_DIS, _JACK_AUTO_CONNECT_EN\n
*/
int mod_synth_audio_event_bool(int audid, int eventid, bool val);

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
*
*   @return 0
*/
int mod_synth_distortion_event_int(int distid, int eventid, int val);

/**
*   @brief  Initiates a distortion effect related event with boolean value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int distid	target distortion: _DISTORTION_1_EVENT, _DISTORTION_2_EVENT
*	@param	int eventid	specific event code:\n
*				_DISTORTION_ENABLE, _DISTORTION_AUTO_GAIN\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_DISTORTION_ENABLE, _DISTORTION_AUTO_GAIN: true - enabled; false - disabled\n
*   @return 0
*/
int mod_synth_distortion_event_bool(int distid, int eventid, bool val);


/**
*   @brief  Initiates a 10 bands equilizer related event with integer value (affects all voices).
*			All available parameters values are defined in LibAPI/synthesizer.h
*   @param  int beqid	target equilizer: _BAND_EQUALIZER_EVENT
*	@param	int eventid	specific event code:\n
*				_BAND_EQUALIZER_BAND_31_LEVEL, _BAND_EQUALIZER_BAND_62_LEVEL\n
*				_BAND_EQUALIZER_BAND_125_LEVEL, _BAND_EQUALIZER_BAND_250_LEVEL\n
*				_BAND_EQUALIZER_BAND_500_LEVEL, _BAND_EQUALIZER_BAND_1K_LEVEL\n
*				_BAND_EQUALIZER_BAND_2K_LEVEL, _BAND_EQUALIZER_BAND_4K_LEVEL\n
*				_BAND_EQUALIZER_BAND_8K_LEVEL, _BAND_EQUALIZER_BAND_16K_LEVEL\n
*				_BAND_EQUILIZER_PRESET, _BAND_EQUALIZER_EVENT\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_BAND_EQUALIZER_BAND_xxx_LEVEL: 0-40; 0 -> -20db   40 -> +20db\n
*				_BAND_EQUILIZER_PRESET: not implemented\n
*				_BAND_EQUALIZER_EVENT: 1;
*
*   @return 0
*/
int mod_synth_band_equilizer_event(int beqid, int eventid, int val);

/**
*   @brief  Initiates a filter related event with integer value (affects all voices).
*			All available parameters values are defined in LibAPI/synthesizer.h
*   @param  int filtid	target filter: _FILTER_1_EVENT, _FILTER_2_EVENT
*	@param	int eventid	specific event code:\n
*				_FILTER_FREQ, _FILTER_OCT, _FILTER_Q\n
*				_FILTER_KBD_TRACK, _FILTER_BAND\n
*				_FILTER_FREQ_MOD_LFO, _FILTER_FREQ_MOD_LFO_LEVEL\n
*				_FILTER_FREQ_MOD_ENV, _FILTER_FREQ_MOD_ENV_LEVEL\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_FILTER_FREQ: 0-100; 0->getFilterMinCenterFreq(); 100-> getFilterMaxCenterFreq()\n
*				_FILTER_OCT: 0-100; 0->0.f; 100->6.9999f\n
*				_FILTER_Q: 0-100: 0->getFilterMinQ();  100->getFilterMaxQ()\n
*				_FILTER_KBD_TRACK: 0-100\n				
*				_FILTER_BAND:\n
\verbatim
_FILTER_BAND_LPF, _FILTER_BAND_HPF, _FILTER_BAND_BPF, _FILTER_BAND_PASS_ALL\n
\endverbatim
*				FILTER_FREQ_MOD_LFO:\n
\verbatim
	_LFO_NONE, _LFO_1, _LFO_2, _LFO_3, _LFO_4, _LFO_5,
	_LFO_1_DELAYED_500MS, _LFO_2_DELAYED_500MS, _LFO_3_DELAYED_500MS, 
	_LFO_4_DELAYED_500MS, _LFO_5_DELAYED_500MS,_LFO_1_DELAYED_1000MS, 
	_LFO_2_DELAYED_1000MS, LFO_3_DELAYED_1000MS, _LFO_3_DELAYED_1000MS, 
	_LFO_4_DELAYED_1000MS, _LFO_5_DELAYED_1000MS, _LFO_1_DELAYED_1500MS, 
	_LFO_2_DELAYED_1500MS, _LFO_3_DELAYED_1500MS, _LFO_4_DELAYED_1500MS,
	_LFO_5_DELAYED_1500MS, _LFO_1_DELAYED_2000MS, _LFO_2_DELAYED_2000MS,
	_LFO_3_DELAYED_2000MS, _LFO_4_DELAYED_2000MS, _LFO_5_DELAYED_2000MS
\endverbatim
*				_FILTER_FREQ_MOD_LFO_LEVEL: 0-100\n
*				_FILTER_FREQ_MOD_ENV:\n
\verbatim
	_ENV_NONE, _ENV_1, _ENV_2, _ENV_3, _ENV_4, _ENV_5
\endverbatim
*				_FILTER_FREQ_MOD_ENV_LEVEL: 0-100\n
*
*   @return 0
*/
int mod_synth_filter_event(int filtid, int eventid, int val);

/**
*   @brief  Initiates keyboard related event with integer value (affects all voices).
*			All available parameters values are defined in LibAPI/synthesizer.h
*   @param  int kbid	target keyboard: _KBD_1_EVENT
*	@param	int eventid	specific event code:\n
*				_KBD_PORTAMENTO_LEVEL, _KBD_SENSITIVITY_LEVEL, _KBD_LOW_SENSITIVITY_LEVEL\n
*				_KBD_SPLIT_POINT\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_KBD_PORTAMENTO_LEVEL: 1-100\n
*				_KBD_SENSITIVITY_LEVEL: 0-100\n
*				_KBD_LOW_SENSITIVITY_LEVEL: 0-100\n
*				_KBD_SPLIT_POINT: _KBD_SPLIT_POINT_NONE, _KBD_SPLIT_POINT_C2, _KBD_SPLIT_POINT_C3\n
*				_KBD_SPLIT_POINT_C4, _KBD_SPLIT_POINT_C5\n
*
*   @return 0
*/
int mod_synth_kbd_event_int(int kbid, int eventid, int val);


/**
*   @brief  Initiates a Karplus-Strong-String generator related event with integer value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int karpid	target generator: _KARPLUS_1_EVENT
*	@param	int eventid	specific event code:\n
*				_KARPLUS_STRONG_EXCITATION_WAVEFORM\n
*				_KARPLUS_STRONG_STRING_DUMP_CALC_MODE\n
*				_KARPLUS_STRONG_STRING_DAMPING, _KARPLUS_STRONG_STRING_DAMPING_VARIATION\n
*				_KARPLUS_STRONG_ON_DECAY, _KARPLUS_STRONG_OFF_DECAY\n
*				_KARPLUS_STRONG_EXCITATION_WAVEFORM_VARIATIONS\n
*				_KARPLUS_STRONG_SEND_1, _KARPLUS_STRONG_SEND_2
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_KARPLUS_STRONG_EXCITATION_WAVEFORM:\n
\verbatim
	_KARPLUS_STRONG_EXCITATION_WHITE_NOISE, _KARPLUS_STRONG_EXCITATION_PINK_NOISE
	_KARPLUS_STRONG_EXCITATION_BROWN_NOISE, _KARPLUS_STRONG_EXCITATION_SINECHIRP
	_KARPLUS_STRONG_EXCITATION_DECAYEDSINE, _KARPLUS_STRONG_EXCITATION_SAWTOOTH_WAVE
	_KARPLUS_STRONG_EXCITATION_SQUARE_WAVE\n
\endverbatim
*				_KARPLUS_STRONG_STRING_DUMP_CALC_MODE:\n
\verbatim
	_KARPLUS_STRONG_STRING_DAMPING_CALC_DIRECT, _KARPLUS_STRONG_STRING_DAMPING_CALC_MAGIC
\endverbatim
*				_KARPLUS_STRONG_STRING_DAMPING: 0-100;\n 
\verbatim
	0->getKarplusStrongMinStringDumping() 
	100 -> getKarplusStrongMaxStringDumping()
\endverbatim
*				_KARPLUS_STRONG_STRING_DAMPING_VARIATION: 0-100\n
\verbatim
	0 -> getKarplusStrongMinStringDumpingVariations() 
	100 -> getKarplusStrongMaxStringDumpingVariations()
\endverbatim
*				_KARPLUS_STRONG_ON_DECAY, _KARPLUS_STRONG_OFF_DECAY: 0-100:\n 
\verbatim
	0 -> getKarplusStrongMinDecay()  
	100 -> getKarplusStrongMaxDecay()
\endverbatim
*				_KARPLUS_STRONG_EXCITATION_WAVEFORM_VARIATIONS: 0-100\n
*				_KARPLUS_STRONG_SEND_1, _KARPLUS_STRONG_SEND_2: 0-100\n
*
*   @return void
*/
int mod_synth_karplus_event_int(int karlplusid, int eventid, int val);

/**
*   @brief  Initiates a Karplus-Strong-String generator related event with boolean value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int karpid	target generator: _KARPLUS_1_EVENT
*	@param	int eventid	specific event code:\n
*				_KARPLUS_STRONG_ENABLE\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				true, false\n
*				
*   @return 0
*/
int mod_synth_karplus_event_bool(int karlplusid, int eventid, int val);

/**
*   @brief  Initiates a modulator (LFO/ADSR) related event with integer value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int beqid	target modulator:
\verbatim
	_LFO_1_EVENT, _LFO_2_EVENT, _LFO_3_EVENT, _LFO_4_EVENT, _LFO_5_EVENT
	_ENV_1_EVENT, _ENV_2_EVENT, _ENV_3_EVENT, _ENV_4_EVENT, _ENV_5_EVENT
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
*   @return 0
*/
int mod_synth_modulator_event_int(int modid, int eventid, int val);

/**
*   @brief  Initiates a Morphed Sinus Oscilator related event with integer value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int msoid	target mso : _MSO_1_EVENT
*	@param	int eventid	specific event code:\n
*				_MSO_SEGMENT_A_POSITION, _MSO_SEGMENT_B_POSITION, _MSO_SEGMENT_C_POSITION\n
*				_MSO_SEGMENT_D_POSITION, _MSO_SEGMENT_E_POSITION, _MSO_SEGMENT_F_POSITION
*				_MSO_SYMETRY

*				_NOISE_AMP_MOD_LFO_LEVEL, _NOISE_AMP_MOD_ENV_LEVEL
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_MSO_SEGMENT_x_POSITION : 0 - _MSO_WAVEFORM_LENGTH - 1 (511)
*				_MSO_SYMETRY : 0 - 100

*				MSO_SEND_1, _NOISE_SEND_2: 0-100\n
*				MSO_AMP_MOD_LFO:\n
\verbatim
	_LFO_NONE, _LFO_1, _LFO_2, _LFO_3, _LFO_4, _LFO_5,
	_LFO_1_DELAYED_500MS, _LFO_2_DELAYED_500MS, _LFO_3_DELAYED_500MS, 
	_LFO_4_DELAYED_500MS, _LFO_5_DELAYED_500MS,_LFO_1_DELAYED_1000MS, 
	_LFO_2_DELAYED_1000MS, LFO_3_DELAYED_1000MS, _LFO_3_DELAYED_1000MS, 
	_LFO_4_DELAYED_1000MS, _LFO_5_DELAYED_1000MS, _LFO_1_DELAYED_1500MS, 
	_LFO_2_DELAYED_1500MS, _LFO_3_DELAYED_1500MS, _LFO_4_DELAYED_1500MS,
	_LFO_5_DELAYED_1500MS, _LFO_1_DELAYED_2000MS, _LFO_2_DELAYED_2000MS,
	_LFO_3_DELAYED_2000MS, _LFO_4_DELAYED_2000MS, _LFO_5_DELAYED_2000MS
\endverbatim
*				MSO_AMP_MOD_LFO_LEVEL: 0-100\n

*				MSO_AMP_MOD_ENV:\n
\verbatim
_ENV_NONE, _ENV_1, _ENV_2, _ENV_3, ENV_4, ENV_5
\endverbatim
*				_MSO_AMP_MOD_ENV_LEVEL: 0-100\n
*
*   @return void
*/
int mod_synthmso_event_int(int msoid, int eventid, int val);

/**
*   @brief  Initiates a Morphed Sinus Oscilator related event with boolean value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int msoid	target mso : _MSO_1_EVENT
*	@param	int eventid	specific event code:\n
*				_MSO_ENABLE
*	@param	boolean val event parameter value true / false:\n*
*	@param	_settings_params_t params	active settings params
*	@param	int program	program number
*   @return void
*/
int mod_synth_mso_event_bool(int msoid, int eventid, bool val);

/**
*   @brief  Initiates a Noise generator related event with integer value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int noiseid	target noise generator: _NOISE_1_EVENT
*	@param	int eventid	specific event code:\n
*				_NOISE_COLOR, _NOISE_SEND_1, _NOISE_SEND_2\n
*				_NOISE_AMP_MOD_LFO, _NOISE_AMP_MOD_ENV
*				_NOISE_AMP_MOD_LFO_LEVEL, _NOISE_AMP_MOD_ENV_LEVEL
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_NOISE_COLOR\n
\verbatim
	_WHITE_NOISE, _PINK_NOISE, _BROWN_NOISE
\endverbatim
*				_NOISE_SEND_1, _NOISE_SEND_2: 0-100\n
*				NOISE_AMP_MOD_LFO:\n
\verbatim
	_LFO_NONE, _LFO_1, _LFO_2, _LFO_3, _LFO_4, _LFO_5,
	_LFO_1_DELAYED_500MS, _LFO_2_DELAYED_500MS, _LFO_3_DELAYED_500MS, 
	_LFO_4_DELAYED_500MS, _LFO_5_DELAYED_500MS,_LFO_1_DELAYED_1000MS, 
	_LFO_2_DELAYED_1000MS, LFO_3_DELAYED_1000MS, _LFO_3_DELAYED_1000MS, 
	_LFO_4_DELAYED_1000MS, _LFO_5_DELAYED_1000MS, _LFO_1_DELAYED_1500MS, 
	_LFO_2_DELAYED_1500MS, _LFO_3_DELAYED_1500MS, _LFO_4_DELAYED_1500MS,
	_LFO_5_DELAYED_1500MS, _LFO_1_DELAYED_2000MS, _LFO_2_DELAYED_2000MS,
	_LFO_3_DELAYED_2000MS, _LFO_4_DELAYED_2000MS, _LFO_5_DELAYED_2000MS
\endverbatim
*				NOISE_AMP_MOD_LFO_LEVEL: 0-100\n
*				NOISE_AMP_MOD_ENV:\n
\verbatim
	_ENV_NONE, _ENV_1, _ENV_2, _ENV_3, _ENV_4, _ENV_5
\endverbatim
*				_NOISE_AMP_MOD_ENV_LEVEL: 0-100\n		
*
*	@param	int program	program number
*   @return 0
*/
int mod_synth_noise_event_int(int noiseid, int eventid, int val);

/**
*   @brief  Initiates a Noise generator related event with booleam value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int noiseid	target noise generator: _NOISE_1_EVENT
*	@param	int eventid	specific event code: _NOISE_ENABLE\n	
*
*	@param	bool val event parameter value  true or false (enable/disable)
*	@param	int program	program number
*   @return 0
*/
int mod_synth_noise_event_bool(int noiseid, int eventid, bool val);

/**
*   @brief  Initiates a PAD related event with integer value (affects all voices).
*			http://zynaddsubfx.sourceforge.net/doc/PADsynth/PADsynth.htm
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int padid	target PAD: _PAD_1_EVENT
*	@param	int eventid	specific event code:\n 
*				_PAD_DETUNE_OCTAVE, _PAD_DETUNE_SEMITONES, _PAD_DETUNE_CENTS\n 
*				_PAD_HARMONY_LEVEL_1, _PAD_HARMONY_LEVEL_2, _PAD_HARMONY_LEVEL_3\n
*				_PAD_HARMONY_LEVEL_4, _PAD_HARMONY_LEVEL_5, _PAD_HARMONY_LEVEL_6\n
*				_PAD_HARMONY_LEVEL_7, _PAD_HARMONY_LEVEL_8, _PAD_DETUNE\n
*				_PAD_FREQ_MOD_LFO, _PAD_FREQ_MOD_LFO_LEVEL\n
*				_PAD_AMP_MOD_LFO, _PAD_AMP_MOD_LFO_LEVEL\n
*				_PAD_QUALITY, _PAD_SHAPE\n
*				_PAD_BASE_NOTE, _PAD_BASE_WIDTH\n
*				_PAD_GENERATE, _PAD_ENABLE\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_PAD_DETUNE_OCTAVE: 0 to (getOscDetuneMaxOctave() - getOscDetuneMinOctave() + 1); 0->min octave-detune\n
*				_PAD_DETUNE_SEMITONES: 0 to (getOscDetuneMaxSemitone() - getOscDetuneMinSemitone() + 1); 0->min semitone-detune\n
*				_PAD_DETUNE_CENTS: 0 to (getOscDetuneMinCents() - getOscDetuneMaxCents() + 1); 0->min cents-detune in 0.25 steps\n
*				_PAD_FILTER_SEND_1, _PAD_FILTER_SEND_2: 0-100\n
*				_PAD_FREQ_MOD_LFO, _PAD_AMP_MOD_LFO:\n
\verbatim
	_LFO_NONE, _LFO_1, _LFO_2, _LFO_3, _LFO_4, _LFO_5,
	_LFO_1_DELAYED_500MS, _LFO_2_DELAYED_500MS, _LFO_3_DELAYED_500MS, 
	_LFO_4_DELAYED_500MS, _LFO_5_DELAYED_500MS,_LFO_1_DELAYED_1000MS, 
	_LFO_2_DELAYED_1000MS, LFO_3_DELAYED_1000MS, _LFO_3_DELAYED_1000MS, 
	_LFO_4_DELAYED_1000MS, _LFO_5_DELAYED_1000MS, _LFO_1_DELAYED_1500MS, 
	_LFO_2_DELAYED_1500MS, _LFO_3_DELAYED_1500MS, _LFO_4_DELAYED_1500MS,
	_LFO_5_DELAYED_1500MS, _LFO_1_DELAYED_2000MS, _LFO_2_DELAYED_2000MS,
	_LFO_3_DELAYED_2000MS, _LFO_4_DELAYED_2000MS, _LFO_5_DELAYED_2000MS
\endverbatim
*				_PAD_FREQ_MOD_LFO_LEVEL, _PAD_AMP_MOD_LFO_LEVEL: 0-100\n
*				_PAD_FREQ_MOD_ENV, _PAD_AMP_MOD_ENV:\n
\verbatim
_ENV_NONE, _ENV_1, _ENV_2, _ENV_3, ENV_4, _ENV_5
\endverbatim
*				_PAD_FREQ_MOD_ENV_LEVEL, _PAD_AMP_MOD_ENV_LEVEL: 0-100\n
*				_PAD_QUALITY:\n 
\verbatim
	_PAD_QUALITY_32K, _PAD_QUALITY_64K, _PAD_QUALITY_128K
	_PAD_QUALITY_256K, _PAD_QUALITY_512K, _PAD_QUALITY_1024K
\endverbatim
*				_PAD_SHAPE:\n
\verbatim
_PAD_SHAPE_RECTANGULAR, _PAD_SHAPE_GAUSSIAN, _PAD_SHAPE_DOUBLE_EXP
\endverbatim
*				_PAD_BASE_NOTE:\n
\verbatim
_PAD_BASE_NOTE_C2, _PAD_BASE_NOTE_G2, _PAD_BASE_NOTE_C3, _PAD_BASE_NOTE_G3
_PAD_BASE_NOTE_C4, _PAD_BASE_NOTE_G4, _PAD_BASE_NOTE_C5, _PAD_BASE_NOTE_G5
_PAD_BASE_NOTE_C6, _PAD_BASE_NOTE_G6
\endverbatim
*				_PAD_BASE_WIDTH: 0-100\n
*				_PAD_GENERATE: (any value)
*				_PAD_DETUNE: 0-100\n
*				_PAD_ENABLE: false, true
*	@param	_settings_params_t params	active settings params
*	@param int program	program number
*
*   @return 0
*/
int mod_synth_pad_event_int(int padid, int eventid, int val);

/**
*   @brief  Initiates a PAD related event with bool value (affects all voices).
*			http://zynaddsubfx.sourceforge.net/doc/PADsynth/PADsynth.htm
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int padid	target PAD: _PAD_1_EVENT
*	@param	int eventid	specific event code: _PAD_ENABLE\n 
*	@param	int val event parameter value true/fale:\n
*
*   @return 0
*/
int mod_synth_pad_event_bool(int padid, int eventid, bool val);

/**
*   @brief  Initiates a reverbration effect related event with integer value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int rvbid	target reverbration: _REVERB_EVENT
*	@param	int eventid	specific event code:\n
*				_REVERB_ROOM_SIZE, _REVERB_DAMP, _REVERB_WET, _REVERB_DRY\n
*				_REVERB_WIDTH, _REVERB_MODE, _REVERB_PRESET
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_REVERB_ROOM_SIZE: 10-98\n
*				_REVERB_DAMP: 0-100\n
*				_REVERB_WET: 0-100\n
*				_REVERB_DRY: 0-100\n
*				_REVERB_WIDTH: 1-100\n
*				_REVERB_MODE: 0-50
*				_REVERB_PRESET: enum sf_reverb_preset (defined in LibAPI/synthesizer.h)\n
*
*   @return 0
*/
int mod_synth_reverb_event_int(int revid, int eventid, int val);

/**
*   @brief  Initiates a reverbration effect related event with boolean value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int rvbid	target reverbration: _REVERB_EVENT
*	@param	int eventid	specific event code:\n
*				_REVERB_ENABLE, _REVERB3M_ENABLE\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_REVERB_ENABLE, _REVERB3M_ENABLE: true - enabled; false - disabled\n
*
*   @return 0
*/
int mod_synth_reverb_event_bool(int revid, int eventid, bool val);

/**
*   @brief  Initiates a VCO related event with integer value (affects all voices).
*			All available parameters values are defined in LibApi/synthesizer.h
*   @param  int vcoid	target VCO: _OSC_1_EVENT or _OSC_2_EVENT
*	@param	int eventid	specific event code:\n
*				_OSC_PARAM_WAVEFORM, _OSC_PWM_SYMMETRY\n
*				_OSC_DETUNE_OCTAVE, _OSC_DETUNE_SEMITONES, _OSC_DETUNE_CENTS\n
*				_OSC_FILTER_SEND_1, _OSC_FILTER_SEND_2\n
*				_OSC_UNISON_MODE, _OSC_UNISON_DISTORTION, _OSC_UNISON_DETUNE  (valid only for OSC 1)\n
*				_OSC_UNISON_LEVEL_1, _OSC_UNISON_LEVEL_2, _OSC_UNISON_LEVEL_3 (valid only for OSC 1)\n
*				_OSC_UNISON_LEVEL_4, _OSC_UNISON_LEVEL_5, _OSC_UNISON_LEVEL_6 (valid only for OSC 1)\n
*				_OSC_UNISON_LEVEL_7, _OSC_UNISON_LEVEL_8, _OSC_UNISON_LEVEL_9 (valid only for OSC 1)\n
*				_OSC_HAMMOND_PERCUSION_MODE									  (valid only for OSC 1)\n
*				_OSC_FREQ_MOD_LFO, _OSC_FREQ_MOD_LFO_LEVEL\n
*				_OSC_PWM_MOD_LFO, _OSC_PWM_MOD_LFO_LEVEL\n
*				_OSC_AMP_MOD_LFO, _OSC_AMP_MOD_LFO_LEVEL\n
*				_OSC_FREQ_MOD_ENV, _OSC_FREQ_MOD_ENV_LEVEL\n
*				_OSC_PWM_MOD_ENV, _OSC_PWM_MOD_ENV_LEVEL\n
*				_OSC_AMP_MOD_ENV, _OSC_AMP_MOD_ENV_LEVEL\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_OSC_PARAM_WAVEFORM:\n
\verbatim
				_OSC_WAVEFORM_SINE, _OSC_WAVEFORM_SQUARE, _OSC_WAVEFORM_PULSE
				_OSC_WAVEFORM_TRIANGLE, _OSC_WAVEFORM_SAMPHOLD
\endverbatim
*				_OSC_PWM_SYMMETRY: 5-95\n
*				_OSC_DETUNE_OCTAVE: 0 to (getOscDetuneMaxOctave() - getOscDetuneMinOctave() + 1); 0->min octave-detune\n
*				_OSC_DETUNE_SEMITONES: 0 to (getOscDetuneMaxSemitone() - getOscDetuneMinSemitone() + 1); 0->min semitone-detune\n
*				_OSC_DETUNE_CENTS: 0 to (getOscDetuneMinCents() - getOscDetuneMaxCents() + 1); 0->min cents-detune in 0.25 steps\n
*				_OSC_FILTER_SEND_1, _OSC_FILTER_SEND_2: 0-100\n
*				_OSC_UNISON_MODE:\n
\verbatim
	_OSC_UNISON_MODE_12345678, _OSC_UNISON_MODE_HAMMOND, _OSC_UNISON_MODE_OCTAVES
	_OSC_UNISON_MODE_C_CHORD, _OSC_UNISON_MODE_Cm_CHORD
	_OSC_UNISON_MODE_C7_CHORD, _OSC_UNISON_MODE_Cm7_CHORD
\endverbatim
*				_OSC_UNISON_DISTORTION, _OSC_UNISON_DETUNE: 0-99\n
*				_OSC_UNISON_LEVEL_1 - _OSC_UNISON_LEVEL_9: 0-100\n
*				_OSC_HAMMOND_PERCUSION_MODE:\n
\verbatim
	_HAMMOND_PERCUSION_MODE_OFF, _HAMMOND_PERCUSION_MODE_2ND_SOFT_SLOW,
	_HAMMOND_PERCUSION_MODE_2ND_SOFT_FAST, _HAMMOND_PERCUSION_MODE_2ND_NORM_SLOW,
	_HAMMOND_PERCUSION_MODE_2ND_NORM_FAST, _HAMMOND_PERCUSION_MODE_3RD_SOFT_SLOW,
	_HAMMOND_PERCUSION_MODE_3RD_SOFT_FAST, _HAMMOND_PERCUSION_MODE_3RD_NORM_SLOW,
	_HAMMOND_PERCUSION_MODE_3RD_NORM_FAST
\endverbatim
*				_OSC_FREQ_MOD_LFO, _OSC_PWM_MOD_LFO, _OSC_AMP_MOD_LFO:\n
\verbatim
	_LFO_NONE, _LFO_1, _LFO_2, _LFO_3, _LFO_4, _LFO_5,
	_LFO_1_DELAYED_500MS, _LFO_2_DELAYED_500MS, _LFO_3_DELAYED_500MS, 
	_LFO_4_DELAYED_500MS, _LFO_5_DELAYED_500MS,_LFO_1_DELAYED_1000MS, 
	_LFO_2_DELAYED_1000MS, LFO_3_DELAYED_1000MS, _LFO_3_DELAYED_1000MS, 
	_LFO_4_DELAYED_1000MS, _LFO_5_DELAYED_1000MS, _LFO_1_DELAYED_1500MS, 
	_LFO_2_DELAYED_1500MS, _LFO_3_DELAYED_1500MS, _LFO_4_DELAYED_1500MS,
	_LFO_5_DELAYED_1500MS, _LFO_1_DELAYED_2000MS, _LFO_2_DELAYED_2000MS,
	_LFO_3_DELAYED_2000MS, _LFO_4_DELAYED_2000MS, _LFO_5_DELAYED_2000MS
\endverbatim
*				_OSC_FREQ_MOD_LFO_LEVEL, _OSC_PWM_MOD_LFO_LEVEL, _OSC_AMP_MOD_LFO_LEVEL: 0-100\n
*				_OSC_FREQ_MOD_ENV, _OSC_PWM_MOD_ENV, _OSC_AMP_MOD_ENV:\n
\verbatim
	_ENV_NONE, _ENV_1, _ENV_2, _ENV_3, _ENV_4, _ENV_5
\endverbatim
*				_OSC_FREQ_MOD_ENV_LEVEL, _OSC_PWM_MOD_ENV_LEVEL, _OSC_AMP_MOD_ENV_LEVEL: 0-100\n
*
*   @return 0 is done.
*/
int mod_synth_vco_event_int(int vcoid, int eventid, int val);

/**
*   @brief  Initiates a VCO related event with boolean value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int vcoid	target VCO: _OSC_1_EVENT or _OSC_2_EVENT
*	@param	int eventid	specific event code:\n
*				_OSC_1_UNISON_SQUARE				(valid only for OSC 1)\n
*				_OSC_SYNC							(valid only for OSC 2)\n
*				_OSC_ENABLE
*	@param	bool val event parameter value  true or false (enable/disable)

*   @return void
*/
int mod_synth_vco_event_bool(int vcoid, int eventid, bool val);











/**
*   @brief  Register a callback function that initiates a full GUI update.
*			May handle multiple registrations.
*   @param  funcPtrVoidVoid ptr  a pointer to the callback function ( void func(void) )
*   @return void
*/
//void mod_synth_register_callback_update_ui(func_ptr_void_void_t ptr);



/**
*   @brief  Register a callback function that closes a module on the main window.
*   @param  func_ptr_void_en_modules_ids_t_t  a pointer to the callback function 
*													( void func(en_modules_ids_t) ) (module id)
*   @return void
*/
void mod_synth_register_callback_wrapper_close_module_pannel_id(func_ptr_void_en_modules_ids_t_t ptr);

/**
*   @brief  Register a callback function that closes a module on the main window.
*   @param  func_ptr_void_string_t  a pointer to the callback function 
*													( void func(std::string) ) (module name)
*   @return void
*/
void mod_synth_register_callback_wrapper_close_module_pannel_name(func_ptr_void_string_t ptr);

/**
*   @brief  Register a callback function that opens a module on the main window.
*   @param  func_ptr_void_string_t  a pointer to the callback function 
*													( void func(std::string) ) (module name)
*   @return void
*/
void mod_synth_register_callback_wrapper_open_module_pannel_name(func_ptr_void_string_t ptr);
