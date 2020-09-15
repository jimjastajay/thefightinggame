#define KINDABLUE makeColorRGB(0, 232, 209)
#define GREY makeColorRGB(128, 128, 128)

enum TileStates {NONE, WAITING, MIDDLEREADY, READY, ATTACKING, DEFENDING, DEAD};
enum blinkFormations {NOCONFIG, BALANCED, ATTACK, DEFENSE, TAIL};

byte tileState = NONE;
byte blinkFormation = NOCONFIG;

byte attackVal[6] = {0, 0, 0, 0, 0, 0};
byte tailType = TAIL;
byte blinkColor = 0;


/////////////////////////////
byte health = 6;
byte attackStrength = 0;
byte defenseStrength = 0;

int faceOne = -1;
int faceTwo = -2;

int teamNumber = 0;

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
byte currentType = -1;

int currentNumberOfConnections;
int configReady = 0;
bool isCenterTile = false;

void setup() {
  resetTile();
}

//Resets the tile to it's basic setting
void resetTile() {
  health = 6;
  currentType = -1;
  tileState = WAITING;
  isCenterTile = false;
  setValueSentOnAllFaces(tileState);
  setColor(WHITE);
}

//Moves to the next color/fighter
void nextColor() {
  tileState = WAITING;
  currentType += 1;
  if (currentType >= (sizeof(fighterList) / sizeof(fighterList[0]))) {
    currentType = 0;
  }
  setColor(fighterList[currentType].color);
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

//Lights up the number of current connections
void getConnections() {
  int count = 0;
  faceOne = -1;
  faceTwo = -1;
  currentNumberOfConnections = 0;
  setColor(OFF);

  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//a neighbor!
      if (faceOne == -1) {
        count += 1;
        faceOne = f;
        setColorOnFace(CYAN, faceOne);
      }
      else {
        if (faceTwo == -1) {
          count += 1;
          faceTwo = f;
          setColorOnFace(CYAN, faceTwo);
        }
      }
    }
  }
  currentNumberOfConnections = count;
  setConfiguration();
}

void setConfiguration() {
  if (currentNumberOfConnections == 2) {
    isCenterTile = true;
    setValueSentOnFace(1, faceOne);
    setValueSentOnFace(1, faceOne);

    int distance = abs(faceOne - faceTwo);
    if (distance == 3) {
      blinkFormation = ATTACK;
      setColor(ORANGE);
    }
    else if (distance == 1 || distance == 5) {
      blinkFormation = DEFENSE;
      setColor(BLUE);
    }
    else {
      blinkFormation = BALANCED;
      setColor(KINDABLUE);
    }

    tileState = MIDDLEREADY;
    setValueSentOnFace(tileState, faceOne);
    setValueSentOnFace(tileState, faceTwo);
  }
}

void listenForConnections() {
  if (tileState == WAITING && !isCenterTile) {
    FOREACH_FACE(f) {
      if (!isValueReceivedOnFaceExpired(f)) {//a neighbor!
        if (getLastValueReceivedOnFace(f) == MIDDLEREADY) {//a neighbor saying GO!
          setColorOnFace(WHITE, getOppositeFace(f));
          tileState = READY;
        }
      }
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

      if (firstNeighborFace == 6) {
        neighborsFound += 1;
        firstNeighborFace = f;
        //        setColorOnFace(CYAN, firstNeighborFace);
        //        setColorOnFace(WHITE, getOppositeFace(f));
      }
      else {
        if (secondNeighborFace == 6) {
          neighborsFound += 1;
          secondNeighborFace = f;
          //          setColorOnFace(WHITE, getOppositeFace(f));
          //          setColorOnFace(CYAN, secondNeighborFace);
        }
      }

      //      if (getBlinkColor(neighborData) == blinkColor) { //same color!
      //        neighborsFound++;
      //        if (firstNeighborFace == 6) {
      //          firstNeighborFace = f;
      //        }
      //        else if (secondNeighborFace == 6) {
      //          secondNeighborFace = f;
      //        }
      //      }

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

}
