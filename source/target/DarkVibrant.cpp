#include "splash/target/DarkVibrant.hpp"

namespace Splash::Target {
    DarkVibrant::DarkVibrant() : Target() {
        this->setDefaultDarkLightnessValues();
        this->setDefaultVibrantSaturationValues();
    }
};