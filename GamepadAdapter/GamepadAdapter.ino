typedef int bool;

// Config
const int NES_LATCH;
const int NES_CLK;
const int NES_DATA;

const int SNES_LATCH = 3;
const int SNES_CLK = 2;
const int SNES_DATA = 4;

typedef struct {
  bool up;
  bool down;
  bool left;
  bool right;
  bool a;
  bool b;
  bool start;
  bool select;
} NesGamepadState;

typedef struct {
  bool up;
  bool down;
  bool left;
  bool right;
  bool a;
  bool b;
  bool x;
  bool y;
  bool l;
  bool r;
  bool start;
  bool select;
} SnesGamepadState;

void startNesGamepad() {
  pinMode(NES_LATCH, OUTPUT);
  pinMode(NES_CLK, OUTPUT);
  pinMode(NES_DATA, INPUT);

  digitalWrite(NES_LATCH, LOW);
  digitalWrite(NES_CLK, HIGH);
}

void startSnesGamepad() {
  pinMode(SNES_LATCH, OUTPUT);
  pinMode(SNES_CLK, OUTPUT);
  pinMode(SNES_DATA, INPUT);

  digitalWrite(SNES_LATCH, LOW);
  digitalWrite(SNES_CLK, HIGH);
}

NesGamepadState getNesGamepadState() {
  NesGamepadState state;
  tickHigh(NES_LATCH);
  
  state.a = digitalRead(NES_DATA) == LOW;
  tickLow(NES_CLK);
  state.b = digitalRead(NES_DATA) == LOW;
  tickLow(NES_CLK);
  state.select = digitalRead(NES_DATA) == LOW;
  tickLow(NES_CLK);
  state.start = digitalRead(NES_DATA) == LOW;
  tickLow(NES_CLK);
  state.up = digitalRead(NES_DATA) == LOW;
  tickLow(NES_CLK);
  state.down = digitalRead(NES_DATA) == LOW;
  tickLow(NES_CLK);
  state.left = digitalRead(NES_DATA) == LOW;
  tickLow(NES_CLK);
  state.right = digitalRead(NES_DATA) == LOW;
 
  return state;
}

SnesGamepadState getSnesGamepadState() {
  SnesGamepadState state;
  tickHigh(SNES_LATCH);

  state.b = digitalRead(SNES_DATA) == LOW;
  tickLow(SNES_CLK);
  state.y = digitalRead(SNES_DATA) == LOW;
  tickLow(SNES_CLK);
  state.select = digitalRead(SNES_DATA) == LOW;
  tickLow(SNES_CLK);
  state.start = digitalRead(SNES_DATA) == LOW;
  tickLow(SNES_CLK);
  state.up = digitalRead(SNES_DATA) == LOW;
  tickLow(SNES_CLK);
  state.down = digitalRead(SNES_DATA) == LOW;
  tickLow(SNES_CLK);
  state.left = digitalRead(SNES_DATA) == LOW;
  tickLow(SNES_CLK);
  state.right = digitalRead(SNES_DATA) == LOW;
  tickLow(SNES_CLK);
  state.a = digitalRead(SNES_DATA) == LOW;
  tickLow(SNES_CLK);
  state.x = digitalRead(SNES_DATA) == LOW;
  tickLow(SNES_CLK);
  state.l = digitalRead(SNES_DATA) == LOW;
  tickLow(SNES_CLK);
  state.r = digitalRead(SNES_DATA) == LOW;
  
  return state;
}

bool isStateSame(SnesGamepadState first, SnesGamepadState second) {
  if (first.up != second.up || first.down != second.down || first.left != second.left || first.right != second.right ||
    first.a != second.a || first.b != second.b || first.x != second.x || first.y != second.y ||
    first.l != second.l || first.r != second.r || first.start != second.start || first.select != second.select) {
    return false;
  } else {
    return true;
  }
}

void tickHigh(int pin) {
  digitalWrite(pin, HIGH);
  digitalWrite(pin, LOW);
}

void tickLow(int pin) {
  digitalWrite(pin, LOW);
  digitalWrite(pin, HIGH);
}

char *descriptionForState(NesGamepadState state) {
  char *buffer = malloc(sizeof(char) * 100);
  strcpy(buffer, "");
  strcat(buffer, state.up ? "UP, " : "up, ");
  strcat(buffer, state.down ? "DOWN, " : "down, ");
  strcat(buffer, state.left ? "LEFT, " : "left, ");
  strcat(buffer, state.right ? "RIGHT, " : "right, ");
  strcat(buffer, state.a ? "A, " : "a, ");
  strcat(buffer, state.b ? "B, " : "b, ");
  strcat(buffer, state.start ? "START, " : "start, ");
  strcat(buffer, state.select ? "SELECT" : "select");
  return buffer;
}

char *descriptionForState(SnesGamepadState state) {
  char *buffer = malloc(sizeof(char) * 100);
  strcpy(buffer, "");
  strcat(buffer, state.up ? "UP, " : "up, ");
  strcat(buffer, state.down ? "DOWN, " : "down, ");
  strcat(buffer, state.left ? "LEFT, " : "left, ");
  strcat(buffer, state.right ? "RIGHT, " : "right, ");
  strcat(buffer, state.a ? "A, " : "a, ");
  strcat(buffer, state.b ? "B, " : "b, ");
  strcat(buffer, state.x ? "X, " : "x, ");
  strcat(buffer, state.y ? "Y, " : "y, ");
  strcat(buffer, state.l ? "L, " : "l, ");
  strcat(buffer, state.r ? "R, " : "r, ");
  strcat(buffer, state.start ? "START, " : "start, ");
  strcat(buffer, state.select ? "SELECT" : "select");
  return buffer;
}

uint8_t encodedButtonsStateForState(SnesGamepadState state) {
  uint8_t buttons = 0;
  buttons |= state.y << 0;
  buttons |= state.b << 1;
  buttons |= state.a << 2;
  buttons |= state.x << 3;
  buttons |= state.l << 4;
  buttons |= state.r << 5;
  buttons |= state.select << 6;
  buttons |= state.start << 7;
  return buttons;
}

uint8_t encodedDirectionForState(SnesGamepadState state) {
  if (state.up && !state.right && !state.down && !state.left) {
    return 1;
  } else if (state.up && state.right && !state.down && !state.left) {
    return 2;
  } else if (!state.up && state.right && !state.down && !state.left) {
    return 3;
  } else if (!state.up && state.right && state.down && !state.left) {
    return 4;
  } else if (!state.up && !state.right && state.down && !state.left) {
    return 5;
  } else if (!state.up && !state.right && state.down && state.left) {
    return 6;
  } else if (!state.up && !state.right && !state.down && state.left) {
    return 7;
  } else if (state.up && !state.right && !state.down && state.left) {
    return 8;
  } else {
    return 0;
  }
}

void setup() {
  startSnesGamepad();
  Serial.begin(9600);
}

SnesGamepadState oldState;
void loop() {
  SnesGamepadState state = getSnesGamepadState();
  if (!isStateSame(state, oldState)) {
    oldState = state;
    //char *description = descriptionForState(state);
    //Serial.println(description);
    //free(description);
    uint8_t buttonsState = encodedButtonsStateForState(state);
    uint8_t directionState = encodedDirectionForState(state);
    Serial.write(buttonsState);
    Serial.write(directionState);
    /*char snum[10];
    itoa(buttonsState, snum, 10);
    Serial.write(snum);
    itoa(directionState, snum, 10);
    Serial.print(" | ");
    Serial.write(snum);
    Serial.println();*/
  }
}
