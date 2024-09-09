/**
* @file		patch.h
*	@author		Nahum Budin
*	@date		29-Aug-2024
*	@version	1.0
*					
*	
*	@brief		Modular synthersizer patches handling.
*
*	History: 
*/

/**
*	Here we handle the sysnthesizer patches.
*	A patch is a set of active modules and their connected MIDI and audio connections.
*	Patches are stored as hirarcial JSON files.
*	A patch general structure looks like:
*	
*	{
*		"name": "patch name string",
*		"modules" : [
*			{
*				"name": "module  1 name string id",
*				"settings_file: "module 1 settings.html file name",
*				"connections": [
*					{ "midi input connections": [
*						{ connection 1 TBD },
*						{ connection 2 TBD },
*						{ connectin n TBD } ]
*					},
*					{ "midi output connections": [
*						{ connection 1 TBD },
*						{ connection 2 TBD },
*						{ connectin n TBD } ]
*					},
*					{ "jack audio output connections": [
*						{ connection 1 TBD },
*						{ connection 2 TBD },
*						{ connectin n TBD } ]
*					},
*					{ "jack audio input connections": [
*						{ connection 1 TBD },
*						{ connection 2 TBD },
*						{ connectin n TBD } ]
*					} ],
*			},
*			{
*				"name": "module  2 name string id",
*				"settings_file: "module 2 settings.html file name",
*				"connections": [
*				
*			}]
*				
*	}
*
*/

#pragma once

#include "../rapidjson/document.h"

#include "../LibAPI/types.h"


class PatchsHandler
{
  public:
	~PatchsHandler();

	static PatchsHandler *get_patchs_handler_instance();

	int save_patch_file(std::string file_path);
	int load_patch_file(std::string file_path);

	int close_current_oppened_modules();
	int disconnect_current_oppened_modules_midi_in_connections();

	void register_callback_get_active_modules_names_list(func_ptr_vector_std_string_void_t ptr);

  private:
	PatchsHandler();

	int create_active_modules_settings_files(vector<string> mod_names, string patch_file_path);

	int implement_patch(vector<string> active_modules, vector<string> settings_files,
						vector<vector<string>> midi_in_connections,
						string file_path);

	static PatchsHandler *patch_handler_instance;

	func_ptr_vector_std_string_void_t callback_get_active_modules_names_list_ptr;
};
