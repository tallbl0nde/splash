#ifndef SPLASH_COLOURUTILS_HPP
#define SPLASH_COLOURUTILS_HPP

#include "splash/Colour.hpp"

namespace Splash::ColourUtils {
    // Struct representing colour value in LAB
    struct LAB {
        double l;
        double a;
        double b;
    };

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

    // Change the given colour by the specified value
    Colour changeColourLightness(Colour &, int);

    // Composite two potentially translucent colours over each other and returns the result
    Colour compositeColours(Colour &, Colour &);

    // Composite alpha value and return
    int compositeAlpha(int, int);

    // Composite given component with alpha
    int compositeComponent(int, int, int, int, int);

    // Calculate luminance of given colour
    double calculateLuminance(Colour &);

    // Returns a suitable colour given the supplied contrast ratio
    Colour findContrastColour(Colour &, Colour &, bool, double);
    Colour findContrastColourAgainstDark(Colour &, Colour &, bool, double);

    double pivotXyzComponent(double);

    // Returns whether the second colour is a sufficient text colour
    // for to show on the first colour
    bool satisfiesTextContrast(Colour &, Colour &);

    // Methods to convert between colour spaces
    Colour HSLToColour(HSL &);
    Colour LABToColour(LAB &);
    Colour XYZToColour(XYZ &);
    LAB colourToLAB(Colour &);
    LAB XYZToLAB(XYZ);
    XYZ colourToXYZ(Colour &);
    XYZ LABToXYZ(LAB &);
};

#endif