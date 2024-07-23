#include <iostream>
#include <cmath>
#include <unistd.h>

int nScreenWidth = 120;
int nScreenHeight = 40;

float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerA = 0.0f;

int nMapHeight = 16;
int nMapWidth = 16;

float fFOV = 3.14159 / 4;

float fDepth = 16;

int main()
{
    // Create screen buffer
    char *screen = new char[nScreenWidth * nScreenHeight];

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

                if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
                {
                    bHitWall = true;
                    fDistanceToWall = fDepth;
                }
                else
                {

                    if (map[nTestY * nMapWidth + nTestX] == '#')
                    {
                        bHitWall = true;
                    }
                }
            }

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

        // screen[nScreenWidth * nScreenHeight - 1] = '\0';

        // Clear the console (Linux specific)
        std::cout << "\033[2J\033[H";

        // Print the screen buffer
        for (int y = 0; y < nScreenHeight; y++)
        {
            for (int x = 0; x < nScreenWidth; x++)
            {
                std::cout << screen[y * nScreenWidth + x];
            }
            std::cout << std::endl;
        }

        // Add a small delay to control frame rate
        usleep(100000); // 100ms delay (adjust as needed)
    }

    delete[] screen;
    return 0;
}