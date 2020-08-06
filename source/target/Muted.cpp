#include "splash/target/Muted.hpp"

namespace Splash::Target {
    Muted::Muted() : Target() {
        this->setDefaultNormalLightnessValues();
        this->setDefaultMutedSaturationValues();
    }
};