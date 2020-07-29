#ifndef PALETTE_TARGET_MUTED_HPP
#define PALETTE_TARGET_MUTED_HPP

#include "target/Target.hpp"

namespace Palette::Target {
    // Inherits Target to invoke methods to create the static MUTED target
    class Muted : public Target {
        public:
            Muted();
    };

    // Static instance
    static Muted MUTED;
};

#endif