#include <iostream>

#include "h/config.h"
#include "h/animation.h"
#include "h/utilities.h"
#include "h/consoleAdobeAnimation.h"
#include <windows.h>

Config config;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
Animation animation;

int menu() {
    int choice = 9;

    std::cout << std::endl << std::endl
        << "[Program do wybierania opcji uzywa tylko cyfr!!!]"         << std::endl
        << "=============== MENU ==============="                      << std::endl
        << "\t1) Zaladuj animacje"                                     << std::endl
        << "\t2) Generowanie animacji w postaci kodu txt [wg wzoru]"   << std::endl

        << "\t6) Edycja/podglad animacji"                              << std::endl

        << "[Program]"                                                 << std::endl
        << "\t9) Ustawienia"                                           << std::endl
        << "\t0) Wyjscie z programu"                                   << std::endl;

    //animation.loadAnimationFromJpgs();
    //return 6;

    std::cin >> choice;
    if (std::cin.fail()) std::cin.ignore(1000, '\n');
    switch (choice) {
        case 1:
        case 2:
        //case 3:
        //case 4:
        //case 5:
        case 6:
        //case 7:
            return choice;
        case 0:
            exit(0);
            return 0;
        case 9:
            config.menu();
        break;
        default:
            return menu();
    }
}

int main() {
    int choice;
    config.loadConfig();
    setupConsoleColors();

//    getLedPositionTest();
//    system("pause");
//    clearConsole();
//    animation.loadAnimationFromJpgs();
//    animation.loadAnimationConfig();
//    animation.save();
//    system("pause");
//    animation.generateAnimateTXTCode();

    do {
        choice = menu();
        switch (choice) {
            case 1:
                clearConsole();
                animation.load();
            break;
            case 2:
                animation.generateAnimateTXTCode();
            break;
            case 3:

            break;
            case 4:

            break;
            case 6:
                AdobeAnimation show;
                show.Begin();
                //getConsoleVisualization();
            break;
        }
    } while (choice != 0);
}
