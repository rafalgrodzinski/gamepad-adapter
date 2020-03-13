// Config
const int SNES_LATCH = 3;
const int SNES_CLK = 2;
const int SNES_DATA = 4;
const unsigned long debounceDuration = 32;

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

// General Functions
void tickHigh(int pin);
void tickLow(int pin);
void debounceAndUpdateInput(DebouncedInput *input, bool newState);

// SNES
void startSnesGamepad();
void updateSnesGamepadState(SnesGamepadState *state);
bool isStateIdentical(SnesGamepadState *first, SnesGamepadState *second);
uint8_t encodedDirectionForState(SnesGamepadState *state);
uint8_t encodedButtonsStateForState(SnesGamepadState *state);
char printDescriptionForState(SnesGamepadState *state);
