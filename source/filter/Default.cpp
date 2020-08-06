#include "splash/filter/Default.hpp"

// Constants
#define BLACK_MAX_LIGHTNESS 0.05f;
#define WHITE_MIN_LIGHTNESS 0.95f;

namespace Splash::Filter {
    bool Default::isBlack(HSL & hsl) {
        return hsl.l <= BLACK_MAX_LIGHTNESS;
    }

    bool Default::isWhite(HSL & hsl) {
        return hsl.l >= WHITE_MIN_LIGHTNESS;
    }

    bool Default::isNearRedILine(HSL & hsl) {
        return (hsl.h >= 10.0f && hsl.h <= 37.0f && hsl.s <= 0.82f);
    }

    bool Default::isAllowed(Colour & c) {
        HSL hsl = c.hsl();
        return (!this->isWhite(hsl) && !isBlack(hsl) && !isNearRedILine(hsl));
    }
};