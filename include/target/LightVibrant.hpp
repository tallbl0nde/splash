#ifndef PALETTE_TARGET_LIGHTVIBRANT_HPP
#define PALETTE_TARGET_LIGHTVIBRANT_HPP

#include "target/Target.hpp"

namespace Palette::Target {
    // Inherits Target to invoke methods to create the static LIGHT_VIBRANT target
    class LightVibrant : public Target {
        public:
            LightVibrant();
    };

    // Static instance
    static LightVibrant LIGHT_VIBRANT;
};

#endif