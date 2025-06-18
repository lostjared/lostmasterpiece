![image](https://github.com/user-attachments/assets/507a0640-4aec-431b-ba1b-293b9430f26f)

![image](https://github.com/user-attachments/assets/ec04aec9-9061-422b-9561-47508e499f15)

![image](https://github.com/user-attachments/assets/88891462-2f05-44a3-993f-9b8b9b66d8e1)


This project presents an enhanced iteration of MasterPiece, incorporating visual elements to enrich the gameplay experience.

Gameplay Instructions: To play, align three blocks of the same color either horizontally, vertically, or diagonally to eliminate them from the grid. Utilize the arrow keys for block movement, and press the 'A' and 'S' keys to interchange the colors of the blocks. Aim to clear as many blocks as possible before the grid reaches its capacity.

Originally developed years ago, this project reflects my early coding endeavors. While the code quality may not be exemplary, the game is functional and served as a valuable learning project. Previously, it was available on PlanetSourceCode.

Included in this project are three makefiles tailored for different environments:

* For *nix systems
* For Windows
* 

To build
```bash
cd source
mkdir build && cd build
cmake ..
make -j$(nproc)
cp LostMasterPiece ../
cd ..
./LostMasterPiece
````

Dependencies: This project requires SDL2
