# (TODO: your game's title)

Author: Yile Xu

Design: spaceman with jetpack

Screen Shot:

![Screen Shot](screenshot.png)

How Your Asset Pipeline Works:

1. use load_png() to load png into rgba format
2. implement palette table base on the rgba values
3. use palette table to make the rgba values represented by 0123
4. seperate the 0123 format to two 01 formats
5. according to the png size, seperate the 01 values to 8*8 and convert them to bit0s and bit1s
6. draw them on screen!

How To Play: Press arrow keys to go up, left, and right

Tool I Used: https://www.pixilart.com/draw

This game was built with [NEST](NEST.md).

