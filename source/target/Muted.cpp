#include "target/Muted.hpp"

namespace Palette::Target {
    Muted::Muted() : Target() {
        this->setDefaultNormalLightnessValues();
        this->setDefaultMutedSaturationValues();
    }
};