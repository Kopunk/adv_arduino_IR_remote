const char bankNo = 10;
const char bankNameLen = 14;


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
