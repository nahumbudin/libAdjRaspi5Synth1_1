/**
* @file		instrumentHammondAnalogSynth.h
*	@author		Nahum Budin
*	@date		12-8-2024
*	@version	1.0	Initial release
*					
*	@brief		Implements an Analog Synthesizer instrument.
*	
*	History:\n
*	
*/

#pragma once

#include "instrument.h"


class InstrumentAnalogSynth : public Instrument
{
public:
	InstrumentAnalogSynth();

	~InstrumentAnalogSynth();
	
	void note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity);
	void note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity);
	void change_program_handler(uint8_t channel, uint8_t program);
	void channel_pressure_handler(uint8_t channel, uint8_t val);
	void controller_event_handler(uint8_t channel, uint8_t num, uint8_t val);
	void pitch_bend_handler(uint8_t channel, int pitch);
	void sysex_handler(uint8_t *message, int len);
	
};
