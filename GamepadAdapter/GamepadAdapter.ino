typedef int bool;

// Config
const int NES_LATCH;
const int NES_CLK;
const int NES_DATA;

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

void startNesGamepad() {
  pinMode(NES_LATCH, OUTPUT);
  pinMode(NES_CLK, OUTPUT);
  pinMode(NES_DATA, INPUT);

  digitalWrite(NES_LATCH, LOW);
  digitalWrite(NES_CLK, HIGH);
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

void tickHigh(int pin) {
  digitalWrite(pin, HIGH);
  delayMicroseconds(1);
  digitalWrite(pin, LOW);
}

void tickLow(int pin) {
  digitalWrite(pin, LOW);
  delayMicroseconds(1);
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

void setup() {
  startNesGamepad();
  Serial.begin(9600);
}

void loop() {
  NesGamepadState state = getNesGamepadState();
  char *description = descriptionForState(state);
  Serial.println(description);
  free(description);
  delay(100);
}
