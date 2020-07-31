#ifndef SPLASH_FILTER_HPP
#define SPLASH_FILTER_HPP

#include "Colour.hpp"

namespace Splash {
    // A filter provides a mechanism for controlling which colours are valid
    // within
    class Filter {
        public:
            // Inherited to form new filters which describe if a colour is allowed
            // Returns true if allowed, false if not
            virtual bool isAllowed(Colour &) = 0;

            virtual ~Filter();
    };
};

#endif