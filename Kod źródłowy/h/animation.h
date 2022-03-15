#ifndef ANIMATION_CPP
#define ANIMATION_CPP

#include <SFML/Graphics.hpp>
#include "config.h"

extern Config config;

class Frame {
    public:
        unsigned int frameTime;
        int width, height;
        sf::Color **pixel;
};

class Animation {
    public:
        bool status = false;
        std::string name = "plane";
        int frameCount = 0;
        int saveIndex = -1;
        Frame *frames;

        void loadNewAnimation();
        void loadSavedAnimation();
        void loadAnimationConfig(std::string path = config.newAnimationPath, bool loadFrameData = true);
        bool loadAnimationFromJpgs(std::string path = config.newAnimationPath);

        void changeName();

        void saveAs();
        void save();
        void saveAsNew(int index = -1);
        void saveAnimationFramesByJPG(std::string path = "");
        void reSave();
        void load();

        void generateAnimateTXTCode();

        void displayFramesTimings();
};


#endif
