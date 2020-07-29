#include "target/LightMuted.hpp"

namespace Palette::Target {
    LightMuted::LightMuted() : Target() {
        this->setDefaultLightLightnessValues();
        this->setDefaultMutedSaturationValues();
    }
};