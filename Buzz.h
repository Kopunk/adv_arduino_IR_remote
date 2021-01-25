// remember to include 'pinMode(buzzPin, OUTPUT);' in setup()
char buzzPin = 15;  // default pin no connected to buzzer is A0

void buzzNote(unsigned int freq = 300, unsigned int len = 200) {
  /*
     Plays single note of given freq for len times.
     Freq is not frequency, rather time between on/off signals.
  */

  for (int i = 0; i < len; i++) {
    digitalWrite(buzzPin, HIGH);  // on
    delayMicroseconds(freq);
    digitalWrite(buzzPin, LOW);  // off
    delayMicroseconds(freq);
  }
}

// 0: tritone down; 1: tritone up; 2: -|-|-; 3: 4down; 4: -|-|-|-|-; 5: 4up
void buzz(char choice = -1) {
  /*
     Plays melody (sequence of notes)
     corresponding to given choice.
  */
  switch (choice) {
    case 0:  // tritone down
      buzzNote(250, 200);
      buzzNote(350, 150);
      buzzNote(500, 100);
      break;
    case 1:  // tritone up
      buzzNote(500, 100);
      buzzNote(350, 150);
      buzzNote(250, 200);
      break;
    case 2: // prlrlr
      buzzNote(80, 200);
      buzzNote(120, 150);
      buzzNote(80, 200);
      buzzNote(120, 150);
      buzzNote(80, 200);
      buzzNote(120, 150);
      break;
    case 3:  // 4 down
      buzzNote(800, 100);
      buzzNote(900, 100);
      buzzNote(1000, 100);
      buzzNote(1100, 100);
      break;
    case 4:  // prlrlrlrlv
      buzzNote(500, 30);
      buzzNote(550, 30);
      buzzNote(600, 30);
      buzzNote(700, 30);
      buzzNote(850, 30);
      buzzNote(1000, 30);
      buzzNote(1200, 30);
      buzzNote(1400, 30);
      break;
    case 5:  // 4 up
      buzzNote(1100, 100);
      buzzNote(1000, 100);
      buzzNote(900, 100);
      buzzNote(800, 100);
      break;
    default:  // error
      buzzNote(1400, 30);
      delay(10);
      buzzNote(1400, 30);
      delay(10);
      buzzNote(1400, 30);
  }
}
