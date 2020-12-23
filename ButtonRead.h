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
