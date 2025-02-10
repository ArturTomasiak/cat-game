# cat game

A simple game centered around color detection, where score is accumulated by hitting a cat with green objects. q/Q to exit.

No download instructions are needed for windows 10/11, as the release exe works out of the box.

> [!NOTE]
> Both the cv2 build as well as the release where made for windows. Linux instructions at the bottom.

---

## Compiling on Windows

  The openCV build was made for running via mingw g++, more speciffically, I compile the program using:
  ```g++ -o cat_game main.cpp -I"include" -L"include/mingw/lib" -lopencv_core4100 -lopencv_imgproc4100 -lopencv_imgcodecs4100 -lopencv_highgui4100 -lopencv_videoio4100 -mwindows```

  to run CV2 within visual studio, the windows release available at [opencv.org](https://opencv.org/) is recommended.

  The openCV build was made via
  ```cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_INSTALL_PREFIX=insert_destination_path -S insert_source_path -B insert_build_path```
  within the build folder
  ```mingw32-make```
  ```mingw32-make install```
  Hovewer, all necessary files to compile and run are in the source code. 

## Linux (debian based)

My cv2 build is probably useless for linux, thus only the main.cpp and 089.jpg files are necessary.
First install opencv and g++ using
```sudo apt install libopencv-dev build-essential```
Then, compile using
```g++ main.cpp -o program `pkg-config --cflags --libs opencv4` ```
