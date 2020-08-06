#include "splash/target/LightVibrant.hpp"

namespace Splash::Target {
    LightVibrant::LightVibrant() : Target() {
        this->setDefaultLightLightnessValues();
        this->setDefaultVibrantSaturationValues();
    }
};