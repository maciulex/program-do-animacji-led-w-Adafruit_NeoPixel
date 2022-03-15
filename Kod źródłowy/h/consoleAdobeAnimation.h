#ifndef CONSOLEADOBEANIMATION_H
#define CONSOLEADOBEANIMATION_H

#include <thread>

class AdobeAnimation {
    int frame = 0;

    std::thread autoplayThread;
    void changeAutoplay();
    void initializeAutoplay();
    void stopAutoplay();
    void autoplayThreadVoid();
    bool autoplay = false;
    bool pauseAutoplay = false;

    void toggleLoop();
    bool loop = false;

    void display(bool autoplay = false);

    void getAnimationHud(int part);
    void getConsoleVisualization(int frameIndex);

    void nextFrame();
    void previusFrame();
    void toFrame();

    void changeTimings();
    public:
        void Begin();
};
#endif
