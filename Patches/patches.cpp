/**
* @file		patch.cpp
*	@author		Nahum Budin
*	@date		29-Aug-2024
*	@version	1.0
*					
*	
*	@brief		Modular synthersizer patches handling.
*
*	History: 
*/

#include <filesystem>
#include <iostream>
#include <list>

#include "patches.h"

#include "../modSynth.h"
#include "../Instrument/instrumentsManager.h"
#include "../Instrument/instrumentFluidSynth.h"
#include "../utils/utils.h"
#include "../LibAPI/connections.h"
#include "../LibAPI/defines.h"
#include "../LibAPI/fluidSynth.h"

#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"


using namespace rapidjson;
using namespace std;


PatchsHandler *PatchsHandler::patch_handler_instance = NULL;

PatchsHandler::PatchsHandler(){

	callback_get_active_modules_names_list_ptr = NULL;
}

PatchsHandler::~PatchsHandler()
{
	
	
}

PatchsHandler *PatchsHandler::get_patchs_handler_instance()
{
	if (patch_handler_instance == NULL)
	{
		patch_handler_instance = new PatchsHandler();
	}

	return patch_handler_instance;
}

int PatchsHandler::save_patch_file(std::string file_path)
{
	vector<string> modules_names;
	list<std::string> connected_to_midi_clients_names_list;
	int module_num = 1;
	int client_number;
	string json_str;
	list<string>::iterator names_it;

	mod_synth_refresh_alsa_clients_data();
	mod_synth_refresh_jack_clients_data();

	if (callback_get_active_modules_names_list_ptr != NULL)
	{
		/* We need to get a list of only the active (oppened) modules */
		modules_names = callback_get_active_modules_names_list_ptr();
	}
	
	/* We need to create and save a current settings file for each module */
	filesystem::path fullpath(file_path);
	// `remove_filename()` does not alter `fullpath`
	string path_without_filename = fullpath.remove_filename();
	create_active_modules_settings_files(modules_names, path_without_filename);

	if (modules_names.size() > 0)
	{
		StringBuffer s;
		Writer<StringBuffer> writer(s);

		writer.StartObject(); // Between StartObject()/EndObject(),

		writer.Key("patch_name");
		writer.String(filesystem::path(file_path).stem().c_str());

		writer.Key("modules");
		writer.StartArray();
		

		for (string module_name : modules_names)
		{
			
			/* Get module Midi Input connections */
			client_number = mod_synth_get_midi_client_connection_num(module_name);	
			connected_to_midi_clients_names_list.clear();
			mod_synth_get_midi_client_connected_to_clients_names_list(client_number, true, 
																	  &connected_to_midi_clients_names_list);
			
			writer.StartObject();
			
			writer.Key("module_name");
			writer.String(module_name.c_str());

			writer.Key("settings_file");
			writer.String((path_without_filename + module_name.c_str() + "-settings.html").c_str());

			writer.Key("connections");
			writer.StartArray();

			writer.StartObject();
			writer.Key("midi_input_connections");
			writer.StartArray();

			if (connected_to_midi_clients_names_list.size() > 0)
			{
				names_it = connected_to_midi_clients_names_list.begin();
				for (int i = 0; i < connected_to_midi_clients_names_list.size(); i++)
				{
					writer.String(names_it->c_str());
					advance(names_it, 1);
				}
			}

			writer.EndArray(); // Midi In
			writer.EndObject();

			writer.StartObject();
			writer.Key("midi_output_connections");
			writer.StartArray();

			
			writer.EndArray(); // Midi Out
			writer.EndObject();

			writer.StartObject();
			writer.Key("jack_audio_input_connections");
			writer.StartArray();

			writer.EndArray(); // Jack In
			writer.EndObject();

			writer.StartObject();
			writer.Key("jack_audio_output_connections");
			writer.StartArray();

			writer.EndArray(); // Jack Out
			writer.EndObject();

			writer.EndArray(); // connections

			writer.EndObject();
			
			module_num++;
		}

		writer.EndArray();

		writer.EndObject();
		json_str = s.GetString();

		return write_text_file(file_path, json_str);
	}

	return -1;
}

int PatchsHandler::load_patch_file(std::string file_path)
{
	vector<char> file_data;
	int res;
	
	Document document; // JSON document
	
	string patch_name;
	string module_name;

	vector<string> active_modules_names; // list of active modules
	vector<string> settings_files; // list of modules settings files in the same order of the modules above.
	vector<vector<string>> midi_input_connections;	// lists of list of midi input intrefaces names. 
													// Each active module is represented as a vector.
													// An empty vector represents no midi input interfaces.

	string interface;
	vector<string> midi_input_interfaces;
		

	res = read_text_file(file_path, &file_data);
	if ((res == 0) && (file_data.size() > 0))
	{
		char json[file_data.size() + 1];

		for (int i = 0; i < file_data.size(); i++)
		{
			json[i] = file_data.at(i);
		}

		json[file_data.size()] = 0;

		if (document.Parse(json).HasParseError())
		{
			return -1;
		}
		
		assert(document.IsObject()); // Document is a JSON value represents the root of DOM. 
									 // Root can be either an object or array.

		for (Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr)
		{ //iterate through document
			const Value &obj_name = document[itr->name.GetString()]; //make object value
			//printf("name %s\n", itr->name.GetString());
			if (strcmp(itr->name.GetString(), "patch_name") == 0)
			{
				patch_name = itr->value.GetString();	
			}
			else if (strcmp(itr->name.GetString(), "modules") == 0)
			{
				// Modules array
				for (SizeType i = 0; i < itr->value.Size(); i++)
				{
					if (itr->value[i].IsObject())
					{
						midi_input_interfaces.clear();

						const Value &m = itr->value[i];
						for (auto &v : m.GetObject())
						{ //iterate through modules array objects
							if (m[v.name.GetString()].IsString())
							{
								if (strcmp(v.name.GetString(), "module_name") == 0)
								{
									active_modules_names.push_back(v.value.GetString());
								}
								else if (strcmp(v.name.GetString(), "settings_file") == 0)
								{
									settings_files.push_back(v.value.GetString());
								}
							}
							if (m[v.name.GetString()].IsArray())
							{
								// Connections array
								const Value &a = m[v.name.GetString()];
								for (SizeType j = 0; j < a.Size(); j++) {
									if(a[j].IsObject()) 
									{
										for (auto &c : a[j].GetObject())
										{
											string name = c.name.GetString();
											if (c.value.IsArray())
											{
												const Value &k = c.value;
												for (SizeType t = 0; t < k.Size(); t++) 
												{
													if (name == "midi_input_connections")
													{
														interface = k[t].GetString();
														midi_input_interfaces.push_back(interface);
													}
													else if (name == "midi_output_connections")
													{
														;
													}
													else if (name == "jack_audio_input_connections")
													{
														;
													}
													else if (name == "jack_audio_output_connections")
													{
														;
													}
												} 
											}
										}
									}	
								} // end of iterating on connections
							}
						}
					}

					midi_input_connections.push_back(midi_input_interfaces);
					
				} // end of iterations over each module
				
			} // end of modules object	
		}

		if (active_modules_names.size() > 0)
		{
			implement_patch(active_modules_names, settings_files, midi_input_connections, file_path);
		}

		return active_modules_names.size();
	}

	return -1;
}


void PatchsHandler::register_callback_get_active_modules_names_list(func_ptr_vector_std_string_void_t ptr)
{
	callback_get_active_modules_names_list_ptr = ptr;
}

int PatchsHandler::close_current_oppened_modules()
{
	vector<string> modules_names;
	
	/* We need to get a list of only the active (oppened) modules */
	modules_names = callback_get_active_modules_names_list_ptr();

	for (int m = 0; m < modules_names.size(); m++)
	{
		/* Disconnect Connections*/
		// TODO:
		/* Close Modules*/
		ModSynth::get_instance()->instruments_manager->close_module_pannel_name(modules_names.at(m));
	}

	return 0;
}

int PatchsHandler::disconnect_current_oppened_modules_midi_in_connections()
{
	vector<string> modules_names;
	list<int> connected_to_midi_clients_numbers_list;
	int module_num = 1;
	int client_number, client_id;
	int client_numberclient_number;

	mod_synth_refresh_alsa_clients_data();
	mod_synth_refresh_jack_clients_data();

	if (callback_get_active_modules_names_list_ptr != NULL)
	{
		/* We need to get a list of only the active (oppened) modules */
		modules_names = callback_get_active_modules_names_list_ptr();
	}

	if (modules_names.size() > 0)
	{
		for (string module_name : modules_names)
		{

			/* Get module Midi Input connections */
			client_number = mod_synth_get_midi_client_connection_num(module_name);
			connected_to_midi_clients_numbers_list.clear();
			mod_synth_get_midi_client_connected_to_clients_numbers_list(client_number,
																	  &connected_to_midi_clients_numbers_list);

			for (int client_num : connected_to_midi_clients_numbers_list)
			{
				mod_synth_connect_midi_clients(module_name, client_num, 0, false, false); // disconnect, use client num
			}

			/* Midi outputs */
		}
	}
}

int PatchsHandler::create_active_modules_settings_files(vector<string> mod_names, string patch_file_path)
{
	string settings_file_path;

	for (string module_name : mod_names)
	{
		if (module_name == _INSTRUMENT_NAME_FLUID_SYNTH_STR_KEY)
		{
			settings_file_path = patch_file_path + module_name + "-settings.html";
			mod_synth_save_fluid_synth_preset_file(settings_file_path);
		}
		else if (module_name == _INSTRUMENT_NAME_HAMMON_ORGAN_STR_KEY)
		{
		}
		else if (module_name == _INSTRUMENT_NAME_ANALOG_SYNTH_STR_KEY)
		{
		}
		else if (module_name == _INSTRUMENT_NAME_KARPLUS_STRONG_STRING_SYNTH_STR_KEY)
		{
		}
		else if (module_name == _INSTRUMENT_NAME_MORPHED_SINUS_SYNTH_STR_KEY)
		{
		}
		else if (module_name == _INSTRUMENT_NAME_PADSYNTH_SYNTH_STR_KEY)
		{
		}
		else if (module_name == _INSTRUMENT_NAME_MIDI_PLAYER_STR_KEY)
		{
		}
		else if (module_name == _INSTRUMENT_NAME_MIDI_MIXER_STR_KEY)
		{
		}
		else if (module_name == _INSTRUMENT_NAME_MIDI_MAPPER_STR_KEY)
		{
		}
		else if (module_name == _INSTRUMENT_NAME_REVERB_STR_KEY)
		{
		}
		else if (module_name == _INSTRUMENT_NAME_DISTORTION_STR_KEY)
		{
		}
		else if (module_name == _INSTRUMENT_NAME_GRAPHIC_EQUALIZER_STR_KEY)
		{
		}
		else if (module_name == _INSTRUMENT_NAME_CONTROL_BOX_HANDLER_STR_KEY)
		{
		}
		else if (module_name == _INSTRUMENT_NAME_EXT_MIDI_INT_CONTROL_STR_KEY)
		{
		}
		else if (module_name == _INSTRUMENT_NAME_KEYBOARD_CONTROL_STR_KEY)
		{
		}
	}

	return 0;
}

int PatchsHandler::implement_patch(vector<string> active_modules, vector<string> settings_files,
								   vector<vector<string>> midi_in_connections,
								   string file_path)
{
	string settings_file_path;
	vector<string> midi_input_connections;
	string midi_in_connection;
	int client_num;


	for (int m = 0; m < active_modules.size(); m++)
	{
		/* Open the modules. */
		ModSynth::get_instance()->instruments_manager->open_module_pannel_name(active_modules.at(m));
		/* Settings Files*/
		if (settings_files.at(m) != "")
		{
			settings_file_path = filesystem::path(file_path).parent_path().c_str();
			settings_file_path += "/" + active_modules.at(m) + "-settings.html";

			if (active_modules.at(m) == _INSTRUMENT_NAME_FLUID_SYNTH_STR_KEY)
			{
				ModSynth::get_instance()->get_fluid_synth()->open_fluid_synth_preset_file(
					settings_file_path,
					&ModSynth::get_instance()->get_fluid_synth()->presets[0],
					ModSynth::get_instance()->get_fluid_synth()->presets_summary_str);
			}
		}
	}
	
	/* Set connections after all modules are oppened */
	
	mod_synth_refresh_alsa_clients_data();
	mod_synth_refresh_jack_clients_data();
	
	for (int m = 0; m < active_modules.size(); m++)
	{
		/* Midi In Connections */
		midi_input_connections = midi_in_connections.at(m);
		if (midi_input_connections.size() > 0)
		{
			for (int c = 0; c < midi_input_connections.size(); c++)
			{
				midi_in_connection = midi_input_connections.at(c);
				client_num = mod_synth_get_midi_input_client_id(midi_in_connection, true); // replace virtual clients names
				mod_synth_connect_midi_clients(active_modules.at(m), client_num, 0, true, true); // connect, use index
			}
		}
	}
	
	return 0;
}
