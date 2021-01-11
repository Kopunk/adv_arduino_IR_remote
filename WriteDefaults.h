void writeDefaultBanknames(int banksNamesAddr = 61, int EObanksNamesAddr = 201) {
  const char bankNameLen = 14;
  const char bankNo = 10;
  char menuSend[bankNo][bankNameLen] = {
    "Bank #0",
    "Bank #1",
    "Bank #2",
    "Bank #3",
    "Bank #4",
    "Bank #5",
    "Bank #6",
    "Bank #7",
    "Bank #8",
    "Bank #9"
  };

  for (int i = banksNamesAddr; i < EObanksNamesAddr; i++) {
    EEPROM.update(i, menuSend[(i - banksNamesAddr) / bankNameLen][(i - banksNamesAddr) % bankNameLen]);
  }
}

void writeDefaultBanks(int banksAddr = 201) {
  int i = banksAddr;
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