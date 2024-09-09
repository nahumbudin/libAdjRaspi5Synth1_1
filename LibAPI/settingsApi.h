/**
* @file			settingsApi.h
*	@author		Nahum Budin
*	@date		25-Jul-2024
*	@version	1.0
*	
*	@brief		Settings API
*	
*	Based on libAdjHeartModSynth_1.h Ver 1.3 9-Jan-2021
*/

#pragma once

#include <string>

#define _SAVE_FLUID_SETTINGS (1 << 1)

#define _READ_FLUID_SETTINGS _SAVE_FLUID_SETTINGS

/**
*   @brief  Save the active FluidSynth settings parameters as XML file
*   @param  path settings XML file full path
*   @return 0 if done
*/
int mod_synth_save_fluid_synth_settings_file(std::string path);

/**
*   @brief  Open settings parameters XML file and set it as the FluidSynth active settings
*   @param  path settings XML file full path
*   @return 0 if done
*/
int mod_synth_open_fluid_synth_settings_file(std::string path);
