# Gamepad Decoder

This is an Arduino based adapter for a numbe of different gamepads.

## Supported gamepads
* NES
* SNES

## SNES Gamepad Description
SNES gamepad is almost identical to a NES gamepad, with the difference of having 12 buttons instead of 8. It also has +5V, Gnd, `LATCH`, `DATA`, and `CLOCK` lines. The remaining two pins on the plug remain unused.

Eletronics of the gamepad consist of a single V520B IC, which is a 16-bit shift register. Gamepad receives a `LATCH` signal (active high), which causes state of the buttons to be stored. state of the first button is then available on the `DATA` line. `CLOCK` (active low) is sent multiple times, which causes subsequent states to be available. There are 12 buttons, so the remaining 4 bits remain unused. Button signal is low when active.

Buttons are received in the following order:

* B
* Y
* Select
* Start
* Up
* Down
* Left
* Right
* A
* X
* L
* R