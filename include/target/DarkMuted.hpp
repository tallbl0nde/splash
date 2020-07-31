#ifndef SPLASH_TARGET_DARKMUTED_HPP
#define SPLASH_TARGET_DARKMUTED_HPP

#include "target/Target.hpp"

namespace Splash::Target {
    // Inherits Target to invoke methods to create the static DARK_MUTED target
    class DarkMuted : public Target {
        public:
            DarkMuted();
    };

    // Static instance
    static DarkMuted DARK_MUTED;
};

#endif