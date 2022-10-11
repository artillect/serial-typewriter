# serial-typewriter

## Introduction

Who doesn't love the sound of an old typewriter? I know I do! That's why I'm working on turning a Brother AX-25 typewriter into a Linux terminal.

Here is the [video I made](https://youtu.be/Pd6pa3Jko3w) about the final version of it. I'm going to be updating

This readme is still a work in progress, but I made a [YouTube video](https://www.youtube.com/watch?v=JvhT_Bru0AA) that explains how it works.

If you need help troubleshooting, [create an issue](https://github.com/artillect/serial-typewriter/issues/new), or ask for help in [my Discord server](https://discord.gg/J7D7KqV3).

## Project Supporters
This project wouldn't be possible without the people who sent me components and helped me sort out the design! To everyone who helped me finish this project, thank you!

CSTRSK - Sent me a Raspberry Pi Zero W and Zero 2 W: [GitHub](https://github.com/CSTRSK), [Website](https://cstrsk.de/)

Whisker - Sent me a bag of Alps keyboard switches and a multi-core microcontroller: [@whixr](https://twitter.com/whixr), [Website](http://tymkrs.com/)

## Hardware

The KiCAD project files for the custom PCB I designed for this project are available [here](https://github.com/artillect/AX-25-mechanical-keyboard).

The typewriter's keyboard is wired up in an 8x11 matrix, and connects to the typewriter using an 8 pin cable and an 11 pin cable. When one of the row pins is connected to a column pin by a keypress, the typewriter detects that and prints that character. 

The typewriter's keyboard matrix is driven by two multiplexers controlled by an Arduino Uno. I used [these multiplexers from Amazon](https://www.amazon.com/Ximimark-Digital-Multiplexer-Breakout-CD74HC4067/dp/B07K7JF3HX/), but any digital or analog multiplexer will work for this as long as it has more than 11 channels. The Arduino can trick the typewriter into thinking that a key has been pressed by selecting a channel on each multiplexer and connecting those pins together.

To connect the Arduino Uno to my Raspberry Pi, I built a voltage divider circuit for the connection between the Arduino's TX pin and the Raspberry Pi's RX pin, to convert the Arduino's 5V output to a safe level for the Raspberry Pi. You can also use a level shifter for this, and I will probably switch to using one in the future since my current breadboard circuit is a bit noisy.

Here's the schematic:

![Schematic](https://raw.githubusercontent.com/artillect/AX-25-mechanical-keyboard/main/Schematic.png)

## Software


The Raspberry Pi's UART login must be enabled using `raspi-config` > Advanced Options > Serial. To prevent ANSI escape sequences from being sent to the Arduino, add `export TERM=dumb` to the end of your .bashrc. I am working on writing code to process escape sequences so this may not be necessary in the future.

Once all of this is set up, flash serial-typewriter.ino to your Arduino, and turn on your typewriter! Let me know if you have any questions about setting this up, I'll try my best to provide assistance, but no guarantees, as I'm not familiar with other brands of typewriters.
