#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() {
  lcd.begin(16, 2);
  byte rows = 5;
  char option[rows][14] = {"Option 1\0", "-Option 2\0", "Option 3\0", "--Option 4\0", "-Option 5\0"}; //temp
  Menu(rows, option);
}

void loop() {
  //Menu();
}


char ButtonRead() {
  int buttonVal = analogRead(A0);
  if (buttonVal >= 1023) { // nothing pressed
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

void Menu(byte rows, char list[][14]) { // strzaleczka w lewo - char 127
  char button;
  bool buttonReleased = true;
  //String option[] = {"Option 1", "-Option 2", "Option 3", "--Option 4", "-Option 5"}; //temp
  bool selectedUpperRow = true;
  byte currTopOption = 0;
  
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
      button = ButtonRead();
      delay(2);
    } while (button == ButtonRead());

    if (button) {
      if (button == 's') { // SELECT
        //lcd.print("Select");
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
