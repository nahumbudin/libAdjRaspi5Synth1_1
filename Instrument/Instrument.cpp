/**
* @file		instrument.cpp
*	@author		Nahum Budin
*	@date		24-06-2024
*	@version	1.0	Initial release
*					
*	@brief		The basic music generating object, e.g., analog-synthesizer,
*				FluidSynth SoundFont synthesizer, organ, etc.
*	
*	History:\n
*	
*/

#include "instrument.h"
#include "../MIDI/midiStream.h"

std::string AlsaMidiSysControl::bt_client_in_name = "";
std::string AlsaMidiSysControl::control_box_client_in_name = "";
std::string AlsaMidiSysControl::control_box_xt_midi_in_client_name = "";
std::string AlsaMidiSysControl::midi_player_client_in_name = "";
std::string AlsaMidiSysControl::midi_mapper_client_in_name = "";

Instrument::Instrument(std::string name, bool with_midi_in,
					   bool with_audio_out, bool with_midi_out,
					   AlsaMidiSysControl *alsa_control,
					   std::string *alsa_client_in_name)
{
	instrument_name = name;

	midi_in_enable = with_midi_in;
	midi_out_enable = with_midi_out;
	audio_out_enable = with_audio_out;

	//	instrument_connections_control = new InstrumentConnectionsControl();

	if (midi_in_enable)
	{
		alsa_connections = AlsaMidiSysControl::get_instance();
	}

	if (audio_out_enable)
	{
		jack_connections = JackConnections::get_instance();
	}

	if (midi_out_enable)
	{
		const char *portname = "virtual";
		int status;

		mode = SND_RAWMIDI_SYNC;

		if ((status = snd_rawmidi_open(NULL, &midiout, portname, mode)) < 0)
		{
			printf("Problem opening %s Alsa output: %s", name.c_str(), snd_strerror(status));
			//		exit(1);
		}
		else
		{
			if ((alsa_control != NULL) && (alsa_client_in_name != NULL))
			{
				// Scan for current ALSA In Instrument client
				alsa_control->refresh_alsa_clients_data();
				int last_input_client_num = alsa_control->get_num_of_input_midi_clients() - 1;
				alsa_control->get_midi_input_client_name_string(last_input_client_num,
																alsa_client_in_name);
			}
		}
	}

	init();
}

Instrument::~Instrument()
{
	
}

int Instrument::init()
{
	if (midi_in_enable)
	{
		alsa_midi_sequencer_input_client = new AlsaMidiSequencerInputClient(instrument_name, &alsa_rx_queue);
		alsa_connections->refresh_alsa_clients_data();
		alsa_input_client_id = alsa_connections->get_midi_output_client_id(instrument_name);

		alsa_midi_sequencer_events_handler = new AlsaMidiSeqencerEventsHandler(0, &alsa_rx_queue);
		alsa_midi_sequencer_events_handler->set_instrument(this);
	}

	active_settings_params = new _settings_params_t(); // TODO: presets or active_settings_param?

	instrument_settings = new Settings(active_settings_params); // TODO: presets or active_settings_param?

	return 0;
}

void Instrument::register_ui_update_callback(func_ptr_void_void_t ptr)
{
	update_ui_callback_ptr = ptr;
}

void Instrument::activate_ui_update_callback()
{
	if (update_ui_callback_ptr)
	{
		update_ui_callback_ptr();
	}
}

void Instrument::set_active_midi_channels(uint16_t act_chans){

	if (midi_in_enable)
	{
		alsa_midi_sequencer_events_handler->set_active_midi_channels(act_chans);
	}
}

uint16_t Instrument::get_active_midi_channels()
{
	if (midi_in_enable)
	{
		return alsa_midi_sequencer_events_handler->get_active_midi_channels();
	}
	else
	{
		return 0;
	}
}

void Instrument::note_on_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
}

void Instrument::note_off_handler(uint8_t channel, uint8_t note, uint8_t velocity)
{
}

void Instrument::change_program_handler(uint8_t channel, uint8_t program)
{
}

void Instrument::channel_pressure_handler(uint8_t channel, uint8_t val)
{
}

void Instrument::controller_event_handler(uint8_t channel, uint8_t num, uint8_t val)
{
}

void Instrument::pitch_bend_handler(uint8_t channel, int pitch)
{
}

void Instrument::sysex_handler(uint8_t *message, int len)
{
}

void Instrument::control_box_events_handler(int event_id, uint16_t event_value)
{
	
}

int Instrument::set_default_settings_parameters(_settings_params_t *params, int prog)
{

	return 0;
}

/**
*   @brief  Initiates an instrument parameter change event with integer value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int moduleid instrument submofule. for example VCO1 VCO2
*	@param	int paramid	specific settings parameter  id: for example _AMP_LEVEL, _AMP_PAN, 
*	@param	int val event parameter value (must be used with the relevant event id):\n
*	@param	*_settings_params_t params a pointer to the relevant settings structure			
*	@param	int prog program num 
*/
int Instrument::api_settings_events_handler(int moduleid, int paramid, int val, _settings_params_t *params, int program)
{

	return 0;
}

/**
*   @brief  Initiates an instrument parameter change event with integer value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int moduleid instrument submofule. for example VCO1 VCO2
*	@param	int paramid	specific settings parameter  id: for example _AMP_LEVEL, _AMP_PAN, 
*	@param	double val event parameter value (must be used with the relevant event id):\n
*	@param	*_settings_params_t params a pointer to the relevant settings structure			
*	@param	int prog program num 
*/
int Instrument::api_settings_events_handler(int moduleid, int paramid, double val, _settings_params_t *params, int program)
{

	return 0;
}

/**
*   @brief  Initiates an instrument parameter change event with integer value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int moduleid instrument submofule. for example VCO1 VCO2
*	@param	int paramid	specific settings parameter  id: for example _AMP_LEVEL, _AMP_PAN, 
*	@param	bool val event parameter value (must be used with the relevant event id):\n
*	@param	*_settings_params_t params a pointer to the relevant settings structure			
*	@param	int prog program num 
*/
int Instrument::api_settings_events_handler(int moduleid, int paramid, bool val, _settings_params_t *params, int program)
{

	return 0;
}

/**
*   @brief  Initiates an instrument parameter change event with integer value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int moduleid instrument submofule. for example VCO1 VCO2
*	@param	int paramid	specific settings parameter  id: for example _AMP_LEVEL, _AMP_PAN, 
*	@param	string val event parameter value 
*	@param	*_settings_params_t params a pointer to the relevant settings structure			
*	@param	int prog program num 
*/
int Instrument::api_settings_events_handler(int moduleid, int paramid, string val, _settings_params_t *params, int program)
{

	return 0;
}
