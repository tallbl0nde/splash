#ifndef PALETTE_PALETTE_HPP
#define PALETTE_PALETTE_HPP

#include "Bitmap.hpp"
#include "Filter.hpp"
#include "Swatch.hpp"
#include "target/Target.hpp"
#include <memory>
#include <unordered_map>

// boost::hash_combine
template <class T>
inline void hash_combine(size_t & seed, const T & v) {
    seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

// Hashing function for a Target object
namespace std {
    template<> struct hash<::Palette::Target::Target> {
        const size_t operator()(const ::Palette::Target::Target & t) const {
            size_t val = 0;
            hash_combine(val, t.getMinimumLightness());
            hash_combine(val, t.getTargetLightness());
            hash_combine(val, t.getMaximumLightness());
            hash_combine(val, t.getMinimumSaturation());
            hash_combine(val, t.getTargetSaturation());
            hash_combine(val, t.getMaximumSaturation());
            hash_combine(val, t.getLightnessWeight());
            hash_combine(val, t.getSaturationWeight());
            hash_combine(val, t.getPopulationWeight());
            hash_combine(val, t.isExclusive());
            return val;
        }
    };
};

namespace Palette {
    // Extracts prominent colours from an image. A number of colours with different profiles
    // are extracted:
    // - Vibrant                - Muted
    // - Vibrant Dark           - Muted Dark
    // - Vibrant Light          - Muted Light
    // Each one can get retrieved by a getter method.
    // Note that creation is done via a Builder instance.
    class Palette {
        private:
            // Array of swatches in palette
            std::vector<Swatch> swatches;
            // Array of targets in palette
            std::vector<Target::Target> targets;

            // Mapping from target to chosen swatch for target
            std::unordered_map<Target::Target, Swatch> selectedSwatches;
            // Mapping from colour (using raw int) to boolean indicating if used
            std::unordered_map<int, bool> usedColours;

            // Dominant swatch in palette
            Swatch dominantSwatch;

            Swatch findDominantSwatch();
            float generateScore(const Swatch &, const Target::Target &);
            Swatch generateScoredTarget(const Target::Target &);
            Swatch getMaxScoredSwatchForTarget(const Target::Target &);
            bool shouldBeScoredForTarget(const Swatch &, const Target::Target &);

            // Actually generates the palette (invoked from Builder)
            void generate();

            // Constructor takes list of swatches and targets which form the palette
            Palette(const std::vector<Swatch> &, const std::vector<Target::Target> &);

            // Copy constructors
            Palette(const Palette &);
            Palette(const Palette *);

        public:
            // Returns all the swatches that form the palette
            std::vector<Swatch> getSwatches();

            // Returns the targets used to generate the palette
            std::vector<Target::Target> getTargets();

            // Returns swatch generated matching the associated colour profile
            // If there is no swatch the returned swatch will be marked invalid
            Swatch getVibrantSwatch();
            Swatch getLightVibrantSwatch();
            Swatch getDarkVibrantSwatch();
            Swatch getMutedSwatch();
            Swatch getLightMutedSwatch();
            Swatch getDarkMutedSwatch();

            // Return Swatch generated for matching target
            // If there is no swatch the returned swatch will be marked invalid
            Swatch getSwatchForTarget(const Target::Target &);

            // Return the dominant swatch (swatch with greatest population)
            // If there is no swatch the returned swatch will be marked invalid
            Swatch getDominantSwatch();

            // Return Colour generated matching the associated colour profile
            // Returns passed colour if no colour as generated
            Colour getVibrantColour(const Colour &);
            Colour getLightVibrantColour(const Colour &);
            Colour getDarkVibrantColour(const Colour &);
            Colour getMutedColour(const Colour &);
            Colour getLightMutedColour(const Colour &);
            Colour getDarkMutedColour(const Colour &);

            // Return Colour generated for matching target
            // Returns passed colour if no colour as generated
            Colour getColourForTarget(const Target::Target &, const Colour &);

            // Return the dominant colour (colour with greatest population)
            // Returns passed colour if no colour as generated
            Colour getDominantColour(const Colour &);

            // Builder class for generating Palette instances
            class Builder {
                private:
                    // Struct representing a region of pixels
                    struct Region {
                        size_t x1;
                        size_t y1;
                        size_t x2;
                        size_t y2;
                    };

                    // List of filters
                    std::vector<Filter *> filters;

                    // Vectors to eventually pass to Palette constructor
                    std::vector<Swatch> swatches;
                    std::vector<Target::Target> targets;

                    // Bitmap used to generate swatches
                    Bitmap bitmap;
                    // Region of bitmap to use for Palette generation
                    Region region;
                    // Variables for bitmap manipulation
                    size_t maxColours;
                    size_t resizeArea;

                    std::vector<Colour> getPixelsFromBitmap(const Bitmap &);
                    Bitmap scaleBitmapDown(const Bitmap &);

                public:
                    // Construct a Builder using a Bitmap
                    Builder(const Bitmap &);

                    // Construct a Builder using a vector of Swatches
                    Builder(const std::vector<Swatch> &);

                    // Set the maximum number of colours to use in the quantization step
                    // when using a Bitmap as the source
                    // For landscapes, 10-16 is a good range
                    // For faces, 24 is a good value
                    // Default is 16 colours
                    Builder & setMaximumColourCount(const size_t);

                    // Set the resize area when using a Bitmap as the source
                    // If the Bitmap's area is greater than the specified value,
                    // it will be resized so it's area matches this value.
                    // This value greatly impacts processing time. The larger the resized image,
                    // the greater the palette generation will take. However, more detail will
                    // be preserved.
                    Builder & resizeBitmapArea(const size_t);

                    // Clear all added filters (including the default ones)
                    Builder & clearFilters();

                    // Add a filter to control which colours are allowed in the
                    // resulting palette. Note that this takes a pointer, delete
                    // WILL be called on the object when the Builder is destroyed
                    Builder & addFilter(Filter *);

                    // Set a region of the Bitmap to be used exclusively when
                    // constructing the palette
                    // Parameters: left x, top y, right x, bottom y
                    Builder & setRegion(const size_t, const size_t, const size_t, const size_t);

                    // Clear any previously set region
                    Builder & clearRegion();

                    // Add a target to be gnerated in the palette
                    // The result can be retrieved using getSwatchForTarget(Target)
                    Builder & addTarget(const Target::Target &);

                    // Clear all added targets (including the default ones)
                    Builder & clearTargets();

                    // Generate and return the generated Palette
                    // This is slow - so preferably use a separate thread!
                    // The returned pointer must be deleted!
                    std::shared_ptr<Palette> generate();

                    // Destructor deletes any added filters
                    ~Builder();
            };

            // The actual method used to create a Palette from a bitmap
            // Returns builder object which can be used to customize generation
            static Builder from(const Bitmap &);
    };
};

#endif