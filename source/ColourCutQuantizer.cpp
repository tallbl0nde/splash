#include "ColourCutQuantizer.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

// Constants
#define QUANTIZE_WORD_WIDTH 5
#define QUANTIZE_WORD_MASK ((1 << QUANTIZE_WORD_WIDTH) - 1)

namespace Splash {
    ColourCutQuantizer::Vbox::Vbox(ColourCutQuantizer * q, size_t lower, size_t upper) {
        this->ccq = q;
        this->lowerIndex = lower;
        this->upperIndex = upper;
        this->fitBox();
    }

    void ColourCutQuantizer::Vbox::fitBox() {
        // Reset min and max to opposite limits
        this->minR = std::numeric_limits<int>::max();
        this->minG = this->minR;
        this->minB = this->minG;
        this->maxR = std::numeric_limits<int>::min();
        this->maxG = this->maxR;
        this->maxB = this->maxG;

        // Perform some magic
        this->population = 0;
        for (size_t idx = this->lowerIndex; idx <= this->upperIndex; idx++) {
            int col = this->ccq->colours[idx];
            this->population += this->ccq->histogram[col];

            // Get quantized values
            int r = quantizedComponent(col, Dimension::Red);
            int g = quantizedComponent(col, Dimension::Green);
            int b = quantizedComponent(col, Dimension::Blue);
            if (r > this->maxR) {
                this->maxR = r;
            }
            if (r < this->minR) {
                this->minR = r;
            }
            if (r > this->maxG) {
                this->maxG = g;
            }
            if (r < this->minG) {
                this->minG = g;
            }
            if (r > this->maxB) {
                this->maxB = b;
            }
            if (r < this->minB) {
                this->minB = b;
            }
        }
    }

    ColourCutQuantizer::Vbox ColourCutQuantizer::Vbox::splitBox() {
        // The Java library throws an exception here, we're going to assume it can be split
        size_t splitPoint = this->findSplitPoint();

        // Create new Vbox using upper half and adjust this box's index
        Vbox newBox = Vbox(this->ccq, splitPoint + 1, this->upperIndex);
        this->upperIndex = splitPoint;
        fitBox();

        return newBox;
    }

    Dimension ColourCutQuantizer::Vbox::getLongestColourDimension() {
        int redL = this->maxR - this->minR;
        int grnL = this->maxG - this->minG;
        int bluL = this->maxB - this->minB;

        if (redL >= grnL && redL >= bluL) {
            return Dimension::Red;

        } else if (grnL >= redL && grnL >= bluL) {
            return Dimension::Green;

        } else {
            return Dimension::Blue;
        }
    }

    size_t ColourCutQuantizer::Vbox::findSplitPoint() {
        // Get longest dimension and copy of colours vector
        Dimension longD = this->getLongestColourDimension();
        std::vector<int> cols = this->ccq->colours;

        // Modify each colour so it's most significant is the desired dimension
        this->modifySignificantOctet(cols, longD, this->lowerIndex, this->upperIndex);
        // Sort colours based on longest colour dimension
        std::sort(cols.begin(), cols.end());
        // Now revert back to RGB format
        this->modifySignificantOctet(cols, longD, this->lowerIndex, this->upperIndex);

        // Now find split point
        int count = 0;
        int midPoint = this->population/2;
        for (size_t i = this->lowerIndex; i <= this->upperIndex; i++) {
            count += this->ccq->histogram[cols[i]];
            if (count >= midPoint) {
                return std::min(this->upperIndex - 1, i);
            }
        }

        return this->lowerIndex;
    }

    Swatch ColourCutQuantizer::Vbox::getAverageColour() {
        int redS = 0;
        int grnS = 0;
        int bluS = 0;
        int totalPop = 0;

        // Sum up all colours
        for (size_t i = this->lowerIndex; i <= this->upperIndex; i++) {
            int col = this->ccq->colours[i];
            int pop = this->ccq->histogram[col];

            totalPop += pop;
            redS += pop * quantizedComponent(col, Dimension::Red);
            grnS += pop * quantizedComponent(col, Dimension::Green);
            bluS += pop * quantizedComponent(col, Dimension::Blue);
        }

        // Calculate means
        int redM = std::round(redS/(float)totalPop);
        int grnM = std::round(grnS/(float)totalPop);
        int bluM = std::round(bluS/(float)totalPop);

        // Create and return Swatch
        Colour c = Colour();
        int raw = approximateToRGB888(redM, grnM, bluM);
        c.setRaw(raw);
        return Swatch(c, totalPop);
    }

    void ColourCutQuantizer::Vbox::modifySignificantOctet(std::vector<int> & cols, Dimension dim, size_t lower, size_t upper) {
        switch (dim) {
            case Dimension::Red:
                // Already RGB, just return
                break;

            case Dimension::Green:
                // Swap R and G
                for (size_t i = lower; i <= upper; i++) {
                    int col = cols[i];
                    cols[i] = (quantizedComponent(col, Dimension::Green) << (QUANTIZE_WORD_WIDTH*2)) | (quantizedComponent(col, Dimension::Red) << QUANTIZE_WORD_WIDTH) | quantizedComponent(col, Dimension::Blue);
                }
                break;

            case Dimension::Blue:
                // Swap R and B
                for (size_t i = lower; i <= upper; i++) {
                    int col = cols[i];
                    cols[i] = (quantizedComponent(col, Dimension::Blue) << (QUANTIZE_WORD_WIDTH*2)) | (quantizedComponent(col, Dimension::Green) << QUANTIZE_WORD_WIDTH) | quantizedComponent(col, Dimension::Red);
                }
                break;
        }
    }

    int ColourCutQuantizer::Vbox::getVolume() {
        return (this->maxR - this->minR + 1) * (this->maxG - this->minG + 1) * (this->maxB - this->minB + 1);
    }

    bool ColourCutQuantizer::Vbox::canSplit() {
        return this->getColourCount() > 1;
    }

    int ColourCutQuantizer::Vbox::getColourCount() {
        return (1 + this->upperIndex - this->lowerIndex);
    }

    // Comparator for priority queue of Vboxes
    bool ColourCutQuantizer::VBOX_COMP(Vbox & lhs, Vbox & rhs) {
        return lhs.getVolume() < rhs.getVolume();
    };

    ColourCutQuantizer::ColourCutQuantizer(std::vector<Colour> & pixels, int maxColours, std::vector<Filter *> & fs) {
        this->filters = fs;

        // First convert from Colours to raw ints
        std::vector<int> intPixels;
        for (size_t i = 0; i < pixels.size(); i++) {
            intPixels.push_back(pixels[i].raw());
        }

        // Count occurrences of quantized colours
        this->histogram.resize(1 << (QUANTIZE_WORD_WIDTH * 3), 0);
        for (size_t i = 0; i < intPixels.size(); i++) {
            int quantizedColour = quantizeFromRGB888(intPixels[i]);
            intPixels[i] = quantizedColour;
            this->histogram[quantizedColour]++;
        }

        // Count distinct colours
        int count = 0;
        for (size_t i = 0; i < this->histogram.size(); i++) {
            if (this->histogram[i] > 0 && this->shouldIgnoreColour565(i)) {
                // Set population to zero if it should be ignored
                this->histogram[i] = 0;
            }

            if (this->histogram[i] > 0) {
                count++;
            }
        }

        // Now create array consisting of distinct colours
        this->colours.resize(count, 0);
        int index = 0;
        for (size_t i = 0; i < this->histogram.size(); i++) {
            if (this->histogram[i] > 0) {
                this->colours[index] = i;
                index++;
            }
        }

        // If the image has fewer colours than requested, use these colours
        if (count <= maxColours) {
            for (size_t i = 0; i < this->colours.size(); i++) {
                int val = this->colours[i];
                int raw = approximateToRGB888(val);
                Colour c = Colour();
                c.setRaw(raw);
                this->quantizedColours.push_back(Swatch(c, this->histogram[val]));
            }

        // Otherwise use quantization to reduce number of colours
        } else {
            this->quantizedColours = quantizePixels(maxColours);
        }
    }

    std::vector<Swatch> ColourCutQuantizer::getQuantizedColours() {
        return this->quantizedColours;
    }

    std::vector<Swatch> ColourCutQuantizer::quantizePixels(int maxColours) {
        // Create the priority queue which is sorted by volume descending
        std::priority_queue<Vbox, std::vector<Vbox>, decltype(&VBOX_COMP)> pq(VBOX_COMP);

        // To start, place a box on the queue which contains all of the colours
        pq.push(Vbox(this, 0, this->colours.size() - 1));

        // Now recursively split boxes until we have reached maxColours or there are
        // no more boxes to split
        this->splitBoxes(pq, maxColours);

        // Need to convert priority queue to vector at this point
        std::priority_queue<Vbox, std::vector<Vbox>, decltype(&VBOX_COMP)> copy = pq;
        std::vector<Vbox> vec;
        while (!pq.empty()) {
            vec.push_back(pq.top());
            pq.pop();
        }

        // Return average colours of each box
        return this->generateAverageColours(vec);
    }

    void ColourCutQuantizer::splitBoxes(std::priority_queue<Vbox, std::vector<Vbox>, decltype(&VBOX_COMP)> queue, int maxSize) {
        while ((int)queue.size() < maxSize) {
            // Return if no more boxes to split
            if (queue.empty()) {
                return;
            }

            // Otherwise get top box and split if possible
            Vbox vbox = queue.top();
            queue.pop();
            if (vbox.canSplit()) {
                queue.push(vbox.splitBox());
                queue.push(vbox);

            // If we can't split just return
            } else {
                return;
            }
        }
    }

    std::vector<Swatch> ColourCutQuantizer::generateAverageColours(std::vector<Vbox> & vboxes) {
        std::vector<Swatch> swatches;
        for (size_t i = 0; i < vboxes.size(); i++) {
            Swatch swatch = vboxes[i].getAverageColour();

            // As the colour is averaged it may not be a colour we want
            if (!this->shouldIgnoreColour888(swatch.getColour().raw())) {
                swatches.push_back(swatch);
            }
        }
        return swatches;
    }

    bool ColourCutQuantizer::shouldIgnoreColour565(int c565) {
        // Convert quantized value (RGB565) to RGB888 before checking
        int c888 = approximateToRGB888(c565);
        return this->shouldIgnoreColour888(c888);
    }

    bool ColourCutQuantizer::shouldIgnoreColour888(int c888) {
        // Convert to colour object
        Colour c = Colour();
        c.setRaw(c888);

        // Now check colour against each filter
        if (this->filters.size() > 0) {
            for (size_t i = 0; i < this->filters.size(); i++) {
                if (!this->filters[i]->isAllowed(c)) {
                    return true;
                }
            }
        }

        return false;
    }

    int ColourCutQuantizer::quantizeFromRGB888(int c888) {
        // Create colour object for easy access to components
        Colour c = Colour();
        c.setRaw(c888);

        // Calculate values
        int r = modifyWordWidth(c.r(), 8, QUANTIZE_WORD_WIDTH);
        int g = modifyWordWidth(c.g(), 8, QUANTIZE_WORD_WIDTH);
        int b = modifyWordWidth(c.b(), 8, QUANTIZE_WORD_WIDTH);

        // Shift bits and return
        return (r << (QUANTIZE_WORD_WIDTH + QUANTIZE_WORD_WIDTH) | g << QUANTIZE_WORD_WIDTH | b);
    }

    int ColourCutQuantizer::approximateToRGB888(int r, int g, int b) {
        int ar = modifyWordWidth(r, QUANTIZE_WORD_WIDTH, 8);
        int ag = modifyWordWidth(g, QUANTIZE_WORD_WIDTH, 8);
        int ab = modifyWordWidth(b, QUANTIZE_WORD_WIDTH, 8);

        // Use colour object to convert
        Colour c = Colour(0, ar, ag, ab);
        return c.raw();
    }

    int ColourCutQuantizer::approximateToRGB888(int c565) {
        return approximateToRGB888(quantizedComponent(c565, Dimension::Red), quantizedComponent(c565, Dimension::Green), quantizedComponent(c565, Dimension::Blue));
    }

    int ColourCutQuantizer::quantizedComponent(int c, Dimension d) {
        switch (d) {
            case Dimension::Red:
                return (c >> (QUANTIZE_WORD_WIDTH*2) & QUANTIZE_WORD_MASK);
                break;

            case Dimension::Green:
                return (c >> (QUANTIZE_WORD_WIDTH) & QUANTIZE_WORD_MASK);
                break;

            case Dimension::Blue:
                return (c & QUANTIZE_WORD_MASK);
                break;
        }

        // Never reached
        return -1;
    }

    int ColourCutQuantizer::modifyWordWidth(int val, int from, int to) {
        int ret;

        // Shift up
        if (to > from) {
            ret = val << (to - from);

        // Shift down
        } else {
            ret = val >> (from - to);
        }

        return ret & ((1 << to) - 1);
    }
};