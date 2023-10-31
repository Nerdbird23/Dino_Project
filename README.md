# Dino_Project
Coding the dino game for a project in C

To run the dino game:
Begin by following this tutorial https://www.matsson.com/prog/sdl2-mingw-w64-tutorial.php
Make sure to change environment variables, you do so by typing in your search bar "Edit the system environment variables"
For environment variables; add "mingw64/bin" after the rest of the file path. It will not work unless you do bin
After, the environment variables are changed, open Windows command prompt and type "gcc"

You should get the following:
C:\Users\14254>gcc
gcc: fatal error: no input files
compilation terminated.

After downloading boo.zip, extract it, and then type the following into the command prompt:
C:\Users\14254\Desktop\boo>gcc -std=c17 Dino.c -IC:\Users\14254\Desktop\boo\SDL2\include -LC:\Users\14254\Desktop\boo\SDL2\lib -Wall -lmingw32 -lSDL2main -lSDL2 -o main

Note: Make sure that you move SDL2.dll to your main file
