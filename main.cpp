#include <iostream>
#include <cmath>
#include <unistd.h>
#include <ncurses.h>

int nScreenWidth = 120;
int nScreenHeight = 40;

float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerA = 0.0f;

int nMapHeight = 16;
int nMapWidth = 16;

float fFOV = 3.14159 / 4.0;

float fDepth = 16;

int main()
{
    initscr();             // Initialize ncurses
    cbreak();              // Disable line buffering
    noecho();              // Don't echo keypresses
    keypad(stdscr, TRUE);  // Enable keypad input
    nodelay(stdscr, TRUE); // Make getch non-blocking

    // Create screen buffer
    char *screen = new char[nScreenWidth * nScreenHeight];

    std::fill_n(screen, nScreenHeight * nScreenWidth, ' ');

    std::string map;

    map += "################";
    map += "#..............#";
    map += "#..............#";
    map += "#..............#";
    map += "#..............#";
    map += "#..............#";
    map += "#..............#";
    map += "#..............#";
    map += "#..............#";
    map += "#..............#";
    map += "#..............#";
    map += "#..............#";
    map += "#..............#";
    map += "#..............#";
    map += "#..............#";
    map += "################";

    // Game loop
    while (1)
    {
        int ch = getch();
        if (ch != ERR)
        {
            switch (ch)
            {
            case 'w': // Move forward
                // Update player position
                screen[0] = 'w';
                break;
            case 's': // Move backward
                screen[0] = 's';
                // Update player position
                break;
            case 'a': // Turn left
                screen[0] = 'a';
                break;
            case 'd': // Turn right
                screen[0] = 'd';
                break;
            case 'q': // Quit
                endwin();
                return 0;
            }
        }

        // Wall collision
        for (int x = 0; x < nScreenWidth; x++)
        {
            float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV;
            float fDistanceToWall = 0;
            bool bHitWall = false;

            float fEyeX = std::sin(fRayAngle);
            float fEyeY = std::cos(fRayAngle);

            while (!bHitWall && fDistanceToWall < fDepth)
            {
                fDistanceToWall += 0.1f;
                int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
                int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

                // Check out of bounds
                if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
                {
                    bHitWall = true;
                    fDistanceToWall = fDepth;
                }
                else
                {

                    // Ray is inbounds so test to see if the ray cell is a wall block
                    if (map[nTestY * nMapWidth + nTestX] == '#')
                    {
                        bHitWall = true;
                    }
                }
            }

            // Calculate distance to ceiling and floor
            int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
            int nFloor = nScreenHeight - nCeiling;

            for (int y = 0; y < nScreenHeight; y++)
            {
                if (y < nCeiling)
                {
                    screen[y * nScreenWidth + x] = ' ';
                }
                else if (y > nCeiling && y <= nFloor)
                {
                    screen[y * nScreenWidth + x] = '#';
                }
                else
                {
                    screen[y * nScreenWidth + x] = ' ';
                }
            }
        }
        for (int x = 0; x < nScreenWidth; x++)
        {
            screen[20 * nScreenWidth + x] = '#';
        }

        clear();
        // Print the screen buffer
        for (int y = 0; y < nScreenHeight; y++)
        {
            for (int x = 0; x < nScreenWidth; x++)
            {
                addch(screen[y * nScreenWidth + x]);
                // addch(' ');
            }
            // addch('\n');
        }
        refresh();

        // Add a small delay to control frame rate
        usleep(100000); // 100ms delay (adjust as needed)
    }

    delete[] screen;
    endwin();
    return 0;
}