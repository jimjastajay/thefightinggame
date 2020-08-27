#define KINDABLUE makeColorRGB(0, 232, 209)
#define GREY makeColorRGB(128, 128, 128)

enum TileStates {NONE, WAITING, MIDDLEREADY, READY, INFIGHT, DEAD};
enum FighterConfigs {NOCONFIG, BALANCED, ATTACK, DEFENSE};

byte tileState = NONE;
byte fighterConfig = WAITING;

byte health = 6;
byte attackStrength = 0;
byte defenseStrength = 0;

int faceOne = -1;
int faceTwo = -2;

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

//Moves to the next color/fighter
void nextColor() {
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

//Resets the tile to it's basic setting
void resetTile() {
  health = 6;
  currentType = -1;
  tileState = WAITING;
  isCenterTile = false;
  setValueSentOnAllFaces(tileState);
  setColor(WHITE);
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
      fighterConfig = ATTACK;
      setColor(ORANGE);
    }
    else if (distance == 1 || distance == 5) {
      fighterConfig = DEFENSE;
      setColor(BLUE);
    }
    else {
      fighterConfig = BALANCED;
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

void loop() {

  listenForConnections();

  if (buttonDoubleClicked()) {
    nextColor();
  }
  if (buttonSingleClicked()) {
    decreaseHealth();
  }

  if (buttonMultiClicked()) {
    if (buttonClickCount() == 3) {
      resetTile();
    }
  }

  if (buttonLongPressed()) {
    getConnections();
  }

}
