#ifndef SPLASH_BLACKWHITEFILTER_HPP
#define SPLASH_BLACKWHITEFILTER_HPP

#include "Filter.hpp"

namespace Splash {
    // Inherits and extends filter to which allows colours that aren't
    // close to black or white
    class BlackWhiteFilter : public Filter {
        private:
            // Returns whether the colour is either white or black
            bool isWhiteOrBlack(Colour &);

        public:
            // Overrides to provide mentioned behaviour
            bool isAllowed(Colour &);
    };
};

#endif