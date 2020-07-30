#ifndef PALETTE_BITMAP_HPP
#define PALETTE_BITMAP_HPP

#include "Colour.hpp"
#include <cstddef>
#include <vector>

namespace Palette {
    // A bitmap in this context is essentially a 2D array (using vectors)
    // of Colour objects which represents the pixels of an image
    class Bitmap {
        private:
            // Actual 2D vector forming grid
            std::vector< std::vector<Colour> > grid;

            // Dimensions
            size_t height;
            size_t width;

        public:
            // Create a bitmap with the given dimensions
            Bitmap(size_t, size_t);

            // Return an individual pixel in the bitmap
            // Returns white with alpha 255 if outside of boundaries
            Colour getPixel(size_t, size_t);

            // Return a 1D vector containing the pixels in the given region
            // Ignores any attempts to read outside of bounds (i.e. skips to next row/column)
            std::vector<Colour> getPixels(size_t, size_t, size_t, size_t);

            // Set an individual pixel in the bitmap
            // Does nothing if outside bounds
            void setPixel(Colour &, size_t, size_t);

            // Set the pixels in the given region using the provided vector
            // If not enough pixels (colours) are provided the remaining pixels
            // won't be altered
            // Returns number of pixels used from source
            size_t setPixels(std::vector<Colour> &, size_t, size_t, size_t, size_t);

            // Return width/height of bitmap
            size_t getHeight();
            size_t getWidth();

            // Set width/height of bitmap
            // If size is increased, new pixels are set to white
            void setHeight(size_t);
            void setWidth(size_t);

            // Return a scaled version of this bitmap using nearest neighbour interpolation
            // Given desired width and height
            Bitmap createScaledBitmap(size_t, size_t);
    };
};

#endif