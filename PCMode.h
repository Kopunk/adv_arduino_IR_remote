const char bankNo = 10;
const char bankNameLen = 14;

void pcMode(char bankName[bankNo][bankNameLen], int banksNamesAddr=61, int EObanksNamesAddr=201) {
    Serial.begin(9600);
    while(!Serial) {}

    for (int i = 0; i < bankNo; i++) {
        if (i % bankNameLen == 0) {
            Serial.println(bankName[i/bankNameLen]);
        }
    }
    // while (true) {} // temporairly
    Serial.end();
}
