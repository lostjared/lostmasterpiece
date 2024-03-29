

![ScreenShot](https://github.com/lostjared/lostmasterpiece/blob/master/lostmasterpiece.png?raw=true "screenshot")


This project presents an enhanced iteration of MasterPiece, incorporating visual elements to enrich the gameplay experience.

Gameplay Instructions: To play, align three blocks of the same color either horizontally, vertically, or diagonally to eliminate them from the grid. Utilize the arrow keys for block movement, and press the 'A' and 'S' keys to interchange the colors of the blocks. Aim to clear as many blocks as possible before the grid reaches its capacity.

Originally developed years ago, this project reflects my early coding endeavors. While the code quality may not be exemplary, the game is functional and served as a valuable learning project. Previously, it was available on PlanetSourceCode.

Included in this project are three makefiles tailored for different environments:

* For *nix systems
* For Windows 32-bit
* For PlayStation Portable (PSP)
* Compilation examples:

For *nix: 
  
    make -f Makefile
    
For Windows: 

    make -f Makefile.win

For PSP: 

    make -f Makefile.psp
    
Please ensure to clean the directory prior to building for a different platform.
Dependencies: This project requires SDL 1.2.
