#include "GamepadAdapter.h"

void tickHigh(int pin) {
  digitalWrite(pin, HIGH);
  digitalWrite(pin, LOW);
}

void tickLow(int pin) {
  digitalWrite(pin, LOW);
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

void startSnesGamepad() {
  pinMode(SNES_LATCH, OUTPUT);
  pinMode(SNES_CLK, OUTPUT);
  pinMode(SNES_DATA, INPUT);

  digitalWrite(SNES_LATCH, LOW);
  digitalWrite(SNES_CLK, HIGH);
}

void updateSnesGamepadState(SnesGamepadState *state) {
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

char printDescriptionForState(SnesGamepadState *state) {
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

void setup() {
  startSnesGamepad();
  Serial.begin(115200);
}

SnesGamepadState state;
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
}
