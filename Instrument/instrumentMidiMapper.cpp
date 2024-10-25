/**
*	@file		instrumenyMidiPlayer.cpp
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
*					- Block messages (e.g. block channel pressure or aftertouch/pressure commands).
*					- Split the keyboard (e.g note < N -> ch1; note >= N -> ch2)
*					- Set channel volume (e.g. scale chanel notes velocity by 0.5)
*
*	History:\n
*
*	Based on my Android TilTune Player Java Project 
*
*/

#include "instrumentMidiMapper.h"
#include "../LibAPI/midi.h"

InstrumentMidiMapper *InstrumentMidiMapper::instrument_midi_mapper_instance = NULL;

func_ptr_void_int_int_t InstrumentMidiMapper::midi_channel_volume_control_command_trap_callback_ptr = NULL;
func_ptr_void_int_int_t InstrumentMidiMapper::midi_channel_change_program_command_trap_callback_ptr = NULL;

InstrumentMidiMapper::InstrumentMidiMapper(AlsaMidiSysControl *alsa_control,
										   std::string *alsa_client_in_name)
	: Instrument(_INSTRUMENT_NAME_MIDI_MAPPER_STR_KEY, true, false, true,		// midi in, no audio out, midi out
				 alsa_control, alsa_client_in_name)
{

	keyboard_split_note = 64;

	for (int ch = 0; ch < 16; ch++)
	{
		midi_channel_volume_scale[ch] = 1.0f; // Future option TODO:?
		midi_channel_block_channel_volume_commands[ch] = false;
	}

	instrument_midi_mapper_instance = this;
	/* Midi mapper input handles all channels */
	this->alsa_midi_sequencer_events_handler->set_active_midi_channels(0xffff);
};

/**
*	@brief	Set a MIDI mixer channel volume
*	@param	int		channel 0-15
*	@param	int		scale 0-100
*	@return void
*/
void InstrumentMidiMapper::set_midi_channel_volume(int chan, int vol)
{
	uint8_t bytes[3];
	int status;
	
	if ((chan < 16) && (chan >= 0) && (vol <= 100) && (vol >= 0))
	{
		float vol_scale = (float)vol / 100.0f * 127.f;
		midi_channel_volume_scale[chan] = vol_scale;

		bytes[0] = _MIDI_EVENT_CONTROL_CHANGE + (chan & 0x0f);
		bytes[1] = _MIDI_EVENT_CHANNEL_VOLUME_BYTE_2;
		bytes[2] = (int)vol_scale;

		if ((status = snd_rawmidi_write(midiout, bytes, 3)) < 0)
		{
			printf("Problem writing midi mapper change channel volume to MIDI output: %s", snd_strerror(status));
		}
	}
}

/**
*	@brief	Returns a MIDI mixer channel volume
*	@param	int		channel 0-15
*	@return scale 0-100
*/
int InstrumentMidiMapper::get_midi_channel_volume(int chan)
{
	if ((chan < 16) && (chan >= 0))
	{
		return (int)(midi_channel_volume_scale[chan] / 127.f * 100);
	}
}

void InstrumentMidiMapper::enable_midi_channel_block_volume_commands(int chan)
{
	if ((chan < 16) && (chan >= 0))
	{
		midi_channel_block_channel_volume_commands[chan] = true;
	}
}

void InstrumentMidiMapper::disable_midi_channel_block_volume_commands(int chan)
{
	if ((chan < 16) && (chan >= 0))
	{
		midi_channel_block_channel_volume_commands[chan] = false;
	}
}

bool InstrumentMidiMapper::get_midi_channel_block_volume_commands_state(int chan)
{
	if ((chan < 16) && (chan >= 0))
	{
		return midi_channel_block_channel_volume_commands[chan];
	}
	else
	{
		return false;
	}
}

void InstrumentMidiMapper::set_keyboard_split_note(int note)
{
	if ((note < 128) && (note >= 0))
	{
		keyboard_split_note = note;
	}
}

int InstrumentMidiMapper::get_keyboard_split_note()
{
	return keyboard_split_note;
}

void InstrumentMidiMapper::register_midi_channel_volume_control_command_trap_callback(func_ptr_void_int_int_t ptr)
{
	midi_channel_volume_control_command_trap_callback_ptr = ptr;
}

void InstrumentMidiMapper::register_midi_channel_change_program_command_trap_callback(func_ptr_void_int_int_t ptr)
{
	midi_channel_change_program_command_trap_callback_ptr = ptr;
}

void InstrumentMidiMapper::note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
	uint8_t bytes[3];
	int status;

	bytes[0] = _MIDI_EVENT_NOTE_ON + (channel & 0x0f);
	bytes[1] = note;
	// bytes[2] = (int)(velocity * midi_channel_volume_scale[channel]);
	bytes[2] = velocity;
	

	if ((status = snd_rawmidi_write(midiout, bytes, 3)) < 0)
	{
		printf("Problem writing midi mapper note on data to MIDI output: %s", snd_strerror(status));
	}
}

void InstrumentMidiMapper::note_off_handler(uint8_t channel, uint8_t num, uint8_t val)
{
	uint8_t bytes[3];
	int status;

	bytes[0] = _MIDI_EVENT_NOTE_OFF + (channel & 0x0f);
	bytes[1] = num;
	bytes[2] = val;

	if ((status = snd_rawmidi_write(midiout, bytes, 3)) < 0)
	{
		printf("Problem writing midi mapper note off data to MIDI output: %s", snd_strerror(status));
	}
}

void InstrumentMidiMapper::change_program_handler(uint8_t channel, uint8_t program)
{
	uint8_t bytes[2];
	int status;

	bytes[0] = _MIDI_EVENT_PROGRAM_CHANGE + (channel & 0x0f);
	bytes[1] = program;

	if (midi_channel_change_program_command_trap_callback_ptr != NULL)
	{
		midi_channel_change_program_command_trap_callback_ptr(channel, program);
	}

	if ((status = snd_rawmidi_write(midiout, bytes, 2)) < 0)
	{
		printf("Problem writing midi mapper change program data to MIDI output: %s", snd_strerror(status));
	}
}

void InstrumentMidiMapper::channel_pressure_handler(uint8_t channel, uint8_t val)
{
	
}

void InstrumentMidiMapper::controller_event_handler(uint8_t channel, uint8_t num, uint8_t val)
{
	uint8_t bytes[3];
	int status;
	int scaled_volume;

	/* Check if channel volume control commands should be blocked. */
	if (num == _MIDI_EVENT_CHANNEL_VOLUME_BYTE_2)
	{
		/* Trap volume commands */
		if (midi_channel_volume_control_command_trap_callback_ptr != NULL)
		{
			scaled_volume = (int)((val * 100) / 127);
			midi_channel_volume_control_command_trap_callback_ptr(channel, scaled_volume);
		}
		
		if (midi_channel_block_channel_volume_commands[channel])
		{
			/* Block - drop the command */
			return;
		}
	}

	bytes[0] = _MIDI_EVENT_CONTROL_CHANGE + (channel & 0x0f);
	bytes[1] = num;
	bytes[2] = val;

	if ((status = snd_rawmidi_write(midiout, bytes, 3)) < 0)
	{
		printf("Problem writing midi mapper change control data to MIDI output: %s", snd_strerror(status));
	}
}

void InstrumentMidiMapper::pitch_bend_handler(uint8_t channel, int pitch)
{
	
}

void InstrumentMidiMapper::sysex_handler(uint8_t *message, int len)
{
	
}

void InstrumentMidiMapper::control_box_events_handler(int event_id, uint16_t event_value)
{
	
}
