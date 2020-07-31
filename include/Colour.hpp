#ifndef PALETTE_COLOUR_HPP
#define PALETTE_COLOUR_HPP

#include <string>

namespace Palette {
    // Struct containing HSL values (returned by class)
    struct HSL {
        float h;    // Hue [0, 360]
        float s;    // Saturation [0, 1]
        float l;    // Lightness [0, 1]
    };

    // A colour is a simple object which stores an ARGB
    // value as an integer, but is set/accessed through
    // member functions which operate on the integer.
    // For those who are familiar with Android's ColorInt,
    // this class essentially provides the same thing.
    class Colour {
        private:
            // ARGB colour stored as 0xAARRGGBB
            int value;

        public:
            // Default constructor initializes to transpatrent black (0)
            Colour();

            // Constructor takes ARGB values (cutoff if outside of 0-255)
            Colour(int, int, int, int);

            // Returns appropriate component
            int a() const;
            int r() const;
            int g() const;
            int b() const;

            // Returns raw value
            int raw() const;

            // Converts value to HSL
            HSL hsl() const;

            // Sets appropriate component
            void setA(int);
            void setR(int);
            void setG(int);
            void setB(int);

            // Set raw value
            void setRaw(int);

            // Print a string describing the contents
            std::string toString();
    };
};

#endif