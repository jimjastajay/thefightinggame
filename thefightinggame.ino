enum TileState {NONE, CHOOSING, INFIGHT, DEAD};

byte health = 6;

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

void nextColor() {
  currentType += 1;
  if (currentType >= (sizeof(fighterList) / sizeof(fighterList[0]))) {
    currentType = 0;
  }
  setColor(fighterList[currentType].color);
}

void decreaseHealth() {

  if (health > 0) {
    health -= 1;
    for (int i = 5; i >= health; i--) {
      setColorOnFace(OFF, i);
    }
  }

}

void resetTile() {
  health = 6;
  currentType = -1;
  setColor(WHITE);
}

void markReady() {

}

void getConnections() {
  int count = 0;
  setColor(OFF);
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//a neighbor!
      count += 1;
      setColorOnFace(CYAN, f);
    }
  }
  currentNumberOfConnections = count;

  
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
