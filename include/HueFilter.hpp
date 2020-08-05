#ifndef SPLASH_HUEFILTER_HPP
#define SPLASH_HUEFILTER_HPP

#include "Filter.hpp"
#include "MediaStyle.hpp"

namespace Splash {
    // This filter is actually a nested class of MediaStyle
    // Allows any colours that have a hue difference of at least 10 degrees
    // compared to the MediaStyle background
    class MediaStyle::HueFilter : public Filter {
        private:
            // Pointer to style object
            MediaStyle * mp;

        public:
            // Constructor accepts pointer to MediaStyle object
            HueFilter(MediaStyle *);

            // Overrides to provide mentioned behaviour
            bool isAllowed(Colour &);
    };
};

#endif