#ifndef PALETTE_TARGET_LIGHTMUTED_HPP
#define PALETTE_TARGET_LIGHTMUTED_HPP

#include "target/Target.hpp"

namespace Palette::Target {
    // Inherits Target to invoke methods to create the static LIGHT_MUTED target
    class LightMuted : public Target {
        public:
            LightMuted();
    };

    // Static instance
    static LightMuted LIGHT_MUTED;
};

#endif