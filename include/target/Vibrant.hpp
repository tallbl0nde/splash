#ifndef SPLASH_TARGET_VIBRANT_HPP
#define SPLASH_TARGET_VIBRANT_HPP

#include "target/Target.hpp"

namespace Splash::Target {
    // Inherits Target to invoke methods to create the static VIBRANT target
    class Vibrant : public Target {
        public:
            Vibrant();
    };

    // Static instance
    static Vibrant VIBRANT;
};

#endif