/**
* @file		instrumentHammondAnalogSynth.cpp
*	@author		Nahum Budin
*	@date		12-8-2024
*	@version	1.0	Initial release
*					
*	@brief		Implements an Analog Synthesizer instrument.
*	
*	History:\n
*	
*/

#include "instrumentAnalogSynth.h"

InstrumentAnalogSynth::InstrumentAnalogSynth()
	: Instrument(_INSTRUMENT_NAME_ANALOG_SYNTH_STR_KEY, true, true, false)
{

	alsa_midi_sequencer_events_handler->set_instrument(this);
}

InstrumentAnalogSynth::~InstrumentAnalogSynth()
{
}

void InstrumentAnalogSynth::note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
}

void InstrumentAnalogSynth::note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
}

void InstrumentAnalogSynth::change_program_handler(uint8_t channel, uint8_t program)
{
}

void InstrumentAnalogSynth::channel_pressure_handler(uint8_t channel, uint8_t val)
{
}

void InstrumentAnalogSynth::controller_event_handler(uint8_t channel, uint8_t num, uint8_t val)
{
}

void InstrumentAnalogSynth::pitch_bend_handler(uint8_t channel, int pitch)
{
}

void InstrumentAnalogSynth::sysex_handler(uint8_t *message, int len)
{
}