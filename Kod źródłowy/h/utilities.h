#ifndef UTILITES_CPP
#define UTILITES_CPP
#include <SFML/Graphics.hpp>

void colorTest();
void setupConsoleColors();
int getClosestColor(sf::Color color);
void pressToContiune();
void clearConsole();
void confirm(std::string text = "");
void getLedPositionTest();
int getLEDPosition(int x, int y);
void loadAnimation();
int getScaledRGBValue(int rgb = 0);
std::string getText(std::string text = "", int finalSize = 10, char fillChar = ' ', std::string fillType = "after");

#endif

