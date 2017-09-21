
#include "PatternSwitch.h"

PatternSwitch::PatternSwitch (uint16_t timeout_ms, uint8_t tollerance)
{
	ck_timeout = timeout_ms / PATTERN_RESOLUTION_MS;
	this -> tollerance = tollerance;

	event_success = NULL;
	event_fail = NULL;
	event_record_complete = NULL;
	event_handler = NULL;

	len = 0;
	recording = false;
	reset();
}


void PatternSwitch::trigger()
{
	uint32_t ck = millis() / PATTERN_RESOLUTION_MS;
	uint32_t diff = ck - ck_last;
	ck_last = ck;
	//Serial << " d:" << diff << " ";
	counter ++;

	// ignore first trigger
	if (counter == 1)
		return;

	// second trigger : calculate time base and speed coefficient
	if (counter == 2 && !recording)
	{
		if (len < 2)
			return;

		k_speed = (float) timetable[pos++] / diff;
		return;
	}
	// next triggers : compare intervals to timetable

	if (recording)
	{	
		// recording mode
		timetable[pos++] = diff;
		if (diff >= ck_timeout || pos >= PATTERN_SIZE)
			stopRecord();			
	}
	else
	{	
		// listening mode
		uint8_t val = timetable[pos++];
		diff *= k_speed;
		ck_toll = val * tollerance / 100;

		if (val <= diff + ck_toll && val >= diff - ck_toll)
		{
			if (pos == len)
			{
				dispatchEvent (SUCCESS);
				reset();
			}
		}
		else
		{
			dispatchEvent (FAIL);
			reset();
		}
	}
}

uint8_t PatternSwitch::getProgress()
{
	return counter;
}

uint8_t PatternSwitch::getLength()
{
	return len;
}

void PatternSwitch::loop()
{
	
	if (counter > 0 && (uint32_t (millis() / PATTERN_RESOLUTION_MS) - ck_last > ck_timeout))
	{
		if (recording && counter > 1)
			stopRecord();
		else if (!recording)
		{
			dispatchEvent (counter == 1 ? TIMEOUT : FAIL);
			reset();
		}
	}
	
}

void PatternSwitch::reset()
{
	counter = 0;
	pos = 0;
}

void PatternSwitch::startRecord ()
{
	len = 0;
	recording = true;
	reset();
}

void PatternSwitch::stopRecord ()
{	
	len = pos;
	recording = false;
	dispatchEvent (RECORDED);
	reset();
}



void PatternSwitch::dispatchEvent (event_t ev)
{
	switch (ev)
	{
		case SUCCESS:
			if (event_success != NULL)
				event_success();
			break;
		case FAIL:
			if (event_fail != NULL)
				event_fail();
			break;
		case RECORDED:
			if (event_record_complete != NULL)
				event_record_complete();
			break;
		case TIMEOUT:
			if (event_timeout != NULL)
				event_timeout();
			break;
	}
	if (event_handler != NULL)
		event_handler (ev);
}

boolean PatternSwitch::saveToEEPROM (uint8_t base_addr)
{
	if (len > 255 - base_addr)
		return false;
	EEPROM.write(base_addr, len);
	EEPROM.put (base_addr + 1, timetable);
	return true;
}

boolean PatternSwitch::loadFromEEPROM (uint8_t base_addr)
{
	len = EEPROM.read(base_addr);
	if (len > 255 - base_addr || len == 0  || len == 0xff)
		return false;
	EEPROM.get (base_addr + 1, timetable);
	return true;
}

void PatternSwitch::setEvent (void (*func)(event_t ev))
{
	event_handler = func;
}

void PatternSwitch::onSuccess (void (*func)())
{
	event_success = func;
}

void PatternSwitch::onFail (void (*func)())
{
	event_fail = func;
}

void PatternSwitch::onRecordComplete (void (*func)())
{
	event_record_complete = func;
}

void PatternSwitch::onTimeout (void (*func)())
{
	event_timeout = func;
}

