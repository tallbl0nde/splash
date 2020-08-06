#include "splash/filter/BlackWhite.hpp"

// Constants
#define BLACK_MAX_LIGHTNESS 0.08f
#define WHITE_MIN_LIGHTNESS 0.90f

namespace Splash::Filter {
    bool BlackWhite::isWhiteOrBlack(Colour & col) {
        float l = col.hsl().l;
        return (l <= BLACK_MAX_LIGHTNESS || l >= WHITE_MIN_LIGHTNESS);
    }

    bool BlackWhite::isAllowed(Colour & col) {
        return !this->isWhiteOrBlack(col);
    }
};