#ifndef SPLASH_TARGET_DARKVIBRANT_HPP
#define SPLASH_TARGET_DARKVIBRANT_HPP

#include "target/Target.hpp"

namespace Splash::Target {
    // Inherits Target to invoke methods to create the static DARK_VIBRANT target
    class DarkVibrant : public Target {
        public:
            DarkVibrant();
    };

    // Static instance
    static DarkVibrant DARK_VIBRANT;
};

#endif