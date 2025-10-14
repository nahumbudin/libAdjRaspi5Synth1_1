/**
* @file		instrumentHammondAnalogSynth.cpp
*	@author		Nahum Budin
*	@date		24-09-2025
*	@version	1.1	
*					1. Adding a pointer to a AdjSynth object
*					
*	@brief		Implements an Analog Synthesizer instrument.
*	
*	History:\n
*		version 1.0		12-8-2024: First version
*	
*/

#include "instrumentAnalogSynth.h"
#include "../AdjSynth/adjSynth.h"

InstrumentAnalogSynth::InstrumentAnalogSynth(AdjSynth *adj_synth)
	: Instrument(_INSTRUMENT_NAME_ANALOG_SYNTH_STR_KEY, true, true, false, 
				 NULL, NULL, adj_synth)
{
	adjheart_synth = adj_synth;
	alsa_midi_sequencer_events_handler->set_instrument(this);

	active_settings_params->name = "Analog Synth Params";
	active_settings_params->settings_type = _ADJ_SYNTH_PRESET_PARAMS;
	active_settings_params->version = instrument_settings->get_settings_version();

	set_default_settings_parameters(active_settings_params, 0);
}

InstrumentAnalogSynth::~InstrumentAnalogSynth()
{
}

void InstrumentAnalogSynth::note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
	AdjSynth::get_instance()->midi_play_note_on(channel, note, velocity);
}

void InstrumentAnalogSynth::note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
	AdjSynth::get_instance()->midi_play_note_off(channel, note, 0);
}

void InstrumentAnalogSynth::change_program_handler(uint8_t channel, uint8_t program)
{
}

void InstrumentAnalogSynth::channel_pressure_handler(uint8_t channel, uint8_t val)
{
}

void InstrumentAnalogSynth::controller_event_handler(uint8_t channel, uint8_t num, uint8_t val)
{
	// All Notes/Sounds Off handler (ignore channel) TODO: channel
	if ((num == _MIDI_ALL_SOUNDS_OFF) || (num == _MIDI_ALL_NOTES_OFF))
	{
		for (int v = 0; v < _SYNTH_MAX_NUM_OF_VOICES; v++)
		{
			AdjSynth::get_instance()->synth_voice[v]->audio_voice->set_inactive();
			AdjSynth::get_instance()->synth_voice[v]->audio_voice->reset_wait_for_not_active();
		}
	}
}

void InstrumentAnalogSynth::pitch_bend_handler(uint8_t channel, int pitch)
{
}

void InstrumentAnalogSynth::sysex_handler(uint8_t *message, int len)
{
}




