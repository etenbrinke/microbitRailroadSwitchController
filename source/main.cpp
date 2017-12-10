#include "MicroBit.h"
#include "inc/neopixel.h"

#define BUTTON_1_FLIP_SWITCH_1      1
#define BUTTON_2_FLIP_SWITCH_2      2
#define BUTTON_3_FLIP_SWITCH_3      3
#define BUTTON_4_FLIP_SWITCH_4      4
#define BUTTON_5_PRESSED            5
#define BUTTON_6_PRESSED            6
#define BUTTON_7_RESET              7
#define BUTTON_8_PRESSED            8

#define SWITCH_1                    1
#define SWITCH_2                    2
#define SWITCH_3                    3
#define SWITCH_4                    4

#define LED_SWITCH_COUNTER          24
#define LED_SIGNAL_COUNTER          250
#define LOOP_DELAY                  300
#define RANDOM_SWITCH               300
#define RANDOM_SIGNAL               200

enum LedGroup {
  PANEL_LEDS = 1, 
  SIGNAL_POLE_LEDS_1 = 2, 
  SIGNAL_POLE_LEDS_2 = 3
};

Color RED_L (0, 40, 0);
Color RED (0, 70, 0);
Color RED_H (0, 100, 0);
Color GREEN (70, 0, 0);
Color GREEN_H (100, 0, 0);
Color YELLOW (35, 80, 0);
Color PURPLE (0, 35, 80);
Color BLACK (0, 0, 0);

class NeoPixelLed {
    private:
        uint8_t pixelNumber;
        LedGroup ledGroup;
        Color color;
    public:
        NeoPixelLed ();
        NeoPixelLed (uint8_t pn, LedGroup lg, Color c);
        void setColor (Color c) {
            color = c;
        }
        uint8_t getPixelNumber() {
            return pixelNumber;
        }
        LedGroup getLedGroup() {
            return ledGroup;
        }
};

NeoPixelLed::NeoPixelLed () {
    pixelNumber = 0;
    ledGroup = PANEL_LEDS;
    color = BLACK;
};

NeoPixelLed::NeoPixelLed (uint8_t pn, LedGroup lg, Color c) {
    pixelNumber = pn;
    ledGroup = lg;
    color = c;
};

class SignalPole {
    private:
        NeoPixelLed neoPixelLed1;
        NeoPixelLed neoPixelLed2;
    public:
        SignalPole ();
        SignalPole (NeoPixelLed np1, NeoPixelLed np2);
        NeoPixelLed getNeoPixelLed1 () {
            return neoPixelLed1;
        }
        NeoPixelLed getNeoPixelLed2 () {
            return neoPixelLed2;
        }
};

SignalPole::SignalPole () {
    neoPixelLed1 = NeoPixelLed();
    neoPixelLed2 = NeoPixelLed();
};

SignalPole::SignalPole (NeoPixelLed np1, NeoPixelLed np2) {
    neoPixelLed1 = np1;
    neoPixelLed2 = np2;
};

class Switch {
    private:
        uint8_t switchNumber;
        uint8_t switchPosition;
        SignalPole signalPole1;
        SignalPole signalPole2;
        NeoPixelLed panelLed;
        bool operate;
    public:
        Switch (uint8_t snr, uint8_t sp, SignalPole sp1, SignalPole sp2, NeoPixelLed p, bool o);
        uint8_t getSwitchNumber () {
            return switchNumber;
        }
        void setSwitchPosition (uint8_t sp) {
            switchPosition = sp;
            operate = true;
        }
        uint8_t getSwitchPosition () {
            return switchPosition;
        }
        SignalPole getSignalPole1 () {
            return signalPole1;
        }
        SignalPole getSignalPole2 () {
            return signalPole2;
        }
        void setOperate (bool o) {
            operate = o;
        }
        bool getOperate () {
            return operate;
        }
        NeoPixelLed getPanelLed () {
            return panelLed;
        }
};

Switch::Switch (uint8_t snr, uint8_t sp, SignalPole sp1, SignalPole sp2, NeoPixelLed p, bool o) {
    switchNumber = snr;
    switchPosition = sp;
    signalPole1 = sp1;
    signalPole2 = sp2;
    panelLed = p;
    operate = o;
};

// Okay, object Switch reached the memory limit, no more properties allowed to add. Therefore we create a new class
// LedCounterSwitch for storing the counter properties instead of making these properties part of the Switch class, Ugly

class LedCounterSwitch {
    private:
        uint8_t switchOperationCounter;
        uint16_t holdCounter;
        uint16_t driveSlowCounter;
        uint16_t driveWarningCounter;
    public:
        LedCounterSwitch (uint8_t soc, uint16_t hc, uint16_t dsc, uint16_t dwc);
        uint8_t getswitchOperationCounter () {
            return switchOperationCounter;
        }
        void setSwitchOperationCounter (uint8_t soc) {
            switchOperationCounter = soc;
        }
        uint8_t getHoldCounter () {
            return holdCounter;
        }
        void setHoldCounter (uint16_t shc) {
            holdCounter = shc;
        }
        uint16_t getDriveSlowCounter () {
            return driveSlowCounter;
        }
        void setDriveSlowCounter (uint16_t dsc) {
            driveSlowCounter = dsc;
        }
        uint16_t getDriveWarningCounter () {
            return driveWarningCounter;
        }
        void setDriveWarningCounter (uint16_t dwc) {
            driveWarningCounter = dwc;
        }
};

LedCounterSwitch::LedCounterSwitch (uint8_t soc, uint16_t hc, uint16_t dsc, uint16_t dwc) {
        switchOperationCounter = soc;
        holdCounter = hc;
        driveSlowCounter = dsc;
        driveWarningCounter = dwc;
};

MicroBit uBit;

MicroBitPin P0(MICROBIT_ID_IO_P0, MICROBIT_PIN_P0, PIN_CAPABILITY_DIGITAL);
MicroBitPin P1(MICROBIT_ID_IO_P1, MICROBIT_PIN_P1, PIN_CAPABILITY_DIGITAL);
MicroBitPin P2(MICROBIT_ID_IO_P2, MICROBIT_PIN_P2, PIN_CAPABILITY_DIGITAL);
MicroBitPin P3(MICROBIT_ID_IO_P3, MICROBIT_PIN_P3, PIN_CAPABILITY_DIGITAL);
MicroBitPin P4(MICROBIT_ID_IO_P4, MICROBIT_PIN_P4, PIN_CAPABILITY_DIGITAL);
MicroBitPin P5(MICROBIT_ID_IO_P5, MICROBIT_PIN_P5, PIN_CAPABILITY_DIGITAL);
MicroBitPin P6(MICROBIT_ID_IO_P6, MICROBIT_PIN_P6, PIN_CAPABILITY_DIGITAL);
MicroBitPin P7(MICROBIT_ID_IO_P7, MICROBIT_PIN_P7, PIN_CAPABILITY_DIGITAL);
MicroBitPin P9(MICROBIT_ID_IO_P9, MICROBIT_PIN_P9, PIN_CAPABILITY_DIGITAL);
MicroBitPin P10(MICROBIT_ID_IO_P10, MICROBIT_PIN_P10, PIN_CAPABILITY_DIGITAL);
MicroBitPin P11(MICROBIT_ID_IO_P11, MICROBIT_PIN_P11, PIN_CAPABILITY_DIGITAL);
// as we will not use the SPI bus anyway lets reserve P13, P14 and P15 for GPIO future extensions
//MicroBitPin P13(MICROBIT_ID_IO_P13, MICROBIT_PIN_P13, PIN_CAPABILITY_DIGITAL);
//MicroBitPin P14(MICROBIT_ID_IO_P14, MICROBIT_PIN_P14, PIN_CAPABILITY_DIGITAL);
//MicroBitPin P15(MICROBIT_ID_IO_P15, MICROBIT_PIN_P15, PIN_CAPABILITY_DIGITAL);

uint8_t buttonPressed;
uint8_t randomSwitch;
uint8_t randomLedOperation;
uint16_t pulseLength;
bool randomSwitchMode;
bool randomLedMode;

// LED's above buttons
Neopixel panelLEDs(MICROBIT_PIN_P16, 8);
// LED's next to switches on pin8
Neopixel signalPoleLEDs1(MICROBIT_PIN_P8, 8);
// LED's next to switches on pin12
Neopixel signalPoleLEDs2(MICROBIT_PIN_P12, 8);

void showColor(LedGroup LedGroup, uint8_t pixel, Color color) {
    switch (LedGroup) {
        case PANEL_LEDS:
            panelLEDs.showColor(pixel, color);
            break;
        case SIGNAL_POLE_LEDS_1:
            signalPoleLEDs1.showColor(pixel, color);
            break;
        case SIGNAL_POLE_LEDS_2:
            signalPoleLEDs2.showColor(pixel, color);
            break;
        default:
            break;
    }    
}

void sendSwitchPositionsToSerial(uint8_t sw1p, uint8_t sw2p, uint8_t sw3p, uint8_t sw4p) {
    uBit.serial.printf("%i|%i|%i|%i\r\n", sw1p, sw2p, sw3p, sw4p);
}

// Power off all switch coins
void removePowerFromSwitches() {
    uBit.io.P0.setDigitalValue(0); // # switch 1, coin 1
    uBit.io.P1.setDigitalValue(0); // # switch 1, coin 2
    uBit.io.P2.setDigitalValue(0); // # switch 2, coin 1
    uBit.io.P3.setDigitalValue(0); // # switch 2, coin 2
    uBit.io.P4.setDigitalValue(0); // # switch 3, coin 1
    uBit.io.P6.setDigitalValue(0); // # switch 3, coin 2
    uBit.io.P7.setDigitalValue(0); // # switch 4, coin 1
    uBit.io.P9.setDigitalValue(0); // # switch 4, coin 2
}

// Read binary representation of the 10 to 4 line encoder (74HCT147) and convert to decimal
// We use it as 8 to 3 encoder
// On the encoder inputs I1..I8 we have 8 buttons
// The encoder outputs Y0, Y1 and Y2 are connected to pin5, pin10 and pin11
uint8_t readButtonsPressed() {
    uint8_t decimal = 0;

    if (uBit.io.P5.getDigitalValue())
        decimal = 1;
    if (uBit.io.P10.getDigitalValue())
        decimal = decimal + 2;
    if (uBit.io.P11.getDigitalValue())
        decimal = decimal + 4;
    // encoder has strange way of binary counting
    // but we do not use output 8 so commenting out
    // uint8_t output = 7 - decimal;
    // if (output == 0)
    //    output = 8;
    // return output;
    return 7 - decimal;
}

void powerLEDOn(bool on) {
    if (on)
        // use red as this uses less power then green or blue
        showColor(PANEL_LEDS, 7, RED_L);
    else
        showColor(PANEL_LEDS, 7, BLACK);
}

// Event on receiving characters '1', '2', '3' or '4' via USB
void onChar(MicroBitEvent) {
    int i = uBit.serial.read(ASYNC);
    switch (i) {
        case int('1'):
            uBit.serial.printf("Flip switch 1 by USB\r\n");
            //flipSwitch(1);
            break;

        case int('2'):
            uBit.serial.printf("Flip switch 2 by USB\r\n");
            //flipSwitch(2);
            break;

        case int('3'):
            uBit.serial.printf("Flip switch 3 by USB\r\n");
            //flipSwitch(3);
            break;

        case int('4'):
            uBit.serial.printf("Flip switch 4 by USB\r\n");
            //flipSwitch(4);
            break;
        default:
            break;
    }

    uBit.serial.clearRxBuffer();
    uBit.serial.eventAfter(1, ASYNC);
}

LedCounterSwitch OperateLeds(Switch sw, LedCounterSwitch lcs) {
    SignalPole sp1 = sw.getSignalPole1();
    SignalPole sp2 = sw.getSignalPole2();
    NeoPixelLed led1Sp1 = sp1.getNeoPixelLed1();
    NeoPixelLed led2Sp1 = sp1.getNeoPixelLed2();
    NeoPixelLed led1Sp2 = sp2.getNeoPixelLed1();
    NeoPixelLed led2Sp2 = sp2.getNeoPixelLed2();
    NeoPixelLed panelLed = sw.getPanelLed();

    uint8_t soc = lcs.getswitchOperationCounter();
    uint16_t hc = lcs.getHoldCounter();
    uint16_t dsc = lcs.getDriveSlowCounter();
    uint16_t dwc = lcs.getDriveWarningCounter();
    if (soc > 0) { // A Switch Operation signal gets preference above a Hold, Drive Slow or Drive Warning signal
        soc--;
        if (soc == 0) {
            showColor(led1Sp1.getLedGroup(), led1Sp1.getPixelNumber(), BLACK);
            showColor(led2Sp1.getLedGroup(), led2Sp1.getPixelNumber(), BLACK);
            showColor(led1Sp2.getLedGroup(), led1Sp2.getPixelNumber(), BLACK);
            showColor(led2Sp2.getLedGroup(), led2Sp2.getPixelNumber(), BLACK);
            if (sw.getSwitchPosition() == 1) {
                showColor(panelLed.getLedGroup(), panelLed.getPixelNumber(), YELLOW);
            } else {
                showColor(panelLed.getLedGroup(), panelLed.getPixelNumber(), PURPLE);
            }
        } else if (soc % 2 == 0) { // even
            showColor(led1Sp1.getLedGroup(), led1Sp1.getPixelNumber(), GREEN_H);
            showColor(led2Sp1.getLedGroup(), led2Sp1.getPixelNumber(), RED_H);
            showColor(led1Sp2.getLedGroup(), led1Sp2.getPixelNumber(), GREEN_H);
            showColor(led2Sp2.getLedGroup(), led2Sp2.getPixelNumber(), RED_H);
            showColor(panelLed.getLedGroup(), panelLed.getPixelNumber(), RED);
        } else { // odd
            showColor(led1Sp1.getLedGroup(), led1Sp1.getPixelNumber(), RED_H);
            showColor(led2Sp1.getLedGroup(), led2Sp1.getPixelNumber(), GREEN_H);
            showColor(led1Sp2.getLedGroup(), led1Sp2.getPixelNumber(), RED_H);
            showColor(led2Sp2.getLedGroup(), led2Sp2.getPixelNumber(), GREEN_H);
            showColor(panelLed.getLedGroup(), panelLed.getPixelNumber(), GREEN);
        }
        lcs.setSwitchOperationCounter(soc);
    } else if (hc > 0) { // Hold Signal
        hc--;
        if (hc == 0) {
            showColor(led2Sp1.getLedGroup(), led2Sp1.getPixelNumber(), BLACK);
            showColor(led2Sp2.getLedGroup(), led2Sp2.getPixelNumber(), BLACK);
            if (sw.getSwitchPosition() == 1) {
                showColor(panelLed.getLedGroup(), panelLed.getPixelNumber(), YELLOW);
            } else {
                showColor(panelLed.getLedGroup(), panelLed.getPixelNumber(), PURPLE);
            }
        } else {
            showColor(led2Sp1.getLedGroup(), led2Sp1.getPixelNumber(), RED_H);
            showColor(led2Sp2.getLedGroup(), led2Sp2.getPixelNumber(), RED_H);
            showColor(panelLed.getLedGroup(), panelLed.getPixelNumber(), RED);
        }
        lcs.setHoldCounter(hc);
    } else if (dsc > 0) { // Drive Slow Signal
        dsc--;
        if (dsc == 0) {
            showColor(led2Sp1.getLedGroup(), led2Sp1.getPixelNumber(), BLACK);
            showColor(led2Sp2.getLedGroup(), led2Sp2.getPixelNumber(), BLACK);
            if (sw.getSwitchPosition() == 1) {
                showColor(panelLed.getLedGroup(), panelLed.getPixelNumber(), YELLOW);
            } else {
                showColor(panelLed.getLedGroup(), panelLed.getPixelNumber(), PURPLE);
            }
        } else if (dsc % 2 == 0) {
            showColor(led2Sp1.getLedGroup(), led2Sp1.getPixelNumber(), BLACK);
            showColor(led2Sp2.getLedGroup(), led2Sp2.getPixelNumber(), BLACK);
            showColor(panelLed.getLedGroup(), panelLed.getPixelNumber(), BLACK);
        } else {
            showColor(led2Sp1.getLedGroup(), led2Sp1.getPixelNumber(), YELLOW);
            showColor(led2Sp2.getLedGroup(), led2Sp2.getPixelNumber(), YELLOW);
            showColor(panelLed.getLedGroup(), panelLed.getPixelNumber(), YELLOW);
        }
        lcs.setDriveSlowCounter(dsc);
    } else if (dwc > 0) { // Drive Warning
        dwc--;
        if (dwc == 0) {
            showColor(led2Sp1.getLedGroup(), led2Sp1.getPixelNumber(), BLACK);
            showColor(led2Sp2.getLedGroup(), led2Sp2.getPixelNumber(), BLACK);
            if (sw.getSwitchPosition() == 1) {
                showColor(panelLed.getLedGroup(), panelLed.getPixelNumber(), YELLOW);
            } else {
                showColor(panelLed.getLedGroup(), panelLed.getPixelNumber(), PURPLE);
            }
        } else {
            showColor(led2Sp1.getLedGroup(), led2Sp1.getPixelNumber(), GREEN_H);
            showColor(led2Sp2.getLedGroup(), led2Sp2.getPixelNumber(), GREEN_H);
            showColor(panelLed.getLedGroup(), panelLed.getPixelNumber(), GREEN);
        }
        lcs.setDriveWarningCounter(dwc);
    }
    return lcs;
}

int main() {
    uBit.init();

    // Reuse GPIO pins P3, P4, P6, P7, P9, P10 as Digital by
    // uncouple the pins from the LED matrix
    uBit.display.disable(); //disables the display

    uBit.serial.printf("Remove power from all switches\r\n");
    removePowerFromSwitches();

    // Setup USB serial and create an event source
    uBit.serial.setRxBufferSize(20);
    uBit.serial.baud(115200);
    uBit.messageBus.listen(MICROBIT_ID_SERIAL,  MICROBIT_SERIAL_EVT_HEAD_MATCH, onChar);
    uBit.serial.eventAfter(1, ASYNC);

    // The pulses we create in this software is used to trigger 8 one-shot monostable multivibrators (4 x IC CMOS 4098)
    // These multivibrators generate pulses with a fixed length of 500 ms and are configured to be triggered on the
    // leading edge of their inputs (connected to the microbit switch outputs)
    // This way we don't depend on the pulse length created by the microbit to prevent overheaded switch coils in case
    // the outputs stays high to long caused by a software failure or crash (it happened to me, two switches destroyed)
    // A pulse length of 6 ms is enough for a trigger (note sleep uses the system timer and works in steps of 6 ms)
    // In case we feed the pulses directly to the ULN2803A IC (according to the hardware circuit in this repo) then a
    // pulse length of 500 ms is necessary
    pulseLength = 6;

    // Create the Neopixels, Signal Poles and Switches

    // Neopixel LEDs for switch 1
    NeoPixelLed led1Sw1Sp1 (2, SIGNAL_POLE_LEDS_2, BLACK);
    NeoPixelLed led2Sw1Sp1 (3, SIGNAL_POLE_LEDS_2, BLACK);
    NeoPixelLed led1Sw1Sp2 (0, SIGNAL_POLE_LEDS_2, BLACK);
    NeoPixelLed led2Sw1Sp2 (1, SIGNAL_POLE_LEDS_2, BLACK);
    NeoPixelLed panelLedSw1 (0, PANEL_LEDS, BLACK);

    // Neopixel LEDs for switch 2
    NeoPixelLed led1Sw2Sp1 (6, SIGNAL_POLE_LEDS_2, BLACK);
    NeoPixelLed led2Sw2Sp1 (7, SIGNAL_POLE_LEDS_2, BLACK);
    NeoPixelLed led1Sw2Sp2 (4, SIGNAL_POLE_LEDS_2, BLACK);
    NeoPixelLed led2Sw2Sp2 (5, SIGNAL_POLE_LEDS_2, BLACK);
    NeoPixelLed panelLedSw2 (1, PANEL_LEDS, BLACK);

    // Neopixel LEDs for switch 3
    NeoPixelLed led1Sw3Sp1 (0, SIGNAL_POLE_LEDS_1, BLACK);
    NeoPixelLed led2Sw3Sp1 (1, SIGNAL_POLE_LEDS_1, BLACK);
    NeoPixelLed led1Sw3Sp2 (2, SIGNAL_POLE_LEDS_1, BLACK);
    NeoPixelLed led2Sw3Sp2 (3, SIGNAL_POLE_LEDS_1, BLACK);
    NeoPixelLed panelLedSw3 (2, PANEL_LEDS, BLACK);

    // Neopixel LEDs for switch 4
    NeoPixelLed led1Sw4Sp1 (6, SIGNAL_POLE_LEDS_1, BLACK);
    NeoPixelLed led2Sw4Sp1 (7, SIGNAL_POLE_LEDS_1, BLACK);
    NeoPixelLed led1Sw4Sp2 (4, SIGNAL_POLE_LEDS_1, BLACK);
    NeoPixelLed led2Sw4Sp2 (5, SIGNAL_POLE_LEDS_1, BLACK);
    NeoPixelLed panelLedSw4 (3, PANEL_LEDS, BLACK);

    // Signal Poles for switch 1
    SignalPole signalPole1Sw1 (led1Sw1Sp1, led2Sw1Sp1);
    SignalPole signalPole2Sw1 (led1Sw1Sp2, led2Sw1Sp2);
    // Signal Poles for switch 2
    SignalPole signalPole1Sw2 (led1Sw2Sp1, led2Sw2Sp1);
    SignalPole signalPole2Sw2 (led1Sw2Sp2, led2Sw2Sp2);
    // Signal Poles for switch 3
    SignalPole signalPole1Sw3 (led1Sw3Sp1, led2Sw3Sp1);
    SignalPole signalPole2Sw3 (led1Sw3Sp2, led2Sw3Sp2);
    // Signal Poles for switch 4
    SignalPole signalPole1Sw4 (led1Sw4Sp1, led2Sw4Sp1);
    SignalPole signalPole2Sw4 (led1Sw4Sp2, led2Sw4Sp2);

    // The start positions depends on our track layout and our needs
    // Switch 1
    Switch switch1 (1, 1, signalPole1Sw1, signalPole2Sw1, panelLedSw1, true);
    // Switch 2
    Switch switch2 (2, 1, signalPole1Sw2, signalPole2Sw2, panelLedSw2, true);
    // Switch 3
    Switch switch3 (3, 2, signalPole1Sw3, signalPole2Sw3, panelLedSw3, true);
    // Switch 4
    Switch switch4 (4, 2, signalPole1Sw4, signalPole2Sw4, panelLedSw4, true);

    LedCounterSwitch ledCounterSwitch1 (0, 0, 0, 0);
    LedCounterSwitch ledCounterSwitch2 (0, 0, 0, 0);
    LedCounterSwitch ledCounterSwitch3 (0, 0, 0, 0);
    LedCounterSwitch ledCounterSwitch4 (0, 0, 0, 0);

    panelLEDs.clear();
    signalPoleLEDs1.clear();
    signalPoleLEDs2.clear();
    powerLEDOn(true);

    randomSwitchMode = false;
    randomLedMode = true;
    showColor(PANEL_LEDS, 5, Color(uBit.random(80), uBit.random(80), uBit.random(80)));

    while (1) {
        if ((randomSwitchMode) && (uBit.random(RANDOM_SWITCH) == 0)) {
            buttonPressed = uBit.random(4)+1; 
            uBit.serial.printf("Random mode will flip switch %i\r\n", buttonPressed); 
        } else {
            buttonPressed = readButtonsPressed();
        }        

        // Set operation tasks on button pressed
        switch (buttonPressed) {
            case BUTTON_1_FLIP_SWITCH_1:
                uBit.serial.printf("Flip switch 1\r\n");
                if (switch1.getSwitchPosition() == 1) {
                    switch1.setSwitchPosition(2);
                } else {
                    switch1.setSwitchPosition(1);
                };
                ledCounterSwitch1.setSwitchOperationCounter(LED_SWITCH_COUNTER);
                break;
            case BUTTON_2_FLIP_SWITCH_2:
                uBit.serial.printf("Flip switch 2\r\n");
                if (switch2.getSwitchPosition() == 1) {
                    switch2.setSwitchPosition(2);
                } else {
                    switch2.setSwitchPosition(1);
                };
                ledCounterSwitch2.setSwitchOperationCounter(LED_SWITCH_COUNTER);
                break;
            case BUTTON_3_FLIP_SWITCH_3:
                uBit.serial.printf("Flip switch 3\r\n");
                if (switch3.getSwitchPosition() == 1) {
                    switch3.setSwitchPosition(2);
                } else {
                    switch3.setSwitchPosition(1);
                };
                ledCounterSwitch3.setSwitchOperationCounter(LED_SWITCH_COUNTER);
                break;
            case BUTTON_4_FLIP_SWITCH_4:
                uBit.serial.printf("Flip switch 4\r\n");
                if (switch4.getSwitchPosition() == 1) {
                    switch4.setSwitchPosition(2);
                } else {
                    switch4.setSwitchPosition(1);
                };
                ledCounterSwitch4.setSwitchOperationCounter(LED_SWITCH_COUNTER);
                break;
            case BUTTON_5_PRESSED:
                if (randomSwitchMode) {
                    randomSwitchMode = false;
                    showColor(PANEL_LEDS, 6, BLACK);
                    uBit.serial.printf("Random switch mode OFF\r\n");
                } else {
                    randomSwitchMode = true;
                    showColor(PANEL_LEDS, 6, Color(uBit.random(80), uBit.random(80), uBit.random(80)));
                    uBit.serial.printf("Random switch mode ON\r\n");
                }
                break;
            case BUTTON_6_PRESSED:
                if (randomLedMode) {
                    randomLedMode = false;
                    showColor(PANEL_LEDS, 5, BLACK);
                    uBit.serial.printf("Random LED mode OFF\r\n");
                } else {
                    randomLedMode = true;
                    showColor(PANEL_LEDS, 5, Color(uBit.random(80), uBit.random(80), uBit.random(80)));
                    uBit.serial.printf("Random LED mode ON\r\n");
                }
                break;
            case BUTTON_7_RESET:
                uBit.serial.printf("Reset to begin state\r\n");
                for (uint8_t i = 0; i++ <= 5;) {
                    showColor(PANEL_LEDS, 4, GREEN);
                    uBit.sleep(200);
                    showColor(PANEL_LEDS, 4, BLACK);
                    uBit.sleep(200);
                }
                switch1.setSwitchPosition(1);
                switch2.setSwitchPosition(1);
                switch3.setSwitchPosition(1);
                switch4.setSwitchPosition(1);
                break;
            default:
                break;
        }

        // Randomly put signal poles on Hold, Drive Slow or Drive Warning signal
        if ((randomLedMode) && (uBit.random(RANDOM_SIGNAL) == 0)) {
            randomSwitch = uBit.random(4)+1;
            randomLedOperation = uBit.random(4);
            switch (randomSwitch) {
                case SWITCH_1:
                    switch (randomLedOperation) {
                        case 0:
                            ledCounterSwitch1.setHoldCounter(LED_SIGNAL_COUNTER);
                            break;
                        case 1:
                            ledCounterSwitch1.setDriveSlowCounter(LED_SIGNAL_COUNTER);
                            break;
                        case 2:
                            ledCounterSwitch1.setDriveWarningCounter(LED_SIGNAL_COUNTER);
                            break;
                    }
                    break;
                case SWITCH_2:
                    switch (randomLedOperation) {
                        case 0:
                            ledCounterSwitch2.setHoldCounter(LED_SIGNAL_COUNTER);
                            break;
                        case 1:
                            ledCounterSwitch2.setDriveSlowCounter(LED_SIGNAL_COUNTER);
                            break;
                        case 2:
                            ledCounterSwitch2.setDriveWarningCounter(LED_SIGNAL_COUNTER);
                            break;
                    }
                    break;
                case SWITCH_3:
                    switch (randomLedOperation) {
                        case 0:
                            ledCounterSwitch3.setHoldCounter(LED_SIGNAL_COUNTER);
                            break;
                        case 1:
                            ledCounterSwitch3.setDriveSlowCounter(LED_SIGNAL_COUNTER);
                            break;
                        case 2:
                            ledCounterSwitch3.setDriveWarningCounter(LED_SIGNAL_COUNTER);
                            break;
                    }
                    break;
                case SWITCH_4:
                    switch (randomLedOperation) {
                        case 0:
                            ledCounterSwitch4.setHoldCounter(LED_SIGNAL_COUNTER);
                            break;
                        case 1:
                            ledCounterSwitch4.setDriveSlowCounter(LED_SIGNAL_COUNTER);
                            break;
                        case 2:
                            ledCounterSwitch4.setDriveWarningCounter(LED_SIGNAL_COUNTER);
                            break;
                    }
                    break;
            }
        }

        // Operate the LEDs
        ledCounterSwitch1 = OperateLeds(switch1, ledCounterSwitch1);
        ledCounterSwitch2 = OperateLeds(switch2, ledCounterSwitch2);
        ledCounterSwitch3 = OperateLeds(switch3, ledCounterSwitch3);
        ledCounterSwitch4 = OperateLeds(switch4, ledCounterSwitch4);

        // Operate the switches
        if (switch1.getOperate()) {
            uBit.serial.printf("Flip switch %i to position %i\r\n", switch1.getSwitchNumber(), switch1.getSwitchPosition());
            if (switch1.getSwitchPosition() == 1) {
                showColor(PANEL_LEDS, 0, YELLOW);
                uBit.io.P7.setDigitalValue(1);
                uBit.sleep(pulseLength);
                uBit.io.P7.setDigitalValue(0);
            } else {
                showColor(PANEL_LEDS, 0, PURPLE);
                uBit.io.P9.setDigitalValue(1);
                uBit.sleep(pulseLength);
                uBit.io.P9.setDigitalValue(0);
            }
            switch1.setOperate(false);
        } else if (switch2.getOperate()) {
            uBit.serial.printf("Flip switch %i to position %i\r\n", switch2.getSwitchNumber(), switch2.getSwitchPosition());
            if (switch2.getSwitchPosition() == 1) {
                showColor(PANEL_LEDS, 1, YELLOW);
                uBit.io.P6.setDigitalValue(1);
                uBit.sleep(pulseLength);
                uBit.io.P6.setDigitalValue(0);
            } else {
                showColor(PANEL_LEDS, 1, PURPLE);
                uBit.io.P4.setDigitalValue(1);
                uBit.sleep(pulseLength);
                uBit.io.P4.setDigitalValue(0);
            }
            switch2.setOperate(false);
        } else if (switch3.getOperate()) {
            uBit.serial.printf("Flip switch %i to position %i\r\n", switch3.getSwitchNumber(), switch3.getSwitchPosition());
            if (switch3.getSwitchPosition() == 1) {
                showColor(PANEL_LEDS, 2, YELLOW);
                uBit.io.P3.setDigitalValue(1);
                uBit.sleep(pulseLength);
                uBit.io.P3.setDigitalValue(0);
            } else {
                showColor(PANEL_LEDS, 2, PURPLE);
                uBit.io.P2.setDigitalValue(1);
                uBit.sleep(pulseLength);
                uBit.io.P2.setDigitalValue(0);
            }
            switch3.setOperate(false);
        } else if (switch4.getOperate()) {
            uBit.serial.printf("Flip switch %i to position %i\r\n", switch4.getSwitchNumber(), switch4.getSwitchPosition());
            if (switch4.getSwitchPosition() == 1) {
                showColor(PANEL_LEDS, 3, YELLOW);
                uBit.io.P1.setDigitalValue(1);
                uBit.sleep(pulseLength);
                uBit.io.P1.setDigitalValue(0);
            } else {
                showColor(PANEL_LEDS, 3, PURPLE);
                uBit.io.P0.setDigitalValue(1);
                uBit.sleep(pulseLength);
                uBit.io.P0.setDigitalValue(0);
            }
            switch4.setOperate(false);
        }

        if (buttonPressed == BUTTON_7_RESET) {
            showColor(PANEL_LEDS, 4, BLACK);
            randomSwitchMode = false;
            showColor(PANEL_LEDS, 6, BLACK);
            randomLedMode = false;
            showColor(PANEL_LEDS, 5, BLACK);
        }
        //sendSwitchPositionsToSerial(switch1.getSwitchPosition(), switch2.getSwitchPosition(), switch3.getSwitchPosition(), switch4.getSwitchPosition());
        uBit.sleep(LOOP_DELAY);
    }

    panelLEDs.clear();
    signalPoleLEDs1.clear();
    signalPoleLEDs2.clear();
    release_fiber();
    return 0;
}
