#ifndef PALETTE_COLOUR_HPP
#define PALETTE_COLOUR_HPP

namespace Palette {
    // Struct containing HSL values (returned by class)
    struct HSL {
        float h;    // Hue [0, 360]
        float s;    // Saturation [0, 1]
        float l;    // Lightness [0, 1]
    };

    // A colour is a simple object which stores an RGB
    // value as an integer, but is set/accessed through
    // member functions which operate on the integer.
    // For those who are familiar with Android's ColorInt,
    // this class essentially provides the same thing.
    class Colour {
        private:
            // RGB colour stored as 0x00RRGGBB
            int value;

        public:
            // Constructor takes RGB values (cutoff if outside of 0-255)
            Colour(int r, int g, int b);

            // Returns appropriate component
            int r();
            int g();
            int b();

            // Returns raw RGB value
            int rgb();

            // Converts RGB value to HSL
            HSL hsl();

            // Sets appropriate component
            void setR(int r);
            void setG(int g);
            void setB(int b);

            // Set raw RGB value (doesn't check!)
            void setRGB(int rgb);
    };
};

#endif