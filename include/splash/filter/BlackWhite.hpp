#ifndef SPLASH_FILTER_BLACKWHITE_HPP
#define SPLASH_FILTER_BLACKWHITE_HPP

#include "splash/filter/Filter.hpp"

namespace Splash::Filter {
    // Inherits and extends filter to which allows colours that aren't
    // close to black or white
    class BlackWhite : public Filter {
        private:
            // Returns whether the colour is either white or black
            bool isWhiteOrBlack(Colour &);

        public:
            // Overrides to provide mentioned behaviour
            bool isAllowed(Colour &);
    };
};

#endif