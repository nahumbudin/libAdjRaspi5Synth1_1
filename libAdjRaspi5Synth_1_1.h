/**
* @file		libAdjRaspi5Synth_1.h
*	@author		Nahum Budin
*	@date		8-May-2024
*	@version	1.0
*
*
*	History:\n
*
*	
*
*	@brief		A raspberry Pi multi-core polyphonic music synthesizer library api.
*
*	\mainpage AdjHeart Polyphonic Synthesizer
*
*	This library was writen to run over a Raspberry Pi 5B.\n
*	The librarry implements a modular synthesizer \n
* 
* 
*	It provides a full API supporting all features incluuding callback functions for updates and events initiation.
*
*/

#pragma once


#include "libAdjRaspi5Synth_1_1.h"
#include "modSynth.h"
#include "./LibAPI/types.h"

#include <cstdint>

// #ifdef __cplusplus
// extern "C" {
// #endif


#define _PAD_SHAPE									1531		

#define _PAD_SHAPE_RECTANGULAR						0
#define _PAD_SHAPE_GAUSSIAN							1
#define _PAD_SHAPE_DOUBLE_EXP						2


#define _PAD_SHAPE_CUTOFF							1540		

#define _PAD_SHAPE_CUTOFF_FULL						0
#define _PAD_SHAPE_CUTOFF_UPPER						1
#define _PAD_SHAPE_CUTOFF_LOWER						2


void callback_message_id(int);


// #ifdef __cplusplus
// }
// #endif

