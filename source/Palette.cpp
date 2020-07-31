#include "ColourCutQuantizer.hpp"
#include "DefaultFilter.hpp"
#include "Palette.hpp"
#include "target/DarkMuted.hpp"
#include "target/DarkVibrant.hpp"
#include "target/Muted.hpp"
#include "target/LightMuted.hpp"
#include "target/LightVibrant.hpp"
#include "target/Vibrant.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

namespace Palette {
    Palette::Palette(const std::vector<Swatch> & s, const std::vector<Target::Target> & t) {
        this->swatches = s;
        this->targets = t;

        this->selectedSwatches.clear();
        this->usedColours.clear();

        this->dominantSwatch = this->findDominantSwatch();
    }

    Palette::Palette(const Palette & p) {
        this->swatches = p.swatches;
        this->targets = p.targets;
        this->selectedSwatches = p.selectedSwatches;
        this->usedColours = p.usedColours;
        this->dominantSwatch = p.dominantSwatch;
    }

    Palette::Palette(const Palette * p) {
        this->swatches = p->swatches;
        this->targets = p->targets;
        this->selectedSwatches = p->selectedSwatches;
        this->usedColours = p->usedColours;
        this->dominantSwatch = p->dominantSwatch;
    }

    Swatch Palette::findDominantSwatch() {
        int maxPop = std::numeric_limits<int>::min();
        Swatch maxSwatch = Swatch();
        for (size_t i = 0; i < this->swatches.size(); i++) {
            Swatch swatch = this->swatches[i];
            if (swatch.getPopulation() > maxPop) {
                maxSwatch = swatch;
                maxPop = swatch.getPopulation();
            }
        }
        return maxSwatch;
    }

    float Palette::generateScore(const Swatch & swatch, const Target::Target & target) {
        HSL hsl = swatch.getColour().hsl();

        float sScore = 0;
        float lScore = 0;
        float popScore = 0;

        size_t maxPop = (this->dominantSwatch.isValid() ? this->dominantSwatch.getPopulation() : 1);

        if (target.getSaturationWeight() > 0) {
            sScore = target.getSaturationWeight() * (1.0f - std::abs(hsl.s - target.getTargetSaturation()));
        }
        if (target.getLightnessWeight() > 0) {
            lScore = target.getLightnessWeight() * (1.0f - std::abs(hsl.l - target.getTargetLightness()));
        }
        if (target.getPopulationWeight() > 0) {
            popScore = target.getPopulationWeight() * (swatch.getPopulation() / (float)maxPop);
        }

        return sScore + lScore + popScore;
    }

    Swatch Palette::generateScoredTarget(const Target::Target & target) {
        Swatch maxScoreSwatch = this->getMaxScoredSwatchForTarget(target);
        if (maxScoreSwatch.isValid() && target.isExclusive()) {
            this->usedColours[maxScoreSwatch.getColour().raw()] = true;
        }
        return maxScoreSwatch;
    }

    Swatch Palette::getMaxScoredSwatchForTarget(const Target::Target & target) {
        float maxScore = 0;
        Swatch maxScoreSwatch = Swatch();

        for (size_t i = 0; i < this->swatches.size(); i++) {
            Swatch swatch = this->swatches[i];
            if (this->shouldBeScoredForTarget(swatch, target)) {
                float score = generateScore(swatch, target);
                if (!maxScoreSwatch.isValid() || score > maxScore) {
                    maxScoreSwatch = swatch;
                    maxScore = score;
                }
            }
        }

        return maxScoreSwatch;
    }

    bool Palette::shouldBeScoredForTarget(const Swatch & swatch, const Target::Target & target) {
        // Check colour hasn't been used already
        Colour col = swatch.getColour();
        if (this->usedColours.count(col.raw()) > 0) {
            return false;
        }

        // Check HSL within range
        HSL hsl = col.hsl();
        return (hsl.s >= target.getMinimumSaturation() && hsl.s <= target.getMaximumSaturation() && hsl.l >= target.getMinimumLightness() && hsl.l <= target.getMaximumLightness());
    }

    void Palette::generate() {
        // Make sure scored targets are generated first
        for (size_t i = 0; i < this->targets.size(); i++) {
            Target::Target target = this->targets[i];
            target.normalizeWeights();
            this->selectedSwatches[target] = this->generateScoredTarget(target);
        }

        // Now clear out used colours
        this->usedColours.clear();
    }

    std::vector<Swatch> Palette::getSwatches() {
        return this->swatches;
    }

    std::vector<Target::Target> Palette::getTargets() {
        return this->targets;
    }

    Swatch Palette::getVibrantSwatch() {
        return this->getSwatchForTarget(Target::VIBRANT);
    }

    Swatch Palette::getLightVibrantSwatch() {
        return this->getSwatchForTarget(Target::LIGHT_VIBRANT);
    }

    Swatch Palette::getDarkVibrantSwatch() {
        return this->getSwatchForTarget(Target::DARK_VIBRANT);
    }

    Swatch Palette::getMutedSwatch() {
        return this->getSwatchForTarget(Target::MUTED);
    }

    Swatch Palette::getLightMutedSwatch() {
        return this->getSwatchForTarget(Target::LIGHT_MUTED);
    }

    Swatch Palette::getDarkMutedSwatch() {
        return this->getSwatchForTarget(Target::DARK_MUTED);
    }

    Swatch Palette::getSwatchForTarget(const Target::Target & t) {
        return this->selectedSwatches[t];
    }

    Swatch Palette::getDominantSwatch() {
        return this->dominantSwatch;
    }

    Colour Palette::getVibrantColour(const Colour & c) {
        return this->getColourForTarget(Target::VIBRANT, c);
    }

    Colour Palette::getLightVibrantColour(const Colour & c) {
        return this->getColourForTarget(Target::LIGHT_VIBRANT, c);
    }

    Colour Palette::getDarkVibrantColour(const Colour & c) {
        return this->getColourForTarget(Target::DARK_VIBRANT, c);
    }

    Colour Palette::getMutedColour(const Colour & c) {
        return this->getColourForTarget(Target::MUTED, c);
    }

    Colour Palette::getLightMutedColour(const Colour & c) {
        return this->getColourForTarget(Target::LIGHT_MUTED, c);
    }

    Colour Palette::getDarkMutedColour(const Colour & c) {
        return this->getColourForTarget(Target::DARK_MUTED, c);
    }

    Colour Palette::getColourForTarget(const Target::Target & t, const Colour & c) {
        Swatch swatch = this->getSwatchForTarget(t);
        return (swatch.isValid() ? swatch.getColour() : c);
    }

    Colour Palette::getDominantColour(const Colour & c) {
        return (this->dominantSwatch.isValid() ? this->dominantSwatch.getColour() : c);
    }

    Palette::Builder::Builder(const Bitmap & b) {
        // Initialize members
        this->bitmap = b;
        this->filters.push_back(new DefaultFilter());
        this->region = Region{0, 0, b.getWidth(), b.getHeight()};
        this->swatches.clear();

        this->maxColours = 0;
        this->resizeArea = 0;

        // Add default targets
        this->targets.push_back(Target::VIBRANT);
        this->targets.push_back(Target::LIGHT_VIBRANT);
        this->targets.push_back(Target::DARK_VIBRANT);
        this->targets.push_back(Target::MUTED);
        this->targets.push_back(Target::LIGHT_MUTED);
        this->targets.push_back(Target::DARK_MUTED);
    }

    Palette::Builder::Builder(const std::vector<Swatch> & s) {
        this->filters.push_back(new DefaultFilter());
        this->swatches = s;
    }

    std::vector<Colour> Palette::Builder::getPixelsFromBitmap(const Bitmap & bitmap) {
        return bitmap.getPixels(this->region.x1, this->region.y1, this->region.x2 - this->region.x1, this->region.y2 - this->region.y1);
    }

    Bitmap Palette::Builder::scaleBitmapDown(const Bitmap & oldB) {
        double scaleRatio = -1;

        // If resizeArea is set
        if (this->resizeArea > 0) {
            size_t bitmapArea = oldB.getWidth() * oldB.getHeight();
            if (bitmapArea > this->resizeArea) {
                scaleRatio = std::sqrt(this->resizeArea/(double)bitmapArea);
            }
        }

        // Return same bitmap if scaling isn't needed
        if (scaleRatio <= 0) {
            return oldB;
        }

        // Otherwise return scaled bitmap
        return oldB.createScaledBitmap(std::ceil(oldB.getWidth() * scaleRatio), std::ceil(oldB.getHeight() * scaleRatio));
    }

    Palette::Builder & Palette::Builder::setMaximumColourCount(const size_t count) {
        this->maxColours = count;
        return *this;
    }

    Palette::Builder & Palette::Builder::resizeBitmapArea(const size_t area) {
        this->resizeArea = area;
        return *this;
    }

    Palette::Builder & Palette::Builder::clearFilters() {
        this->filters.clear();
        return *this;
    }

    Palette::Builder & Palette::Builder::addFilter(Filter * f) {
        if (f != nullptr) {
            this->filters.push_back(f);
        }
        return *this;
    }

    Palette::Builder & Palette::Builder::setRegion(const size_t l, const size_t t, const size_t r, const size_t b) {
        // Only set if using a bitmap
        if (this->swatches.empty()) {
            this->region = Region{l, t, r, b};
        }
        return *this;
    }

    Palette::Builder & Palette::Builder::clearRegion() {
        this->region = {0, 0, this->bitmap.getWidth(), this->bitmap.getHeight()};
        return *this;
    }

    Palette::Builder & Palette::Builder::addTarget(const Target::Target & t) {
        if (std::find(this->targets.begin(), this->targets.end(), t) == this->targets.end()) {
            this->targets.push_back(t);
        }
        return *this;
    }

    Palette::Builder & Palette::Builder::clearTargets() {
        this->targets.clear();
        return *this;
    }

    Palette Palette::Builder::generate() {
        std::vector<Swatch> sws;

        // If we have a bitmap use quantization to reduce the number of colours
        if (this->swatches.empty()) {
            // Scale bitmap down if needed
            Bitmap bmap = scaleBitmapDown(this->bitmap);

            // Scale down the region if the bitmap was scaled
            if (bmap.getWidth() != this->bitmap.getWidth() || bmap.getHeight() != this->bitmap.getHeight()) {
                double scale = bmap.getWidth() / (double)this->bitmap.getWidth();
                this->region.x1 = std::floor(this->region.x1 * scale);
                this->region.y1 = std::floor(this->region.y1 * scale);
                this->region.x2 = std::min((size_t)std::ceil(this->region.x2 * scale), bmap.getWidth());
                this->region.y2 = std::min((size_t)std::ceil(this->region.y2 * scale), bmap.getHeight());
            }

            // Now generate swatches
            std::vector<Colour> pixels = this->getPixelsFromBitmap(bmap);
            ColourCutQuantizer quantizer = ColourCutQuantizer(pixels, this->maxColours, this->filters);
            sws = quantizer.getQuantizedColours();

        // Otherwise use provided swatches
        } else {
            sws = this->swatches;
        }

        // Create Palette using swatches
        Palette p = Palette(sws, this->targets);
        p.generate();
        return p;
    }

    Palette::Builder::~Builder() {
        for (size_t i = 0; i < this->filters.size(); i++) {
            delete this->filters[i];
        }
    }
};