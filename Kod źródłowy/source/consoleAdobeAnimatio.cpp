#include "../h/consoleAdobeAnimation.h"

#include "../h/animation.h"
#include "../h/utilities.h"
#include "../h/config.h"

#include <windows.h>
#include <chrono>
#include <iostream>
#include <iostream>

extern Animation animation;
extern HANDLE hConsole;
extern Config config;

void AdobeAnimation::previusFrame() {
    if (frame == 0) frame = animation.frameCount-1;
    else frame -= 1;
}

void AdobeAnimation::nextFrame() {
    if (frame == animation.frameCount-1) frame = 0;
    else frame += 1;
}

void AdobeAnimation::Begin() {
    if (!animation.status) {
        confirm("Nie ma zaladowanej animacji!" );

        return;
    }

    clearConsole();
    display();

    int choice = -1;
    bool AdobeAnimationStatus = true;
    do {
        std::cin >> choice;
        switch (choice) {
            case 4:
                changeTimings();
            break;
            case 5:
                changeAutoplay();
            break;
            case 6:
                toggleLoop();
            break;
            case 7:
                previusFrame();
            break;
            case 8:
                nextFrame();
            break;
            case 0:
                AdobeAnimationStatus = false;
            break;
            case 10:
                animation.save();
            break;
            case 11:
                animation.saveAs();
            break;
            case 12:
                loadAnimation();
            break;
            case 13:
                animation.changeName();
            break;
            case 14:
                toFrame();
        }
        display();
        if (std::cin.fail()) std::cin.ignore(1000, '\n');
    } while (AdobeAnimationStatus);
}

void AdobeAnimation::toFrame() {
    clearConsole();
    unsigned int frameChange;
    std::cout << "Prosze podac klatke do ktorej chcesz przejsc: ";
    std::cin >> frameChange;
    if (std::cin.fail()) {std::cin.ignore(1000, '\n'); confirm("Zle dane"); return;}
    frameChange-=1;
    if (frameChange > animation.frameCount || frameChange <= 0) {confirm("Zle dane"); return;}
    frame = frameChange;
}

void AdobeAnimation::getAnimationHud(int part) {
    if (!config.adobeAnimateFirstTimeWarning) {
        std::cout << "[Kolory sa pogladowe z powodow technicznych jest ich makx 15]" << std::endl
        << "[Kolory w rzeczywistosci beda jak w na obrazku jest to czysto konsolowa rzecz]" << std::endl
        << "[Kolory mozna zmienic w ustawieniach (prawdopodobnie)]" << std::endl
        << "[W razie czego moga sie nawet wcale nie zgadzac]" << std::endl;
        config.adobeAnimateFirstTimeWarning = true;
        confirm("");
        clearConsole();
    }
    std::string tmp[2];
            std::cout << "--------------------------------------------------------------------------------------------------------------------" << std::endl;
    switch (part) {
        default:
        case 0:
            tmp[0] = (frame > 0) ? getText(std::to_string(animation.frames[frame-1].frameTime), 4, '0', "before") : "----";
            tmp[1] = (animation.frameCount > frame+1) ? getText(std::to_string(animation.frames[frame+1].frameTime), 4, '0', "before") : "----";
            std::cout << "|  Nazwa: "<< getText(animation.name) <<"  Klatka: "<< getText(std::to_string(frame+1), 4, '0', "before") <<"/"<<getText(std::to_string(animation.frameCount), 4, '0', "before")
                      <<"                                                                                |" << std::endl;
            std::cout << "|------------------------------------------------------------------------------------------------------------------|" << std::endl;
            std::cout << "| | w ms | | Czas wyswietlania: " << getText(std::to_string(animation.frames[frame].frameTime), 4, '0', "before")<<" | | Czas nastepnej klatki: "<<tmp[1]<<" | | Czas poprzedniej klatki: "<<tmp[0]<<" |           |" << std::endl;
            std::cout << "|------------------------------------------------------------------------------------------------------------------|" << std::endl;
            std::cout << "| | 4) Zmiana opoznien |                                                                         | 14) Do klatki | |" << std::endl;
        break;
        case 1:
            std::cout << "| | Auto odtwarzanie: "<<getText(((autoplay) ? "on" : "off"), 3)<<" | | Zapentlenie: "<<getText(((loop) ? "on" : "off"), 3)<<" |                                                                   |" << std::endl;
            std::cout << "|------------------------------------------------------------------------------------------------------------------|" << std::endl;
            std::cout << "| | 5) Auto odtwarzanie   | | 6) Zapentlenie   |                   | 7) Poprzednia klatka | | 8) Nastepna klatka | |" << std::endl;
            std::cout << "|------------------------------------------------------------------------------------------------------------------|" << std::endl;
            std::cout << "| | 10) Zapisz | | 11) Zapisz jako | | 12) Wczytaj |   | 13) Zmiana nazwy |            | 9) Pomoc | | 0) Powrot  | |" << std::endl;
        break;
    }
            std::cout << "--------------------------------------------------------------------------------------------------------------------" << std::endl;
}

void AdobeAnimation::getConsoleVisualization(int frameIndex) {
    Frame frame = animation.frames[frameIndex];
    for (int y = 0; y < frame.height; y++) {
        for (int x = 0; x < frame.width; x++) {
            //getClosestColor(frame.pixel[x][y]);
            SetConsoleTextAttribute(hConsole, getClosestColor(frame.pixel[x][y]));
            std::cout << (char)(219);
        }
        std::cout << std::endl;
    }
    SetConsoleTextAttribute(hConsole, 15);
}

void AdobeAnimation::changeTimings() {
    pauseAutoplay = true;

    animation.displayFramesTimings();

    int frame;
    unsigned int time;

    std::cout <<  "-1) Edycja wszystkich klatek" << std::endl;
    std::cout << "Prosze podac numer klatki do edycji i po spacji czas przez jaki klatka ma sie wyswietlac: ";
    std::cin >> frame >> time;

    if (std::cin.fail()) {std::cin.ignore(1000, '\n');confirm("Nie poprawne wartosci"); return;}
    if ((time < 0) || (frame < -1 || frame > animation.frameCount-1)) {confirm("Nie poprawne wartosci"); return;};

    if (frame == -1) for (int frameI = 0; frameI < animation.frameCount; frameI++) animation.frames[frameI].frameTime = time;
    else animation.frames[frame-1].frameTime = time;

    pauseAutoplay = false;
}


void AdobeAnimation::display(bool autoplay) {
        clearConsole();

        getAnimationHud(0);
        getConsoleVisualization(frame);
        getAnimationHud(1);
}

void AdobeAnimation::autoplayThreadVoid() {
    bool lastframe = false;
    while (autoplay) {
            std::chrono::milliseconds timespan(animation.frames[frame].frameTime);
            std::this_thread::sleep_for(timespan);
            if (pauseAutoplay) continue;
            if (!loop && frame+1 == animation.frameCount) {
                if (!lastframe) {display(); lastframe = true;}
                continue;
            }
            lastframe = false;
            display();
            nextFrame();
    }
}

void AdobeAnimation::toggleLoop() {
    if (loop) loop = false;
    else loop = true;
}
void AdobeAnimation::changeAutoplay() {
    if (autoplay) return stopAutoplay();
    initializeAutoplay();
}

void AdobeAnimation::initializeAutoplay() {
    autoplay = true;
    autoplayThread = std::thread(autoplayThreadVoid, this);
}

void AdobeAnimation::stopAutoplay() {
    autoplay = false;
    autoplayThread.detach();
    display();
}
