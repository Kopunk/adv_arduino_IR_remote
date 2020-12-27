#include "LiquidCrystal/src/LiquidCrystal.cpp"
//#include "IRremote/src/IRremote.cpp"

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// placeholder voids
//void forwardIR() {}

// ALL MENU CONTENTS
const byte maxColumns = 14;
const char option[][maxColumns] = {"Option 1", "-Option 2", "Option 3", "--Option 4", "-Option 5"}; //testing
const char menuMain[][maxColumns] = {"Forward IR", "Send IR", "Receive IR", "Connect PC", "Settings"};
const char menuSend[][maxColumns] = {"Bank #1", "Bank #2", "Bank #3", "Bank #4", "Bank #5"};


char choice = -1;
char subchoice = -1;

void setup() {
  lcd.begin(16, 2);
}

void loop() {
  choice = Menu((sizeof(menuMain)/sizeof(menuMain[0])), menuMain);
  
  switch(choice) {
    
    case 0: // "Forward IR"
      lcd.print("Forward IR");
      delay(2000);
      break;
      
    case 1: // "Send IR"
      subchoice = Menu((sizeof(menuSend)/sizeof(menuSend[0])), menuSend);
      if(subchoice >= 0) {
        lcd.print(menuSend[subchoice]);
        delay(2000);
      }
      break;
      
    case 2: // "Receive IR"
      lcd.print("Receive IR");
      delay(2000);
      break;
      
    case 3: // "Connect PC"
      lcd.print("Connect PC");
      delay(2000);
      break;

    case 4: // "Settings"
      lcd.print("Settings");
      delay(2000);
      break;

  }
}

char Menu(const byte rows, const char list[][maxColumns]) {
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
          if (currTopOption != rows-2) {
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
  //int buttonVal = analogRead(A0);
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