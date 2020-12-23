#include <LiquidCrystal.h>sssssss
#include "ButtonRead.h"sssssssssssssssssssssssssssssss

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