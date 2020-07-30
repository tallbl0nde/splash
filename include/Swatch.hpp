#ifndef PALETTE_SWATCH_HPP
#define PALETTE_SWATCH_HPP

#include "Colour.hpp"
#include <string>

namespace Palette {
    // Represents a colour swatch generated from an image's palette.
    class Swatch {
        private:
            // Number of pixels that formed this swatch
            int population;

            // The original colour
            Colour colour;

            // Generated colours
            bool coloursGenerated;
            Colour titleTextColour;
            Colour bodyTextColour;

            // Generate colours above and set flag
            void generateColours();

        public:
            // Constructor takes colour and population
            Swatch(Colour, int);

            // Returns the swatch's colour
            Colour getColour();

            // Returns number of pixels represented by swatch
            int getPopulation();

            // Returns an appropriate colour to use for any title text
            // to display on top of the swatch's colour
            Colour getTitleTextColour();

            // Returns an appropriate colour to use for any body text
            // to display on top of the swatch's colour
            Colour getBodyTextColour();

            // Return the contents of the swatch as a string
            std::string toString();
    };
}

#endif