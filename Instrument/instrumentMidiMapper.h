/**
*	@file		instrumenyMidiPlayer.h
*	@author		Nahum Budin
*	@date		23-Aug-2024
*	@version	1.0
*
*	@brief		MIDI mapper instrument.
*				It processes incoming MIDI events and generates control events for the synth. 
*				It can be used to filter or modify events prior to sending them to the synthesizer modules. 
*				In default state it is transparent and simply passes all MIDI events.
*				
*				Major capabilities:
*				
*					- Capture messages and activate callbacks (e.g. change program).
*					- Block messages (e.g. block channel pressure or aftertouch commands).
*					- Split the keyboard (e.g note < N -> ch1; note >= N -> ch2)
*					- Set channel volume (e.g. scale chanel notes velocity by 0.5)
*
*	History:\n
*
*	Based on my Android TilTune Player Java Project 
*
*/

#pragma once

#include "instrument.h"

using namespace std;

class InstrumentMidiMapper : public Instrument
{
  public:
	
	InstrumentMidiMapper(AlsaMidiSysControl *alsa_control = NULL,
						 string *alsa_client_in_name = NULL);
	~InstrumentMidiMapper();

	static InstrumentMidiMapper *get_instrument_midi_mapper_instance();

	void set_midi_channel_volume(int chan, int vol);
	int get_midi_channel_volume(int chan);

	void enable_midi_channel_block_volume_commands(int chan);
	void disable_midi_channel_block_volume_commands(int chan);
	bool get_midi_channel_block_volume_commands_state(int chan);

	void set_keyboard_split_note(int note);
	int get_keyboard_split_note();

	void register_midi_channel_volume_control_command_trap_callback(func_ptr_void_int_int_t ptr);
	void register_midi_channel_change_program_command_trap_callback(func_ptr_void_int_int_t ptr);
	

	void note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity);
	void note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity);
	void change_program_handler(uint8_t channel, uint8_t program);
	void channel_pressure_handler(uint8_t channel, uint8_t val);
	void controller_event_handler(uint8_t channel, uint8_t num, uint8_t val);
	void pitch_bend_handler(uint8_t channel, int pitch);
	void sysex_handler(uint8_t *message, int len);
	void control_box_events_handler(int event_id, uint16_t event_value);

  private:
	static InstrumentMidiMapper *instrument_midi_mapper_instance;

	float midi_channel_volume_scale[16];
	bool midi_channel_block_channel_volume_commands[16];
	int keyboard_split_note;

	static func_ptr_void_int_int_t midi_channel_volume_control_command_trap_callback_ptr;
	static func_ptr_void_int_int_t midi_channel_change_program_command_trap_callback_ptr;
};
