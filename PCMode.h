const char bankNo = 10;
const char bankNameLen = 14;

void pcMode(int banksNamesAddr=61, int EObanksNamesAddr=201) {
    char bankName[bankNameLen] = "";

    Serial.begin(9600);
    while(!Serial) {}

    for (int j = banksNamesAddr; j < EObanksNamesAddr; j = j + bankNameLen) {
        for (int i = 0; i < bankNameLen; i++) {
            bankName[i] = char(EEPROM.read(j+i));
        }
        Serial.println(bankName);
    }
    // while (true) {} // temporairly
    Serial.end();
}
