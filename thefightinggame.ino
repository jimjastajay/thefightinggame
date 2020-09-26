byte blinkColor = 0;
enum blinkFormations {TAIL, DEFENSE, BALANCE, ATTACK};
byte blinkFormation = TAIL;
byte attackVal[6] = {0, 0, 0, 0, 0, 0};

byte tailType = TAIL;
byte health = 6;

bool isReady = false;
byte attackingFace = -1;

void setup() {}

void resetTile() {
  health = 6;
  setColor(WHITE);
}

//Lowers the fighter's health by 1
void decreaseHealth() {
  if (health > 0) {
    health -= 1;
    for (int i = 5; i >= health; i--) {
      setColorOnFace(OFF, i);
    }
  }
}

void loop() {
  //listen for double clicks to change color
  if (buttonDoubleClicked()) {
    blinkColor = (blinkColor + 1) % 4;
  }
  if (buttonMultiClicked()) {
    if (buttonClickCount() == 3)
      resetTile();
  }
  if (buttonSingleClicked()) {
    decreaseHealth();
  }


  //listen for formations
  byte firstNeighborFace = 6;
  byte secondNeighborFace = 6;
  byte thirdNeighborFace = 6;
  byte neighborsFound = 0;



  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//neighbor!
      byte neighborData = getLastValueReceivedOnFace(f);
      if (getBlinkColor(neighborData) == blinkColor) { //same color!
        neighborsFound++;
        if (firstNeighborFace == 6) {
          firstNeighborFace = f;
        }
        else if (secondNeighborFace == 6) {
          secondNeighborFace = f;
        }
      }
    }
  }//end face loop


  //evaluate my neighborhood
  if (neighborsFound == 1) {
    byte neighborFormation = getBlinkFormation(getLastValueReceivedOnFace(firstNeighborFace));
    switch (neighborFormation) {
      case BALANCE:
        tailType = BALANCE;
        break;
      case ATTACK:
        tailType = ATTACK;
        break;
      case TAIL:
        tailType = TAIL;
        break;
    }
  }
  else if (neighborsFound == 2) {
    if (abs(firstNeighborFace - secondNeighborFace) == 1 || abs(firstNeighborFace - secondNeighborFace) == 5) {//defensive cluster
      blinkFormation = DEFENSE;
    }
    else if (abs(firstNeighborFace - secondNeighborFace) == 3) {//attacky formation
      blinkFormation = ATTACK;
    }
    else {//balance formation
      blinkFormation = BALANCE;
    }
  }
  else {
    tailType = TAIL;
    blinkFormation = TAIL;
  }



  //send out face data
  byte sendData = (blinkColor << 4) + (blinkFormation << 2);
  setValueSentOnAllFaces(sendData);

  //temp display
  Color displayColor;
  switch (blinkColor) {
    case 0:
      displayColor = RED;
      break;
    case 1:
      displayColor = YELLOW;
      break;
    case 2:
      displayColor = GREEN;
      break;
    case 3:
      displayColor = MAGENTA;
      break;
  }
  setColor(displayColor);

  //  switch (blinkFormation) {
  //    case ATTACK:
  //      setColorOnFace(ORANGE, random(5));
  //      break;
  //    case BALANCE:
  //      setColorOnFace(BLUE, random(5));
  //      break;
  //    case DEFENSE:
  //      setColorOnFace(OFF, random(5));
  //      break;
  //  }
  //
  //  switch (tailType) {
  //    case ATTACK:
  //      setColorOnFace(WHITE, random(5));
  //      break;
  //    case BALANCE:
  //      setColorOnFace(MAGENTA, random(5));
  //      break;
  //  }


  //  if (thirdNeighborFace != 6) {
  //    setColorOnFace(RED, 0);
  //    setColorOnFace(ORANGE, 1);
  //    setColorOnFace(YELLOW, 2);
  //    setColorOnFace(GREEN, 3);
  //    setColorOnFace(BLUE, 4);
  //    setColorOnFace(MAGENTA, 5);
  //  }



  for (int i = 5; i >= health; i--) {
    setColorOnFace(OFF, i);
  }

  //For testing
  if (neighborsFound == 1) {
    setColorOnFace(WHITE, getOppositeFace(firstNeighborFace));
  }

}

byte getBlinkColor(byte data) {
  return (data >> 4);//returns 1st and 2nd bit
}

byte getBlinkFormation(byte data) {
  return ((data >> 2) & 3);//returns 3rd and 4th bit
}

void initiateAttack(){
//  setValueSentOnFace(9, 
}


int getOppositeFace(int s) {

  int r = -1;
  switch (s) {
    case 0:
      r = 3;
      break;
    case 1:
      r = 4;
      break;
    case 2:
      r = 5;
      break;
    case 3:
      r = 0;
      break;
    case 4:
      r = 1;
      break;
    case 5:
      r = 2;
      break;
  }
  return r;

}
