#include "target/LightVibrant.hpp"

namespace Palette::Target {
    LightVibrant::LightVibrant() : Target() {
        this->setDefaultLightLightnessValues();
        this->setDefaultVibrantSaturationValues();
    }
};