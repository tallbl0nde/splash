#include "target/DarkMuted.hpp"

namespace Splash::Target {
    DarkMuted::DarkMuted() : Target() {
        this->setDefaultDarkLightnessValues();
        this->setDefaultMutedSaturationValues();
    }
};