/**
* @file		instrumentsManager.cpp
*	@author		Nahum Budin
*	@date		10-Jul-2024
*	@version	1.0	Initial release
*					
*	@brief		Manages active instruments
*	
*	History:\n
*	
*/

#include <stddef.h>

#include "instrumentsManager.h"

InstrumentsManager *InstrumentsManager::instruments_manager_instance = NULL;

InstrumentsManager::InstrumentsManager(){
	
	callback_close_module_pannel_id_ptr = NULL;
	callback_close_module_pannel_name_ptr = NULL;
}

InstrumentsManager::~InstrumentsManager()
{
	
	
}

InstrumentsManager *InstrumentsManager::get_instance()
{
	if (instruments_manager_instance == NULL)
	{
		instruments_manager_instance = new InstrumentsManager();
	}

	return instruments_manager_instance;
}

int InstrumentsManager::add_instrument(string ins_name, Instrument *instrument)
{
	if (instrument == NULL)
	{
		//TODO: find base on name?
	}

	if (active_instruments.find(ins_name) == active_instruments.end())
	{
		/* Instrument not found - add it*/
		active_instruments.insert({ins_name, instrument});
		
		return 0;
	}
	/* Instrument already exists */
	return -1;
}

int InstrumentsManager::remove_instrument(string ins_name){
	
	active_instruments.extract(ins_name);

	return 0;
}

Instrument *InstrumentsManager::get_instrument(std::string ins_name)
{
	if (active_instruments.find(ins_name) != active_instruments.end())
	{
		/* Instrument found */
		return active_instruments[ins_name];
	}
	/* Not Found*/
	return NULL;
}


void InstrumentsManager::close_module_pannel_id(en_modules_ids_t mod_id)
{
	if (callback_close_module_pannel_id_ptr != NULL)
	{
		callback_close_module_pannel_id_ptr(mod_id);
	}
}

void InstrumentsManager::close_module_pannel_name(string mod_name)
{
	if (callback_close_module_pannel_name_ptr != NULL)
	{
		callback_close_module_pannel_name_ptr(mod_name);
	}
}

void InstrumentsManager::open_module_pannel_name(string mod_name)
{
	if (callback_open_module_pannel_name_ptr != NULL)
	{
		callback_open_module_pannel_name_ptr(mod_name);
	}
}

void InstrumentsManager::register_callback_close_module_pannel_id(func_ptr_void_en_modules_ids_t_t ptr)
{
	callback_close_module_pannel_id_ptr = ptr;
}

void InstrumentsManager::register_callback_close_module_pannel_name(func_ptr_void_string_t ptr)
{
	callback_close_module_pannel_name_ptr = ptr;
}

void InstrumentsManager::register_callback_open_module_pannel_name(func_ptr_void_string_t ptr)
{
	callback_open_module_pannel_name_ptr = ptr;
}
