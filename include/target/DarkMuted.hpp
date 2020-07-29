#ifndef PALETTE_TARGET_DARKMUTED_HPP
#define PALETTE_TARGET_DARKMUTED_HPP

#include "target/Target.hpp"

namespace Palette::Target {
    // Inherits Target to invoke methods to create the static DARK_MUTED target
    class DarkMuted : public Target {
        public:
            DarkMuted();
    };

    // Static instance
    static DarkMuted DARK_MUTED;
};

#endif