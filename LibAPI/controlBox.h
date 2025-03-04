/**
* @file			controlBox.h
*	@author		Nahum Budin
*	@date		6-Jul-2024
*	@version	1.0
*	
*	@brief		MIDI control box API
*	
*	Based on libAdjHeartModSynth_1.h Ver 1.3 9-Jan-2021
*/

#pragma once

/* Sysex Vendor ID - TEMP TODO: */
#define _MIDI_CONTROL_BOX_SYSEX_VENDOR_ID_0 0
#define _MIDI_CONTROL_BOX_SYSEX_VENDOR_ID_1 0x35
#define _MIDI_CONTROL_BOX_SYSEX_VENDOR_ID_2 0x56

/* Controls IDs */
#define _CONTROL_PUSHBUTTON_BLUE_BLACK		0x10
#define _CONTROL_PUSHBUTTON_BLUE_GREEN		0x11
#define _CONTROL_PUSHBUTTON_BLUE_RED		0x12
#define _CONTROL_PUSHBUTTON_BLUE_BLUE		0x13
#define _CONTROL_PUSHBUTTON_BLUE_WHITE		0x14
#define _CONTROL_PUSHBUTTON_BLUE_YELLOW		0x15
#define _CONTROL_PUSHBUTTON_ORANGE_BLACK	0x16
#define _CONTROL_PUSHBUTTON_ORANGE_GREEN	0x17
#define _CONTROL_PUSHBUTTON_ORANGE_RED		0x18
#define _CONTROL_PUSHBUTTON_ORANGE_BLUE		0x19
#define _CONTROL_PUSHBUTTON_ORANGE_WHITE	0x1a
#define _CONTROL_PUSHBUTTON_ORANGE_YELLOW	0x1b

#define _CONTROL_FUNCTION_PUSHBUTTON_UP		0x30
#define _CONTROL_FUNCTION_PUSHBUTTON_REDOO	0x31
#define _CONTROL_FUNCTION_PUSHBUTTON_ENTER	0x32

#define _CONTROL_ENCODER_BLUE_PURPLE		0x40
#define _CONTROL_ENCODER_BLUE_GREEN			0x41
#define _CONTROL_ENCODER_BLUE_RED			0x42
#define _CONTROL_ENCODER_BLUE_BLUE			0x43
#define _CONTROL_ENCODER_BLUE_WHITE			0x44
#define _CONTROL_ENCODER_BLUE_YELLOW		0x45
#define _CONTROL_ENCODER_ORANGE_PURPLE		0x46
#define _CONTROL_ENCODER_ORANGE_GREEN		0x47
#define _CONTROL_ENCODER_ORANGE_RED			0x48
#define _CONTROL_ENCODER_ORANGE_BLUE		0x49
#define _CONTROL_ENCODER_ORANGE_WHITE		0x4a
#define _CONTROL_ENCODER_ORANGE_YELLOW		0x4b

#define _CONTROL_ENCODER_PUSHBUTTON_BLUE_BLACK		0x50
#define _CONTROL_ENCODER_PUSHBUTTON_BLUE_GREEN		0x51
#define _CONTROL_ENCODER_PUSHBUTTON_BLUE_RED		0x52
#define _CONTROL_ENCODER_PUSHBUTTON_BLUE_BLUE		0x53
#define _CONTROL_ENCODER_PUSHBUTTON_BLUE_WHITE		0x54
#define _CONTROL_ENCODER_PUSHBUTTON_BLUE_YELLOW		0x55
#define _CONTROL_ENCODER_PUSHBUTTON_ORANGE_BLACK	0x56
#define _CONTROL_ENCODER_PUSHBUTTON_ORANGE_GREEN	0x57
#define _CONTROL_ENCODER_PUSHBUTTON_ORANGE_RED		0x58
#define _CONTROL_ENCODER_PUSHBUTTON_ORANGE_BLUE		0x59
#define _CONTROL_ENCODER_PUSHBUTTON_ORANGE_WHITE	0x5a
#define _CONTROL_ENCODER_PUSHBUTTON_ORANGE_YELLOW	0x5b

#define _CONTROL_SLIDER_BLUE_BLACK					0x60
#define _CONTROL_SLIDER_BLUE_GRAY					0x61
#define _CONTROL_SLIDER_BLUE_RED					0x62
#define _CONTROL_SLIDER_BLUE_BLUE					0x63
#define _CONTROL_SLIDER_BLUE_WHITE					0x64
#define _CONTROL_SLIDER_BLUE_YELLOW					0x65
#define _CONTROL_SLIDER_ORANGE_BLACK				0x66
#define _CONTROL_SLIDER_ORANGE_GRAY					0x67
#define _CONTROL_SLIDER_ORANGE_RED					0x68
#define _CONTROL_SLIDER_ORANGE_BLUE					0x69
#define _CONTROL_SLIDER_ORANGE_WHITE				0x6a
#define _CONTROL_SLIDER_ORANGE_YELLOW				0x6b

#define _CONTROL_YELLOW_LED							0x70
#define _CONTROL_GREEN_LED							0x71
