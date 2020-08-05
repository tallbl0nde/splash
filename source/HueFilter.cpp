#include "HueFilter.hpp"
#include <cmath>

namespace Splash {
    MediaStyle::HueFilter::HueFilter(MediaStyle * ptr) {
        this->mp = ptr;
    }

    bool MediaStyle::HueFilter::isAllowed(Colour & col) {
        // Want at least 10 degrees hue difference
        HSL hsl = col.hsl();
        float diff = std::abs(hsl.h - this->mp->filteredBackgroundHSL.h);
        return (diff > 10.0f && diff < 350.0f);
    }
};