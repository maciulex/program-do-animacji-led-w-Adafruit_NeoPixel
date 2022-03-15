#include "../h/animation.h"

#include "../h/config.h"
#include "../h/utilities.h"
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>

//#include <filesystem>

extern Config config;

bool Animation::loadAnimationFromJpgs(std::string path) {
    sf::Image frame;

    while (frame.loadFromFile(path+std::to_string(frameCount)+".jpg")) frameCount+=1;
    frames = new Frame [frameCount];


    int frameNumber = 0;
    while (frame.loadFromFile(path+std::to_string(frameNumber)+".jpg")) {
        int width  = frame.getSize().x;
        int height = frame.getSize().y;

        frames[frameNumber].frameTime = config.deafultFrameTime;
        frames[frameNumber].width = width;
        frames[frameNumber].height = height;

        frames[frameNumber].pixel = new sf::Color *[width];

        for (int x = 0; x < width; x++) {
            frames[frameNumber].pixel[x] = new sf::Color [height];
            for (int y = 0; y < height; y++) {
                    int r = frame.getPixel(x,y).r, g = frame.getPixel(x,y).g, b = frame.getPixel(x,y).b;

                    //frames[frameNumber].pixel[x][y] = sf::Color(getScaledRGBValue(r), getScaledRGBValue(g), getScaledRGBValue(b));
                    frames[frameNumber].pixel[x][y] = sf::Color(r, g, b);
            };
        }
        frameNumber += 1;
    }
    status = true;
    return true;
}

void Animation::displayFramesTimings() {
    std::cout << std::endl << std::endl;
    for (int i = 0; i < frameCount; i++) std::cout << i+1 << ") " << frames[i].frameTime << std::endl;
}

void Animation::loadAnimationConfig(std::string path, bool loadFrameData) {
    std::string line;

    std::fstream config;
    config.open(path+"config.txt", std::ios::in);
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
        if (what == ':') {
            switch (std::stoi(index)) {
                case 1:
                    name = result;
                break;
                case 2:
                    saveIndex = std::stoi(result);
                break;
            }
        } else if (loadFrameData) frames[std::stoi(index)-1].frameTime = std::stoi(result);
    }

    config.close();
}
void Animation::saveAs() {
    clearConsole();
    int choice;
    std::cout << "Save as:" << std::endl
    << "   1) Animacja na ledy" << std::endl
    << "   2) W schowku jako nowe" << std::endl
    << "   3) W schowku nadpisac" << ((saveIndex == -1) ? "[NIEDOSTEPNE]" : "") << std::endl
    << "   0) Powrot" << std::endl;
    std::cin >> choice;
    switch (choice) {
        case 1:
            generateAnimateTXTCode();
        break;
        case 2:
            saveAsNew();
        break;
        case 3:
            reSave();
        break;
        case 0:
            return;
    }
}
void Animation::saveAsNew(int index) {
    int fileIndex = 0;
    std::string path = config.storeAnimationPath;
    std::fstream file;

    if (index == -1) {
        do {
            fileIndex += 1;
            if (file.is_open()) file.close();
            file.open(path+std::to_string(fileIndex)+"/config.txt");
        } while (file.good());
    } else fileIndex = index;
    if (CreateDirectory((config.storeAnimationPath+std::to_string(fileIndex)).c_str(), NULL) == 0) std::cout << "ERROR CEREATE DIR" << std::endl;

    file.open(path+std::to_string(fileIndex)+"/config.txt", std::fstream::out);

    file << "1:" << name      << std::endl;
    file << "2:" << fileIndex << std::endl;

    for (int i = 0; i < frameCount; i++) {
        file << i << "." << frames[i].frameTime << std::endl;
    }

    file.close();

    saveAnimationFramesByJPG(path+std::to_string(fileIndex)+"/");

    saveIndex = fileIndex;
}

void Animation::saveAnimationFramesByJPG(std::string path) {
    for (int i = 0; i < frameCount; i++) {
        sf::Image toSave;
        toSave.create(frames[i].width, frames[i].height);
        for (int x = 0; x < frames[i].width; x++) {
            for (int y = 0; y < frames[i].height; y++) toSave.setPixel(x, y, frames[i].pixel[x][y]);
        }
        toSave.saveToFile(path+std::to_string(i)+".jpg");
    }
}

void Animation::reSave() {
    if (saveIndex == -1) return;
    RemoveDirectoryA(((config.storeAnimationPath+std::to_string(saveIndex)).c_str()));
    saveAsNew(saveIndex);
}

void Animation::save() {
    if (saveIndex == -1) saveAsNew();
    else reSave();
}

void Animation::load() {
    int choice;

    std::cout << std::endl << std::endl
    << "Skad chcesz wczytac?" << std::endl
    << "   1) Nowa animacja" << std::endl
    << "   2) Zapisana animacja" << std::endl;
    std::cin >> choice;
    if (std::cin.fail()) {std::cin.ignore(1000, '\n'); return;}
    switch (choice) {
        case 1:
            loadNewAnimation();
        break;
        case 2:
            loadSavedAnimation();
        break;
    }
}

void Animation::loadSavedAnimation() {
    clearConsole();
    int animations = 1, choice, animationIndex[1000];
    for (int i = 0; i < 1000; i++) animationIndex[i] = -1;
    std::cout << "Prosze wybrac animacje" << std::endl;
    for (int i = 1; i < 1000; i++) {
        std::fstream file;
        file.open(config.storeAnimationPath+std::to_string(i)+"/config.txt", std::ios::in);
        if (file.good()) {
            Animation ani;
            ani.loadAnimationConfig(config.storeAnimationPath+std::to_string(i)+"/", false);
            std::cout << animations << ") " << ani.name << std::endl;
            animationIndex[animations] = i;
            animations += 1;
        }
        if (file.is_open()) file.close();
    }
    std::cin >> choice;
    if (std::cin.fail()) {std::cin.ignore(1000, '\n'); return;}
    if (choice > 9999|| animationIndex[choice] == -1) {return;}
    loadAnimationFromJpgs(config.storeAnimationPath+std::to_string(animationIndex[choice])+"/");
    loadAnimationConfig(config.storeAnimationPath+std::to_string(animationIndex[choice])+"/");
}

void Animation::changeName() {
    std::cout << std::endl << std::endl << std::endl
        << "Podaj nowa nazwe: ";
    std::cin >> name;
    if (std::cin.fail()) {std::cin.ignore(1000, '\n');}
}
void Animation::loadNewAnimation() {
    loadAnimationFromJpgs();
    loadAnimationConfig();
}
void Animation::generateAnimateTXTCode() {
    clearConsole();
    std::fstream outputFile;
    outputFile.open(config.readyAnimationPath+name+".txt", std::ios::in);
    if (!outputFile.fail()) {
        char decision;
        std::cout << "!!!UWAGA!!!" << std::endl
            << "Taki plik |"<<config.readyAnimationPath+name+".txt"<<"| juz istnieje kontynuowanie bedzie oznaczac nadpisanie!!!" << std::endl
            << "W celu zapobiegniecia prosze o anulowanie i zmiane nazwy animacji"  << std::endl
            << "Kontynuowac? (T/N): ";
        std::cin >> decision;
        if (decision != 'T' && decision != 't') return;
    }
    outputFile.close();
    outputFile.open(config.readyAnimationPath+name+".txt", std::ios::out | std::ios::trunc);

    std::string frameLatency = "";
    for(int i = 0; i < frameCount-1; i++) frameLatency += std::to_string(frames[i].frameTime)+",";
    frameLatency += std::to_string(frames[frameCount-1].frameTime);

    outputFile << "#include <Adafruit_NeoPixel.h>"                                   << std::endl
    << "   namespace NeoPixel {"                                                     << std::endl
    << "      const int pixelPin = " << config.pixelPin << ";"                       << std::endl
    << "      const int btnPin = " << config.btnPin   << ";"                         << std::endl
    << "      const int potPin = " << config.potPin   << ";"                         << std::endl
    << "      const int pixelCount = " << config.pixelCount << ";"                   << std::endl
    << "      Adafruit_NeoPixel pixels(pixelCount, pixelPin, NEO_GRB + NEO_KHZ800);" << std::endl << std::endl
    << "      int frameCount = "<< frameCount << ";"                                 << std::endl
    << "      int pixelPerFrame = " << config.animationHeight*config.animationWidth  << ";" << std::endl<< std::endl
    << "      int latency["<<frameCount<<"] = {"<<frameLatency<<"};"                 << std::endl
    << "                                                                           " << std::endl
    << "      int animation ["<<frameCount<<"]["<<(config.animationWidth*config.animationHeight)<<"][4] =  {" << std::endl;
    for (int i = 0; i < frameCount; i++) {
        outputFile << "{";
        for (int x = 0; x < config.animationWidth; x++) {
            for (int y = 0; y < config.animationHeight; y++) {
                outputFile
                    << "{"
                        << getLEDPosition(x,y)
                        << ", " << std::to_string(getScaledRGBValue(frames[i].pixel[x][y].r))
                        << ", " << std::to_string(getScaledRGBValue(frames[i].pixel[x][y].g))
                        << ", " << std::to_string(getScaledRGBValue(frames[i].pixel[x][y].b))
                    << "},";
            }
        }
        outputFile << "}," << std::endl;
    }
    outputFile
    << "      };"                                                                    << std::endl
    << "      void setup() {"                                                        << std::endl
    << "         pixels.begin();"                                                    << std::endl
    << "         pixels.show();"                                                     << std::endl
    << "      }"                                                                     << std::endl
    << "      void loop() {"                                                         << std::endl
    << "            for (int frame = 0; frame < frameCount; frame++) {"              << std::endl
    << "                for (int pixel = 0; pixel < pixelPerFrame; pixel++) {"       << std::endl
    << "                    pixels.setPixelColor(animation[frame][pixel][0],"        << std::endl
    << "                                         animation[frame][pixel][1],"        << std::endl
    << "                                         animation[frame][pixel][2],"        << std::endl
    << "                                         animation[frame][pixel][3]);"       << std::endl
    << "                 }"                                                          << std::endl
    << "                pixels.show();"                                              << std::endl
    << "                delay(latency[frame]);"                                      << std::endl
    << "            }"                                                               << std::endl
    << "      }"                                                                     << std::endl
    << "   }"                                                                        << std::endl


    ;

    outputFile.close();
}
