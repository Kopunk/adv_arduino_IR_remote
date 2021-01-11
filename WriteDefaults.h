void writeDefaultBanknames(int banksNamesAddr=61, int EObanksNamesAddr=201) {
    char menuSend[bankNo][bankNameLen] = {
        "Bank #0      ", 
        "Bank #1      ", 
        "Bank #2      ", 
        "Bank #3      ", 
        "Bank #4      ", 
        "Bank #5      ", 
        "Bank #6      ", 
        "Bank #7      ", 
        "Bank #8      ", 
        "Bank #9      "
    };
    for (int j = banksNamesAddr; j < EObanksNamesAddr; j = j + bankNameLen) {
        for (int i = 0; i < bankNameLen; i++) {
            EEPROM.update(j+i, menuSend[j][i]);
        }
    }

}

void writeDefaultBanks() {
      int i = 173;
  //Bank 1
  EEPROM.put(i, '0');
  i++;
  EEPROM.put(i, 255);
  i++;
  EEPROM.put(i, 255);
  i++;
  EEPROM.put(i, 255);
  i++;
  //Bank 2
  EEPROM.put(i, '1');
  i++;
  EEPROM.put(i, 255);
  i++;
  EEPROM.put(i, 255);
  i++;
  EEPROM.put(i, 255);
  i++;
  //Bank 3
  EEPROM.put(i, '2');
  i++;
  EEPROM.put(i, 255);
  i++;
  EEPROM.put(i, 255);
  i++;
  EEPROM.put(i, 255);
  i++;
  //Bank 4
  EEPROM.put(i, '3');
  i++;
  EEPROM.put(i, 255);
  i++;
  EEPROM.put(i, 255);
  i++;
  EEPROM.put(i, 255);
  i++;
  //Bank 5
  EEPROM.put(i, '4');
  i++;
  EEPROM.put(i, 255);
  i++;
  EEPROM.put(i, 255);
  i++;
  EEPROM.put(i, 255);
  i++;
  //Bank 6
  EEPROM.put(i, '0');
  i++;
  EEPROM.put(i, '1');
  i++;
  EEPROM.put(i, '2');
  i++;
  EEPROM.put(i, '0');
  i++;
  //Bank 7
  EEPROM.put(i, '5');
  i++;
  EEPROM.put(i, '6');
  i++;
  EEPROM.put(i, '7');
  i++;
  EEPROM.put(i, '8');
  i++;
  //Bank 8
  EEPROM.put(i, '0');
  i++;
  EEPROM.put(i, '0');
  i++;
  EEPROM.put(i, 255);
  i++;
  EEPROM.put(i, 255);
  i++;
  //Bank 9
  EEPROM.put(i, '1');
  i++;
  EEPROM.put(i, '1');
  i++;
  EEPROM.put(i, '2');
  i++;
  EEPROM.put(i, '2');
  i++;
  //Bank 10
  EEPROM.put(i, '3');
  i++;
  EEPROM.put(i, '0');
  i++;
  EEPROM.put(i, '3');
  i++;
  EEPROM.put(i, '0');
  i++;
}