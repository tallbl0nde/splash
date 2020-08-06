#include "splash/ColourUtils.hpp"
#include "splash/filter/BlackWhite.hpp"
#include "splash/filter/Hue.hpp"
#include "splash/MediaStyle.hpp"
#include <cmath>

// Constants
#define LIGHTNESS_TEXT_DIFFERENCE_DARK -10
#define LIGHTNESS_TEXT_DIFFERENCE_LIGHT 20
#define POPULATION_FRACTION_FOR_WHITE_OR_BLACK 2.5f
#define POPULATION_FRACTION_FOR_DOMINANT 0.01f
#define POPULATION_FRACTION_FOR_MORE_VIBRANT 1.0f
#define MINIMUM_IMAGE_FRACTION 0.002f
#define MIN_SATURATION_WHEN_DECIDING 0.19f
#define RESIZE_BITMAP_AREA (150 * 150)
#define BLACK_MAX_LIGHTNESS 0.08f
#define WHITE_MIN_LIGHTNESS 0.90f

namespace Splash {
    // Static colours
    static Colour COLOUR_BLACK = Colour(255, 0, 0, 0);
    static Colour COLOUR_WHITE = Colour(255, 255, 255, 255);

    // Helper function to check if colour is black or white
    bool isWhiteOrBlack(Colour & col) {
        HSL hsl = col.hsl();
        return (hsl.l <= BLACK_MAX_LIGHTNESS || hsl.l >= WHITE_MIN_LIGHTNESS);
    }

    MediaStyle::MediaStyle(Bitmap & bmap) {
        this->emptyHSL = true;
        this->image = bmap;
        this->generatePalette();
    }

    void MediaStyle::ensureColours(Colour & bg, Colour & fg) {
        double backLum = ColourUtils::calculateLuminance(bg);
        double textLum = ColourUtils::calculateLuminance(fg);
        double contrast = ColourUtils::calculateContrast(fg, bg);

        bool bgLight = (((backLum > textLum) && ColourUtils::satisfiesTextContrast(bg, COLOUR_BLACK)) || ((backLum <= textLum) && !ColourUtils::satisfiesTextContrast(bg, COLOUR_WHITE)));
        if (contrast < 4.5f) {
            if (bgLight) {
                this->secondaryTextColour = ColourUtils::findContrastColour(fg, bg, true, 4.5f);
                this->primaryTextColour = ColourUtils::changeColourLightness(this->secondaryTextColour, -LIGHTNESS_TEXT_DIFFERENCE_LIGHT);

            } else {
                this->secondaryTextColour = ColourUtils::findContrastColourAgainstDark(fg, bg, true, 4.5f);
                this->primaryTextColour = ColourUtils::changeColourLightness(this->secondaryTextColour, -LIGHTNESS_TEXT_DIFFERENCE_DARK);
            }

        } else {
            this->primaryTextColour = fg;
            this->secondaryTextColour = ColourUtils::changeColourLightness(this->primaryTextColour, bgLight ? LIGHTNESS_TEXT_DIFFERENCE_LIGHT : LIGHTNESS_TEXT_DIFFERENCE_DARK);
            if (ColourUtils::calculateContrast(this->secondaryTextColour, bg) < 4.5f) {
                if (bgLight) {
                    this->secondaryTextColour = ColourUtils::findContrastColour(this->secondaryTextColour, bg, true, 4.5f);
                } else {
                    this->secondaryTextColour = ColourUtils::findContrastColourAgainstDark(this->secondaryTextColour, bg, true, 4.5f);
                }
                this->primaryTextColour = ColourUtils::changeColourLightness(this->secondaryTextColour, bgLight ? -LIGHTNESS_TEXT_DIFFERENCE_LIGHT : -LIGHTNESS_TEXT_DIFFERENCE_DARK);
            }
        }
    }

    void MediaStyle::generatePalette() {
        // Only do something if the bitmap is valid
        if (!this->image.isValid()) {
            return;
        }

        // Define some useful variables
        Colour fgColour;
        size_t height = this->image.getHeight();
        size_t width = this->image.getWidth();
        size_t area = width * height;

        // Resize the image if it is too large
        if (area > RESIZE_BITMAP_AREA) {
            double factor = std::sqrt(RESIZE_BITMAP_AREA/(float)area);
            width *= factor;
            height *= factor;
            this->image = this->image.createScaledBitmap(width, height);
        }

        // Generate palette from image
        Palette::Builder builder = Palette::from(this->image);
        builder.clearFilters();
        builder.resizeBitmapArea(RESIZE_BITMAP_AREA);
        std::shared_ptr<Palette> palette = builder.generate();

        // Get colours
        this->backgroundColour = this->findBackgroundColour();
        if (!this->emptyHSL) {
            Filter::Hue * f = new Filter::Hue(this->filteredBackgroundHSL.h);
            builder.addFilter(f);
        }
        Filter::BlackWhite * ff = new Filter::BlackWhite();
        builder.addFilter(ff);
        palette = builder.generate();
        fgColour = this->selectForegroundColour(this->backgroundColour, palette);
        this->ensureColours(this->backgroundColour, fgColour);
    }

    Colour MediaStyle::selectForegroundColour(Colour & bgColour, std::shared_ptr<Palette> p) {
        if (this->isColourLight(bgColour)) {
            return selectForegroundColourForSwatches(p->getDarkVibrantSwatch(), p->getVibrantSwatch(), p->getDarkMutedSwatch(), p->getMutedSwatch(), p->getDominantSwatch(), COLOUR_BLACK);
        } else {
            return selectForegroundColourForSwatches(p->getLightVibrantSwatch(), p->getVibrantSwatch(), p->getLightMutedSwatch(), p->getMutedSwatch(), p->getDominantSwatch(), COLOUR_WHITE);
        }
    }

    bool MediaStyle::isColourLight(Colour & col) {
        return (ColourUtils::calculateLuminance(col) > 0.5f);
    }

    bool MediaStyle::isLight() {
        return this->isColourLight(this->backgroundColour);
    }

    Colour MediaStyle::selectForegroundColourForSwatches(Swatch mVibrant, Swatch vibrant, Swatch mMuted, Swatch muted, Swatch dominant, Colour & fallback) {
        // Try to find a fitting vibrant or muted swatch
        Swatch colouredCandidate = this->selectVibrantCandidate(mVibrant, vibrant);
        if (!colouredCandidate.isValid()) {
            colouredCandidate = this->selectMutedCandidate(muted, mMuted);
        }

        // If a swatch if found determine which one to return
        if (colouredCandidate.isValid()) {
            if (dominant == colouredCandidate) {
                return colouredCandidate.getColour();

            } else if ((float) colouredCandidate.getPopulation()/dominant.getPopulation() < POPULATION_FRACTION_FOR_DOMINANT && dominant.getColour().hsl().s > MIN_SATURATION_WHEN_DECIDING) {
                return dominant.getColour();

            } else {
                return colouredCandidate.getColour();
            }

        // If no suitable swatch try dominant colour
        } else if (this->hasEnoughPopulation(dominant)) {
            return dominant.getColour();
        }

        // Otherwise return fallback colour
        return fallback;
    }

    Swatch MediaStyle::selectMutedCandidate(Swatch & first, Swatch & second) {
        bool firstValid = this->hasEnoughPopulation(first);
        bool secondValid = this->hasEnoughPopulation(second);

        // If both are valid find one to return
        if (firstValid && secondValid) {
            float firstS = first.getColour().hsl().s;
            float secondS = second.getColour().hsl().s;
            float popFraction = first.getPopulation() / (float)second.getPopulation();
            if (firstS * popFraction > secondS) {
                return first;
            } else {
                return second;
            }

        // Otherwise return valid one
        } else if (firstValid) {
            return first;

        } else if (secondValid) {
            return second;
        }

        // If neither are valid return an invalid swatch
        return Swatch();
    }

    Swatch MediaStyle::selectVibrantCandidate(Swatch & first, Swatch & second) {
        bool firstValid = this->hasEnoughPopulation(first);
        bool secondValid = this->hasEnoughPopulation(second);

        // If both are valid find one to return
        if (firstValid && secondValid) {
            float firstPop = first.getPopulation();
            float secondPop = second.getPopulation();
            if (firstPop / (float)secondPop < POPULATION_FRACTION_FOR_MORE_VIBRANT) {
                return second;
            } else {
                return first;
            }

        // Otherwise return valid one
        } else if (firstValid) {
            return first;

        } else if (secondValid) {
            return second;
        }

        // If neither are valid return an invalid swatch
        return Swatch();
    }

    bool MediaStyle::hasEnoughPopulation(Swatch & swatch) {
        return (swatch.isValid() && (swatch.getPopulation()/(float)RESIZE_BITMAP_AREA) > MINIMUM_IMAGE_FRACTION);
    }

    Colour MediaStyle::findBackgroundColour() {
        // Generate the palette
        Palette::Builder builder = Palette::from(this->image);
        builder.clearFilters();
        builder.resizeBitmapArea(RESIZE_BITMAP_AREA);
        std::shared_ptr<Palette> palette = builder.generate();

        // Check if we can use the dominant swatch
        Swatch dominant = palette->getDominantSwatch();
        if (!dominant.isValid()) {
            this->emptyHSL = true;
            return COLOUR_WHITE;
        }

        // If the dominant swatch is valid check if it is a colour
        Colour dom = dominant.getColour();
        if (!isWhiteOrBlack(dom)) {
            this->emptyHSL = false;
            this->filteredBackgroundHSL = dom.hsl();
            return dom;
        }

        // If not then it's black or white so check the second colour
        std::vector<Swatch> swatches = palette->getSwatches();
        float highestNonWhitePop = -1;
        Swatch second = Swatch();
        for (size_t i = 0; i < swatches.size(); i++) {
            // Don't check the dominant swatch again
            if (swatches[i] == dominant) {
                continue;
            }

            // If good enough mark it
            Colour col = swatches[i].getColour();
            if (swatches[i].getPopulation() > highestNonWhitePop && !isWhiteOrBlack(col)) {
                second = swatches[i];
                highestNonWhitePop = swatches[i].getPopulation();
            }
        }

        // If we don't have a second swatch just return the dominant one
        if (!second.isValid()) {
            this->emptyHSL = true;
            return dom;
        }

        // Otherwise check if it's worth picking the second one over the dominant swatch
        if (dominant.getPopulation()/highestNonWhitePop > POPULATION_FRACTION_FOR_WHITE_OR_BLACK) {
            this->emptyHSL = true;
            return dom;

        } else {
            this->filteredBackgroundHSL = second.getColour().hsl();
            return second.getColour();
        }
    }

    Colour MediaStyle::getPrimaryTextColour() {
        return this->primaryTextColour;
    }

    Colour MediaStyle::getSecondaryTextColour() {
        return this->secondaryTextColour;
    }

    Colour MediaStyle::getBackgroundColour() {
        return this->backgroundColour;
    }
};