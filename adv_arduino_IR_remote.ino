#include <LiquidCrystal.h>
#include <IRremote.h>
#include <EEPROM.h>
#include "Buzz.h"  // includes buzz() function: arg 0 - 5 or other for default
#include "WriteDefaults.h"


//IRremote initializations and variables.
const char irPin = 2;
IRrecv irrecv(irPin);
decode_results results;
IRsend irsend;

//LCD initialization
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// number of banks
#define bankNo 10

// max bank name length
#define bankNameLen 14

// number of signals from basic buttons
#define basicButtonNo 10

// number of signals from additional buttons
#define additionalButtonNo 5

// number of signals in sequence
#define sequenceLen 4

// number of bytes ir signal takes
#define signalLen 4  // size of long

// menu constants
const char maxColumns = 14;
const char menuMain[][maxColumns] = {"Forward IR", "Send IR", "Receive IR", "Connect PC", "Settings"};
const char menuSettings[][maxColumns] = {"Cal. basic", "Cal. addit.", "Test Buzzer"};

// bankNames, menu can be conf by user
char menuSend[bankNo][bankNameLen];

// variables for handling Menu() in loop()
char choice = -1;
char subchoice = -1;

//constants for EEPROM addressing
#define startupModeAddr 0
#define basicButtonsSignalsAddr 1
#define additionalButtonsSignalsAddr ( basicButtonsSignalsAddr + (signalLen * basicButtonNo) )
#define banksNamesAddr ( additionalButtonsSignalsAddr + (signalLen * additionalButtonNo) )
#define sequenceBanksAddr ( banksNamesAddr + (bankNameLen * bankNo) )
// #define otherSignals ( banksAddr + (4 * 10) ) currently unused

// default buzzPin in Buzz.h is 15 (A1)

//arrays for button names
const char basicButtons[basicButtonNo][maxColumns] = {"POWER", "VOLUME UP", "VOLUME DOWN", "Przycisk 1", "Przycisk 2", "Przycisk 3", "Przycisk 4", "Przycisk 5", "Przycisk 6", "Przycisk 7"};
const char additionalButtons[additionalButtonNo][maxColumns] = {"RIGHT", "UP", "DOWN", "LEFT", "OK"};

//arrays for buttons signals
long basicButtonsSignals[basicButtonNo];
long additionalButtonsSignals[additionalButtonNo];

//array for signal sequences
long sequences[bankNo][sequenceLen];

// functions -------------------------------------------------------------------

// ---- load data from eeprom ----

void loadSignals() {
  /*
    Reads signal values from EEPROM and saves them to basicButtonsSignals array.
    Previously named assignButtons.
  */
  EEPROM.get(basicButtonsSignalsAddr, basicButtonsSignals);
  EEPROM.get(additionalButtonsSignalsAddr, additionalButtonsSignals);
}

void loadBankNames() {
  /*
    Reads bank names from eeprom and loads to menuSend array.
  */
  EEPROM.get(banksNamesAddr, menuSend);
}

void loadSequences() {
  /*
    Loads sequences of 4 signals from EEPROM.
    If sequence element value = 255, writes 0 to sequences array.
  */
  int addr = sequenceBanksAddr;
  unsigned char read = 0;
  for (int j = 0; j < bankNo; j++) {
    for (int i = 0; i < sequenceLen; i++) {
      read = EEPROM.read(addr++);
      if (read == 255) {
        sequences[j][i] = 0;
      }
      else if (read < 10) {
        sequences[j][i] = basicButtonsSignals[read];
      }
      else if (read >= 10 && read < 15) {
        sequences[j][i] = additionalButtonsSignals[read];
      }
      else {
        sequences[j][i] = 0; // wrongly programmed sequence
      }
      // Serial.println(read); // DEBUGGING
    }
  }
}

// ---- Menu, iu and keypad shield button handling ----

char ButtonRead(int buttonVal) {
  /*
    Returns lowercase first character of button name
    given buttonVal - usually analogRead(A0).
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

char Menu(const char rows, const char list[][maxColumns]) {
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

// ---- IR handling ----

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
      // Serial.println(results.value); //  DEBUGGING
      irrecv.resume();
      return results.value;
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
        sendIR(sequences[i]);
      }
    }
  }
}

void sendIR(long signalSequence[sequenceLen]) {
  /*
    Sends given sequence of IR signals using most common protocols.
    Interval between signals is 1 second.
    Default length of sequence is 4 signals
  */
  for (int i = 0; i < sequenceLen; i++) {
    if (signalSequence[i] != 0) {
      // Serial.println(signalSequence[i]);  // DEBUGGING
      irsend.sendSony(signalSequence[i], 32); delay(200);
      irsend.sendLG(signalSequence[i], 32); delay(200);
      irsend.sendSAMSUNG(signalSequence[i], 32); delay(200);
      irsend.sendPanasonic(signalSequence[i], 32);
      delay(1000);
    }
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
      // Serial.println(results.value); // DEBUGGING
      if (results.value == basicButtonsSignals[0]) {
        buzz(2);
      }
      else if (results.value == basicButtonsSignals[1]) {
        buzz(1);
        lcd.clear();
        lcd.print("VOLUME UP!");
        delay(2000);
      }
      else if (results.value == basicButtonsSignals[2]) {
        buzz(0);
        lcd.clear();
        lcd.print("VOLUME DOWN!");
        delay(2000);
      }
      else if (results.value == basicButtonsSignals[3]) {
        buzz(2);
        lcd.clear();
        lcd.print("WYBRANO P1!");
        delay(2000);
      }
      else if (results.value == basicButtonsSignals[4]) {
        buzz(2);
        lcd.clear();
        lcd.print("WYBRANO P2!");
        delay(2000);
      }
      else {
        buzz(2);
      }
      lcd.clear();
      lcd.print("Receive IR");
      irrecv.resume();
    }
  }
}

// ---- PC connection ----

void PCsendButtonSignals() {
  // Sends all button signals (basic + additional) through Serial.
  for (int i = 0; i < basicButtonNo; i++) {
    Serial.println(basicButtonsSignals[i]);
  }
  for (int i = 0; i < additionalButtonNo; i++) {
    Serial.println(additionalButtonsSignals[i]);
  }
}

void PCsendBankNames() {
  // Sends all bank names by line through Serial.
  for (int i = 0; i < bankNo; i++) {
    Serial.println(menuSend[i]);
  }
}

void PCsendSequences() {
  /*
    Reads from EEPROM and sends all sequences byte by byte through Serial.
    Although sends only 1 byte at a time uses println (delete trailing \r\n).
    Testing with print() instead of println
    Remember to add linger sleep() due to slow EEPROM reading time.
  */
  int addr = sequenceBanksAddr;
  unsigned char read = 0;

  for (int j = 0; j < bankNo; j++) {
    for (int i = 0; i < sequenceLen; i++) {
      read = EEPROM.read(addr++);
//      if (read < 15 || read == 255) {
        Serial.println(read);
//      }
//      else {
//        buzz(); buzz(); buzz(); // error - wrongly programmed sequence
//      }
    }
  }
}

void PCreceiveUpdateEEPROM() {
  /*
    Receives bytes from Serial and stores them in EEPROM.
    Allows for PC - edited banknames and sequences
  */
  unsigned char read = 0;
  char readChr[14];  // 11 is max no of chars that written long can take
  String readStr = "";
  int addr = basicButtonsSignalsAddr;
  long signal;

  Serial.write(1); // write 1 byte to start
  Serial.read();

  // buttons' signals
  lcd.clear();
  lcd.print("button");
  lcd.setCursor(0, 1);
  lcd.print("transfer");
  for (int i = 0; i < basicButtonNo + additionalButtonNo; i++) {
    for (int j = 0; j < 11; j++) {
      readChr[j] = ' ';
    }
    readStr = Serial.readStringUntil('\n');
    readStr.toCharArray(readChr, 11);
    signal = atol(readChr);
    EEPROM.put(addr, signal);
    addr = addr + 4;
    Serial.write(1);
  }

  // bank names
  lcd.clear();
  lcd.print("bankname");
  lcd.setCursor(0, 1);
  lcd.print("transfer");
  for (int i = 0; i < bankNo; i++) {
    readStr = Serial.readStringUntil('\n');
    readStr.toCharArray(readChr, bankNameLen);
    EEPROM.put(addr, readChr);
    addr = addr + bankNameLen;
    Serial.write(1);
  }


  // sequences
  lcd.clear();
  lcd.print("sequence");
  lcd.setCursor(0, 1);
  lcd.print("transfer");
  for (int i = addr; i < ( addr + (4 * 10) ); i++) {
    for (int j = 0; j < 4; j++) {
      readChr[j] = ' ';
    }
    readStr = Serial.readStringUntil('\n');
    readStr.toCharArray(readChr, 3);
    read = atoi(readChr);
    EEPROM.update(i, read);
    Serial.write(1);
  }

  lcd.clear();
  lcd.print("done");
}

// ---- button calibration ----

void calibrateButtons(const char words[][maxColumns], int addr, const int len) {
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
    long x = 0, y = 0, z = 0, test;
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
  loadSignals();
  loadSequences();
  lcd.clear();
  buzz(2);
  lcd.print("Succesfully");
  lcd.setCursor(0, 1);
  lcd.print("callibrated");
  delay(2000);
}

// reset program function
void (*resetFunc) (void) = 0; //declare reset function at address 0

// setup() and loop() ----------------------------------------------------------

void setup() {
  // Serial.begin(9600); //DEBUGGING

  pinMode(buzzPin, OUTPUT);  // buzzPin defined in Buzz.h
  buzz(1);

  lcd.begin(16, 2);

  irrecv.enableIRIn();

  // load first byte from EEPROM
  unsigned char startupMode = EEPROM.read(startupModeAddr);

  // Set all EEPROM bytes to 0s and set addr 0 to 255
  if (startupMode == 1) {
    lcd.print("CLEARING EEPROM");
    buzz(4);
    for (int i = 0 ; i < EEPROM.length() ; i++) {
      EEPROM.update(i, 0);
    }
    EEPROM.update(startupModeAddr, 255);
    // restart
    resetFunc();
  }
  // Write default banknames and sequences to EEPROM (WriteDefaults.h)
  if (startupMode == 255) {
    lcd.print("UPDATING EEPROM");
    buzz(4);
    writeDefaultBanknames(banksNamesAddr, sequenceBanksAddr);
    writeDefaultSequences(sequenceBanksAddr);
    EEPROM.update(startupModeAddr, 0);
  }

  // Load data from EEPROM
  loadSignals();
  loadBankNames();
  loadSequences();
}

void loop() {
  choice = Menu((sizeof(menuMain) / sizeof(menuMain[0])), menuMain);

  switch (choice) {

    // "Forward IR"
    case 0:
      lcd.print("Forward IR");
      forwardIR();
      break;

    // "Send IR"
    case 1:
      subchoice = Menu((sizeof(menuSend) / sizeof(menuSend[0])), menuSend);
      if (subchoice == -1) {
        break;
      }
      else {
        // Serial.println(sequences[subchoice][0]); // DEBUGGING
        sendIR(sequences[subchoice]);
      }
      break;

    // "Receive IR"
    case 2:
      lcd.print("Receive IR");
      receiveIR();
      break;

    // "Connect PC"
    case 3:
      Serial.begin(9600);
      lcd.print("Connecting");
      buzz(5);
      delay(500);

      lcd.setCursor(0, 1);
      lcd.print("Sending data");
      delay(500);
      PCsendButtonSignals();
      PCsendBankNames();
      PCsendSequences();

      lcd.clear();
      lcd.print("Data sent");
      delay(500);
      lcd.clear();
      lcd.print("Waiting for res-");
      lcd.setCursor(0, 1);
      lcd.print("ponse PC signal");

      // waiting for byte
      while (!Serial.available()) {}

      // possible error
      if (Serial.available() > 1) {
        buzz();
      }

      // waiting for last byte
      // while (!Serial.available()) {}
      // if PC sends back 1 - start receiving
      if (Serial.read() == 1) {
        lcd.clear();
        lcd.print("Writing...");
        PCreceiveUpdateEEPROM();
        // // load received data from EEPROM to memory
        // loadSignals();
        // loadBankNames();
        // loadSequences();

        // restart
        resetFunc();
      }
      Serial.end();
      buzz(0);
      break;

    // "Settings"
    case 4:
      subchoice = Menu((sizeof(menuSettings) / sizeof(menuSettings[0])), menuSettings);
      if (ButtonRead(analogRead(A0)) == 'l') {
        break;
      }
      switch (subchoice) {
        case 0:
          calibrateButtons(basicButtons, basicButtonsSignalsAddr, sizeof(basicButtons) / sizeof(basicButtons[0]));
          break;

        case 1:
          calibrateButtons(additionalButtons, additionalButtonsSignalsAddr, sizeof(additionalButtons) / sizeof(additionalButtons[0]));
          break;

        case 2:
          buzz(2);
          break;
      }
      break;
  }
}
