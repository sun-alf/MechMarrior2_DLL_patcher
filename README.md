# MechMarrior2_DLL_patcher
Explore and patch your MechWarrior2 DLLs ("mercsw.dll" and "MW2SHELL.DLL") to change weapons and ammunitions at your will. Linux or Windows complilation.

Quick start steps:
1. Download the project files as usually.
2. Compile your command-line application, the following command is enough for this tiny program:
   >> gcc ./main.c ./ClearScreen.c -lncurses -o myapp.elf
3. Run your compiled program, i.e. myapp.elf:
   >> ./myapp.elf
4. Follow given menu items, just put a number and hit Enter.

Now let's see what the Main menu provides:
1. Export weapons from DLLs        // Export all the stuff into a human-readable file, read it and edit at will, 
2. Import weapons from text file   // and then use this option to load your tweaked stuff back into memeory.
3. Patch DLLs                      // Now use this one to patch both DLLs.
0. Exit program

The program also shows basic constants it works with. It is self-explanatory if you can read the source code.
DLL info:
  Total amount of weapons = 66
  DLL name = mercsw.dll  (start offset = 0x00098638)
  Shell DLL name = MW2SHELL.DLL  (start offset = 0x0005CC0C)

If you are awared your DLLs contain different amount of weapons or all the params start from a different offset, you have to
change it in "main.c" accordingly. The default ones are for "MechWarrior 2: 31st Century Combat (Titanium edition)". I did
not explore any other version of MW2, so sorry. Please feel free to make a pull request if you made an addition or so.
