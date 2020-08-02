#include "Bitmap.hpp"
#include <cmath>

namespace Splash {
    static Colour COLOUR_WHITE = Colour(255, 255, 255, 255);

    Bitmap::Bitmap() {
        this->valid = false;
    }

    Bitmap::Bitmap(size_t w, size_t h) {
        // Set dimensions
        this->height = h;
        this->width = w;

        // Create 2D vector matching these dimensions
        // Fill with opaque white
        for (size_t y = 0; y < h; y++) {
            this->grid.push_back(std::vector<Colour>());
            for (size_t x = 0; x < w; x++) {
                this->grid[y].push_back(COLOUR_WHITE);
            }
        }

        this->valid = true;
    }

    bool Bitmap::isValid() const {
        return this->valid;
    }

    Colour Bitmap::getPixel(size_t x, size_t y) const {
        if (y < this->grid.size()) {
            if (x < this->grid[y].size()) {
                return this->grid[y][x];
            }
        }

        return COLOUR_WHITE;
    }

    std::vector<Colour> Bitmap::getPixels(size_t x, size_t y, size_t w, size_t h) const {
        std::vector<Colour> v;

        // Don't bother checking if requesting pixels outside of dimensions
        if (x > this->grid.size() || y > this->grid[0].size()) {
            return v;
        }

        // Ensure width and height are within bounds while checking
        h = (h > this->grid.size()-y ? this->grid.size()-y : h);
        for (size_t r = y; r < y+h; r++) {
            w = (w > this->grid[r].size()-x ? this->grid[r].size()-x : w);
            for (size_t c = x; c < x+w; c++) {
                v.push_back(this->grid[r][c]);
            }
        }

        return v;
    }

    void Bitmap::setPixel(Colour & c, size_t x, size_t y) {
        if (y < this->grid.size()) {
            if (x < this->grid[y].size()) {
                this->grid[y][x] = c;
            }
        }
    }

    size_t Bitmap::setPixels(std::vector<Colour> & cols, size_t x, size_t y, size_t w, size_t h) {
        size_t nextIdx = 0;
        for (size_t r = x; r < this->grid.size(); r++) {
            for (size_t c = y; c < this->grid[r].size(); c++) {
                // Stop if at the end of the vector
                if (nextIdx >= cols.size()) {
                    return nextIdx;
                }

                this->grid[r][c] = cols[nextIdx];
                nextIdx++;
            }
        }
        return nextIdx;
    }

    size_t Bitmap::getHeight() const {
        return this->height;
    }

    size_t Bitmap::getWidth() const {
        return this->width;
    }

    void Bitmap::setHeight(size_t h) {
        // Increase if new height is larger
        while (this->grid.size() < h) {
            std::vector<Colour> row;
            for (size_t x = 0; x < this->width; x++) {
                row.push_back(COLOUR_WHITE);
            }
            this->grid.push_back(row);
        }

        // Decrease if new height is smaller
        while (this->grid.size() > h) {
            this->grid.erase(this->grid.end() - 1);
        }

        this->height = h;
    }

    void Bitmap::setWidth(size_t w) {
        // Increase if new width is larger
        if (this->width < w) {
            for (size_t r = 0; r < this->grid.size(); r++) {
                while (this->grid[r].size() < w) {
                    this->grid[r].push_back(COLOUR_WHITE);
                }
            }

        // Decrease if new width is smaller
        } else if (this->width > w) {
            for (size_t r = 0; r < this->grid.size(); r++) {
                while (this->grid[r].size() > w) {
                    this->grid[r].erase(this->grid[r].end() - 1);
                }
            }
        }

        this->width = w;
    }

    Bitmap Bitmap::createScaledBitmap(size_t nw, size_t nh) const {
        // Create new bitmap
        Bitmap b = Bitmap(nw, nh);

        // Use nearest neighbour to populate vector with colours
        std::vector<Colour> pixels;
        double px, py;
        double xr = this->width/(double)b.getWidth();
        double yr = this->height/(double)b.getHeight();
        for (size_t y = 0; y < b.getHeight(); y++) {
            for (size_t x = 0; x < b.getWidth(); x++) {
                px = std::floor((double)x*xr);
                py = std::floor((double)y*yr);
                Colour c = this->grid[py][px];
                pixels.push_back(Colour(c.a(), c.r(), c.g(), c.b()));
            }
        }

        // Set bitmap pixels and return
        b.setPixels(pixels, 0, 0, b.getWidth(), b.getHeight());
        return b;
    }
};