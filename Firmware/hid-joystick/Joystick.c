/*
             LUFA Library
     Copyright (C) Dean Camera, 2019.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2019  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

#include "Joystick.h"

/** Buffer to hold the previously generated HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevJoystickHIDReportBuffer[sizeof(DualShockState)];

/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_HID_Device_t Joystick_HID_Interface = {
		.Config = {
				.InterfaceNumber = INTERFACE_ID_Joystick,
				.ReportINEndpoint = {
						.Address = JOYSTICK_EPADDR,
						.Size = JOYSTICK_EPSIZE,
						.Banks = 1,
					},
				.PrevReportINBuffer = PrevJoystickHIDReportBuffer,
				.PrevReportINBufferSize = sizeof(PrevJoystickHIDReportBuffer),
		},
};

uint8_t data[8];
uint8_t dataCounter;

int main(void) {
	SetupHardware();
	GlobalInterruptEnable();

	for (;;) {
		HID_Device_USBTask(&Joystick_HID_Interface);
		USB_USBTask();

        // Read data on serial connection from the main MCU
        if (Serial_IsCharReceived()) {
            LEDs_TurnOnLEDs(LEDS_LED1);

            if(dataCounter == 8)
                dataCounter = 0;
            data[dataCounter] = Serial_ReceiveByte();
            dataCounter++;
        }
	}
}

void SetupHardware(void) {
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	/* Hardware Initialization */
	LEDs_Init();
    Serial_Init(115200, true);
	USB_Init();
}

void EVENT_USB_Device_Connect(void)
{
}

void EVENT_USB_Device_Disconnect(void) {
}

void EVENT_USB_Device_ConfigurationChanged(void) {
	HID_Device_ConfigureEndpoints(&Joystick_HID_Interface);
	USB_Device_EnableSOFEvents();
}

void EVENT_USB_Device_ControlRequest(void) {
	HID_Device_ProcessControlRequest(&Joystick_HID_Interface);
}

void EVENT_USB_Device_StartOfFrame(void) {
	HID_Device_MillisecondElapsed(&Joystick_HID_Interface);
}

bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t *const HIDInterfaceInfo, uint8_t *const ReportID,
                                         const uint8_t ReportType, void *ReportData, uint16_t *const ReportSize) {
    // Wait for the two values to be filled in
    if (dataCounter != 8)
        return false;

    DualShockState *state = (DualShockState *)ReportData;
    state->direction = data[0];
    state->buttons[0] = data[1];
    state->buttons[1] = data[2];
    // Convert sticks data from 0 to 255 into -128 to 127
    state->leftStick[0] = data[3] - 0x80;
    state->leftStick[1] = data[4] - 0x80;
    state->rightStick[0] = data[5] - 0x80;
    state->rightStick[1] = data[6] - 0x80;
	*ReportSize = sizeof(DualShockState);

    LEDs_TurnOffLEDs(LEDS_LED1);

	return true;
}

void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo, const uint8_t ReportID,
                                          const uint8_t ReportType, const void* ReportData, const uint16_t ReportSize) {
}

