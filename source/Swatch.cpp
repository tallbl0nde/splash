#include "ColourUtils.hpp"
#include "Swatch.hpp"
#include <algorithm>

// Constants
#define MIN_CONTRAST_BODY_TEXT 4.5f
#define MIN_CONTRAST_TITLE_TEXT 3.0f

// Helper function to convert integer to hexadecimal string
static std::string intToHex(unsigned int i) {
    std::string str = "";

    // Repeat until number is zero
    while (i != 0) {
        int r = i%16;
        if (r < 10) {
            str.insert(str.begin(), '0'+r);
        } else {
            str.insert(str.begin(), 'a'+r%10);
        }
        i /= 16;
    }

    // If less than two chars pad with zero
    while (str.length() < 8) {
        str.insert(str.begin(), '0');
    }

    return str;
}

namespace Splash {
    // Colours
    static Colour COLOUR_BLACK = Colour(255, 0, 0, 0);
    static Colour COLOUR_WHITE = Colour(255, 255, 255, 255);

    void Swatch::generateColours() {
        if (!this->coloursGenerated) {
            this->coloursGenerated = true;

            // Check light colours first
            int lightBodyAlpha = ColourUtils::calculateMinimumAlpha(COLOUR_WHITE, this->colour, MIN_CONTRAST_BODY_TEXT);
            int lightTitleAlpha = ColourUtils::calculateMinimumAlpha(COLOUR_WHITE, this->colour, MIN_CONTRAST_TITLE_TEXT);

            // If there are valid light values, use those
            if (lightBodyAlpha != -1 && lightTitleAlpha != -1) {
                this->bodyTextColour = COLOUR_WHITE;
                this->bodyTextColour.setA(lightBodyAlpha);
                this->titleTextColour = COLOUR_WHITE;
                this->titleTextColour.setA(lightTitleAlpha);
                return;
            }

            // Check dark colours next
            int darkBodyAlpha = ColourUtils::calculateMinimumAlpha(COLOUR_BLACK, this->colour, MIN_CONTRAST_BODY_TEXT);
            int darkTitleAlpha = ColourUtils::calculateMinimumAlpha(COLOUR_BLACK, this->colour, MIN_CONTRAST_TITLE_TEXT);

            // If there are valid dark values, use those
            if (darkBodyAlpha != -1 && darkTitleAlpha != -1) {
                this->bodyTextColour = COLOUR_BLACK;
                this->bodyTextColour.setA(darkBodyAlpha);
                this->titleTextColour = COLOUR_BLACK;
                this->titleTextColour.setA(darkTitleAlpha);
                return;
            }

            // Otherwise use mismatched values
            if (lightBodyAlpha != -1) {
                this->bodyTextColour = COLOUR_WHITE;
                this->bodyTextColour.setA(lightBodyAlpha);
            } else {
                this->titleTextColour = COLOUR_BLACK;
                this->titleTextColour.setA(darkBodyAlpha);
            }
            if (lightTitleAlpha != -1) {
                this->bodyTextColour = COLOUR_WHITE;
                this->bodyTextColour.setA(lightTitleAlpha);
            } else {
                this->titleTextColour = COLOUR_BLACK;
                this->titleTextColour.setA(darkTitleAlpha);
            }
        }
    }

    Swatch::Swatch() {
        this->valid = false;
    }

    Swatch::Swatch(Colour c, int pop) {
        this->colour = c;
        this->colour.setA(255);
        this->population = pop;
        this->coloursGenerated = false;
        this->valid = true;
    }

    bool Swatch::isValid() const {
        return this->valid;
    }

    Colour Swatch::getColour() const {
        return this->colour;
    }

    int Swatch::getPopulation() const {
        return this->population;
    }

    Colour Swatch::getTitleTextColour() {
        this->generateColours();
        return this->titleTextColour;
    }

    Colour Swatch::getBodyTextColour() {
        this->generateColours();
        return this->bodyTextColour;
    }

    std::string Swatch::toString() const {
        HSL hsl = this->colour.hsl();
        std::string str = "[RGB: #" + intToHex(this->colour.raw()) + "] ";
        str += "[HSL: " + std::to_string(hsl.h) + ", " + std::to_string(hsl.s) + ", " + std::to_string(hsl.l) + "] ";
        str += "[Population: " + std::to_string(this->population) + "] ";
        str += "[Title Text: #" + intToHex(this->titleTextColour.raw()) + "] ";
        str += "[Body Text: #" + intToHex(this->bodyTextColour.raw()) + "]";
        return str;
    }
};