/**
* @file		instrumentControlBoxHandler.cpp
*	@author		Nahum Budin
*	@date		7-Jul-2024
*	@version	1.0	Initial release
*					
*	@brief		Provides an events handler to handle the control box events.
*	
*	History:\n
*	
*/

#include "instrumentControlBoxHandler.h"

InstrumentControlBoxEventsHandler::InstrumentControlBoxEventsHandler()
	: Instrument(_INSTRUMENT_NAME_CONTROL_BOX_HANDLER_STR_KEY, true, true, false)
{
	
	alsa_midi_sequencer_events_handler->set_instrument(this);

	for (int s = 0; s < _NUM_OF_CONTROL_BOX_SLIDERS; s++)
	{
		/* These values will be set for each individual sliders to
		 * compensate for the ranges variability of the sliders */
		sliders_input_value_max[s] = 2000;
		sliders_input_value_min[s] = 100;
	}
	
	/* Control box input handles all channels */
	this->alsa_midi_sequencer_events_handler->set_active_midi_channels(0xffff);
}

InstrumentControlBoxEventsHandler::~InstrumentControlBoxEventsHandler()
{
	
}

void InstrumentControlBoxEventsHandler::sysex_handler(uint8_t *message, int len)
{
	int event_id, slider_num;
	uint16_t val;

	if ((*(message + 1) == _MIDI_CONTROL_BOX_SYSEX_VENDOR_ID_0) &&
		(*(message + 2) == _MIDI_CONTROL_BOX_SYSEX_VENDOR_ID_1) &&
		(*(message + 3) == _MIDI_CONTROL_BOX_SYSEX_VENDOR_ID_2))
	{
		/* A control box message */
		event_id = *(message + 4);
		val = (*(message + 5) << 7) + *(message + 6);

		if ((event_id >= _CONTROL_SLIDER_BLUE_BLACK) && (event_id <= _CONTROL_SLIDER_ORANGE_YELLOW))
		{
			/* Slider command */
			slider_num = event_id - _CONTROL_SLIDER_BLUE_BLACK;
			/* Hold each slider actual min and max values */
			if (val > sliders_input_value_max[slider_num])
			{
				sliders_input_value_max[slider_num] = val;
			}
			else if (val < sliders_input_value_min[slider_num])
			{
				sliders_input_value_min[slider_num] = val;
			}
			
			/* Normalize  to 0-100*/
			val = (int)(((float)(val - sliders_input_value_min[slider_num]) /
						(float)(sliders_input_value_max[slider_num] - sliders_input_value_min[slider_num])) *
				  100.0);

			if (val > 100)
			{
				val = 100;
			}
			else if (val < 0)
			{
				val = 0;
			}
		}

		control_box_events_handler(event_id, val);
	}
}

void InstrumentControlBoxEventsHandler::register_callback_control_box_event_update_ui(func_ptr_void_int_uint16_t ptr)
{
	int i;
	bool found = false;

	std::list<func_ptr_void_int_uint16_t>::iterator it_clbk =
		callback_ptr_control_box_event_update_ui_list.begin();
	/* Already registered? */
	for (i = 0; i < callback_ptr_control_box_event_update_ui_list.size(); i++)
	{
		if (*it_clbk == ptr)
		{
			/* Already registered */
			found = true;
			break;
		}

		it_clbk++;
	}

	if (!found)
	{
		callback_ptr_control_box_event_update_ui_list.push_back(ptr);
	}
}

void InstrumentControlBoxEventsHandler::unregister_callback_control_box_event_update_ui(func_ptr_void_int_uint16_t ptr)
{
	callback_ptr_control_box_event_update_ui_list.remove(ptr);
}

void InstrumentControlBoxEventsHandler::control_box_events_handler(int event_id, uint16_t event_value)
{
	int i;
	func_ptr_void_int_uint16_t clbk;

	std::list<func_ptr_void_int_uint16_t>::iterator it_clbk =
		callback_ptr_control_box_event_update_ui_list.begin();

	for (i = 0; i < callback_ptr_control_box_event_update_ui_list.size(); i++)
	{
		if (*it_clbk != NULL)
		{
			clbk = *it_clbk;
			clbk(event_id, event_value);
		}

		it_clbk++;
	}
}