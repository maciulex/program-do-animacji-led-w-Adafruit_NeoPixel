#include "../h/utilities.h"

#include "../h/animation.h"
#include "../h/config.h"

#include <iostream>
#include <windows.h>
#include <math.h>

extern Animation animation;
extern HANDLE hConsole;
extern Config config;

struct {
    int colorNumber = 11;
    int colorsRGB[14][3] = {
        //black
        {0,   0,   0  },
        //white
        {255, 255, 255},
        //red
        {255, 0,   0  },
        //green
        {0,   255, 0  },
        //blue
        {0,   0,   255},
        //yellow
        {255, 255, 0  },
        //orange
        {255, 165, 0  },
        //pink
        {255, 192, 203},
        //purple
        {128, 0,   128},
        //aqua
        {0,   255, 255},
        //brown
        {150, 75,  0},
        //grey
        {128, 128, 128},
    };
    unsigned long long colors32[14];
    std::string names[14] = {
        "Black",
        "White",
        "Red",
        "Green",
        "Blue",
        "Yellow",
        "Orange",
        "Pink",
        "Purple",
        "Aqua",
        "Brown",
        "Grey"
    };
} Colors;

void setupConsoleColors() {
    CONSOLE_SCREEN_BUFFER_INFOEX info;
    info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
    GetConsoleScreenBufferInfoEx(hConsole, &info);

    for (int i = 0; i < Colors.colorNumber; i++) {
        //std::cout << Colors.colorsRGB[i][0] << " " << Colors.colorsRGB[i][1] << " " << Colors.colorsRGB[i][2]
        //<< " " << (Colors.colorsRGB[i][0]+1) * (Colors.colorsRGB[i][1]+1) * (Colors.colorsRGB[i][2]+1) << '\n';
        Colors.colors32[i] = 1;
        if (Colors.colorsRGB[i][0] != 0) {Colors.colors32[i] *= Colors.colorsRGB[i][0];}
        if (Colors.colorsRGB[i][1] != 0) {Colors.colors32[i] *= Colors.colorsRGB[i][1];}
        if (Colors.colorsRGB[i][2] != 0) {Colors.colors32[i] *= Colors.colorsRGB[i][2];}
        Colors.colors32[i] -= 1;

        info.ColorTable[i] = RGB(Colors.colorsRGB[i][0],
                                 Colors.colorsRGB[i][1],
                                 Colors.colorsRGB[i][2]);
        SetConsoleScreenBufferInfoEx(hConsole, &info);
    }
    SetConsoleTextAttribute(hConsole, 15);
}

void loadAnimation() {
    clearConsole();
    std::cout << "0) Wczytaj nowa animacje" << std::endl;


}

void colorTest() {
    for (int i = 0; i < Colors.colorNumber; i++) {
        SetConsoleTextAttribute(hConsole, i);
        std::cout << i+1 << ") " << (char)(219) << " " << Colors.names[i] << std::endl;
    }

    SetConsoleTextAttribute(hConsole, 15);
    std::cout << "15) !C White" << std::endl;
}

int getClosestColor(sf::Color color) {
    //double diffrence = sqrt(pow(color.a - Colors.colorsRGB[0][0] ,2) + pow(color.g - Colors.colorsRGB[0][1] ,2) + pow(color.b - Colors.colorsRGB[0][2] ,2));
    double diffrence = abs(color.r-Colors.colorsRGB[0][0]) + abs(color.g-Colors.colorsRGB[0][1]) + abs(color.b-Colors.colorsRGB[0][2]);
    int closestIndex = 0;

    for (int i = 1; i < Colors.colorNumber; i++) {
        //double newDiffrence = sqrt(pow(color.a - Colors.colorsRGB[i][0] ,2) + pow(color.g - Colors.colorsRGB[i][1] ,2) + pow(color.b - Colors.colorsRGB[i][2] ,2));
        double newDiffrence = abs(color.r-Colors.colorsRGB[i][0]) + abs(color.g-Colors.colorsRGB[i][1]) + abs(color.b-Colors.colorsRGB[i][2]);
        if (newDiffrence < diffrence) {
            diffrence = newDiffrence;
            closestIndex = i;
        }
    }
    return closestIndex;
}


void pressToContiune() {
    std::cout << std::endl << "Aby kontynuowac prosze wcisnac ENTER" << std::endl;
    getchar();
}

void confirm(std::string text) {
    std::cout << std::endl << text << std::endl;
    std::cout << std::endl << "Aby kontynuowac prosze wcisnac ENTER" << std::endl;
    std::cin.get();
    std::cin.get();
}
int getScaledRGBValue(int rgb) {
    return round((float)rgb / (float)(((float)255)/((float)config.maxRgbValue)));
}
void clearConsole() {
    std::cout << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl
    << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl;
}

std::string getText(std::string text, int finalSize, char fillChar, std::string fillType) {
    if (text.length() > finalSize) return text.substr(0, finalSize-3) + "...";

    for (int i = 0; i < finalSize-text.length(); i++) {
        if (fillType == "after") text += fillChar;
        else text = fillChar + text;
    }
    return text;
}
void getLedPositionTest() {
    for (int y = 0; y < config.animationHeight; y++) {
        std::cout << y+1 << ") " << getLEDPosition(0,y) << " "  << getLEDPosition(71,y) << std::endl;
    }
}
int getLEDPosition(int x, int y) {
    if ((y+1) % 2 == 0) return (config.animationWidth-x)+(y*config.animationWidth)-1;
    return x+(y*config.animationWidth);
}

