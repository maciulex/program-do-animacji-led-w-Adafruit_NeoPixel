#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config {
    public:
        bool adobeAnimateFirstTimeWarning = false;

        unsigned int deafultFrameTime = 500;

        int animationWidth = 72;
        int animationHeight = 7;

        int pixelPin = 13;
        int btnPin = 1;
        int potPin = 0;
        int pixelCount = 504;
        int maxRgbValue = 100;

        std::string configPath          = "ProgramData/Config/";

        std::string newAnimationPath    = "ProgramData/nowaAnimacja/";
        std::string storeAnimationPath  = "ProgramData/zapisaneAnimacje/";
        std::string readyAnimationPath  = "ProgramData/GotoweAnimacje/";

        void saveConfig();
        void loadConfig();

        void menu();
        void pinConfig();
        void animationConfig();
};
#endif // CONFIG_H
