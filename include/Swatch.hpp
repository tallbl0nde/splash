#ifndef SPLASH_SWATCH_HPP
#define SPLASH_SWATCH_HPP

#include "Colour.hpp"
#include <string>

namespace Splash {
    // Represents a colour swatch generated from an image's palette.
    class Swatch {
        private:
            // Is this swatch valid?
            bool valid;

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
            // Creates an invalid swatch
            Swatch();

            // Constructor takes colour and population
            Swatch(Colour, int);

            // Returns if the swatch is valid
            bool isValid() const;

            // Returns the swatch's colour
            Colour getColour() const;

            // Returns number of pixels represented by swatch
            int getPopulation() const;

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