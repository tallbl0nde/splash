#include "target/Vibrant.hpp"

namespace Splash::Target {
    Vibrant::Vibrant() : Target() {
        this->setDefaultNormalLightnessValues();
        this->setDefaultVibrantSaturationValues();
    }
};