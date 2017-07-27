// Voltage Pin IN
#define READ_PIN 3
// The pin connected to the transistors base
#define SWITCH_PIN 9
// built-in LED
#define LED_PIN 13
#define REF_VOLT 5.0

//Define ON and OFF values
#define ON 	0xff
#define OFF 0x00

#define SWITCH_CHANNEL 1

//turn on serial debuging
#define DEBUG

// current state of device
byte state = OFF;

// if Z-Wave has asked us to press the switch
bool press_switch = false;

ZUNO_SETUP_CHANNELS(ZUNO_SWITCH_BINARY(getter, setter));


void setup() {
	pinMode(SWITCH_PIN, OUTPUT);
	pinMode(READ_PIN, INPUT);
	openingCeremony();

#ifdef DEBUG
	//Serial begin for Debugging
	Serial.begin();
#endif
}


// the loop routine runs over and over again forever:
void loop() {
	if (press_switch){
		digitalWrite (SWITCH_PIN, HIGH);
		delay(1000);
		digitalWrite (SWITCH_PIN, LOW);
		press_switch = false;
	}

	//Check for state here
	int input = analogRead(READ_PIN);
	if (input > 512 && state == OFF){
		//IF pin 3 is HIGH, and we are OFF
		//Set to ON and notify controller
		state = ON;
		zunoSendReport(SWITCH_CHANNEL);
	} else if(input < 512 && state == ON){
		//IF pin 3 is LOW, and we are ON
		//Set to OFF and notify controller
		state = OFF;
		zunoSendReport(SWITCH_CHANNEL);
	}
}
void setter(byte value) {
	//press switch if we get a command, on or off.
#ifdef DEBUG
	Serial.print("Got Value: ");
	Serial.print(value, DEC);
	Serial.println();
#endif
	press_switch = true;
}

byte getter() {
	return state
}

void openingCeremony() {
	// ---...
	digitalWrite (LED_PIN, HIGH);
	delay(270);
	digitalWrite (LED_PIN, LOW);
	delay(270);
	digitalWrite (LED_PIN, HIGH);
	delay(270);
	digitalWrite (LED_PIN, LOW);
	delay(270);
	digitalWrite (LED_PIN, HIGH);
	delay(270);
	digitalWrite (LED_PIN, LOW);
	delay(270);

	digitalWrite (LED_PIN, HIGH);
	delay(90);
	digitalWrite (LED_PIN, LOW);
	delay(90);
	digitalWrite (LED_PIN, HIGH);
	delay(90);
	digitalWrite (LED_PIN, LOW);
	delay(90);
	digitalWrite (LED_PIN, HIGH);
	delay(90);
	digitalWrite (LED_PIN, LOW);
}

float getVoltage(int value){
	//returns voltage between 0 = 0v 1023.0 = REF_VOLT
	return value * (REF_VOLT / 1023.0);
}

