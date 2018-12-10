/*********************************************************************************\
;---------------------------------------------------------------------------------;
;                         SOURCE AND IDEA IS FULLY BASED ON                       ;
;---------------------------------------------------------------------------------;
;                             CommandLineFPS by javidx9                           ;
;---------------------------------------------------------------------------------;
;                                 aka OneLoneCoder                                ;
;---------------------------------------------------------------------------------;  
\*********************************************************************************/

/*********************************************************************************\
;                                      3D Walk                                    ;
;---------------------------------------------------------------------------------;
;                        Simple ray-casting pseudo 3D engine                      ;
;---------------------------------------------------------------------------------;
;                            linux port by Maksim Korzh                           ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

/*********************************************************************************\
;---------------------------------------------------------------------------------;
;          Copyright © 2018 Maksim Korzh <freesoft.for.people@gmail.com>          ;
;---------------------------------------------------------------------------------;
;     This work is free. You can redistribute it and/or modify it under the       ;
;      terms of the Do What The Fuck You Want To Public License, Version 2,       ;
;       as published by Sam Hocevar. See the COPYING file for more details.       ;
;---------------------------------------------------------------------------------;
'       THIS PROGRAM IS FREE SOFTWARE. IT COMES WITHOUT ANY WARRANTY, TO          ;
;        THE EXTENT PERMITTED BY APPLICABLE LAW. YOU CAN REDISTRIBUTE IT          ;
;       AND/OR MODIFY IT UNDER THE TERMS OF THE DO WHAT THE FUCK YOU WANT         ;
;          TO PUBLIC LICENCE, VERSION 2, AS PUBLISHED BY SAM HOCEVAR.             ;
;                SEE http://www.wtfpl.net/ FOR MORE DETAILS.                      ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

/*********************************************************************************\
;---------------------------------------------------------------------------------;
;                   DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE                   ;
;                           Version 2, December 2004                              ;
;                                                                                 ;
;        Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>                         ;
;                                                                                 ;
;        Everyone is permitted to copy and distribute verbatim or modified        ;
;        copies of this license document, and changing it is allowed as long      ;
;        as the name is changed.                                                  ;
;                                                                                 ;
;                   DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE                   ;
;          TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION        ;
;                                                                                 ;
;         0. You just DO WHAT THE FUCK YOU WANT TO.                               ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

#include <stdlib.h>
#include <locale.h>
#include <ncurses.h>
#include <math.h>

int screenWidth;                // Console screen size X (columns)
int screenHeight;               // Console screen size Y (rows)
int mapWidth = 16;              // World dimensions
int mapHeight = 16;

float posX = 8.0f;              // Player start position
float posY = 8.0f;
float posA = 2.0f;              // Player start rotation
float fov = 3.14159f / 2.0f;    // Field of view
float depth = 16.0f;            // Maximum rendering distance
float speed = 5.0f;             // Walking speed

char map[] =                    // World map: '#' - wall, '.' - space
"################\
#..............#\
#..............#\
#...########...#\
#.......#..#...#\
#..........#...#\
#..............#\
#...########...#\
#.......#..#...#\
#.......####...#\
#..............#\
#....##..##....#\
#...#...#..#...#\
#....###...#...#\
#..............#\
################";

int main()
{
    setlocale(LC_ALL, "");      // Needed to output unicode characters
    
    // Initialize ncurses screen
    initscr();
    getmaxyx(stdscr, screenHeight, screenWidth);
    noecho();
    curs_set(0);
    
    // Initialize colors
    start_color();
    init_color(COLOR_GREEN, 0, 700, 200);
	init_pair(1, COLOR_WHITE, COLOR_BLACK);

	attron(COLOR_PAIR(1));
    
    int exit = 1;

    // Game loop
    while(exit)
    {    
        for(int col = 0; col < screenWidth; col++)
        {
            // For each column, calculate the projected ray angle into world space
            float rayAngle = (posA - fov / 2.0f) + ((float)col / (float)screenWidth) * fov;            
            float distance = 0.0f;
            
            int hitWall = 0;
            
            float eyeX = sinf(rayAngle);    // Unit vector for ray in player space
            float eyeY = cosf(rayAngle);
            
            while(!hitWall && distance < depth)     // Find distance to wall
            {
                distance += 0.5f; // resolution
                
                int test_col = (int)(posX + eyeX * distance);
                int test_row = (int)(posY + eyeY * distance);
                
                // Test if ray is out of bounds
                if(test_col < 0 || test_col >= mapWidth || test_row < 0 || test_row >= mapHeight)
                {
                    hitWall = 1;
                    distance = depth;
                }
                
                else
                {
                    if(map[test_col * mapWidth + test_row] == '#') hitWall = 1;
                }
            }    
            
            // Calculate distance to ceiling and floor
            int ceiling = (float)(screenHeight / 2.0f) - screenHeight / ((float)distance);
            int floor = screenHeight - ceiling;
            
            // Shader walls based on distance
            const wchar_t* shade;
            
            if(distance <= depth / 4.0) shade = L"\x2588"; // close
            else if(distance <= depth / 3.0) shade = L"\x2593"; // further;
            else if(distance <= depth / 2.0) shade = L"\x2592";
            else if(distance <= depth) shade = L"\x2591";
            
            else shade = L" ";
            
            for(int row = 0; row < screenHeight; row++)
            {
                if(row <= ceiling)
                {
                    mvaddch(row, col, '`');
                }
                
                else if(row > ceiling && row <= floor)
                {
                    mvaddwstr(row, col, shade);
                }
                
                else
                {
                    // Shade floor based on distance
                    float b = 1.0f - (((float)row - screenHeight / 2.0f) / ((float)screenHeight / 2.0f));
                    
					if (b < 0.25) shade = L"#";
					else if (b < 0.5) shade = L"x";
					else if (b < 0.75) shade = L".";
					else if (b < 0.9) shade = L"-";
					else shade = L" ";
                    
                    mvaddwstr(row, col, shade);
                }
            }
            
            refresh();
        }
        
        int key = getch();
        
        switch(key)
        {
            case 'w':   // Move forward and collisions
                posX += sinf(posA) * 0.5f;
                posY += cosf(posA) * 0.5f;
                
                if(map[(int)posX * mapWidth + (int)posY] == '#')
                {
                    posX -= sinf(posA) * 0.5f;
                    posY -= cosf(posA) * 0.5f;
                } 
                
                break;
            
            case 's':   // Move backward and collisions
                posX -= sinf(posA) * 0.5f;
                posY -= cosf(posA) * 0.5f;
                
                if(map[(int)posX * mapWidth + (int)posY] == '#')
                {
                    posX += sinf(posA) * 0.5f;
                    posY += cosf(posA) * 0.5f;
                }
                
                break;
            
            case 'a':   // Rotate left
                posA -= 0.1f;
                break;
            
            case 'd':   // Rotate right
                posA += 0.1f;
                break;
        
            default:    // Any key to exit
                exit = 0;
                break;
        }    
    }
    
    attroff(COLOR_PAIR(1));
    endwin();        
}

