/**
* @file		libAdjRaspi5Synth_1_1.cpp
*	@author		Nahum Budin
*	@date		8-May-2024
*	@version	1.0
*
*
*	History:\n
*
*
*
*	@brief		A raspberry Pi multi-core polyphonic music synthesizer library api.
*
*	\mainpage AdjHeart Polyphonic Synthesizer
*
*	This library was writen to run over a Raspberry Pi 5B.\n
*	The librarry implements a modular synthesizer \n
*
*
*	It provides a full API supporting all features incluuding callback functions for updates and events initiation.
*
*/


//#include "libAdjRaspi5Synth_1.h"



#include "utils\log.h"
#include <stdio.h>
#include <string>

#include "modSynth.h"
#include "./Settings/settings.h"

#include "Bluetooth\rspiBluetoothServicesQueuesVer.h"

#include "MIDI/midiStream.h"

#include "./Instrument/instrumentsManager.h"
#include "./Instrument/instrumentControlBoxHandler.h"
#include "./Instrument/instrumentFluidSynth.h"
#include "./Instrument/instrumentMidiPlayer.h"
#include "./Instrument/instrumentMidiMapper.h"

#include "libAdjRaspi5Synth_1_1.h"

/******************************************************************
 *********************** ModSynth Management API **************************
 ******************************************************************/

func_ptr_void_int_t callback_ptr_message_id = NULL;

/** The main modular synthesizer object instance */
ModSynth* mod_synthesizer;

Settings *settings_manager;


/**
*   @brief  Initializes the synthesizer engine.
*			Must be called first before any other call.
*   @param  none
*   @return 0 if done
*/
int mod_synth_init()
{
	// Debuger
	FILELog::ReportingLevel() = FILELog::FromString("DEBUG4");
	FILE* pFile = fopen("AdjRaspi5Synth_1.log", "a");
	Output2FILE::Stream() = pFile;
	FILE_LOG(logINFO) << "...";

	mod_synthesizer = ModSynth::get_instance(); // new ModSynth();

	return 0;
}

int mod_synth_start_audio()
{

	return 0;
}

int mod_synth_stop_audio()
{

	return 0;
}

int mod_synth_init_bt_services()
{
	/* Inilize */
	Raspi3BluetoothQ* bluetooth = Raspi3BluetoothQ::get_instance();
	bluetooth->start_bt_main_thread();
	mod_synthesizer->get_bt_alsa_out()->start_bt_alsa_out_thread();
	return 0;
}

int mod_synth_deinit_bt_services()
{
	Raspi3BluetoothQ::stop_bt_main_thread();
	mod_synthesizer->get_bt_alsa_out()->stop_bt_alsa_out_thread();
	return 0;
}

/******************************************************************
 *********************** Midi Services API **************************
 ******************************************************************/

int mod_synth_init_ext_midi_services(int port_num)
{
	
	mod_synthesizer->init_midi_ext_interface(_MIDI_EXT_INTERFACE_SERIAL_PORT_NUM);

	return 0;
}

int mod_synth_deinit_ext_midi_services()
{
	
	return 0;
}

int mod_synth_init_midi_services()
{
	// Allocate midi blocks data memory pool
	allocate_midi_stream_messages_memory_pool(_MAX_MIDI_STREAM_MESSAGES_POOL_SIZE);
	allocate_raw_data_mssgs_memory_pool(_MAX_RAWDATA_MSSGS_POOL_SIZE);
	MidiStream::start_thread();


	return 0;
}

void mod_synth_on_exit()
{
	//	jackExit(); ???
	mod_synth_deinit_bt_services();
	mod_synth_deinit_midi_services();
	
	
	ModSynth::get_instance()->get_fluid_synth()->get_fluid_synth_interface()->deinitialize_fluid_synthesizer();

	ModSynth::get_instance()->adj_synth->audio_manager->stop_audio_service();
}


int mod_synth_deinit_midi_services()
{
	MidiStream::stop_thread();

	return 0;
}

int mod_synth_get_cpu_utilization() 
{ 
	return ModSynth::cpu_utilization; 
}

std::list<std::string> mod_synth_get_midi_input_client_name_strings()
{
	std::list<std::string> names_list;
	std::string name;

	int num_of_names = mod_synthesizer->alsa_midi_system_control->get_num_of_input_midi_clients();

	for (int i = 0; i < num_of_names; i++)
	{
		mod_synthesizer->alsa_midi_system_control->get_midi_input_client_name_string(i, &name, true); // replace virtual clients names
		names_list.push_back(name);
	}

	return names_list;
}

int mod_synth_connect_midi_clients(std::string instrument_name, int in_client, int in_port, bool connect, bool use_index)
{
	std::string client_name;
	int client_num;
	bool client_found = false;
	int num_of_output_clients = mod_synthesizer->alsa_midi_system_control->get_num_of_output_midi_clients();

	if (num_of_output_clients > 0)
	{
		if (use_index)
		{
			for (client_num = 0; client_num < num_of_output_clients; client_num++)
			{
				mod_synthesizer->alsa_midi_system_control->get_midi_output_client_name_string(client_num, &client_name);
				if (client_name.find(instrument_name) != std::string::npos)
				{
					client_found = true;
					break;
				}
			}
		}
		else
		{
			client_num = mod_synthesizer->alsa_midi_system_control->get_midi_client_connection_num(instrument_name);
			client_found = true;
		}

		if (client_found)
		{			
			if (connect)
			{
				return mod_synthesizer->alsa_midi_system_control->connect_midi_clients(in_client, in_port, client_num, 0, use_index);
			}
			else
			{
				return mod_synthesizer->alsa_midi_system_control->disconnect_midi_clients(in_client, in_port, client_num, 0, use_index);
			}
		}
	}

	return -1;
}

int mod_synth_refresh_alsa_clients_data()
{
	int res = mod_synthesizer->alsa_midi_system_control->refresh_alsa_clients_data();

	/* Tests */
	//int num = mod_synthesizer->alsa_midi_system_control->get_midi_client_connection_num(_INSTRUMENT_NAME_FLUID_SYNTH_STR_KEY);
	//std::list<int> con_to;
	//std::list<std::string> names_list;

	// mod_synthesizer->alsa_midi_system_control->get_midi_client_connected_to_clients_numbers_list(num, &con_to);
	//mod_synthesizer->alsa_midi_system_control->get_midi_client_connected_to_clients_names_list(num, true, &names_list);
	/* Teset End*/

	return res;
}

int mod_synth_get_midi_client_connection_num(std::string instrument_name)
{
	return mod_synthesizer->alsa_midi_system_control->get_midi_client_connection_num(instrument_name);
}

int mod_synth_get_midi_input_client_id(string name, bool replace)
{
	return mod_synthesizer->alsa_midi_system_control->get_midi_input_client_id(name, replace);
}

int mod_synth_get_midi_client_connected_to_clients_names_list(int cln_num, bool replace,
															  std::list<std::string> *cln_lst)
{
	return mod_synthesizer->alsa_midi_system_control->get_midi_client_connected_to_clients_names_list(cln_num, replace,
																									  cln_lst);
}

int mod_synth_get_midi_client_connected_to_clients_numbers_list(int cln_num, std::list<int> *cln_lst)
{
	return mod_synthesizer->alsa_midi_system_control->get_midi_client_connected_to_clients_numbers_list(cln_num, cln_lst);
}

int mod_synth_set_instrument_active_midi_channels(std::string instrument_name, uint16_t active_midi_ch_mask)
{
	Instrument *instrument;

	instrument = mod_synthesizer->instruments_manager->get_instrument(instrument_name);
	if (instrument != NULL)
	{
		instrument->set_active_midi_channels(active_midi_ch_mask);

		return 0;
	}

	return -1;
}

uint16_t mod_synth_get_instrument_active_midi_channels(std::string instrument_name)
{
	Instrument *instrument;

	instrument = mod_synthesizer->instruments_manager->get_instrument(instrument_name);
	if (instrument != NULL)
	{
		return instrument->get_active_midi_channels();
	}

	return 0;
}

/******************************************************************
 *********************** JackAudio API **************************
 ******************************************************************/


int mod_synth_get_num_of_output_jack_clients()
{
	return mod_synthesizer->jack_connections->get_num_of_output_jack_clients();
}

std::string mod_synth_get_jack_output_client_name(int cln)
{
	return mod_synthesizer->jack_connections->get_jack_output_client_name(cln);
}

int mod_synth_get_num_of_jack_output_client_ports(int cln)
{
	return mod_synthesizer->jack_connections->get_num_of_jack_output_client_ports(cln);
}

std::string mod_synth_get_jack_output_client_port_name(int cln, int prt)
{
	return mod_synthesizer->jack_connections->get_jack_output_client_port_name(cln, prt);
}

int mod_synth_get_num_of_input_jack_clients()
{
	return mod_synthesizer->jack_connections->get_num_of_input_jack_clients();
}

std::string mod_synth_get_jack_input_client_name(int cln)
{
	return mod_synthesizer->jack_connections->get_jack_input_client_name(cln);
}

int mod_synth_get_num_of_jack_input_client_ports(int cln)
{
	return mod_synthesizer->jack_connections->get_num_of_jack_input_client_ports(cln);
}

std::string mod_synth_get_jack_input_client_port_name(int cln, int prt)
{
	return mod_synthesizer->jack_connections->get_jack_input_client_port_name(cln, prt);
}

int mod_synth_connect_jack_connection(
	std::string in_client_name,
	std::string in_client_port_name,
	std::string out_client_name,
	std::string out_client_port_name,
	bool connect)
{
	int res;
	
	if (connect)
	{
		res = mod_synthesizer->jack_connections->connect_jack_connection(in_client_name,
																		  in_client_port_name, 
																		  out_client_name, 
																		  out_client_port_name);
	}
	else
	{
		res = mod_synthesizer->jack_connections->disconnect_jack_connection(in_client_name,
																		  in_client_port_name,
																		  out_client_name,
																		  out_client_port_name);
	}
	
	return res;
}

int mod_synth_refresh_jack_clients_data()
{
	return mod_synthesizer->jack_connections->refresh_jack_clients_data();
}

int mod_synth_get_jack_input_connections(std::string out_client_name, 
									std::string out_client_port_name,
									std::list<std::string> *outputs_list)
{
	return mod_synthesizer->jack_connections->get_connected_jack_input_connections(out_client_name,
											out_client_port_name,
											outputs_list);
}



/******************************************************************
 *********************** Modules Management API *******************
 ******************************************************************/

int mod_synth_add_module(string ins_name, Instrument *instrument)
{
	mod_synthesizer->instruments_manager->add_instrument(ins_name, instrument);

	return 0;
}

int mod_synth_remove_module(string ins_name)
{
	int client_number;
	list<int> connected_to_midi_clients_numbers_list;
	
	mod_synthesizer->instruments_manager->remove_instrument(ins_name);
	/* Disconnect midi in connections */
	mod_synth_refresh_alsa_clients_data();
	mod_synth_refresh_jack_clients_data();
	/* Get module Midi Input connections */
	client_number = mod_synth_get_midi_client_connection_num(ins_name);
	connected_to_midi_clients_numbers_list.clear();
	mod_synth_get_midi_client_connected_to_clients_numbers_list(client_number,
		&connected_to_midi_clients_numbers_list);

	for (int client_num : connected_to_midi_clients_numbers_list)
	{
		mod_synth_connect_midi_clients(ins_name, client_num, 0, false, false); // disconnect, use client num
	}
	

	return 0;
}


void mod_synth_register_callback_wrapper_close_module_pannel_id(func_ptr_void_en_modules_ids_t_t ptr)
{
	mod_synthesizer->instruments_manager->register_callback_close_module_pannel_id(ptr);
}

void mod_synth_register_callback_wrapper_close_module_pannel_name(func_ptr_void_string_t ptr)
{
	mod_synthesizer->instruments_manager->register_callback_close_module_pannel_name(ptr);
}

void mod_synth_register_callback_wrapper_open_module_pannel_name(func_ptr_void_string_t ptr)
{
	mod_synthesizer->instruments_manager->register_callback_open_module_pannel_name(ptr);
}


/******************************************************************
 *********************** AdjSynth API **************************
 ******************************************************************/


void mod_synth_set_active_sketch(int ask)
{
	AdjSynth::get_instance()->set_active_sketch(ask);
}

int mod_synth_get_active_sketch()
{
	return AdjSynth::get_instance()->get_active_sketch();
}

int mod_synth_amp_event(int ampid, int eventid, int val)
{
	return AdjSynth::get_instance()->amp_event(ampid, eventid, val, 
		AdjSynth::get_instance()->get_active_patch_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_audio_event_int(int audid, int eventid, int val)
{
	return AdjSynth::get_instance()->audio_event_int(audid, eventid, val,
		AdjSynth::get_instance()->get_active_patch_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_audio_event_bool(int audid, int eventid, bool val)
{
	return AdjSynth::get_instance()->audio_event_bool(audid, eventid, val, 
		AdjSynth::get_instance()->get_active_patch_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_distortion_event_int(int distid, int eventid, int val)
{
	return AdjSynth::get_instance()->distortion_event_int(distid, eventid, val, 
		AdjSynth::get_instance()->get_active_patch_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_distortion_event_bool(int distid, int eventid, bool val)
{
	return AdjSynth::get_instance()->distortion_event_bool(distid, eventid, val, 
		AdjSynth::get_instance()->get_active_patch_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_band_equilizer_event(int beqid, int eventid, int val)
{
	return AdjSynth::get_instance()->band_equilizer_event(beqid, eventid, val, 
		AdjSynth::get_instance()->get_active_patch_params());
}

int mod_synth_filter_event(int filtid, int eventid, int val)
{
	return AdjSynth::get_instance()->filter_event(filtid, eventid, val, 
		AdjSynth::get_instance()->get_active_patch_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_kbd_event_int(int kbid, int eventid, int val)
{
	return AdjSynth::get_instance()->kbd_event_int(kbid, eventid, val, 
		AdjSynth::get_instance()->get_active_patch_params());
}

int mod_synth_karplus_event_int(int karlplusid, int eventid, int val)
{
	return AdjSynth::get_instance()->karplus_event_int(karlplusid,
		eventid,
		val, 
		AdjSynth::get_instance()->get_active_patch_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_karplus_event_bool(int karlplusid, int eventid, bool val)
{
	return AdjSynth::get_instance()->karplus_event_bool(karlplusid,
		eventid,
		val, 
		AdjSynth::get_instance()->get_active_patch_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_modulator_event(int modid, int eventid, int val)
{
	return AdjSynth::get_instance()->modulator_event_int(modid,
		eventid,
		val, 
		AdjSynth::get_instance()->get_active_patch_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synthmso_event_int(int msoid, int eventid, int val)
{
	return AdjSynth::get_instance()->mso_event_int(msoid,
		eventid,
		val, 
		AdjSynth::get_instance()->get_active_patch_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_mso_event_bool(int msoid, int eventid, bool val)
{
	return AdjSynth::get_instance()->mso_event_bool(msoid,
		eventid,
		val, 
		AdjSynth::get_instance()->get_active_patch_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_noise_event_int(int noiseid, int eventid, int val)
{
	return AdjSynth::get_instance()->noise_event_int(noiseid,
		eventid,
		val, 
		AdjSynth::get_instance()->get_active_patch_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_noise_event_bool(int noiseid, int eventid, bool val)
{
	return AdjSynth::get_instance()->noise_event_bool(noiseid,
		eventid,
		val, 
		AdjSynth::get_instance()->get_active_patch_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_pad_event_int(int padid, int eventid, int val)
{
	return AdjSynth::get_instance()->pad_event_int(padid,
		eventid,
		val, 
		AdjSynth::get_instance()->get_active_patch_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_pad_event_bool(int padid, int eventid, bool val)
{
	return AdjSynth::get_instance()->pad_event_bool(padid,
		eventid,
		val, 
		AdjSynth::get_instance()->get_active_patch_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_reverb_event_int(int revid, int eventid, int val)
{
	return AdjSynth::get_instance()->reverb_event_int(revid,
		eventid,
		val, 
		AdjSynth::get_instance()->get_active_patch_params());
}

int mod_synth_reverb_event_bool(int revid, int eventid, bool val)
{
	return AdjSynth::get_instance()->reverb_event_bool(revid,
		eventid,
		val, 
		AdjSynth::get_instance()->get_active_patch_params());
}

int mod_synth_vco_event_int(int vcoid, int eventid, int val)
{
	return AdjSynth::get_instance()->vco_event_int(vcoid,
		eventid,
		val, 
		AdjSynth::get_instance()->get_active_patch_params(),
		AdjSynth::get_instance()->get_active_sketch());
}

int mod_synth_vco_event_bool(int vcoid, int eventid, bool val)
{
	return AdjSynth::get_instance()->vco_event_bool(vcoid,
		eventid,
		val, 
		AdjSynth::get_instance()->get_active_patch_params(),
		AdjSynth::get_instance()->get_active_sketch());
}












void mod_synth_activate_callback_update_ui(func_ptr_void_void_t ptr)
{
	//if (callback_ptr_update_ui != NULL)
	//{
	//	callback_ptr_update_ui();
	//}
}

void mod_synth_register_callback_update_ui(func_ptr_void_void_t ptr)
{
	//callback_ptr_update_ui = ptr;
}





/******************************************************************
 *********************** FLUID SYNTH API **************************
 ******************************************************************/

void mod_synth_set_fluid_synth_volume(int vol)
{
	mod_synthesizer->get_fluid_synth()->set_volume(vol);
}

void mod_synth_set_fluid_synth_reverb_room_size(int val)
{
	mod_synthesizer->get_fluid_synth()->set_reverb_room_size(val);
}

void mod_synth_set_fluid_synth_reverb_damp(int val)
{
	mod_synthesizer->get_fluid_synth()->set_reverb_damp(val);
}

void mod_synth_set_fluid_synth_reverb_width(int val)
{
	mod_synthesizer->get_fluid_synth()->set_reverb_width(val);
}

void mod_synth_set_fluid_synth_reverb_level(int val)
{
	mod_synthesizer->get_fluid_synth()->set_reverb_level(val);
}

void mod_synth_set_fluid_synth_chorus_number(int val)
{
	mod_synthesizer->get_fluid_synth()->set_chorus_number(val);
}

void mod_synth_set_fluid_synth_chorus_level(int val)
{
	mod_synthesizer->get_fluid_synth()->set_chorus_level(val);
}

void mod_synth_set_fluid_synth_chorus_speed(int val)
{
	mod_synthesizer->get_fluid_synth()->set_chorus_speed(val);
}

void mod_synth_set_fluid_synth_chorus_depth(int val)
{
	mod_synthesizer->get_fluid_synth()->set_chorus_depth(val);
}

void mod_synth_set_fluid_synth_chorus_waveform(int val)
{
	mod_synthesizer->get_fluid_synth()->set_chorus_waveform(val);
}

void mod_synth_set_fluid_synth_enable_reverb()
{
	mod_synthesizer->get_fluid_synth()->enable_reverb();
}

void mod_synth_set_fluid_synth_disable_reverb()
{
	mod_synthesizer->get_fluid_synth()->disable_reverb();
}

void mod_synth_set_fluid_synth_enable_chorus()
{
	mod_synthesizer->get_fluid_synth()->enable_chorus();
}

void mod_synth_set_fluid_synth_disable_chorus()
{
	mod_synthesizer->get_fluid_synth()->disable_chorus();
}

int mod_synth_get_active_fluid_synth_volume()
{
	fluid_res_t res = 0;

	_settings_float_param_t param;

	res = mod_synthesizer->get_fluid_synth()->instrument_settings->get_float_param(
		mod_synthesizer->get_fluid_synth()->active_settings_params, 
		"fluid.synth.volume", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}

	// 0-1.0 -> 0-200
	return (int)(param.value * 200.0);
}

int mod_synth_get_active_fluid_synth_reverb_room_size()
{
	fluid_res_t res = 0;

	_settings_float_param_t param;

	res = mod_synthesizer->get_fluid_synth()->instrument_settings->get_float_param(
		mod_synthesizer->get_fluid_synth()->active_settings_params, 
		"fluid.synth.reverb.room-size", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0-1.0 -> 0-100
	return (int)(param.value * 100.0);
}

int mod_synth_get_active_fluid_synth_reverb_damp()
{
	fluid_res_t res = 0;
	_settings_float_param_t param;

	res = mod_synthesizer->get_fluid_synth()->instrument_settings->get_float_param(
		mod_synthesizer->get_fluid_synth()->active_settings_params, 
		"fluid.synth.reverb.damp", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0-1.0 -> 0-100
	return (int)(param.value * 100);
}

int mod_synth_get_active_fluid_synth_reverb_width()
{
	fluid_res_t res = 0;
	_settings_float_param_t param;

	res = mod_synthesizer->get_fluid_synth()->instrument_settings->get_float_param(
		mod_synthesizer->get_fluid_synth()->active_settings_params, 
		"fluid.synth.reverb.width", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0-1.0 -> 0-100
	return (int)(param.value * 100);
}

int mod_synth_get_active_fluid_synth_reverb_level()
{
	fluid_res_t res = 0;
	_settings_float_param_t param;

	res = mod_synthesizer->get_fluid_synth()->instrument_settings->get_float_param(
		mod_synthesizer->get_fluid_synth()->active_settings_params, 
		"fluid.synth.reverb.level", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0-1.0 -> 0-100
	return (int)(param.value * 100);
}

int mod_synth_get_active_fluid_synth_chorus_number()
{
	fluid_res_t res = 0;
	_settings_int_param_t param;

	res = mod_synthesizer->get_fluid_synth()->instrument_settings->get_int_param(
		mod_synthesizer->get_fluid_synth()->active_settings_params, 
		"fluid.synth.chorus.nr", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0-99
	return param.value;
}

int mod_synth_get_active_fluid_synth_chorus_level()
{
	fluid_res_t res = 0;
	_settings_float_param_t param;

	res = mod_synthesizer->get_fluid_synth()->instrument_settings->get_float_param(
		mod_synthesizer->get_fluid_synth()->active_settings_params, 
		"fluid.synth.chorus.level", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0-10.0 -> 0-100
	return (int)(param.value * 10);
}

int mod_synth_get_active_fluid_synth_chorus_speed()
{
	fluid_res_t res = 0;
	_settings_float_param_t param;

	res = mod_synthesizer->get_fluid_synth()->instrument_settings->get_float_param(
		mod_synthesizer->get_fluid_synth()->active_settings_params, 
		"fluid.synth.chorus.speed", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0.29-5.0 -> 29-500
	return (int)(param.value * 100);
}

int mod_synth_get_active_fluid_synth_chorus_depth()
{
	fluid_res_t res = 0;
	_settings_float_param_t param;

	res = mod_synthesizer->get_fluid_synth()->instrument_settings->get_float_param(
		mod_synthesizer->get_fluid_synth()->active_settings_params, 
		"fluid.synth.chorus.depth", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0-21.0 -> 0-210
	return (int)(param.value * 10);
}

int mod_synth_get_active_fluid_synth_chorus_waveform()
{
	fluid_res_t res = 0;
	_settings_int_param_t param;

	res = mod_synthesizer->get_fluid_synth()->instrument_settings->get_int_param(
		mod_synthesizer->get_fluid_synth()->active_settings_params, 
		"fluid.synth.chorus.waveform", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0, 1
	return param.value;
}

bool mod_synth_get_active_fluid_synth_reverb_activation_state()
{
	fluid_res_t res = 0;
	_settings_bool_param_t param;

	res = mod_synthesizer->get_fluid_synth()->instrument_settings->get_bool_param(
		mod_synthesizer->get_fluid_synth()->active_settings_params, 
		"fluid.synth.reverb.active", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return false;
	}

	return param.value;
}

bool mod_synth_get_active_fluid_synth_chorus_activation_state()
{
	fluid_res_t res = 0;
	_settings_bool_param_t param;

	res = mod_synthesizer->get_fluid_synth()->instrument_settings->get_bool_param(
		mod_synthesizer->get_fluid_synth()->active_settings_params, 
		"fluid.synth.chorus.active", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}

	return param.value;
}

void mod_synth_set_fluid_synth_soundfont(string path)
{
	mod_synthesizer->get_fluid_synth()->set_soundfont(path);
}

void mod_synth_load_and_set_fluid_synth_soundfont(string path)
{
	mod_synthesizer->get_fluid_synth()->load_and_set_fluid_synth_soundfont(path);
}

int mod_synth_set_fluid_synth_select_program(int chan,
											 unsigned int sfid,
											 unsigned int bank,
											 unsigned int program)
{
	mod_synthesizer->get_fluid_synth()->select_program(chan, sfid, bank, program);

	return 0;
}

int mod_synth_set_fluid_synth_program_select(int chan,
											 unsigned int sfid,
											 unsigned int bank,
											 unsigned int program)
{
	return mod_synthesizer->get_fluid_synth()->set_fluid_synth_program_select(chan, sfid, bank, program);
}

int mod_synth_collect_fluid_synth_preset_parms(_settings_params_t *params)
{
	return mod_synthesizer->get_fluid_synth()->collect_fluid_synth_preset_parms(params);
}

int mod_synth_save_fluid_synth_preset_file(string path)
{
	return mod_synthesizer->get_fluid_synth()->save_fluid_synth_preset_file(path);
}

int mod_synth_open_fluid_synth_preset_file(string path, int preset_num){

	int res;

	mod_synthesizer->get_fluid_synth()->open_fluid_synth_preset_file(
		path,
		mod_synthesizer->get_fluid_synth()->active_settings_params,
		&mod_synthesizer->get_fluid_synth()->presets_summary_str[preset_num]);

	mod_synthesizer->get_fluid_synth()->generate_fluid_synth_prest_summary(
		&mod_synthesizer->get_fluid_synth()->channels_presets[preset_num][0],
		&mod_synthesizer->get_fluid_synth()->presets_summary_str[preset_num]);
}

std::string mod_synth_get_fluid_synth_soundfont_id_name(int sfid)
{
	return mod_synthesizer->get_fluid_synth()->get_fluid_synth_interface()->get_fluid_synth_soundfont_id_name(sfid);
}

int mod_synth_get_fluid_synth_loaded_soundfonts_presets(std::vector<_soundfont_presets_data_t> *presets)
{
	return mod_synthesizer->get_fluid_synth()->get_fluid_synth_interface()->get_fluid_synth_loaded_sound_fonts_presets(presets);
}

int mod_synth_get_fluid_synth_channel_bank(int chan)
{
	return mod_synthesizer->get_fluid_synth()->get_fluid_synth_channel_bank(chan);
}

int mod_synth_get_fluid_synth_channel_program(int chan)
{
	return mod_synthesizer->get_fluid_synth()->get_fluid_synth_interface()->get_fluid_synth_channel_program(chan);
}

std::string mod_synth_get_fluid_synth_channel_preset_name(int chan)
{
	return mod_synthesizer->get_fluid_synth()->get_fluid_synth_interface()->get_fluid_synth_channel_preset_name(chan);
}

int mod_synth_get_fluid_synth_channel_sfont_id(int chan)
{
	return mod_synthesizer->get_fluid_synth()->get_fluid_synth_interface()->get_fluid_synth_channel_sfont_id(chan);
}

std::string mod_synth_get_fluid_synth_channel_preset_soundfont_name(int chan)
{
	return mod_synthesizer->get_fluid_synth()->get_fluid_synth_interface()->get_fluid_synth_channel_preset_soundfont_name(chan);
}

/* Settings Files Handling */

int mod_synth_save_fluid_synth_settings_file(string path)
{
	return mod_synthesizer->get_fluid_synth()->save_fluid_synth_settings_file(path);
}

int mod_synth_open_fluid_synth_settings_file(string path)
{
	return mod_synthesizer->get_fluid_synth()->open_fluid_synth_settings_file(path);
}

int mod_synth_save_adj_synth_patch_file(string path)
{
	
}

int mod_synth_open_adj_synth_patch_file(std::string path, int channel)
{
	return mod_synthesizer->open_adj_synth_patch_file(path, channel);
}


std::string mod_synth_get_program_patch_name(int prog)
{
	
}

int mod_synth_save_adj_synth_settings_file(string path)
{
	
}

int mod_synth_open_adj_synth_settings_file(std::string path)
{
	
}

int mod_synth_save_mod_synth_general_settings_file(string path)
{
	
}

int mod_synth_open_mod_synth_general_settings_file(std::string path)
{
	
}

int mod_synth_save_mod_synth_preset_file(string path)
{
	
}

int mod_synth_open_mod_synth_preset_file(std::string path, int presetNum)
{
	
}

void mod_synth_set_mod_synth_active_preset_num(int presetNum)
{
	
}

int mod_synth_get_mod_synth_active_preset_num()
{
	
}

string *mod_synth_get_preset_summary_string(int presetNum)
{
	
}

/* UI Callback Functions Registration */

void mod_synth_fluid_synth_register_ui_update_callback(func_ptr_void_void_t ptr)
{
	mod_synthesizer->get_fluid_synth()->register_ui_update_callback(ptr);
}

void mod_synth_fluid_synth_activate_ui_update_callback()
{
	mod_synthesizer->get_fluid_synth()->activate_ui_update_callback();
}

void mod_synth_fluid_synth_register_callback_update_preset_text(func_ptr_void_string_t ptr)
{
	mod_synthesizer->get_fluid_synth()->register_callback_update_preset_text(ptr);
}

void mod_synth_fluid_synth_activate_ui_update_preset_text_callback(std::string text)
{
	mod_synthesizer->get_fluid_synth()->activate_ui_update_preset_text_callback(text);
}

void mod_synth_fluid_synth_set_active_preset(int preset_num)
{
	if ((preset_num >= 0) && (preset_num < _NUM_OF_FLUID_PRESETS))
	{
		mod_synthesizer->get_fluid_synth()->set_active_preset(preset_num);

		mod_synth_fluid_synth_activate_ui_update_callback();
	}
}

int mod_synth_fluid_synth_get_active_preset()
{
	return mod_synthesizer->get_fluid_synth()->active_preset;
}

/******************************************************************
 *********************** MIDI Player API **************************
 ******************************************************************/

int mod_synth_open_midi_file(std::string path)
{
	return mod_synthesizer->get_midi_player()->init_new_file(path);
}

void mod_synth_midi_player_play()
{
	mod_synthesizer->get_midi_player()->play();
}

void mod_synth_midi_player_pause()
{
	mod_synthesizer->get_midi_player()->pause();
}

void mod_synth_midi_player_stop()
{
	mod_synthesizer->get_midi_player()->stop();
}

void mod_synth_register_midi_player_potision_update_callback(func_ptr_void_int_t ptr)
{
	mod_synthesizer->midi_player->register_midi_player_potision_update_callback(ptr);
}

void mod_synth_register_midi_player_total_song_playing_time_update_callback(func_ptr_void_int_int_t ptr)
{
	mod_synthesizer->midi_player->register_midi_player_total_playing_time_update_callback(ptr);
}

void mod_synth_register_midi_player_song_playing_time_update_callback(func_ptr_void_int_int_t ptr)
{
	mod_synthesizer->midi_player->register_midi_player_playing_time_update_callback(ptr);
}

void mod_synth_register_midi_player_song_remaining_playing_time_update_callback(func_ptr_void_int_int_t ptr)
{
	mod_synthesizer->midi_player->register_midi_player_song_remaining_time_update_callback(ptr);
}


/******************************************************************
 *********************** Control Box API **************************
 ******************************************************************/


/**
*   @brief  Register a callback function that initiates a full GUI update based on control box event.
*   @param  funcPtrVoidVoid ptr  a pointer to the callback function ( void func(void) )
*   @return void
*/
void mod_synth_register_callback_control_box_event_update_ui(func_ptr_void_int_uint16_t ptr)
{
	mod_synthesizer->control_box_events_handler->register_callback_control_box_event_update_ui(ptr);
}

void mod_synth_unregister_callback_control_box_event_update_ui(func_ptr_void_int_uint16_t ptr)
{
	mod_synthesizer->control_box_events_handler->unregister_callback_control_box_event_update_ui(ptr);
}



/******************************************************************
 *********************** MIDI Mixer API **************************
 ******************************************************************/

void mod_synth_midi_mixer_set_channel_volume(int chan, int vol)
{
	mod_synthesizer->midi_mapper->set_midi_channel_volume(chan, vol);
}

void mod_synth_midi_mixer_set_channel_pan(int chan, int pan)
{
}

void mod_synth_register_midi_mixer_channel_volume_update_callback(func_ptr_void_int_int_t ptr)
{
	
}

void mod_synth_midi_mixer_set_channel_static_volume(int chan, bool state)
{
	if (state)
	{
		mod_synthesizer->midi_mapper->enable_midi_channel_block_volume_commands(chan);
	}
	else
	{
		mod_synthesizer->midi_mapper->disable_midi_channel_block_volume_commands(chan);
	}
}

void mod_synth_register_midi_mixer_channel_pan_update_callback(func_ptr_void_int_int_t ptr)
{
	
}

void mod_synth_register_midi_mixer_channel_static_volume_update_callback(func_ptr_void_int_bool_t ptr)
{
	
}

/******************************************************************
 *********************** MIDI Mapper API **************************
 ******************************************************************/

void mod_synth_register_midi_channel_volume_control_command_trapped_callback(func_ptr_void_int_int_t ptr)
{
	mod_synthesizer->midi_mapper->register_midi_channel_volume_control_command_trap_callback(ptr);
}

void mod_synth_register_midi_channel_change_program_command_trapped_callback(func_ptr_void_int_int_t ptr)
{
	mod_synthesizer->midi_mapper->register_midi_channel_change_program_command_trap_callback(ptr);
}

/******************************************************************
 *********************** Patches Management API **************************
 ******************************************************************/

void mod_synth_register_callback_get_active_modules_names_list(func_ptr_vector_std_string_void_t ptr)
{
	return mod_synthesizer->patches_handler->register_callback_get_active_modules_names_list(ptr);
}

int mod_synth_save_patch_file(std::string file_path)
{
	return mod_synthesizer->patches_handler->save_patch_file(file_path);
}

int mod_synth_load_patch_file(std::string file_path)
{
	mod_synthesizer->patches_handler->disconnect_current_oppened_modules_midi_in_connections();
	mod_synthesizer->patches_handler->close_current_oppened_modules();
	return mod_synthesizer->patches_handler->load_patch_file(file_path);
}


void callback_message_id(int mssgid)
{
	if (callback_ptr_message_id)
	{	
		(*callback_ptr_message_id)(mssgid);
	}
}



