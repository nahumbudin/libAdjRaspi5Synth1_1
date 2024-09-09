/**
* @file			patch.h
*	@author		Nahum Budin
*	@date		30-Aug-2024
*	@version	1.0
*	
*	@brief		Provides the Patches handling related API
*	
*/

#pragma once

#include "types.h"

int mod_synth_save_patch_file(std::string file_path);
int mod_synth_load_patch_file(std::string file_path);

void mod_synth_register_callback_get_active_modules_names_list(func_ptr_vector_std_string_void_t ptr);


