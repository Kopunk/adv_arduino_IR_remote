#include <LiquidCrystal.h>
#include <IRremote.h>
#include <EEPROM.h>
#include "Buzz.h"
#include "PCMode.h"
#include "WriteDefaults.h"

// Buzz includes buzz() function:
// takes char argument 0 - 4 or other for default
// see Buzz.h for sound descriptions.


//IRremote initializations and variables.
const int irPin = 2;
IRrecv irrecv(irPin);
decode_results results;
IRsend irsend;
//LCD initialization
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


// menu constants
const byte maxColumns = 14;
const char option[][maxColumns] = {"Option 1", "-Option 2", "Option 3", "--Option 4", "-Option 5"}; //testing
const char menuMain[][maxColumns] = {"Forward IR", "Send IR", "Receive IR", "Connect PC", "Settings"};
const char menuSettings[][maxColumns] = {"Cal. basic", "Cal. addit.", "Test Buzzer"};
// menus available for user option naming
char menuSend[bankNo][maxColumns]; // = {"Bank #1", "Bank #2", "Bank #3", "Bank #4", "Bank #5", "Bank #6", "Bank #7", "Bank #8", "Bank #9", "Bank #10"};

// variables for handling Menu() in loop()
char choice = -1;
char subchoice = -1;

//variable for EEPROM addressing
int eeAddress = 0;

//constants for EEPROM addressing
const int connectPCAddr = 0;
const int basicButtonsAddr = 1;
const int additionalButtonsAddr = basicButtonsAddr + (4 * 10); // (4 * bankNo)
const int banksNamesAddr = additionalButtonsAddr + (4 * 5);
const int banksAddr = banksNamesAddr + (14 * 10);  // (bankNameLen * bankNo))
const int otherSignals = banksAddr + (4 * 10);

//constant for buzz pin
//pin 15 is A1
// default buzzPin in Buzz.h is 15

// testing purposes
long testsignals[3] = {0x20DF40BF, 0x20DFC03F, 0x20DFD02F}; // volume up, volume down, source (may vary on device)

//arrays for buttons
const String basicButtons[10] = {"POWER", "VOLUME UP", "VOLUME DOWN", "Przycisk 1", "Przycisk 2", "Przycisk 3", "Przycisk 4", "Przycisk 5", "Przycisk 6", "Przycisk 7"};
const String additionalButtons[5] = {"RIGHT", "UP", "DOWN", "LEFT", "OK"};

//arrays for buttons signals
long basicButtonsSignals[10];
long additionalButtonsSignals[5];

//array for signal sequences
long sequences[10][4];

// functions ----------

void sendIR(const long signals[], const int len, const String protocol = "LG") {
  /*
     This function sends specified IR signals using specified protocol.
     Right now it sends signals in intervals of 1 second.
     The pin used to send signals is digital pin 3 - it is hard coded in the
     IRremote library, so I decided not to try to change it.
  */
  for (int x = 0; x < len; x++) {
    if (signals[x] != 0) {
      if (protocol == "LG") {
        irsend.sendLG(signals[x], 32);
        //Serial.println(signals[x]); //Debug
      }
      delay(1000);
    }
  }
}

long receiveSignal(const long skip = 0xFFFFFFFF) {
  /*
     Awaits a signal and returns it. You can also specify
     a 'skip' signal which the function will ignore and wait for next one.
  */
  while (true) {
    if (ButtonRead(analogRead(A0)) == 'l') {
      return 123;
    }

    if (irrecv.decode(&results)) {
      if (results.value == skip) {
        irrecv.resume();
        continue;
      }
      //Serial.println(results.value, HEX); // Debug
      irrecv.resume();
      return results.value;
    }
  }
}

long readHexFromEEPROM(int addr, int howMuch = 4) {
  String y = "";
  long x = 0;
  for (int i = addr; i < addr + howMuch; i++) {
    x = EEPROM.read(i);
    if (x <= 15) {
      y = "0" + String(x, HEX) + y;
    }
    else {
      y = String(EEPROM.read(i), HEX) + y;
    }
  }
  y.toUpperCase();
  //Serial.println(y); //Debug
  return strtol(y.c_str(), NULL, 16);
}

void assignButtons(int len1, int len2, int addr1, int addr2) {
  //Reads the signal values stored in EEPROM and saves in arrays for ease of use
  for (int i = 0; i < len1; i++) {
    basicButtonsSignals[i] = readHexFromEEPROM(addr1);
    addr1 = addr1 + 4;
  }
  for (int i = 0; i < len2; i++) {
    additionalButtonsSignals[i] = readHexFromEEPROM(addr2);
    addr2 = addr2 + 4;
  }
}

void calibrateButtons(const String words[], int addr, const int len) {
  /*
     Used to calibrate basic remote buttons listed in words[].
     For it to work properly you have to click each button 3 times
     and all of the signals received have to be the same to prevent misscalibration.
  */

  //Start, shows messages on lcd
  lcd.clear();
  lcd.print("Press each");
  lcd.setCursor(0, 1);
  lcd.print("button 3 times");
  delay(2000);
  lcd.clear();
  //function on array containing buttons to click
  for (int i = 0; i < len; i++) {
    long x = 0, y = 0, z = 0;
    while (true) {
      lcd.clear();
      lcd.print(words[i]);

      x = receiveSignal(); // first signal

      //this shows progress of callibration of specific button on screen
      lcd.setCursor(0, 1);
      lcd.print("3");

      y = receiveSignal(); // second signal
      if (x != y) { // checking if first and second are the same
        lcd.clear();
        lcd.print("ERROR!");
        lcd.setCursor(0, 1);
        lcd.print("Try once again");
        delay(1000);
        continue;
      }

      //further progress
      lcd.print("2");

      z = receiveSignal(); // third signal
      if (y != z) { // checking if second and third are the same
        lcd.clear();
        lcd.print("ERROR!");
        lcd.setCursor(0, 1);
        lcd.print("Try once again");
        delay(1000);
        continue;
      }

      //if all 3 signals are the same callibration of this button is succesful
      lcd.print("1");
      delay(300);
      lcd.clear();
      lcd.print("Button");
      lcd.setCursor(0, 1);
      lcd.print("calibrated");
      delay(1000);
      lcd.clear();
      //saving the button signal to EEPROM
      EEPROM.put(addr, x);
      break;
    }
    addr = addr + 4;
  }
  //renew the buttons arrays after calibration
  assignButtons(sizeof(basicButtonsSignals) / sizeof(basicButtonsSignals[0]), sizeof(additionalButtonsSignals) / sizeof(additionalButtonsSignals[0]), basicButtonsAddr, additionalButtonsAddr );
  loadSequences(sizeof(sequences) / sizeof(sequences[0]), sizeof(sequences[0]) / sizeof(sequences[0][0]), banksAddr);
  lcd.clear();
  lcd.print("Succesfully");
  lcd.setCursor(0, 1);
  lcd.print("callibrated");
  delay(2000);
}

char Menu(const byte rows, const char list[][maxColumns]) {
  /*
     Returns index of chosen element of list[] - menu option
     returns -1 when going back (left) was chosen.
     Displays simple scrollable menu with
     arrow (char 127) indicating current choice.
     up / down - navigation
     right / select - select option (return)
     left - go back (return -1)
  */
  char button;
  bool buttonReleased = true;
  bool selectedUpperRow = true;
  char currTopOption = 0;

  while (true) {

    lcd.clear();
    if (selectedUpperRow) {
      lcd.print(String(list[currTopOption]) + " " + char(127));
      lcd.setCursor(0, 1);
      lcd.print(String(list[currTopOption + 1]));
    }
    else {
      lcd.print(String(list[currTopOption]));
      lcd.setCursor(0, 1);
      lcd.print(String(list[currTopOption + 1]) + " " + char(127));
    }

    do {
      button = ButtonRead(analogRead(A0));
      delay(2);
    } while (button == ButtonRead(analogRead(A0)));

    if (button) {
      if (button == 's' || button == 'r') { // SELECT or RIGHT
        lcd.clear();
        return currTopOption + byte(!selectedUpperRow);
      }
      else if (button == 'l') { // LEFT
        lcd.clear();
        return -1;
      }
      else if (button == 'u') { // UP
        if (selectedUpperRow) {
          if (currTopOption != 0) {
            currTopOption--;
          }
        }
        else {
          selectedUpperRow = !selectedUpperRow;
        }
      }
      else if (button == 'd') { // DOWN
        if (!selectedUpperRow) {
          if (currTopOption != rows - 2) {
            currTopOption++;
          }
        }
        else {
          selectedUpperRow = !selectedUpperRow;
        }
      }
    }
  }
}

char ButtonRead(int buttonVal) {
  /*
     Returns lowercase first character of button name
     given buttonVal - usually analogRead(A0)
  */
  if (buttonVal >= 950) { // nothing pressed
    return 0;
  }
  else if (buttonVal <= 50) { // RIGHT
    return 'r';
  }
  else if (buttonVal > 50 && buttonVal <= 200) { // UP
    return 'u';
  }
  else if (buttonVal > 200 && buttonVal <= 350) { // DOWN
    return 'd';
  }
  else if (buttonVal > 350 && buttonVal <= 600) { // LEFT
    return 'l';
  }
  else if (buttonVal > 600) { // SELECT
    return 's';
  }
}

void receiveIR() {
  /*
     This function receives signals and handles them,
     executing specific code depending on what singal has been received.
  */
  while (true) {
    if (ButtonRead(analogRead(A0)) == 'l') {
      return;
    }
    if (irrecv.decode(&results)) {
      if (results.value == basicButtonsSignals[0]) {
        buzz(3);
      }
      else if (results.value == basicButtonsSignals[1]) {
        buzz(3);
      }
      else if (results.value == basicButtonsSignals[2]) {
        lcd.clear();
        lcd.print("NAPIS");
        delay(2000);
        lcd.clear();
        lcd.print("Receive IR");
      }
      irrecv.resume();
    }
  }
}


void forwardIR() {
  //sends sequences of signals depending on received signal
  while (true) {
    if (ButtonRead(analogRead(A0)) == 'l') {
      return;
    }

    long x = receiveSignal();

    if (x == 123) {
      return;
    }
    for (int i = 0; i < 10; i++) {
      if (x == basicButtonsSignals[i]) {
        sendIR(sequences[i], sizeof(sequences[i]) / sizeof(sequences[i][0]));
      }
    }
  }
}

void loadSequences(const int len1, const int len2, int addr) {
  //load sequences from EEPROM
  for (int i = 0; i < len1; i++) {
    for (int j = 0; j < len2; j++) {
      int x = EEPROM.read(addr) - '0';
      if (x == 207) {
        sequences[i][j] = 0;
      }
      else if (i >= len1 - 2) {
        sequences[i][j] = additionalButtonsSignals[x];
      }
      else {
        sequences[i][j] = basicButtonsSignals[x];
      }
      addr++;
    }
  }
}

void loadBankNames() {
  buzz(4);
  const char bankNameLen = 14;
  for (int i = banksNamesAddr; i < banksAddr; i++) {
    menuSend[(i - banksNamesAddr) / bankNameLen][(i - banksNamesAddr) % bankNameLen] = EEPROM.read(i);
  }
}

// reset program function
void (*resetFunc) (void) = 0; //declare reset function at address 0

// setup() and loop() ----------

void setup() {
  pinMode(buzzPin, OUTPUT);  // buzzPin defined in Buzz.h
  buzz(1);

  lcd.begin(16, 2);

  // load data from EEPROM
  loadBankNames();

  byte connPC = EEPROM.read(0);
  // load defaults to EEPROM
  if (connPC == 255)  {
    lcd.print("LOADING DEFAULTS");
    writeDefaultBanknames(banksNamesAddr, banksAddr);
    writeDefaultBanks(banksAddr);
    EEPROM.update(0, 0);
  }
  // enable pc communication
  if (connPC == 1) {
    //Serial.end();
    lcd.print("PC CONN MODE");
    pcMode(menuSend);
    EEPROM.update(connectPCAddr, 0);
  }

  //Serial.begin(9600);
  irrecv.enableIRIn();
  assignButtons(sizeof(basicButtonsSignals) / sizeof(basicButtonsSignals[0]), sizeof(additionalButtonsSignals) / sizeof(additionalButtonsSignals[0]), basicButtonsAddr, additionalButtonsAddr );
  loadSequences(sizeof(sequences) / sizeof(sequences[0]), sizeof(sequences[0]) / sizeof(sequences[0][0]), banksAddr);
  for (int i = 0; i < 9; i++) {
    //Serial.println(basicButtonsSignals[i], HEX);
  }
}

void loop() {
  choice = Menu((sizeof(menuMain) / sizeof(menuMain[0])), menuMain);

  switch (choice) {

    case 0: // "Forward IR"
      lcd.print("Forward IR");
      forwardIR();
      break;

    case 1: // "Send IR"
      subchoice = Menu((sizeof(menuSend) / sizeof(menuSend[0])), menuSend);
      if (subchoice == -1) {
        break;
      }
      else {
        sendIR(sequences[subchoice], sizeof(sequences[subchoice]) / sizeof(sequences[subchoice][0]));
      }
      break;

    case 2: // "Receive IR"
      lcd.print("Receive IR");
      receiveIR();
      //delay(2000);
      break;

    case 3: // "Connect PC"
      lcd.print("Connect PC");

      EEPROM.update(connectPCAddr, 1);
      delay(2000);
      resetFunc();  // start program from 0
      break;

    case 4: // "Settings"
      subchoice = Menu((sizeof(menuSettings) / sizeof(menuSettings[0])), menuSettings);
      if (ButtonRead(analogRead(A0)) == 'l') {
        break;
      }
      switch (subchoice) {

        case 0:
          calibrateButtons(basicButtons, basicButtonsAddr, sizeof(basicButtons) / sizeof(basicButtons[0]));
          break;

        case 1:
          calibrateButtons(additionalButtons, additionalButtonsAddr, sizeof(additionalButtons) / sizeof(additionalButtons[0]));
          break;

        case 2:
          buzz();
          break;
      }
      break;

  }
}
