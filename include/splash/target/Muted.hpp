#ifndef SPLASH_TARGET_MUTED_HPP
#define SPLASH_TARGET_MUTED_HPP

#include "splash/target/Target.hpp"

namespace Splash::Target {
    // Inherits Target to invoke methods to create the static MUTED target
    class Muted : public Target {
        public:
            Muted();
    };

    // Static instance
    static Muted MUTED;
};

#endif