#ifndef SPLASH_COLOURCUTQUANTIZER_HPP
#define SPLASH_COLOURCUTQUANTIZER_HPP

#include "Filter.hpp"
#include "Swatch.hpp"
#include <queue>
#include <vector>

namespace Splash {
    // Dimension/component of RGB int to operate on
    enum class Dimension {
        Red,
        Green,
        Blue
    };

    // Colour Quantizer based on the median-cut algorithm, but optimized for picking out
    // distinct colours in stead of representation colours.
    // The colour space is represented as a 3D cube with each dimension being an RGB
    // component. This cube is repeatedly divided until it is reduced to the requested
    // number of colours.
    class ColourCutQuantizer {
        private:
            // Represents a tightly fitting box around a colour space
            class Vbox {
                private:
                    // Reference to ColourCutQuantizer that this Vbox is a member of
                    // (needed so it can access members)
                    ColourCutQuantizer * ccq;

                    // Lower and upper indexes are inclusive
                    size_t lowerIndex;
                    size_t upperIndex;

                    // Population (number) of colours within this box
                    int population;

                    // Min and max values
                    int minR, minG, minB;
                    int maxR, maxG, maxB;

                    // Recompute box boundaries to tightly fit the colours within the box
                    void fitBox();

                public:
                    // Constructor takes lower and upper indexes and fits the box
                    // Unlike the Java version it also takes a pointer to the ColourCutQuantizer object
                    // it is a part of
                    Vbox(ColourCutQuantizer *, size_t, size_t);

                    // Split this box at the midpoint along it's longest dimension
                    Vbox splitBox();

                    // Return the dimension which this box is largest in
                    Dimension getLongestColourDimension() const;

                    // Finds the point within the box's indexes of where to split
                    // Returns the index in the CCQ's colours vector to split from
                    size_t findSplitPoint();

                    // Returns the average colour of this box
                    Swatch getAverageColour() const;

                    // Modify the significant octet in a colour. Allows sorting based on the
                    // value of a single colour component
                    static void modifySignificantOctet(std::vector<int> &, Dimension, size_t, size_t);

                    // Return characteristics of box
                    int getVolume() const;
                    bool canSplit() const;
                    int getColourCount() const;
            };

            // Comparator for Vboxes
            static bool VBOX_COMP(Vbox &, Vbox &);

            // Vector of colours (i.e. pixels) stored as raw integers
            std::vector<int> colours;
            // Histogram of quantized colour frequency
            std::vector<int> histogram;

            // Pointers to passed filters (not deleted!)
            std::vector<Filter *> filters;
            // Quantized colours stored as Swatches
            std::vector<Swatch> quantizedColours;

            // Quantizes stored pixels to given number of colours
            std::vector<Swatch> quantizePixels(int);

            // Iterate through the given priority queue and split Vboxes until
            // the queue contains the given number of Vboxes
            void splitBoxes(std::priority_queue<Vbox, std::vector<Vbox>, decltype(&VBOX_COMP)> &, int);

            // Return the average colours in the given vector of Vboxes
            std::vector<Swatch> generateAverageColours(std::vector<Vbox> &);

            // Returns if the colour should be ignored
            bool shouldIgnoreColour565(int);
            bool shouldIgnoreColour888(int);

            // Quantize an RGB888 value
            static int quantizeFromRGB888(int);

            // Convert RGB565 values to RGB888
            static int approximateToRGB888(int, int, int);
            static int approximateToRGB888(int);

            // Modify the word with used for the given dimension
            static int modifyWordWidth(int, int, int);

        public:
            // Constructor takes pixels (vector of colours), maximum number of colours in resulting
            // palette and a vector of filters to use for quantization
            ColourCutQuantizer(std::vector<Colour> &, int, std::vector<Filter *> &);

            // Returns vector of quantized colours as Swatches
            std::vector<Swatch> getQuantizedColours();

            // Quantize the given component (given RGB565 int)
            static int quantizedComponent(int, Dimension);
    };
};

#endif