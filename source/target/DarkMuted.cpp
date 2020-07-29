#include "target/DarkMuted.hpp"

namespace Palette::Target {
    DarkMuted::DarkMuted() : Target() {
        this->setDefaultDarkLightnessValues();
        this->setDefaultMutedSaturationValues();
    }
};