#include "GamepadAdapter.h"

// General
void tickHigh(int pin) {
  digitalWrite(pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin, LOW);
}

void tickLow(int pin) {
  digitalWrite(pin, LOW);
  delayMicroseconds(10);
  digitalWrite(pin, HIGH);
}

void debounceAndUpdateInput(DebouncedInput *input, bool newState) {
  unsigned long currentTime = millis();
  if (currentTime - input->newReadingTime > debounceDuration) {
    if (newState == input->newState) {
      input->state = newState;
    } else {
      input->newState = newState;
      input->newReadingTime = currentTime;
    }
  }
}

void sendByte(uint8_t data, int dataPin, int clockPin) { 
  for (int i=0; i<8; i++) {
    uint8_t bit = (data >> i) & 0x01;
    digitalWrite(dataPin, bit ? HIGH : LOW);
  }
}

uint8_t receiveByte(int dataPin, int clockPin) {
  uint8_t data = 0;
  for (int i=0; i<8; i++) {
    tickLow(clockPin);
    uint8_t bit = digitalRead(dataPin) == HIGH;
    data |= bit << i;
  }
  return data;
}

uint8_t exchangeBytes(uint8_t txData, int txPin, int rxPin, int clockPin) {
  uint8_t rxData = 0;
  for (int i=0; i<8; i++) {
    uint8_t txBit = (txData >> i) & 0x01;
    digitalWrite(txPin, txBit ? HIGH : LOW);
    tickLow(clockPin);
    uint8_t rxBit = digitalRead(rxPin) == HIGH;
    rxData |= rxBit << i;
  }
  return rxData;
}

// SNES
void startSnesGamepad() {
  pinMode(SNES_LATCH, OUTPUT);
  pinMode(SNES_CLK, OUTPUT);
  pinMode(SNES_DATA, INPUT);

  digitalWrite(SNES_LATCH, LOW);
  digitalWrite(SNES_CLK, HIGH);
}

void updateState(SnesGamepadState *state) {
  tickHigh(SNES_LATCH);

  debounceAndUpdateInput(&state->b, digitalRead(SNES_DATA) == LOW);
  tickLow(SNES_CLK);
  debounceAndUpdateInput(&state->y, digitalRead(SNES_DATA) == LOW);
  tickLow(SNES_CLK);
  debounceAndUpdateInput(&state->select, digitalRead(SNES_DATA) == LOW);
  tickLow(SNES_CLK);
  debounceAndUpdateInput(&state->start, digitalRead(SNES_DATA) == LOW);
  tickLow(SNES_CLK);
  debounceAndUpdateInput(&state->up, digitalRead(SNES_DATA) == LOW);
  tickLow(SNES_CLK);
  debounceAndUpdateInput(&state->down, digitalRead(SNES_DATA) == LOW);
  tickLow(SNES_CLK);
  debounceAndUpdateInput(&state->left, digitalRead(SNES_DATA) == LOW);
  tickLow(SNES_CLK);
  debounceAndUpdateInput(&state->right, digitalRead(SNES_DATA) == LOW);
  tickLow(SNES_CLK);
  debounceAndUpdateInput(&state->a, digitalRead(SNES_DATA) == LOW);
  tickLow(SNES_CLK);
  debounceAndUpdateInput(&state->x, digitalRead(SNES_DATA) == LOW);
  tickLow(SNES_CLK);
  debounceAndUpdateInput(&state->l, digitalRead(SNES_DATA) == LOW);
  tickLow(SNES_CLK);
  debounceAndUpdateInput(&state->r, digitalRead(SNES_DATA) == LOW);
  
  return state;
}

bool isStateIdentical(SnesGamepadState *first, SnesGamepadState *second) {
  if (first->up.state != second->up.state ||
    first->down.state != second->down.state ||
    first->left.state != second->left.state ||
    first->right.state != second->right.state ||
    first->a.state != second->a.state ||
    first->b.state != second->b.state ||
    first->x.state != second->x.state ||
    first->y.state != second->y.state ||
    first->l.state != second->l.state ||
    first->r.state != second->r.state ||
    first->start.state != second->start.state ||
    first->select.state != second->select.state) {
    return false;
  } else {
    return true;
  }
}

uint8_t encodedDirectionForState(SnesGamepadState *state) {
  bool up = state->up.state;
  bool right = state->right.state;
  bool down = state->down.state;
  bool left = state->left.state;
 
  // Up
  if (up && !right && !down && !left) {
    return 1;
  // Up Right
  } else if (up && right && !down && !left) {
    return 2;
  // Right
  } else if (!up && right && !down && !left) {
    return 3;
  // Right Down
  } else if (!up && right && down && !left) {
    return 4;
  // Down
  } else if (!up && !right && down && !left) {
    return 5;
  // Down Left
  } else if (!up && !right && down && left) {
    return 6;
  // Left
  } else if (!up && !right && !down && left) {
    return 7;
  // Left Up
  } else if (up && !right && !down && left) {
    return 8;
  } else {
    return 0;
  }
}

uint8_t encodedButtonsStateForState(SnesGamepadState *state) {
  uint8_t buttons = 0;
  buttons |= state->y.state << 0;
  buttons |= state->b.state << 1;
  buttons |= state->a.state << 2;
  buttons |= state->x.state << 3;
  buttons |= state->l.state << 4;
  buttons |= state->r.state << 5;
  buttons |= state->select.state << 6;
  buttons |= state->start.state << 7;
  return buttons;
}

void printDescriptionForState(SnesGamepadState *state) {
  Serial.print(state->up.state ? "UP, " : "up, ");
  Serial.print(state->down.state ? "DOWN, " : "down, ");
  Serial.print(state->left.state ? "LEFT, " : "left, ");
  Serial.print(state->right.state ? "RIGHT, " : "right, ");
  Serial.print(state->a.state ? "A, " : "a, ");
  Serial.print(state->b.state ? "B, " : "b, ");
  Serial.print(state->x.state ? "X, " : "x, ");
  Serial.print(state->y.state ? "Y, " : "y, ");
  Serial.print(state->l.state ? "L, " : "l, ");
  Serial.print(state->r.state ? "R, " : "r, ");
  Serial.print(state->start.state ? "START, " : "start, ");
  Serial.print(state->select.state ? "SELECT" : "select");
  Serial.println();
}

// DualShock
void startDualShock() {
  pinMode(DS_DATA, INPUT);
  pinMode(DS_CMD, OUTPUT);
  pinMode(DS_ATT, OUTPUT);
  pinMode(DS_CLK, OUTPUT);

  digitalWrite(DS_CLK, HIGH);
  digitalWrite(DS_ATT, HIGH);
  // Has to be pulled-up because the gamepad can't do it itself
  digitalWrite(DS_DATA, HIGH);
}

void updateState(DualShockState *state) {
  digitalWrite(DS_ATT, LOW);
  exchangeBytes(0x01, DS_CMD, DS_DATA, DS_CLK);
  exchangeBytes(0x42, DS_CMD, DS_DATA, DS_CLK);
  receiveByte(DS_DATA, DS_CLK);
  uint8_t first = ~receiveByte(DS_DATA, DS_CLK);
  uint8_t second = ~receiveByte(DS_DATA, DS_CLK);
  digitalWrite(DS_ATT, HIGH);

  state->select = (first >> 0) & 0x01;
  state->start  = (first >> 3) & 0x01;
  state->up     = (first >> 4) & 0x01;
  state->right  = (first >> 5) & 0x01;
  state->down   = (first >> 6) & 0x01;
  state->left   = (first >> 7) & 0x01;

  state->l2       = (second >> 0) & 0x01;
  state->r2       = (second >> 1) & 0x01;
  state->l1       = (second >> 2) & 0x01;
  state->r1       = (second >> 3) & 0x01;
  state->triangle = (second >> 4) & 0x01;
  state->circle   = (second >> 5) & 0x01;
  state->cross    = (second >> 6) & 0x01;
  state->square   = (second >> 7) & 0x01;
}

bool isStateIdentical(DualShockState *first, DualShockState *second) {
  if (first->up != second->up || first->down != second->down || first->left != second->left || first->right != second->right ||
    first->circle != second->circle || first->cross != second->cross || first->triangle != second->triangle || first->square != second->square ||
    first->l1 != second->l1 || first->l2 != second->l2 || first->r1 != second->r1 || first->r2 != second->r2 ||
    first->start != second->start || first->select != second->select) {
    return false;
  } else {
    return true;
  }
}

void printDescriptionForState(DualShockState *state) {
  Serial.print(state->up ? "UP, " : "up, ");
  Serial.print(state->down ? "DOWN, " : "down, ");
  Serial.print(state->left ? "LEFT, " : "left, ");
  Serial.print(state->right ? "RIGHT, " : "right, ");
  Serial.print(state->circle ? "O, " : "o, ");
  Serial.print(state->cross ? "X, " : "x, ");
  Serial.print(state->triangle ? "T, " : "t, ");
  Serial.print(state->square ? "S, " : "s, ");
  Serial.print(state->l1 ? "L1, " : "l1, ");
  Serial.print(state->l2 ? "L2, " : "l2, ");
  Serial.print(state->r1 ? "R1, " : "r1, ");
  Serial.print(state->r2 ? "R2, " : "r2, ");
  Serial.print(state->start ? "START, " : "start, ");
  Serial.print(state->select ? "SELECT" : "select");
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  //startSnesGamepad();
  startDualShock();
}

/*SnesGamepadState state;
SnesGamepadState oldState;
void loop() {
  updateSnesGamepadState(&state);
  if (!isStateIdentical(&state, &oldState)) {
    oldState = state;
    uint8_t directionState = encodedDirectionForState(&state);
    uint8_t buttonsState = encodedButtonsStateForState(&state);
    Serial.write(directionState);
    Serial.write(buttonsState);
    //printDescriptionForState(&state);
  }
}*/

//SnesGamepadState state;
//SnesGamepadState oldState;
DualShockState state;
DualShockState oldState;
void loop() {
  updateState(&state);
  if (!isStateIdentical(&state, &oldState)) {
    oldState = state;
    /*uint8_t directionState = encodedDirectionForState(&state);
    uint8_t buttonsState = encodedButtonsStateForState(&state);
    Serial.write(directionState);
    Serial.write(buttonsState);*/
    printDescriptionForState(&state);
  }
}
