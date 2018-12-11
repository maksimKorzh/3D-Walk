# 3D-Walk
Simple ray-casting pseudo 3D engine (linux port of CommandLineFPS by javidx9 aka OneLoneCoder)

![3D-Walk](https://github.com/maksimKorzh/3D-Walk/blob/master/3D-Walk.gif)

# Dependencies

  3D-Walk is based on ncurses library, execute following command to install it:
    
    sudo apt-get install libncurses5-dev libncursesw5-dev

# Compile

    gcc 3D-Walk.c $(ncursesw5-config --cflags) -o 3D-Walk -lncursesw -lm

# Credits

[OneLoneCoder](https://www.youtube.com/channel/UC-yuWVUplUJZvieEligKBkA)            -    YouTube channel

[Code-It-Yourself! First Person Shooter (Quick and Simple C++)](https://www.youtube.com/watch?v=xW8skO7MFYw)                       -    video tutorial by OneLoneCoder

[CommandLineFPS](https://github.com/OneLoneCoder/CommandLineFPS)    -    original MS Windows version
