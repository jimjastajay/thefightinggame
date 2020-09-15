#define KINDABLUE makeColorRGB(0, 232, 209)
#define GREY makeColorRGB(128, 128, 128)

enum blinkFormations {NOCONFIG, BALANCED, ATTACK, DEFENSE, TAIL};

byte blinkFormations = NOCONFIG;

byte tailType = TAIL;
byte blinkColor = 0;
byte canTakeDamage = true;
byte attacking = true;


/////////////////////////////
byte health = 6;
byte attackStrength = 0;
byte defenseStrength = 0;
byte attackVal[6] = {0, 0, 0, 0, 0, 0};

struct FighterType {
  Color color;
  int h;
  int as;
  int ds;
};

FighterType one = {RED, 3, 3};
FighterType two = {YELLOW, 3, 3};
FighterType three = {GREEN, 3, 3};
FighterType four = {MAGENTA, 3, 3};
FighterType fighterList[4] = {{one}, {two}, {three}, {four}};

byte teamNumber = 0;

void setup() {
  resetTile();
}

//Resets the tile to it's basic setting
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

///// Getting blink color & formation data //////
byte getBlinkColor(byte data) {
  return (data >> 4);//returns 1st and 2nd bit
}
byte getBlinkFormation(byte data) {
  return ((data >> 2) & 3);//returns 3rd and 4th bit
}
////////////////

void loop() {

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

  byte firstNeighborFace = 6;
  byte secondNeighborFace = 6;
  byte neighborsFound = 0;

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

  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//neighbor!
      byte neighborData = getLastValueReceivedOnFace(f);
      attacking = false;
      if (firstNeighborFace == 6) {
        neighborsFound += 1;
        firstNeighborFace = f;
      }
      else {
        if (secondNeighborFace == 6) {
          neighborsFound += 1;
          secondNeighborFace = f;
        }
      }
    }
  }



  // If you find one neighbor, you're checking to see what config your neighbor is in.
  if (neighborsFound == 1) {
    byte neighborFormation = getBlinkFormation(getLastValueReceivedOnFace(firstNeighborFace));
    switch (neighborFormation) {
      case BALANCED:
        tailType = BALANCED;
        break;
      case ATTACK:
        tailType = ATTACK;
        break;
      case TAIL:
        tailType = TAIL;
        break;
    }
    //Light up opposite side (hopefully)
    FOREACH_FACE(f) {
      if (!isValueReceivedOnFaceExpired(f)) {//a neighbor!
        setColorOnFace(WHITE, getOppositeFace(f));
        setValueSentOnFace(attacking, f);
      }
    }

  }
  else if (neighborsFound == 2) {
    if (abs(firstNeighborFace - secondNeighborFace) == 1 || abs(firstNeighborFace - secondNeighborFace) == 5) {//defensive cluster
      blinkFormation = DEFENSE;
      setColor(BLUE);
    }
    else if (abs(firstNeighborFace - secondNeighborFace) == 3) {//attack formation
      blinkFormation = ATTACK;
      setColor(ORANGE);
    }
    else {//balance formation
      blinkFormation = BALANCED;
      setColor(KINDABLUE);
    }
  }
  else {
    tailType = TAIL;
    blinkFormation = TAIL;
    setColor(WHITE);
  }

  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//neighbor!
      byte attackData = getLastValueReceivedOnFace(f);

      if (attackData == true && canTakeDamage) {
        decreaseHealth();
      }
      canTakeDamage = false;


    }
  }



  //  //send out face data

  //    byte sendData = (blinkColor << 4) + (blinkFormation << 2);
  //    setValueSentOnAllFaces(sendData);


  //
  //  switch (blinkFormation) {
  //    case ATTACK:
  //      setColorOnFace(ORANGE, random(5));
  //      break;
  //    case BALANCED:
  //      setColorOnFace(BLUE, random(5));
  //      break;
  //    case DEFENSE:
  //      setColorOnFace(OFF, random(5));
  //      break;
  //  }
  //
  //  switch (tailType) {
  //    case ATTACK:
  //      //      setColorOnFace(WHITE, random(5));
  //      setColor(ORANGE);
  //      break;
  //    case BALANCED:
  //      //      setColorOnFace(MAGENTA, random(5));
  //      setColor(WHITE);
  //      break;
  //    case DEFENSE:
  //      //      setColorOnFace(MAGENTA, random(5));
  //      setColor(BLUE);
  //      break;
  //  }

  for (int i = 5; i >= health; i--) {
    setColorOnFace(OFF, i);
  }
}
