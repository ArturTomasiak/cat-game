# Cat Game

A simple game centered around color detection, where the score is accumulated by hitting a cat with green objects. Press `q` or `Q` to exit.

For Windows 10/11 the release `.exe` works out of the box.

<img src="https://github.com/user-attachments/assets/136a5528-c019-4471-8246-b351943d246a" style="max-width:200px; height:auto;"/>

> gif of the initial build of cat game

<br>

> [!NOTE]
> Both the OpenCV build and the release were made for Windows. Linux instructions are provided at the bottom.

---

## Compiling on Windows

The OpenCV build was made for running via MinGW `g++`. Specifically, I compile the program using:

```bash
g++ -o cat_game main.cpp -I"include" -L"include/mingw/lib" -lopencv_core4100 -lopencv_imgproc4100 -lopencv_imgcodecs4100 -lopencv_highgui4100 -lopencv_videoio4100 -mwindows
```

To run OpenCV within Visual Studio, the Windows release available at [opencv.org](https://opencv.org/) is recommended.

The OpenCV build was made via:

```bash
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DCMAKE_INSTALL_PREFIX=insert_destination_path -S insert_source_path -B insert_build_path
```

Within the build folder:

```bash
mingw32-make
```

```bash
mingw32-make install
```

However, all necessary files to compile and run are included in the source code.

## Linux (Debian-based)

My OpenCV build is likely not suitable for Linux; thus, only the `main.cpp` and `089.jpg` files are necessary.

First, install OpenCV and `g++` using:

```bash
sudo apt install libopencv-dev build-essential
```

Then, compile using:

```bash
g++ main.cpp -o program `pkg-config --cflags --libs opencv4`
```
