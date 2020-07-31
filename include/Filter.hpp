#ifndef PALETTE_FILTER_HPP
#define PALETTE_FILTER_HPP

#include "Colour.hpp"

namespace Palette {
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