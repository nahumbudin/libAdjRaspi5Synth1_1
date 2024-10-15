/**
*	@file		modSynth.h
*	@author		Nahum Budin
*	@date		11-May-2024
*	@version	1.0
*
*	@brief		This is the main modular synthesizer libraray object.
*
*	History:\n
*
*	Based on libAdjHeartModSynth_2 library
*
*/

#include "modSynth.h"
#include "commonDefs.h"

#include "./CPU/CPUSnapshot.h"

#include "./MIDI/midiAlsaQclient.h"

#include "./ALSA/controlBoxExtMidiInClientAlsaOutput.h"


#include "./ALSA/alsaMidiSequencerClient.h"
#include "./ALSA/alsaMidiSequencerEventsHandler.h"

#include "./Instrument/instrumentsManager.h"
#include "./Instrument/instrumentFluidSynth.h"
#include "./Instrument/instrumentHammondOrgan.h"

#include "./Instrument/instrumentMidiPlayer.h"
#include "./Instrument/instrumentMidiMapper.h"

#include "./Instrument/instrumentControlBoxHandler.h"

// Mutex to controll audio memory blocks allocation
//pthread_mutex_t voice_mem_blocks_allocation_control_mutex;

int ModSynth::cpu_utilization = 0;
volatile bool cheack_cpu_utilization_thread_is_running;


/*************************** ALSA MIDI *******************************************/

/* ALSA MIDI sequencer client - Receives input midi data stream from ALSA midi source.
 *								Connect to it using, for example, a JACK Audio Connection Kit.
 *								Received data is pushed into the alsa_seq_client_rx_queue */
//AlsaMidiSequencerInputClient* alsa_midi_sequencer_input_client = new AlsaMidiSequencerInputClient();



/* Pulls out the events from alsa_seq_client_rx_queue and executes commands */
//AlsaMidiSeqencerEventsHandler alsa_midi_seqencer_events_handler_1(_MIDI_STAGE_2);



#if (_MAX_NUM_OF_MIDI_CONNECTIONS > 0)
/* MIDI ALSA input client - waits for data in the alsa_rx_queue */
MidiAlsaQclientIn alsa_midi_in_1(&AlsaMidi::get_instance()->alsa_rx_queue[0], _MIDI_STAGE_0);
//MidiParser midi_parser_alsa_1(_MIDI_STAGE_1);
//MidiHandler midi_handler_alsa_1(_MIDI_STAGE_2);

/* ALSA MIDI connection ALSA-client -> parser */
//MidiConnection midi_connection_alsa_1(&alsa_midi_in_1, 0, &midi_parser_alsa_1, 0);
/* ALSA MIDI connection parser -> handler */
//MidiConnection midi_connection_alsa_handler_1(&midi_parser_alsa_1, 0, &midi_handler_alsa_1, 0);

#endif

// Callback that is initiated by the AudioManager audio - update thread.
void callback_audio_update_cycle_start_tasks_wrapper(int param)
{
	ModSynth::get_instance()->update_tasks(param);
}



/* modSynth instance */
ModSynth* ModSynth::mod_synth = NULL;



ModSynth::ModSynth()
{
	mod_synth = this;
	int i, res, last_input_client_num;	

	alsa_midi_system_control = AlsaMidiSysControl::get_instance();

	jack_connections = JackConnections::get_instance();

	// Create a Bluetooth ALSA output Client.
	bt_alsa_out = AlsaBtClientOutput::get_instance();

	// Scan for current ALSA MIDI In client - the last is the BT client above
	alsa_midi_system_control->refresh_alsa_clients_data();
	last_input_client_num = alsa_midi_system_control->get_num_of_input_midi_clients() - 1;
	alsa_midi_system_control->get_midi_input_client_name_string(last_input_client_num,
																&alsa_midi_system_control->bt_client_in_name);

	// Create the Control Box MIDI ALSA output Client
	control_box_alsa_out = ControlBoxClientAlsaOutput::get_instance();
	// Scan for current ALSA Control Box In client
	alsa_midi_system_control->refresh_alsa_clients_data();
	last_input_client_num = alsa_midi_system_control->get_num_of_input_midi_clients() - 1;
	alsa_midi_system_control->get_midi_input_client_name_string(last_input_client_num,
																&alsa_midi_system_control->control_box_client_in_name);
	
	// Create the Control Box external MIDI in port ALSA output Client
	control_box_ext_midi_in_alsa_out = ControlBoxExtMidiInClientAlsaOutput::get_instance();
	// Scan for current ALSA Control Box Ext MIDI In client
	alsa_midi_system_control->refresh_alsa_clients_data();
	last_input_client_num = alsa_midi_system_control->get_num_of_input_midi_clients() - 1;
	alsa_midi_system_control->get_midi_input_client_name_string(last_input_client_num,
																&alsa_midi_system_control->control_box_xt_midi_in_client_name);
	
	// Create an external MIDI interface
	midi_ext_interface = MidiExtInterface::get_midi_ext_interface_instance();

	instruments_manager = InstrumentsManager::get_instance();
	
	// TODO: create instances only when oppened ?.

	fluid_synth = new InstrumentFluidSynth();
	instruments_manager->add_instrument(_INSTRUMENT_NAME_FLUID_SYNTH_STR_KEY, 
										fluid_synth);

	hammond_organ = new InstrumentHammondOrgan();
	instruments_manager->add_instrument(_INSTRUMENT_NAME_HAMMON_ORGAN_STR_KEY, 
										hammond_organ);

	midi_mapper = new InstrumentMidiMapper(alsa_midi_system_control,
										   &alsa_midi_system_control->midi_mapper_client_in_name);
	instruments_manager->add_instrument(_INSTRUMENT_NAME_MIDI_MAPPER_STR_KEY,
										midi_mapper);

	midi_player = new InstrumentMidiPlayer(alsa_midi_system_control, 
										   &alsa_midi_system_control->midi_player_client_in_name);
	instruments_manager->add_instrument(_INSTRUMENT_NAME_MIDI_PLAYER_STR_KEY,
										midi_player);
	
	midi_player->register_clear_all_playing_notes_callback(NULL);  // TODO:
	midi_player->register_midi_change_channel_volume_callback(NULL);
	midi_player->register_midi_change_program_callback(NULL);
	midi_player->register_midi_player_song_remaining_time_update_callback(
		InstrumentMidiPlayer::midi_player_total_playing_time_update_callback_ptr);
	midi_player->register_midi_player_song_remaining_time_update_callback(
		InstrumentMidiPlayer::midi_player_playing_time_update_callback_ptr);
	midi_player->register_midi_player_playing_time_update_callback(
		InstrumentMidiPlayer::midi_player_file_remaining_time_update_callback_ptr);
	midi_player->register_midi_player_potision_update_callback(NULL);
	// midi_player->register_send_midi_events_vector_callback(NULL);  in player
	

	control_box_events_handler = new InstrumentControlBoxEventsHandler();
	instruments_manager->add_instrument(_INSTRUMENT_NAME_CONTROL_BOX_HANDLER_STR_KEY, 
										control_box_events_handler);

	alsa_midi_system_control->refresh_alsa_clients_data();

	jack_connections->refresh_jack_clients_data();

	std::list<std::string> in_clients;

	int num_of_alsa_clients =
		fluid_synth->alsa_connections->get_alsa_midi_in_clients_names_list(&in_clients);
	
	//fluid_synth->instrument_connections_control->alsa_midi_system_contrl->connect_midi_clients(5, 0, 2, 0);

	int num_of_jack_clients = 
		fluid_synth->jack_connections->get_num_of_output_jack_clients();

	i = alsa_midi_system_control->get_midi_input_client_id(_ALSA_NAME_CLIENT_CONTROL_BOX_EXT_MIDI_STR, true);
	
	/* Connect the control box handler to the midi input client */
	/* Remove the xxx: prefix*/
	std::string removed_name;
	int loc = alsa_midi_system_control->control_box_client_in_name.find_first_of(":");
	if (loc != std::string::npos)
	{
		removed_name = alsa_midi_system_control->control_box_client_in_name.substr(loc + 1);
	}

	int in_dev = alsa_midi_system_control->get_midi_input_client_id(removed_name);
	int out_dev = alsa_midi_system_control->get_midi_output_client_id(_INSTRUMENT_NAME_CONTROL_BOX_HANDLER_STR_KEY);
	alsa_midi_system_control->connect_midi_clients(in_dev, 0, out_dev, 0);

	patches_handler = PatchsHandler::get_patchs_handler_instance();
	
	for (i = _PROGRAM_0; i < _PROGRAM_15; i++)
	{
		// Init all programs to synthersizer not assigned state
		midi_channel_synth[i] = _MIDI_CHAN_ASSIGNED_SYNTH_NONE;
	}
	
	// TODO: Get the default settings directories from a file - currentlly hard coded below
	
	// ModSynth default settings directory
	mod_synth_general_settings_file_path_name = "/home/pi/AdjRaspi5Synth/ModSynth/Default_Settings";
	
	// create the general setting manager(mange audio and midi settings)
	general_settings_manager = new Settings(&active_general_synth_settings_params);
	// Create the FLuidSynth settings manager
	fluid_synth_settings_manager = new Settings(&active_fluid_synth_settings_params);
	
	
	adj_synth = AdjSynth::get_instance();
	// Init the Adj synthesizers
	init();
	
	
	// Start the CPU utilization measuring thread.
	start_cheack_cpu_utilization_thread();
}


ModSynth::~ModSynth()
{
	on_exit();
}

/**
*   @brief  retruns the single modular synth instance
*
*   @param  none
*   @return the single modular synth instance
*/
ModSynth *ModSynth::get_instance()
{
	if (mod_synth == NULL)
	{
		mod_synth = new ModSynth();
	}

	return mod_synth;
}

int ModSynth::init()
{
	settings_res_t res;	
	
	/* Assign the ModSynth general settings manager, active general settings settings parameters.
	 * Mixer, Keyboard, Eqalizer and reverb */
	adj_synth->set_settings_params(general_settings_manager, &active_general_synth_settings_params);
	// Create and initialize the ModSynth default General Settings parameters values.
	set_default_general_settings_parameters(&active_general_synth_settings_params);
	
	// Assign the AdjSynth settings manager, active settings parameters..
	adj_synth->set_settings_params(adj_synth->adj_synth_settings_manager,
		adj_synth->get_active_settings_params());
	// Create and initialize the AdjSynth default Settings parameters values.
	set_adj_synth_default_settings(adj_synth->get_active_settings_params());
	
	// Init programs
	adj_synth->init_synth_programs(/*&active_adj_synth_patch*/);
	

	return 0;
}

/**
*   @brief  Closing and cleanup when application goes down.
*			Must be called whenever terminating the application.
*   @param  none
*   @return void
*/
void ModSynth::on_exit()
{
	if (fluid_synth != NULL)
	{
		fluid_synth->get_fluid_synth_interface()->deinitialize_fluid_synthesizer();
	}
	
	this->stop_cheack_cpu_utilization_thread();
	//TODO: stop whatever else should be terminated
}

/**
*   @brief  performs periodic update tasks - called by the audio processing update cycle process
*   @param  voc	voice number (na - global)
*   @return pointer to adjheart synthesizr
*/
void ModSynth::update_tasks(int voc)
{
	if (adj_synth->kbd1->portamento_is_enabled())
	{
		adj_synth->kbd1->update_actual_frequency();
		AdjSynth::get_instance()->synth_voice[_SYNTH_VOICE_1]->dsp_voice->set_voice_frequency(
					adj_synth->kbd1->get_note_frequency());
	}
}

void ModSynth::start_cheack_cpu_utilization_thread()
{
	cheack_cpu_utilization_thread_is_running = true;
	pthread_create(&cheack_cpu_utilization_thread_id, NULL, cheack_cpu_utilization_thread, NULL);
	pthread_setname_np(cheack_cpu_utilization_thread_id, "cpuutilthread");
}

void ModSynth::stop_cheack_cpu_utilization_thread()
{
	cheack_cpu_utilization_thread_is_running = false;
	//	pthread_create(&cheack_cpu_utilization_thread_id, NULL, cheackCpuUtilizationThread, NULL);
	//	pthread_setname_np(cheack_cpu_utilization_thread_id, "cpuutilthread");
}


int ModSynth::init_midi_ext_interface(int ser_port_num)
{
	midi_ext_interface->init_serial_port_services(ser_port_num, 115200, "8N1", 0);
	
	control_box_ext_midi_in_alsa_out->startAlsaOutThread();

	control_box_alsa_out->startAlsaOutThread();
	
	return 0;
}

int ModSynth::deinit_midi_ext_interface()
{
	
	
	return 0;
}



/**
*   @brief  retruns a pointer to the global bluetooth alsa midi out client
*   @param  none
*   @return the global bluetooth alsa midi out client
*/
AlsaBtClientOutput* ModSynth::get_bt_alsa_out()
{
	return bt_alsa_out;
}

/**
*   @brief  retruns a pointer to the FluidSynth instrument object
*   @param  none
*   @return a pointer to the FluidSynth instrument object
*/
InstrumentFluidSynth *ModSynth::get_fluid_synth()
{
	return fluid_synth;
}

/**
*   @brief  retruns a pointer to the MIDI-Player instrument object
*   @param  none
*   @return a pointer to the MIDI-Player instrument object
*/
InstrumentMidiPlayer *ModSynth::get_midi_player()
{
	return midi_player;
}

/**
*   @brief  Set the master volume level.
*   @param  vol volume level (0-100)
*   @return none
*/
void ModSynth::set_master_volume(int vol)
{
	double gain;
	settings_res_t res;

	// for correct limit testing, synth.master_volume param must be already intialized
	res = general_settings_manager->set_int_param(
											&active_general_synth_settings_params,
		"synth.master_volume",
		vol,
		100,
		0,
		"adj_synth_settings_param",
		NULL,
		_SET_VALUE);

	if (res == _SETTINGS_OK)
	{
		master_volume = vol;
		// Update all other volume levels
		// Fluid synth - max gain 0.25
		gain = (float)(fluid_synth_volume*master_volume) / 40000.f;
		fluid_synth->get_fluid_synth_interface()->set_fluid_synth_gain(gain);

		AdjSynth::get_instance()->set_master_volume(vol);
	}
}

/**
*   @brief  Get the master volume level.
*   @param  none
*   @return volume level (0-100)
*/
int ModSynth::get_master_volume() 
{ 
	return 
		master_volume; 
}

/**
*   @brief  Set the fluid synth volume level.
*   @param  vol volume level (0-100)
*   @return none
*/
void ModSynth::set_fluid_synth_volume(int vol)
{
	// 0-0.25
	double gain = (float)(vol*master_volume) / 40000.f; // 100 * 100 / 40000 = 0.25

	settings_res_t res = fluid_synth->get_fluid_synth_interface()->set_fluid_synth_gain(gain);
	// set gain also verifies range
	if (res == _SETTINGS_OK)
	{
		fluid_synth_volume = vol;			
	}
} 

/**
*   @brief  Get the fluid synth volume level.
*   @param  none
*   @return volume level (0-100)
*/
int ModSynth::get_fluid_synth_volume() 
{ 
	return fluid_synth_volume; 
}

/**
*	@brief	Sets the sample-rate
*	@param	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).
*					Also sets WTAB funfemental and maxfrequencies based on sample rate.
*	@param	restart_audio if true only restart audio
*					
*	@return set sample-rate
*/
int ModSynth::set_sample_rate(int samp_rate, bool restart_audio)
{
	if (!is_valid_sample_rate(samp_rate))
	{
		sample_rate = _DEFAULT_SAMPLE_RATE;
	}
	else
	{
		sample_rate = samp_rate;
	}
	
	//	adj_synth->set_sample_rate(sample_rate);
		// TODO: fluid_synth
	
	if (restart_audio)
	{
		stop_audio();
		usleep(100000);
		start_audio();
	}
	
	return sample_rate;
}

/**
*   @brief  Starts the audio service.
*			Must be called after setting the audio driver type, 
*			sample-rate and audio block-size (else, default values will be used)..
*   @param  none
*   @return 0 if done
*/
int ModSynth::start_audio()
{
	int res = 0;
	// TODO:: res = start FluidSynth audio handling
	res |= adj_synth->start_audio(audio_driver, sample_rate, audio_block_size);
	
	return res;
}

/**
*   @brief  Stops the audio service.
*   @param  none
*   @return 0 if done
*/
int ModSynth::stop_audio()
{
	int res = 0;
	// TODO:: res = stop FluidSynth audio handling
	res |= adj_synth->stop_audio();
	
	return res;
}

/**
*	@brief	Sets the sample-rate value only (without restarting audio)
*			Settings will be effective only after the next call to start_audio().
*	@param	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).
*					Also sets WTAB funfemental and maxfrequencies based on sample rate.
*					
*	@return set sample-rate
*/
int ModSynth::set_sample_rate(int samp_rate)
{
	return set_sample_rate(samp_rate, false);
}

/**
*	@brief	Returns the set sample-rate
*	@param	none
*	@return set sample-rate
*/	
int ModSynth::get_sample_rate() { return sample_rate; }

/**
*   @brief  sets the audio block size 
*			Settings will be effective only after the next call to start_audio().
*   @param  int size: _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
*   @param	restart_audio if true restart audio
*   @return size if OK; -1 param out of range
*/
int ModSynth::set_audio_block_size(int size, bool restart_audio)
{	
	if (is_valid_audio_block_size(size))
	{
		audio_block_size = size;		
		
		//		adj_synth->set_audio_block_size(audio_block_size);
				// TODO: fluid_synth
		
		if (restart_audio)
		{
			stop_audio();
			usleep(100000);
			start_audio();
		}
		
		return audio_block_size;
	}
	else
	{
		return -1;
	}
}

/**
*   @brief  sets the audio block size value only (without restarting audio)
*			Settings will be effective only after the next call to start_audio().
*   @param  int size: _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
*   @return size if OK; -1 param out of range
*/
int ModSynth::set_audio_block_size(int size)
{
	return set_audio_block_size(size, false);
}

/**
*	@brief	Sets the audio driver type
*	@param	driver  _AUDIO_JACK, _AUDIO_ALSA (default: _DEFAULT_AUDIO)
*	@param	restart_audio if true set value and restart audio
*	@return set audio-driver
*/
int ModSynth::set_audio_driver_type(int driver, bool restart_audio)
{
	if (is_valid_audio_driver(driver))
	{
		audio_driver = driver;
	}
	else
	{
		audio_driver = _DEFAULT_AUDIO_DRIVER;
	}
	
	if (restart_audio)
	{
		stop_audio();
		usleep(100000);
		start_audio();
	}
	
	return audio_driver;
}

/**
*	@brief	Sets the audio driver type  value only (without restarting audio)
*			Settings will be effective only after the next call to start_audio().
*	@param	driver  _AUDIO_JACK, _AUDIO_ALSA (default: _DEFAULT_AUDIO)
*	@param	set_only if true only set value, false - set value and restart audio
*	@return set audio-driver
*/
int ModSynth::set_audio_driver_type(int driver)
{
	return set_audio_driver_type(driver, false);
}

/**
*	@brief	Returns the audio-driver type
*	@param	none
*	@return audio driver type
*/	
int ModSynth::get_audio_driver_type() { return audio_driver; }


	
/**
*   @brief  retruns the audio block size  
*   @param  none
*   @return buffer size
*/	
int ModSynth::get_audio_block_size() { return audio_block_size; }


void ModSynth::note_on(uint8_t channel, uint8_t note, uint8_t velocity)
{

}

void ModSynth::note_off(uint8_t channel, uint8_t note)
{

}

void ModSynth::change_program(uint8_t channel, uint8_t program)
{

}

void ModSynth::channel_pressure(uint8_t channel, uint8_t val)
{

}

void ModSynth::controller_event(uint8_t channel, uint8_t num, uint8_t val)
{

}

void ModSynth::pitch_bend(uint8_t channel, int pitch)
{

}


void *cheack_cpu_utilization_thread(void *arg)
{
	sleep(1);
	while (cheack_cpu_utilization_thread_is_running)
	{
		CPUSnapshot previousSnap;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		CPUSnapshot curSnap;

		const float ACTIVE_TIME = curSnap.GetActiveTimeTotal() - previousSnap.GetActiveTimeTotal();
		const float IDLE_TIME = curSnap.GetIdleTimeTotal() - previousSnap.GetIdleTimeTotal();
		const float TOTAL_TIME = ACTIVE_TIME + IDLE_TIME;
		ModSynth::cpu_utilization = (int)(100.f * ACTIVE_TIME / TOTAL_TIME);
	}

	return NULL;
}


