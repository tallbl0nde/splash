#include "target/DarkVibrant.hpp"

namespace Palette::Target {
    DarkVibrant::DarkVibrant() : Target() {
        this->setDefaultDarkLightnessValues();
        this->setDefaultVibrantSaturationValues();
    }
};