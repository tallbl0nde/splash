#ifndef SPLASH_TARGET_LIGHTVIBRANT_HPP
#define SPLASH_TARGET_LIGHTVIBRANT_HPP

#include "target/Target.hpp"

namespace Splash::Target {
    // Inherits Target to invoke methods to create the static LIGHT_VIBRANT target
    class LightVibrant : public Target {
        public:
            LightVibrant();
    };

    // Static instance
    static LightVibrant LIGHT_VIBRANT;
};

#endif