#include <algorithm>
#include <cmath>
#include "ColourUtils.hpp"

namespace Palette::ColourUtils {
    // Constants
    static const int MIN_ALPHA_SEARCH_MAX_ITERATIONS = 10;
    static const int MIN_ALPHA_SEARCH_PRECISION = 1;

    double calculateContrast(Colour & fg, Colour & bg) {
        // Official library throws an exception here, instead we'll return -1
        if (bg.a() != 255) {
            return -1;
        }
        if (fg.a() < 255) {
            // If foreground is translucent, composite foreground over background
            fg = compositeColours(fg, bg);
        }

        double lum1 = calculateLuminance(fg) + 0.05;
        double lum2 = calculateLuminance(bg) + 0.05;

        // Return lighter luminance divided by darker luminance
        return (std::max(lum1, lum2) / std::min(lum1, lum2));
    }

    int calculateMinimumAlpha(Colour & fg, Colour & bg, float ratio) {
        // Check background is not translucent
        // Official library throws an exception here, instead we'll return -1
        if (bg.a() != 255) {
            return -1;
        }

        // Check a fully opaque foreground has sufficient contrast
        Colour tmpFg = fg;
        tmpFg.setA(255);
        double tmpRatio = calculateContrast(tmpFg, bg);
        if (tmpRatio < ratio) {
            return -1;
        }

        // Binary search to find a value that provides sufficient contrast
        int numIt = 0;
        int minAlpha = 0;
        int maxAlpha = 0;
        while (numIt <= MIN_ALPHA_SEARCH_MAX_ITERATIONS && (maxAlpha-minAlpha) > MIN_ALPHA_SEARCH_PRECISION) {
            int testAlpha = (minAlpha + maxAlpha)/2;

            tmpFg = fg;
            tmpFg.setA(testAlpha);
            tmpRatio = calculateContrast(tmpFg, bg);

            if (tmpRatio < ratio) {
                minAlpha = testAlpha;
            } else {
                maxAlpha = testAlpha;
            }

            numIt++;
        }

        // Return maximum of the range of alphas that will pass
        return maxAlpha;
    }

    Colour compositeColours(Colour & fg, Colour & bg) {
        // Composite individual components
        int a = compositeAlpha(fg.a(), bg.a());
        int r = compositeComponent(fg.r(), fg.a(), bg.r(), bg.a(), a);
        int g = compositeComponent(fg.g(), fg.a(), bg.g(), bg.a(), a);
        int b = compositeComponent(fg.b(), fg.a(), bg.b(), bg.a(), a);

        // Create and return new colour
        return Colour(a, r, g, b);
    }

    int compositeAlpha(int fgA, int bgA) {
        return 0xff - (((0xff - bgA) * (0xff - fgA)) / 0xff);
    }

    int compositeComponent(int fgC, int fgA, int bgC, int bgA, int a) {
        if (a == 0) {
            return 0;
        }
        return ((0xff * fgC * fgA) + (bgC * bgA * (0xff - fgA))) / (a * 0xff);
    }

    double calculateLuminance(Colour & c) {
        XYZ xyz = colourToXYZ(c);
        return xyz.y/100;
    }

    XYZ colourToXYZ(Colour & c) {
        XYZ out;

        double sr = c.r()/255.0;
        sr = (sr < 0.04045 ? sr/12.92 : std::pow((sr + 0.055)/1.055, 2.4));
        double sg = c.g()/255.0;
        sg = (sg < 0.04045 ? sg/12.92 : std::pow((sg + 0.055)/1.055, 2.4));
        double sb = c.b()/255.0;
        sb = (sb < 0.04045 ? sg/12.92 : std::pow((sb + 0.055)/1.055, 2.4));

        out.x = 100 * (sr * 0.4124 + sg * 0.3576 + sb * 0.1805);
        out.y = 100 * (sr * 0.2126 + sg * 0.7152 + sb * 0.0722);
        out.z = 100 * (sr * 0.0193 + sg * 0.1192 + sb * 0.9505);

        return out;
    }
};