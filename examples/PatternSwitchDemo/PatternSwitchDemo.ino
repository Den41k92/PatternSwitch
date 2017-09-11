/*	PatternSwitch example
 *	https://github.com/DireCat/PatternSwitch/
 */

// Connect hardware buttons to these pins
#define KEY_TRIGGER		4	// knock button
#define KEY_RECORD  	5	// record mode button

#include <PatternSwitch.h>

// Create a pattern detector
// Timeout = 2000 ms, max. timing error +/- 30%
PatternSwitch pattern (2000, 30);

// hardware buttons polling function
uint8_t getKey ()
{
	static uint8_t previous_key = 0;
	uint8_t key = 0;
	if (!digitalRead(KEY_RECORD))
		key = KEY_RECORD;
	else if (!digitalRead(KEY_TRIGGER))
		key = KEY_TRIGGER;
	else
		key = 0;

	if (key == previous_key)
		return 0;
		
	previous_key = key;
	return key;
}

// pattern match function
void success()
{
	Serial.println("\n[i] *** Match! ***");
}

// pattern non-match function
void fail()
{
	Serial.println("\n[x] Wrong pattern. Try again");
}

// pattern record complete function
void recordComplete()
{
	Serial.print("Record complete. Samples: ");
	Serial.println(pattern.getProgress());

	// save pattern to EEPROM
	if (pattern.saveToEEPROM())
		Serial.println("saved to eeprom.");
}

void setup()
{
	Serial.begin(115200);
	
	// hardware buttons must be connected to GND
	pinMode (KEY_TRIGGER, INPUT_PULLUP);
	pinMode (KEY_RECORD, INPUT_PULLUP);

	// load a pattern from eeprom if exists
	if (pattern.loadFromEEPROM())		
		Serial.println("Using pattern from EEPROM  or");
		
	Serial.println("Press key 'record' to record a new pattern");

	pattern.onSuccess(success);		// match event callback
	pattern.onFail (fail);				// non-match event callback
	pattern.onRecordComplete(recordComplete);		// record complete event callback
}


void loop()
{
	pattern.loop();
	if (getKey() == KEY_RECORD)
	{
		pattern.startRecord();
		Serial.println("recording started ");
	}

	if (getKey() == KEY_TRIGGER)
	{
		pattern.trigger();
		Serial.print('.');
	}
}
