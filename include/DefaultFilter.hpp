#ifndef PALETTE_DEFAULTFILTER_HPP
#define PALETTE_DEFAULTFILTER_HPP

#include "Filter.hpp"

namespace Palette {
    // Inherits and extends filter to form the default filter.
    // Allows any colour that isn't black, white or near red side of I line.
    class DefaultFilter : public Filter {
        private:
            // Values used to determine black/white boundaries
            static const float BLACK_MAX_LIGHTNESS;
            static const float WHITE_MIN_LIGHTNESS;

            // Returns true if the colour is close to black
            bool isBlack(HSL &);
            // Returns true if the colour is close to white
            bool isWhite(HSL &);
            // Returns true if the colour is near the red side of the I line
            bool isNearRedILine(HSL &);

        public:
            // Overrides to provide mentioned checks
            bool isAllowed(Colour &);
    };
};

#endif