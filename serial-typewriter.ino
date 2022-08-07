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

int colPins[] = {S10, S11, S12, S13};
int rowPins[] = {S20, S21, S22};

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
  0, // <
  61, // =
  0, // >
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
  0, // Backslash
  39, // ]
  0, // ^
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
  0, // {
  0, // |
  0, // }
  9 // ~
  };


AltSoftSerial PiSerial;

//SoftwareSerial PiSerial = SoftwareSerial(8,9);
int lastKeycode = -1;
int newChar = 0;
bool flowControl = false;

bool isBold = false;

void setup() {
  pinMode(EN, OUTPUT);
  digitalWrite(EN, HIGH);

  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);  

  for (int i = 0; i < sizeof(rowPins) / sizeof(int); i++) {
    pinMode(rowPins[i], OUTPUT);
  }
  for (int i = 0; i < sizeof(colPins) / sizeof(int); i++) {
    pinMode(colPins[i], OUTPUT);
  }
  Serial.begin(1200);
  PiSerial.begin(1200);

  PiSerial.write(0x11); // XON

  // int CodeKeycode = 19;
  // byte row = CodeKeycode / 11;
  // byte col = CodeKeycode % 11;
  // selectCol(col);
  // selectRow(row);
  // digitalWrite(EN, LOW);
  // delay(38);
  // sendKeycode(42);
  // typeString("Hello");
  // selectCol(col);
  // selectRow(row);
  // digitalWrite(EN, LOW);
  // delay(38);
  // sendKeycode(42);
  // typeString(" world!");

  bold();
  typeString("Hello!");
  unBold();


  //typeString("Hello world!\n\n");
  //typeString("The quick red fox jumps over the lazy brown dog.\n\n");
  // typeString("This is my typewriter.\n");
  // typeString("There are many like it,\n");
  // typeString("but this one is mine.\n");

  // typeString("             __..--''``---....___   _..._    __\n");
  // typeString(" /// //_.-'    .-/\";  `        ``<._  ``.''_ `. / // /\n");
  // typeString("///_.-' _..--.'_    \                    `( ) ) // //\n");
  // typeString("/ (_..-' // (< _     ;_..__               ; `' / ///\n");
  // typeString("/ // // //  `-._,_)' // / ``--...____..-' /// / //");

// typeString("          .__....._             _.....__,\n");
// typeString("            .\": o :':         ;': o :\".\n");
// typeString("            `. `-' .'.       .'. `-' .'\n");
// typeString("              `---'             `---'\n");
// typeString("\n");
// typeString("    _...----...      ...   ...      ...----..._\n");
// typeString(" .-'__..-\"\"'----    `.  `\"`  .'    ----'\"\"-..__`-.\n");
// typeString("'.-'   _.--\"\"\"'       `-._.-'       '\"\"\"--._   `-.`\n");
// typeString("'  .-\"'                  :                  `\"-.  `\n");
// typeString("  '   `.              _.'\"'._              .'   `\n");
// typeString("        `.       ,.-'\"       \"'-.,       .'\n");
// typeString("          `.                           .'\n");
// typeString("            `-._                   _.-'\n");
// typeString("                `\"'--...___...--'\"`\n");
}

void loop() {
  char incoming = 0;
  if (PiSerial.available() >= 24 && flowControl == false) {
    PiSerial.write(0x13); // XOFF
    //PiSerial.write(0x13); // XOFF, twice for good luck
    flowControl = true;
  } else if (flowControl == true && PiSerial.available() < 8) {
    PiSerial.write(0x11); // XON
    //PiSerial.write(0x11); // XON
    flowControl = false;
  }

  if (PiSerial.available()) {
    incoming = PiSerial.read();
    // PiSerial.write(0x13); // XOFF
    if (incoming == '\e') {
      //Serial.print("Escape!");
      PiSerial.read();
      int param;
      // while (PiSerial.available() < 8);
      // PiSerial.write(0x13); // XOFF
      // flowControl = true;
      param = PiSerial.parseInt(SKIP_NONE);
      switch (param) {
        case 0:
          unBold();
          break;
        case 1:
          Serial.print("Bold!");
          bold();
          break;
        default:
          break;
      }
      PiSerial.read();
      // if (incoming = ';') {
      //   while (PiSerial.available() <=4);
      //   PiSerial.parseInt(SKIP_NONE);
      //   PiSerial.read();
      // }
      incoming = 0;
      // PiSerial.write(0x11); // XON
    } else {
      Serial.write(incoming);
      //newChar = 1;
      typeChar(incoming);
    }
    // PiSerial.write(0x11); // XON
  } 

	if (Serial.available()) {
    PiSerial.write(Serial.read());
  }

  // if (newChar) {
  //   //PiSerial.write(0x13); // XOFF
  //   typeChar(incoming);
  //   newChar = 0;
  //   //PiSerial.write(0x11); // XON
  // }
}

void selectRow(int row) {
  for (int pin = 0; pin < 3; pin++) {
    digitalWrite(rowPins[pin], rowChannels[row][pin]);
  }
}

void selectCol(int col) {
  for (int pin = 0; pin < 4; pin++) {
    digitalWrite(colPins[pin], colChannels[col][pin]);
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
      sendKeycode(80); // Shift Lock
      sendKeycode(keycode);
      sendKeycode(85); // Shift    
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
  if (isBold) {
    delay(38*2);
  }
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