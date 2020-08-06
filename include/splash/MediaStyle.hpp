#ifndef SPLASH_MEDIASTYLE_HPP
#define SPLASH_MEDIASTYLE_HPP

#include "splash/Palette.hpp"

namespace Splash {
    // Uses the Palette API to generate appropriate colours to show for an album cover
    class MediaStyle {
        private:
            // Copy of image to use
            Bitmap image;

            // Generated colours
            bool emptyHSL;
            HSL filteredBackgroundHSL;
            Colour backgroundColour;
            Colour secondaryTextColour;
            Colour primaryTextColour;

            // Select colours
            void ensureColours(Colour &, Colour &);
            // Generate the palette to extract colours from
            void generatePalette();

            // Find and return a fitting background colour
            Colour findBackgroundColour();

            // Returns whether the given swatch makes up enough of the image
            static bool hasEnoughPopulation(Swatch &);
            // Returns whether the provided colour is light
            static bool isColourLight(Colour &);

            // Choose a foreground colour using the background colour and palette
            Colour selectForegroundColour(Colour &, std::shared_ptr<Palette>);
            // Choose a background colour using the provided swatches
            // Returns the provided colour if no swatch matches
            Colour selectForegroundColourForSwatches(Swatch, Swatch, Swatch, Swatch, Swatch, Colour &);

            // Return a Swatch that qualifies as muted (may not be valid!)
            Swatch selectMutedCandidate(Swatch &, Swatch &);
            // Return a Swatch that qualifies as vibrant (may not be valid!)
            Swatch selectVibrantCandidate(Swatch &, Swatch &);

        public:
            // Constructor generates palette (may want to use another thread)
            MediaStyle(Bitmap &);

            // Returns derived colours
            Colour getBackgroundColour();
            Colour getPrimaryTextColour();
            Colour getSecondaryTextColour();

            // Returns whether the background colour is light
            bool isLight();
    };
};

#endif