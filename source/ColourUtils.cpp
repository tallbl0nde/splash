#include <algorithm>
#include <cmath>
#include "ColourUtils.hpp"

// Constant magic numbers
#define MIN_ALPHA_SEARCH_MAX_ITERATIONS 10
#define MIN_ALPHA_SEARCH_PRECISION 1
#define XYZ_WHITE_REFERENCE_X 95.047
#define XYZ_WHITE_REFERENCE_Y 100
#define XYZ_WHITE_REFERENCE_Z 108.883
#define XYZ_EPSILON 0.008856
#define XYZ_KAPPA 903.3

namespace Splash::ColourUtils {
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
        int maxAlpha = 255;
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

    Colour changeColourLightness(Colour & base, int amount) {
        LAB lab = colourToLAB(base);
        lab.l = std::max(std::min(100.0d, lab.l + amount), 0.0d);
        return LABToColour(lab);
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
        return xyz.y/100.0d;
    }

    Colour findContrastColour(Colour & col, Colour & other, bool findFg, double ratio) {
        Colour fg = (findFg ? col : other);
        Colour bg = (findFg ? other : col);
        if (calculateContrast(fg, bg) >= ratio) {
            return col;
        }

        LAB lab = colourToLAB(findFg ? fg : bg);
        double low = 0;
        double high = lab.l;
        double a = lab.a;
        double b = lab.b;
        for (size_t i = 0; i < 15 && (high - low) > 0.00001d; i++) {
            double l = (low + high)/2.0d;
            LAB tmp = LAB{l, a, b};
            if (findFg) {
                fg = LABToColour(tmp);
            } else {
                bg = LABToColour(tmp);
            }

            if (calculateContrast(fg, bg) > ratio) {
                low = l;
            } else {
                high = l;
            }
        }
        lab = LAB{low, a, b};
        return LABToColour(lab);
    }

    Colour findContrastColourAgainstDark(Colour & col, Colour & other, bool findFg, double ratio) {
        Colour fg = (findFg ? col : other);
        Colour bg = (findFg ? other : col);
        if (calculateContrast(fg, bg) >= ratio) {
            return col;
        }

        HSL hsl = (findFg ? fg.hsl() : bg.hsl());

        float low = hsl.l;
        float high = 1;
        for (size_t i = 0; i < 15 && (high - low) > 0.00001d; i++) {
            float l = (low + high)/2.0d;
            hsl.l = l;
            if (findFg) {
                fg = HSLToColour(hsl);
            } else {
                bg = HSLToColour(hsl);
            }

            if (calculateContrast(fg, bg) > ratio) {
                high = l;
            } else {
                low = l;
            }
        }
        return (findFg ? fg : bg);
    }

    double pivotXyzComponent(double component) {
        return (component > XYZ_EPSILON ? std::pow(component, 1/3.0d) : (XYZ_KAPPA * component + 16)/116.0d);
    }

    bool satisfiesTextContrast(Colour & bg, Colour & fg) {
        return (calculateContrast(fg, bg) >= 4.5f);
    }

    Colour HSLToColour(HSL & hsl) {
        float c = (1.0f - std::abs(2 * hsl.l - 1.0f)) * hsl.s;
        float m = hsl.l - 0.5f * c;
        float x = c * (1.0f - std::abs(std::fmod(hsl.h/60.0f, 2.0f) - 1.0f));

        int segment = hsl.h/60;
        int r, g, b;
        r = g = b = 0;
        switch (segment) {
            case 0:
                r = std::round(255 * (c + m));
                g = std::round(255 * (x + m));
                b = std::round(255 * m);
                break;
            case 1:
                r = std::round(255 * (x + m));
                g = std::round(255 * (c + m));
                b = std::round(255 * m);
                break;
            case 2:
                r = std::round(255 * m);
                g = std::round(255 * (c + m));
                b = std::round(255 * (x + m));
                break;
            case 3:
                r = std::round(255 * m);
                g = std::round(255 * (x + m));
                b = std::round(255 * (c + m));
                break;
            case 4:
                r = std::round(255 * (x + m));
                g = std::round(255 * m);
                b = std::round(255 * (c + m));
                break;
            case 5:
            case 6:
                r = std::round(255 * (c + m));
                g = std::round(255 * m);
                b = std::round(255 * (x + m));
                break;
        }

        // Ensure within bounds
        r = (r < 0 ? 0 : r);
        r = (r > 255 ? 255 : r);
        g = (g < 0 ? 0 : g);
        g = (g > 255 ? 255 : g);
        b = (b < 0 ? 0 : b);
        b = (b > 255 ? 255 : b);
        return Colour(255, r, g, b);
    }

    Colour LABToColour(LAB & lab) {
        XYZ xyz = LABToXYZ(lab);
        return XYZToColour(xyz);
    }

    Colour XYZToColour(XYZ & xyz) {
        double r = (xyz.x * 3.2406d + xyz.y * -1.5372d + xyz.z * -0.4986d) / 100.0d;
        double g = (xyz.x * -0.9689d + xyz.y * 1.8758d + xyz.z * 0.0415d) / 100.0d;
        double b = (xyz.x * 0.0557d + xyz.y * -0.2040d + xyz.z * 1.0570d) / 100.0d;
        r = r > 0.0031308d ? 1.055d * std::pow(r, 1.0d / 2.4d) - 0.055d : 12.92d * r;
        g = g > 0.0031308d ? 1.055d * std::pow(g, 1.0d / 2.4d) - 0.055d : 12.92d * g;
        b = b > 0.0031308d ? 1.055d * std::pow(b, 1.0d / 2.4d) - 0.055d : 12.92d * b;

        // Ensure within bounds
        r = std::round(255 * r);
        g = std::round(255 * g);
        b = std::round(255 * b);
        r = (r < 0 ? 0 : r);
        r = (r > 255 ? 255 : r);
        g = (g < 0 ? 0 : g);
        g = (g > 255 ? 255 : g);
        b = (b < 0 ? 0 : b);
        b = (b > 255 ? 255 : b);
        return Colour(255, r, g, b);
    }

    LAB colourToLAB(Colour & col) {
        XYZ xyz = colourToXYZ(col);
        return XYZToLAB(xyz);
    }

    LAB XYZToLAB(XYZ xyz) {
        LAB lab;
        xyz.x = pivotXyzComponent(xyz.x/XYZ_WHITE_REFERENCE_X);
        xyz.y = pivotXyzComponent(xyz.y/XYZ_WHITE_REFERENCE_Y);
        xyz.z = pivotXyzComponent(xyz.z/XYZ_WHITE_REFERENCE_Z);
        lab.l = std::max(0.0d, 116 * xyz.y - 16);
        lab.a = 500 * (xyz.x - xyz.y);
        lab.b = 200 * (xyz.y - xyz.z);
        return lab;
    }

    XYZ colourToXYZ(Colour & c) {
        XYZ out;

        double sr = c.r()/255.0d;
        sr = (sr < 0.04045d ? sr/12.92d : std::pow((sr + 0.055d)/1.055d, 2.4d));
        double sg = c.g()/255.0;
        sg = (sg < 0.04045d ? sg/12.92d : std::pow((sg + 0.055d)/1.055d, 2.4d));
        double sb = c.b()/255.0;
        sb = (sb < 0.04045d ? sg/12.92d : std::pow((sb + 0.055d)/1.055d, 2.4d));

        out.x = 100.0d * (sr * 0.4124d + sg * 0.3576d + sb * 0.1805d);
        out.y = 100.0d * (sr * 0.2126d + sg * 0.7152d + sb * 0.0722d);
        out.z = 100.0d * (sr * 0.0193d + sg * 0.1192d + sb * 0.9505d);

        return out;
    }

    XYZ LABToXYZ(LAB & lab) {
        double fy = (lab.l + 16)/116;
        double fx = (lab.a/500) + fy;
        double fz = fy - lab.b/200;

        double tmp = std::pow(fx, 3);
        double xr = (tmp > XYZ_EPSILON ? tmp : (116 * fx - 16)/XYZ_KAPPA);
        double yr = (lab.l > XYZ_KAPPA * XYZ_EPSILON ? std::pow(fy, 3) : lab.l/XYZ_KAPPA);

        tmp = std::pow(fz, 3);
        double zr = (tmp > XYZ_EPSILON ? tmp : (116 * fz - 16)/XYZ_KAPPA);

        XYZ xyz;
        xyz.x = xr * XYZ_WHITE_REFERENCE_X;
        xyz.y = yr * XYZ_WHITE_REFERENCE_Y;
        xyz.z = zr * XYZ_WHITE_REFERENCE_Z;
        return xyz;
    }
};