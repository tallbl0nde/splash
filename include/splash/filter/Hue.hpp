#ifndef SPLASH_FILTER_HUE_HPP
#define SPLASH_FILTER_HUE_HPP

#include "splash/filter/Filter.hpp"

namespace Splash::Filter {
    // This filter is actually a nested class of MediaStyle
    // Allows any colours that have a hue difference of at least 10 degrees
    // compared to the MediaStyle background
    class Hue : public Filter {
        private:
            // Hue valued copied when constructed
            double hue;

        public:
            // Constructor accepts hue value
            Hue(double);

            // Overrides to provide mentioned behaviour
            bool isAllowed(Colour &);
    };
};

#endif