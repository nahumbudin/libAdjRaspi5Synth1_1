/**
* @file		instrumentsManager.h
*	@author		Nahum Budin
*	@date		10-Jul-2024
*	@version	1.0	Initial release
*					
*	@brief		Manages active instruments
*	
*	History:\n
*	
*/

#pragma once

#include <map>
#include <string>

#include "instrument.h"

using namespace std;

class InstrumentsManager
{
  public:
	static InstrumentsManager *get_instance();
	
	~InstrumentsManager();

	int add_instrument(string ins_name, Instrument *inst=NULL);
	int remove_instrument(string ins_name);
	Instrument *get_instrument(string ins_name);

	void close_module_pannel_id(en_modules_ids_t mod_id);
	void close_module_pannel_name(string mod_name);
	void open_module_pannel_name(string mod_name);
	
	int set_instrument_active_midi_channels_mask(string instrument_key_str, uint16_t mask);
	uint16_t get_instrument_active_midi_channels_mask(string instrument_key_str);
	
	en_modules_types_t get_instrument_type(en_modules_ids_t inst_id);
	
	void add_active_instrument(en_modules_ids_t inst);
	void remove_active_instrument(en_modules_ids_t inst);
	
	int allocate_midi_channel_synth(int ch, en_modules_ids_t synth);
	en_modules_ids_t get_allocated_midi_channel_synth(int ch);

	void register_callback_close_module_pannel_id(func_ptr_void_en_modules_ids_t_t ptr);
	void register_callback_close_module_pannel_name(func_ptr_void_string_t ptr);
	void register_callback_open_module_pannel_name(func_ptr_void_string_t ptr);

  private:
	InstrumentsManager();

	static InstrumentsManager *instruments_manager_instance;

	map<string, Instrument*> map_active_instruments;
	map<string, string> map_instruments_names;
	map<en_modules_ids_t, en_modules_types_t> map_instruments_type;
	
	vector <en_modules_ids_t> active_instruments;
	
	en_modules_ids_t midi_channels_allocated_synth[16] = { en_modules_ids_t::none_module_id };
	

	func_ptr_void_en_modules_ids_t_t callback_close_module_pannel_id_ptr;
	func_ptr_void_string_t callback_close_module_pannel_name_ptr;
	func_ptr_void_string_t callback_open_module_pannel_name_ptr;
};
