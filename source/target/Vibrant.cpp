#include "target/Vibrant.hpp"

namespace Palette::Target {
    Vibrant::Vibrant() : Target() {
        this->setDefaultNormalLightnessValues();
        this->setDefaultVibrantSaturationValues();
    }
};