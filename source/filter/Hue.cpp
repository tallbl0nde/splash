#include "splash/filter/Hue.hpp"
#include <cmath>

namespace Splash::Filter {
    Hue::Hue(double h) {
        this->hue = h;
    }

    bool Hue::isAllowed(Colour & col) {
        // Want at least 10 degrees hue difference
        HSL hsl = col.hsl();
        float diff = std::abs(hsl.h - this->hue);
        return (diff > 10.0f && diff < 350.0f);
    }
};