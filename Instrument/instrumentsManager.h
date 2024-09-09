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

	int add_instrument(string ins_name, Instrument *instrument=NULL);
	int remove_instrument(string ins_name);
	Instrument *get_instrument(string ins_name);

	void close_module_pannel_id(en_modules_ids_t mod_id);
	void close_module_pannel_name(string mod_name);
	void open_module_pannel_name(string mod_name);

	void register_callback_close_module_pannel_id(func_ptr_void_en_modules_ids_t_t ptr);
	void register_callback_close_module_pannel_name(func_ptr_void_string_t ptr);
	void register_callback_open_module_pannel_name(func_ptr_void_string_t ptr);

  private:
	InstrumentsManager();

	static InstrumentsManager *instruments_manager_instance;

	map<string, Instrument*> active_instruments;

	func_ptr_void_en_modules_ids_t_t callback_close_module_pannel_id_ptr;
	func_ptr_void_string_t callback_close_module_pannel_name_ptr;
	func_ptr_void_string_t callback_open_module_pannel_name_ptr;
};
