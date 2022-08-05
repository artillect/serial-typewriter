# serial-typewriter

## Introduction

Who doesn't love the sound of an old typewriter? I know I do! That's why I'm working on turning a Brother AX-25 typewriter into a Linux terminal.

This readme is still a work in progress, but I made [a YouTube video](https://www.youtube.com/watch?v=JvhT_Bru0AA) that explains how it works.

## Setup

CIRCUIT DIAGRAM/PICTURES HERE

The Arduino controls two multiplexers that select the rows and columns of the typewriter's keyboard matrix to type characters. The Arduino is connected via serial to the Raspberry Pi, which has been configured to output the terminal over its hardware UART port.
