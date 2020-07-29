#ifndef PALETTE_COLOURUTILS_HPP
#define PALETTE_COLOURUTILS_HPP

#include "Colour.hpp"

namespace Palette::ColourUtils {
    // Struct representing colour value in CIE XYZ
    struct XYZ {
        double x;
        double y;
        double z;
    };

    // Returns the contrast ratio between foreground (first arg) and background (second arg)
    // (background must be opaque)
    double calculateContrast(Colour &, Colour &);

    // Returns the minimum alpha value which can be applied to foreground (first argument) so
    // that it would have a minimum contrast value of at least ratio (third argument) when
    // compared to background (second argument)
    int calculateMinimumAlpha(Colour &, Colour &, float);

    // Composite two potentially translucent colours over each other and returns the result
    Colour compositeColours(Colour &, Colour &);

    // Composite alpha value and return
    int compositeAlpha(int, int);

    // Composite given component with alpha
    int compositeComponent(int, int, int, int, int);

    // Calculate luminance of given colour
    double calculateLuminance(Colour &);

    // Convert given colour to XYZ
    XYZ colourToXYZ(Colour &);
};

#endif