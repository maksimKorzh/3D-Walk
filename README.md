# 3D-Walk
Simple ray-casting pseudo 3D engine (linux port of CommandLineFPS by javidx9 aka OneLoneCoder)

![3D-Walk](https://github.com/maksimKorzh/3D-Walk/blob/master/3D-Walk.gif)

# Dependencies

    3D-Walk is based on ncurses library, execute following command to install it:
    sudo apt-get install libncurses5-dev libncursesw5-dev

# Compile

    gcc 3D-Walk.c $(ncursesw5-config --cflags) -o 3D-Walk -lncursesw -lm
