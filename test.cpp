#include <iostream>

int nScreenWidth = 120;
int nScreenHeight = 40;

int main()
{
    char *screen = new char[nScreenHeight * nScreenWidth];

    screen[0] = '#';
    for (int x = 0; x < nScreenWidth; x++) {
        screen[20 * nScreenWidth + x] = '#';
    }

    // Clear the console (Linux specific)
    std::cout << "\033[2J\033[H";

    for (int y = 0; y < nScreenHeight; y++)
    {
        for (int x = 0; x < nScreenWidth; x++)
        {
            
            std::cout << screen[y * nScreenWidth + x];
        }
        std::cout << std::endl;
    }

    delete[] screen;
    return 0;
}