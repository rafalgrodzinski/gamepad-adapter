// Config
const unsigned long debounceDuration = 32;

const int SNES_LATCH = 3;
const int SNES_CLK = 2;
const int SNES_DATA = 4;

const int DS_DATA = A5;
const int DS_CMD = A4;
const int DS_ATT = A3;
const int DS_CLK = A2;

// Types
typedef int bool;

typedef struct {
  bool state;
  bool newState;
  unsigned long newReadingTime;
} DebouncedInput;

typedef struct {
  DebouncedInput up;
  DebouncedInput down;
  DebouncedInput left;
  DebouncedInput right;
  DebouncedInput a;
  DebouncedInput b;
  DebouncedInput x;
  DebouncedInput y;
  DebouncedInput l;
  DebouncedInput r;
  DebouncedInput start;
  DebouncedInput select;
} SnesGamepadState;

typedef struct {
  uint8_t lx;
  uint8_t ly;
  uint8_t rx;
  uint8_t ry;
  bool up;
  bool down;
  bool left;
  bool right;
  bool circle;
  bool cross;
  bool square;
  bool triangle;
  bool l1;
  bool l2;
  bool l3;
  bool r1;
  bool r2;
  bool r3;
  bool start;
  bool select;
} DualShockState;

// General Functions
void tickHigh(int pin);
void tickLow(int pin);
void debounceAndUpdateInput(DebouncedInput *input, bool newState);

// SNES
void startSnesGamepad();
void updateState(SnesGamepadState *state);
bool isStateIdentical(SnesGamepadState *first, SnesGamepadState *second);
void encodeState(SnesGamepadState *state, uint8_t *data);
void printDescriptionForState(SnesGamepadState *state);

// DualShock
void startDualShock();
void updateState(DualShockState *state);
bool isStateIdentical(DualShockState *first, DualShockState *second);
void encodeState(DualShockState *state, uint8_t *data);
void printDescriptionForState(DualShockState *state);
