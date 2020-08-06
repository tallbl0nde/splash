#include "splash/target/LightMuted.hpp"

namespace Splash::Target {
    LightMuted::LightMuted() : Target() {
        this->setDefaultLightLightnessValues();
        this->setDefaultMutedSaturationValues();
    }
};