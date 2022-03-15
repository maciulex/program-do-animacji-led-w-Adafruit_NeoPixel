#include "../h/config.h"

#include <iostream>
#include <fstream>
#include "../h/utilities.h"

void Config::saveConfig() {
    std::fstream output;
    output.open(configPath+"config.txt", std::fstream::out | std::fstream::trunc);
    output
        << "#Plik konfiguracyjny animacji" << std::endl
        << "#" << std::endl
        << "#indeks opcji" << std::endl
        << "#  1 - domyœlny czas na klatke w milisekundach" << std::endl
        << "#  2 - œcie¿ka do plików nowej animacji" << std::endl
        << "#  3 - œcie¿ka do plików starych animacji" << std::endl
        << "#  4 - œcie¿ka do  plików configu" << std::endl
        << "#  5 - œcie¿ka dla gotowych animacji" << std::endl
        << "#  6 - czy by³a ju¿ informacja o kolorach" << std::endl
        << "#  7 - maksymalna wartosc r g b" << std::endl
        << "#" << std::endl
        << "#  10 - pixelpin" << std::endl
        << "#  11 - btnPin" << std::endl
        << "#  12 - potPin" << std::endl
        << "#  13 - pixelCount" << std::endl
        << "#" << std::endl
        << "# budowa:" << std::endl
        << "#  1:x LUB 1-x" << std::endl
        << "#  1 <- pe³ni role numeru indeksu opcji (rozpiska wy¿ej) lub klatka animacji o któr¹ chodzi" << std::endl
        << "#  : <- to oznacza ¿e chodzi o indeks opcji" << std::endl
        << "#  x <- wartoœæ" << std::endl
        << "1:"<<deafultFrameTime<< std::endl
        << "2:"<<newAnimationPath<< std::endl
        << "3:"<<storeAnimationPath<< std::endl
        << "4:"<<configPath<< std::endl
        << "5:"<<readyAnimationPath<< std::endl
        << "6:"<<((adobeAnimateFirstTimeWarning) ? 1 : 0)<< std::endl
        << "7:"<<maxRgbValue<<std::endl<<std::endl

        << "10:"<<pixelPin<< std::endl
        << "11:"<<btnPin<< std::endl
        << "12:"<<potPin<< std::endl
        << "13:"<<pixelCount<< std::endl;
    output.close();
}

void Config::loadConfig() {
    std::string line;
    std::fstream config;
    config.open(configPath+"config.txt", std::ios::in);
    if (!config.good()) {
        std::cout << "Nie znaleziono pliku konfigu" << std::endl;
        return;
    };
    while (std::getline(config, line)) {
        if (line[0] == '#') continue;
        char what = 'c';
        std::string index = "", result = "";
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == ':' || line[i] == '.') {
                what = line[i];
                continue;
            }
            if (what == 'c') index += line[i];
            else result += line[i];
        }
        if (index == "") continue;
        switch (std::stoi(index)) {
            case 1:
                deafultFrameTime = std::stoi(result);
            break;
            case 2:
                newAnimationPath = result;
            break;
            case 3:
                storeAnimationPath = result;
            break;
            case 4:
                configPath = result;
            break;
            case 5:
                readyAnimationPath = result;
            break;
            case 6:
                adobeAnimateFirstTimeWarning = (result == "1") ? true : false;
            break;
            case 7:
                maxRgbValue = std::stoi(result);
            break;
            case 10:
                pixelPin = std::stoi(result);
            break;
            case 11:
                btnPin = std::stoi(result);
            break;
            case 12:
                potPin = std::stoi(result);
            break;
            case 13:
                pixelCount = std::stoi(result);
            break;

        }
    }

    config.close();
}

void Config::menu() {
    bool state = true;
    while (state) {
        clearConsole();
        int choice;

        std::cout << "-----Ustawienia-----" << std::endl
        << "   1) Ustawienia pinow" << std::endl
        << "   2) Ustawienia animacji" << std::endl
        << "   0) Powrot";
        std::cin >> choice;
        if (std::cin.fail()) continue;
        switch (choice) {
            case 1:
                pinConfig();
            break;
            case 2:
                animationConfig();
            break;
            case 0:
                state = false;
            break;
        }
        saveConfig();
    }
}

void Config::pinConfig() {
    int choice, value;
    std::cout << "Pin Config" << std::endl
        << "   1) PixelPin val: " << pixelPin << std::endl
        << "   2) BtnPin val: " << btnPin << std::endl
        << "   3) potPin val: " << potPin << std::endl
        << "   4) pixelCount val: " << pixelCount << std::endl
        << "   0) Powrot" << std::endl;
    std::cin >> choice;
    if (std::cin.fail()) {std::cin.ignore(1000, '\n'); return confirm("Cos poszlo nie tak");}
    switch (choice) {
        case 1:
            std::cin >> value;
            if (std::cin.fail()) {std::cin.ignore(1000, '\n'); return confirm("Cos poszlo nie tak");}
            pixelPin = value;
        break;
        case 2:
            std::cin >> value;
            if (std::cin.fail()) {std::cin.ignore(1000, '\n'); return confirm("Cos poszlo nie tak");}
            btnPin = value;
        break;
        case 3:
            std::cin >> value;
            if (std::cin.fail()) {std::cin.ignore(1000, '\n'); return confirm("Cos poszlo nie tak");}
            potPin = value;
        break;
        case 4:
            std::cin >> value;
            if (std::cin.fail()) {std::cin.ignore(1000, '\n'); return confirm("Cos poszlo nie tak");}
            pixelCount = value;
        break;
        case 0:
            return;
        break;
    }
}

void Config::animationConfig() {
    int choice, value;
    std::cout << "Animation Config" << std::endl
        << "   1) Szerokosc val: " << animationWidth << std::endl
        << "   2) Wysokosc val: " << animationHeight << std::endl
        << "   3) Maks jasnosc <0, 255> val: " << maxRgbValue << std::endl
        << "   4) Domyslny czas na klatke val: " << deafultFrameTime << std::endl
        << "   0) Powrot" << std::endl;
    std::cin >> choice;
    if (std::cin.fail()) {std::cin.ignore(1000, '\n'); return confirm("Cos poszlo nie tak");}
    switch (choice) {
        case 1:
            std::cin >> value;
            if (std::cin.fail()) {std::cin.ignore(1000, '\n'); return confirm("Cos poszlo nie tak");}
            if (value < 0) return confirm("Zla wartosc");
            animationWidth = value;
        break;
        case 2:
            std::cin >> value;
            if (std::cin.fail()) {std::cin.ignore(1000, '\n'); return confirm("Cos poszlo nie tak");}
            if (value < 0) return confirm("Zla wartosc");
            animationHeight = value;
        break;
        case 3:
            std::cin >> value;
            if (std::cin.fail()) {std::cin.ignore(1000, '\n'); return confirm("Cos poszlo nie tak");}
            if (value < 0 || value > 255) return confirm("Zla wartosc");
            maxRgbValue = value;
        break;
        case 4:
            std::cin >> value;
            if (std::cin.fail()) {std::cin.ignore(1000, '\n'); return confirm("Cos poszlo nie tak");}
            if (value < 0) return confirm("Zla wartosc");
            deafultFrameTime = value;
        break;
        case 0:
            return;
        break;
    }
}
