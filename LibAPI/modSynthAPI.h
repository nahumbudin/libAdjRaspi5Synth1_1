/**
* @file			types.h
*	@author		Nahum Budin
*	@date		6-Jul-2024
*	@version	1.0
*	
*	@brief		Provides the synthesizer high level API
*	
*	Based on libAdjHeartModSynth_1.h Ver 1.3 9-Jan-2021
*/

#pragma once

#include <string>
#include <list>

#include "../LibAPI/types.h"

using namespace std;

class Instrument;

/**
*   @brief  Initializes the synthesizer engine.
*			Must be called first before any other call.
*   @param  none
*   @return 0 if done
*/
int mod_synth_init();

/**
*   @brief  Initializes the Bluetooth services.
*   @param  none
*   @return 0 if done
*/
int mod_synth_init_bt_services();

/**
*   @brief  Deinitializes the Bluetooth services.
*			terminate al BT related threads
*   @param  none
*   @return 0 if done
*/
int mod_synth_deinit_bt_services();

/**
*   @brief  Initializes the external MIDI interface service.
*   @param  int port_num  serial port number
*   @return 0 if done
*/
int mod_synth_init_ext_midi_services(int port_num);

/**
*   @brief  Deinitializes the external MIDI interface service.
*   @param  none
*   @return 0 if done
*/
int mod_synth_deinit_ext_midi_services();

/**
*   @brief  Initializes the MIDI streaming service.
*   @param  none
*   @return 0 if done
*/
int mod_synth_init_midi_services();

/**
*   @brief  Deinitializes the MIDI streaming service.
*   @param  none
*   @return 0 if done
*/
int mod_synth_deinit_midi_services();

/**
*   @brief  Closing and cleanup when application goes down.
*			Must be called whenever terminating the application.
*   @param  none
*   @return void
*/
void mod_synth_on_exit();	

/**
*   @brief  Returns the total (all cores) CPU utilization.
*   @param  none
*   @return int	the total (all cores) CPU utilization in precetages (0 to 100).
*/
int mod_synth_get_cpu_utilization();

/**
*   @brief  Adds an active instrument.
*   @param  string	instrument name string
*   @param	pointer to the instrument instance
*   @return 0 if done
*/
int mod_synth_add_module(string ins_name, Instrument *instrument=NULL);

/**
*   @brief  Removes an active instrument.
*   @param  string	instrument name string
*   @return 0 if done
*/
int mod_synth_remove_module(string ins_name);







/**
*   @brief  Register a callback function that closes a module on the main window.
*   @param  func_ptr_void_en_modules_ids_t_t  a pointer to the callback function 
*													( void func(en_modules_ids_t) ) (module id)
*   @return void
*/
void mod_synth_register_callback_wrapper_close_module_pannel_id(func_ptr_void_en_modules_ids_t_t ptr);

/**
*   @brief  Register a callback function that closes a module on the main window.
*   @param  func_ptr_void_string_t  a pointer to the callback function 
*													( void func(std::string) ) (module name)
*   @return void
*/
void mod_synth_register_callback_wrapper_close_module_pannel_name(func_ptr_void_string_t ptr);

/**
*   @brief  Register a callback function that opens a module on the main window.
*   @param  func_ptr_void_string_t  a pointer to the callback function 
*													( void func(std::string) ) (module name)
*   @return void
*/
void mod_synth_register_callback_wrapper_open_module_pannel_name(func_ptr_void_string_t ptr);
