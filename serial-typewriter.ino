#include <AltSoftSerial.h>

#include <SoftwareSerial.h>

// TODO add pin numbers
#define RX 2
#define TX 3
#define EN 4
#define S10 5
#define S11 6
#define S12 7
#define S13 12
#define S20 13
#define S21 10
#define S22 11

int TWcolPins[] = {S10, S11, S12, S13};
int TWrowPins[] = {S20, S21, S22};

int KBrowPins[] = {A2, A1, A0};
int KBcolPins[] = {3, 2, A5, A4};

int colChannels[][4] = {
	{0, 0, 0, 0},
	{0, 0, 0, 1}, 
	{0, 0, 1, 0}, 
	{0, 0, 1, 1}, 
	{0, 1, 0, 0}, 
	{0, 1, 0, 1}, 
	{0, 1, 1, 0}, 
	{0, 1, 1, 1},
	{1, 0, 0, 0},
	{1, 0, 0, 1}, 
	{1, 0, 1, 0}, 
	{1, 0, 1, 1}, 
	{1, 1, 0, 0}, 
	{1, 1, 0, 1}, 
	{1, 1, 1, 0}, 
	{1, 1, 1, 1}
	};

int rowChannels[][3] = {
	{0, 0, 0},
	{0, 0, 1}, 
	{0, 1, 0}, 
	{0, 1, 1}, 
	{1, 0, 0}, 
	{1, 0, 1}, 
	{1, 1, 0}, 
	{1, 1, 1}
	};

// TODO: Everything with a keycode of 0 is 'i'
// TODO: No keycodes for newline, backspace, etc.
int asciiToKeycodes[] = {
	84, // SPACE
	42, // !
	40, // "
	18, // #
	27, // $
	51, // %
	21, // &
	40, // '
	53, // (
	20, // )
	54, // *
	61, // +
	71, // ,
	9, // -
	62, // .
	72, // /
	20, // 0
	42, // 1
	29, // 2
	18, // 3
	27, // 4
	51, // 5
	31, // 6
	21, // 7
	54, // 8
	53, // 9
	49, // :
	49, // ;
	-1, // < TODO
	61, // =
	-1, // > TODO
	72, // ?
	29, // @
	37, // A 
	56, // B
	58, // C
	25, // D
	14, // E
	35, // F
	23, // G
	46, // H
	0, // I
	22, // J
	44, // K
	34, // L
	55, // M
	67, // N
	16, // O
	5, // P
	47, // Q
	2, // R
	26, // S
	13, // T
	11, // U
	68, // V
	4, // W
	70, // X
	1, // Y
	59, // Z
	39, // [
	-1, // Backslash
	39, // ]
	-1, // ^
	9, // _
	40, // `
	37, // a 
	56, // b
	58, // c
	25, // d
	14, // e
	35, // f
	23, // g
	46, // h
	0, // i
	22, // j
	44, // k
	34, // l
	55, // m
	67, // n
	16, // o
	5, // p
	47, // q
	2, // r
	26, // s
	13, // t
	11, // u
	68, // v
	4, // w
	70, // x
	1, // y
	59, // z
	-1, // {
	-1, // |
	-1, // }
	9 // ~
	};

int keycodeToAscii[] = {
    105,     // i
    121,    // y
    114,    // r
    0,      // unused
    119,    // w
    112,    // p
    0,      // not a character
    0,
    0,
    45,     // -
    0,
    117,    // u
    0,
    116,    // t
    101,    // e
    0,
    111,    // o
    0,
    51,     // 3
    0,
    48,     // 0
    55,     // 7
    106,    // j
    103,    // g
    0,
    100,    // d
    115,    // s
    52,     // 4
    0,
    50,     // 2
    32,		// L MAR but actually space since the last row isn't working
    54,     // 6
    177,    // plus or minus
    0,
    108,    // l
    102,    // f
    0,
    97,     // a
    189,    // 1/2
    93,     // ]
    39,     // '
    0,
    49,     // 1
    0,
    107,    // k
    0,
    104,    // h
    113,    // q
    0,
    38,     // ;
    177,    // plus or minus, again???? still confused about that
    53,     // 5
    0,
    57,     // 9
    56,     // 8
    109,    // m
    98,		// b
	0,
	99,		// c
	122,	// z
	10,		// return
	61,		// =
	46, 	// .
	0,
	0,
	0,
	0,
	110,	// n
	118,	// v
	0,
	120, 	// x
	44,		// ,
	47,		// /
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	8,		// backspace
	0,
	' ',		// space
	0,
	0,
	0
};


AltSoftSerial PiSerial;

//SoftwareSerial PiSerial = SoftwareSerial(8,9);
int lastKeycode = -1;
bool newChar = false;
bool flowControl = false;

bool isBold = false;

int now = 0;
bool keyDown;

int lastKBKeycode;
int last = 0;
long lastKeyPressedTime;

void setup() {
	pinMode(EN, OUTPUT);
	digitalWrite(EN, HIGH);

	pinMode(RX, INPUT);
	pinMode(TX, OUTPUT);  

	for (int i = 0; i < sizeof(TWrowPins) / sizeof(int); i++) {
		pinMode(TWrowPins[i], OUTPUT);
	}
	for (int i = 0; i < sizeof(TWcolPins) / sizeof(int); i++) {
		pinMode(TWcolPins[i], OUTPUT);
	}
	for (int i = 0; i < sizeof(KBrowPins) / sizeof(int); i++) {
    	pinMode(KBrowPins[i], OUTPUT);
  	}
  	for (int i = 0; i < sizeof(KBcolPins) / sizeof(int); i++) {
    	pinMode(KBcolPins[i], OUTPUT);
  	}

	pinMode(A3, INPUT);

	Serial.begin(1200);
	PiSerial.begin(1200);
 
	PiSerial.write(0x11); // XON
}

void loop() {
	char incoming = 0;
	if (PiSerial.available() >= 24 && flowControl == false) {
		PiSerial.write(0x13); // XOFF
		flowControl = true;
	} else if (flowControl == true && PiSerial.available() < 8) {
		PiSerial.write(0x11); // XON
		flowControl = false;
	}

	if (PiSerial.available() ) {//&& !keyDown && millis() - now > 38 * 2) {
		incoming = PiSerial.read();
		if (incoming == '\e') {
			PiSerial.read();
			int param;
			param = PiSerial.parseInt(SKIP_NONE);
			switch (param) {
				case 0:
					unBold();
					break;
				case 1:
					bold();
					break;
				default:
					break;
			}
			PiSerial.read();
			incoming = 0;
		} else {
			keyDown = true;
			Serial.write(incoming);
			//PiSerial.write(0x13); // XOFF
    		typeChar(incoming);
			now = millis();
		}
	}

	// if (keyDown && millis() - now > 38) {
	// 	sendKeyUp();
	// 	keyDown = false;
	// 	PiSerial.write(0x11); // XON
	// }

	int keycode = -1;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 11; j++) {
			selectKBRow(i);
			selectKBCol(j);
			if (digitalRead(A3) == HIGH) {
				keycode = i * 11 + j;
			}
		}
	}
	if (keycode != -1 && keycode != lastKBKeycode) {
		char character = keycodeToAscii[keycode];
		lastKBKeycode = keycode;
		PiSerial.write(character);
	} else if (keycode == -1) {
		lastKBKeycode = -1;
	}

	if (Serial.available()) {
		PiSerial.write(Serial.read());
	}
}

void selectRow(int row) {
	for (int pin = 0; pin < 3; pin++) {
		digitalWrite(TWrowPins[pin], rowChannels[row][pin]);
	}
}

void selectCol(int col) {
	for (int pin = 0; pin < 4; pin++) {
		digitalWrite(TWcolPins[pin], colChannels[col][pin]);
	}
}

void selectKBRow(int row) {
	for (int pin = 0; pin < 3; pin++) {
		digitalWrite(KBrowPins[pin], rowChannels[row][pin]);
	}
}

void selectKBCol(int col) {
	for (int pin = 0; pin < 4; pin++) {
		digitalWrite(KBcolPins[pin], colChannels[col][pin]);
	}
}

int charToRow(char c) {
	int keycode = charToKeycode(c);
	return keycode/11; // I think this works due to rounding?
}

int charToCol(char c) {
	int keycode = charToKeycode(c);
	return keycode % 11;
}

int charToKeycode(char c) {
	switch (c) {
		case '\n': // Newline
			return 60;
			break;
		case '\r':
			return -1;
			break;
		default:
			return asciiToKeycodes[c - 32];
			break;
	}
}

void typeChar(char c) {
	int keycode = charToKeycode(c);

	if ((c < ' ' && c != '\n') || c > '~') {
		keycode = -1;
	}

	// Check if c is "uppercase"
	if ((c >= 'A' && c <= 'Z') 
		||(c >= '!' && c <= '&')
		||(c >= '(' && c <= '+')
		|| c == ':' || c == '?'
		|| c == '@' || c == '['
		|| c == '_') {
			selectCol(8);
			selectRow(7);
			digitalWrite(EN, LOW);
			delay(38);
			sendKeycode(keycode);  
	} else if (keycode != -1) {
		sendKeycode(keycode);
	}
	//Serial.println(keycode);
}

void typeString(String s) {
	for (int i = 0; i < s.length(); i++) {
		typeChar(s[i]);
	}
}

void sendKeycode(int keycode) {
	if (isBold) {
		PiSerial.write(0x13); // XOFF
	}
	byte row = keycode / 11;
	byte col = keycode % 11;
	selectCol(col);
	selectRow(row);
	if (lastKeycode == keycode) {
		delay(24);
	}
	digitalWrite(EN, LOW);
	delay(38);
	digitalWrite(EN, HIGH);
	delay(45);
	lastKeycode = keycode;
	if (isBold) {
		delay(38*2);
		PiSerial.write(0x11); // XON
	}
}

void sendKeyUp() {
	digitalWrite(EN, HIGH);
}

void sendCodeKeycode(int keycode) {
	byte row = keycode / 11;
	byte col = keycode % 11;
	selectCol(col);
	selectRow(row);
	if (lastKeycode == keycode) {
		delay(24);
	}
	digitalWrite(EN, LOW);
	delay(38);
	digitalWrite(EN, HIGH);
	delay(38);
	lastKeycode = keycode;
}

void bold() {
	if(~isBold) {
		selectCol(8);
		selectRow(1);
		digitalWrite(EN, LOW);
		delay(38);
		sendKeycode(42);
		isBold = true;
	}
}

void unBold() {
	if (isBold) {
		selectCol(8);
		selectRow(1);
		digitalWrite(EN, LOW);
		delay(38);
		sendKeycode(42);
		isBold = false;
	}
}