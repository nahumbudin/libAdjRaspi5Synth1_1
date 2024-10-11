/**
*	@file		modSynth.h
*	@author		Nahum Budin
*	@date		11-May-2024
*	@version	1.0
*
*	@brief		This is the main/top modular synthesizer libraray object.
*
*	History:\n
*
*	Based on libAdjHeartModSynth_2 library 
*
*/

#pragma once

#include <cstddef>
#include <cstdint>

#include "./AdjSynth/adjSynth.h"

#include "./AdjSynth/adjSynthPolyphony.h"

#include "./ALSA/alsaBtClientOutput.h"
#include "./ALSA/controlBoxClientAlsaOutput.h"
#include "./ALSA/controlBoxExtMidiInClientAlsaOutput.h"
#include "./ALSA/alsaMidiSystemControl.h"

#include "./Jack/jackConnections.h"

#include "./MIDI/midiExtInterface.h"

#include "./Settings/settings.h"

#include "./Patches/patches.h"

//#include "./Instrument/instrumentFluidSynth.h"
//#include "./Instrument/instrumentHammondOrgan.h"

//#include "./AdjSynth/adjSynth.h"

class InstrumentsManager;
class InstrumentFluidSynth;
class InstrumentHammondOrgan;
class InstrumentMidiPlayer;
class InstrumentMidiMapper;
class InstrumentControlBoxEventsHandler;

class AdjSynth;

class ModSynth
{
public:
	~ModSynth();
	static ModSynth *get_instance();

	int init();
	
	void on_exit();	
	
	void start_cheack_cpu_utilization_thread();
	void stop_cheack_cpu_utilization_thread();
	int mod_synth_get_cpu_utilization();
	
	int init_midi_ext_interface(int ser_port_num);
	int deinit_midi_ext_interface();


	AlsaBtClientOutput *get_bt_alsa_out();

	InstrumentFluidSynth *get_fluid_synth();
	InstrumentMidiPlayer *get_midi_player();
	InstrumentMidiMapper *get_midi_mapper();

	void note_on(uint8_t channel, uint8_t note, uint8_t velocity);
	void note_off(uint8_t channel, uint8_t note);
	void change_program(uint8_t channel, uint8_t program);
	void channel_pressure(uint8_t channel, uint8_t val);
	void controller_event(uint8_t channel, uint8_t num, uint8_t val);
	void pitch_bend(uint8_t channel, int pitch);

	AlsaMidiSysControl *alsa_midi_system_control;

	JackConnections *jack_connections;

	InstrumentControlBoxEventsHandler *control_box_events_handler;

	InstrumentsManager *instruments_manager;
	
	InstrumentMidiPlayer *midi_player;

	InstrumentMidiMapper *midi_mapper;

	PatchsHandler *patches_handler;
	
	AdjSynth *adj_synth;
	
	static int cpu_utilization;

  private:
	
	ModSynth();

	static ModSynth *mod_synth;

	AlsaBtClientOutput *bt_alsa_out;
	ControlBoxClientAlsaOutput *control_box_alsa_out;
	ControlBoxExtMidiInClientAlsaOutput *control_box_ext_midi_in_alsa_out;

	MidiExtInterface *midi_ext_interface;
	
	InstrumentFluidSynth *fluid_synth;
	InstrumentHammondOrgan *hammond_organ;

	pthread_t cheack_cpu_utilization_thread_id;	

};

// Thread checking cpu utilization
void *cheack_cpu_utilization_thread(void *arg);

