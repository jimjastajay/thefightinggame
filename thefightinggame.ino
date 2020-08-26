enum TileStates {NONE, CHOOSING, INFIGHT, DEAD};
enum FighterConfigs {NOCONFIG, BALANCED, ATTACK, DEFENSE};

byte tileState = NONE;
byte fighterConfig = NOCONFIG;

byte health = 6;

int faceOne = -1;
int faceTwo = -2;

#define PURPLE makeColorRGB(105, 5, 163)
#define GREY makeColorRGB(128, 128, 128)

struct FighterType {
  Color color;
};

FighterType one = {RED};
FighterType two = {YELLOW};
FighterType three = {GREEN};
FighterType four = {MAGENTA};
FighterType fighterList[4] = {{one}, {two}, {three}, {four}};
byte currentType = -1;

int currentNumberOfConnections;

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
      setColor(PURPLE);
    }
  }

}

void loop() {

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
