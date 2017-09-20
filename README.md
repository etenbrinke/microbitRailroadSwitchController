# microbit Railroad Switch Controller written in C++

## The hardware

The purpose of this project is to control four analog H0 railroad switches by a BBC microbit. These old switches has tree wires, one is connected to 12-20 VAC or VDC and the other two can be used to flip over the switch by connecting one of them to the ground. We operate them on 18 VDC.
This project uses Zettler AZ850-12 relays to control the power on the switches. These relays are very small and still can drive up to 1A and 30V. As VDC is uses instead of VAC a simple RC snubber circuit is placed accross the relais switches to prevent voltage spikes.
The relays are controller via an ULN2803A Darlington Transistor Arrays IC directly by the microbit via digital outputs on pin0+pin1 for switch 1, pin2+pin3 for switch 2, pin4+pin6 for switch 3 and pin7+pin9 for switch 4. The position of the switches can be changed by setting a high for a short period (< 1 sec) on the outputs. (see note 1)
The switches can be controller by buttons. Each switch has it's own button to flip over the switch to the other position. The program keeps track of the current positions of each switch. By pressing the random button switches are flipped over randomly ones a while. 
A 10-Line to 4-Line encoder (CD40147 IC) is used as a 8-Line to 3-Line encoder to feed the buttons states to the microbit. Zeven buttons are used and connected to the input of the encoder and the 3 line BCD output is read by the microbit digital input on pin5, pin10 and pin11. This way only three microbit input ports are neccessary to read the button states. To make the operation of the switches more visual the microbit also controls NeoPixel LEDs on for both the control panel and the dwarf signal poles in the scenery next to the switches. Each signal pole has two NeoPixel LEDs and each switch has two signal poles located next to the railroad switch and pointing in opposite direction.

The following buttons with LEDs are used on the control panel:

* button 1 = flip switch 1 : NeoPixel LED 0 on pin16
* button 2 = flip switch 2 : NeoPixel LED 1 on pin16
* button 3 = flip switch 3 : NeoPixel LED 2 on pin16
* button 4 = flip switch 4 : NeoPixel LED 3 on pin16
* button 5 = random switch mode on/off : NeoPixel LED 6 on pin16
* button 6 = random signal mode on/off : NeoPixel LED 5 on pin16
* button 7 = reset switches to begin position : NeoPixel LED 4 on pin16
* Power LED : NeoPixel LED 7 on pin16

The following LEDs are used for the dwarf signal poles:

* Signal pole 1, NeoPixel LED 2 and 3 on pin12
* Signal pole 2, NeoPixel LED 0 and 1 on pin12
* Signal pole 3, NeoPixel LED 6 and 7 on pin12
* Signal pole 4, NeoPixel LED 4 and 5 on pin12
* Signal pole 5, NeoPixel LED 0 and 1 on pin8
* Signal pole 6, NeoPixel LED 2 and 3 on pin8
* Signal pole 7, NeoPixel LED 6 and 7 on pin8
* Signal pole 8, NeoPixel LED 4 and 5 on pin8

If you count you will notice that every microbit pin with digital capability is used.

![Signal Pole with two NeoPixel LEDs](https://github.com/etenbrinke/microbitRailroadSwitchController/tree/master/images/signalPole.jpg)

![Controlling the switches](https://github.com/etenbrinke/microbitRailroadSwitchController/tree/master/images/microbitRailroadSwitchController1.jpg)

![Input buttons](https://github.com/etenbrinke/microbitRailroadSwitchController/tree/master/images/microbitRailroadSwitchController2.jpg)

![NeoPixel LEDs](https://github.com/etenbrinke/microbitRailroadSwitchController/tree/master/images/microbitRailroadSwitchController3.jpg)

Note 1:
A solenoid coil can only resist a current for a short period of time. If put on for a long time the coils will burn out. If the microbit outputs keeps high by a software error or memory issue then this could destroy your switches!
For that reason a monostable multivibrator circuit has been added between the microbit switch outputs and the ULN2803A to convert a leading edge on a microbit output for switching to an one shot pulse of 500 ms. That way we are sure not burning any switches by software mistakes.

Note 2:
Depending on your switches it could be possible to drive the switches directly by the ULN2803A and skip the whole relais setup.

## NeoPixel LEDs

In this project no ready to use NeoPixel strips are used. Instead every LED is a NeoPixel Diffused 5mm through-hole RGB LED.
Each LED looks just like a shorter version of the classic through-hole 5mm RGB LED with 4 legs, but instead of just red/green/blue there's a little chip inside that can control the LED with high speed PWM for 24-bit color.
By our software we can assign every color and brightness we like to. How cool is that.

There are some best practices to read before you use NeoPixel LEDs, some important ones are:
* Calculate the neccesary power consumption for all your NeoPixel LEDs, make sure you use a stable 5V power supply which is powerfull enough to drive them.
* Don't power the LEDs by the microbit 3V outputs. You need an appropiate external power supply for the LEDs.
* Add a capacitor (1000 µF, 6.3V or higher) across the + and – terminals of your power supply. The capacitor buffers sudden changes in the current drawn by the strip.
* Add a 100 µF capacitor across the + and – pins of every Neopixel LED. If you don't do this some LEDs will start living their own lives and showing colors you didn't ask for
* NeoPixels powered by 5V require a 5V data signal. As we using a 3.3V microcontroller we must use a logic level shifter such as a 74AHCT125 or 74HCT245 to step up the signal from the microcontroller to the first pixel.

I noticed that it's a bit of a hassle to solder the 4 legs of the LEDs and add capacitor across the + and - pins and still manage to position them in the LED support. You need good isolation between all the legs as they will sit close together.
Your are better of using strips if you don't like this hassle but then you don't have the flexability to place them exactly where you want them on your controller.

## The software

There are two version of this software. A python version which you find here: https://github.com/etenbrinke/microbitRailroadSwitchController-python
and a C++ version in this repo.
When working on the python version first I reached both the limit of the program size (it has to fit in the .hex files that is getting generated) and the size of memory when the program uses while running. That's why in the python program only libraries are imported which are neccesary (to reduce memory while the program runs) and all unnecessary empty lines are removed (the Mu code editor Check is not happy) and the use of python functions is limited.  
This is when I started converting the program to C++ to allow more functionality with less memory foodprint. 

### Compile project
```
yt clean
yt init
yt target bbc-microbit-classic-gcc
yt install lancaster-university/microbit
yt build
```

The HEX file for you micro:bit will be generated and stored in build/bbc-microbit-classic-gcc/source/microbitRailroadSwitchController-combined.hex

### Neopixel library

https://github.com/mathisgerdes/bitbot.git
