#include "splash/Colour.hpp"
#include "splash/Utils.hpp"
#include <algorithm>
#include <cmath>

namespace Splash {
    Colour::Colour() {
        this->value = 0;
    }

    Colour::Colour(int a, int r, int g, int b) {
        // Encode value
        this->value = 0;
        this->setA(a);
        this->setR(r);
        this->setG(g);
        this->setB(b);
    }

    int Colour::a() const {
        return (this->value & 0xff000000) >> 24;
    }

    int Colour::r() const {
        return (this->value & 0x00ff0000) >> 16;
    }

    int Colour::g() const {
        return (this->value & 0x0000ff00) >> 8;
    }

    int Colour::b() const {
        return (this->value & 0x000000ff);
    }

    unsigned int Colour::raw() const {
        return this->value;
    }

    HSL Colour::hsl() const {
        // Struct to return
        HSL hsl;

        // Convert from [0, 255] to [0, 1]
        float fr = this->r()/255.0d;
        float fg = this->g()/255.0d;
        float fb = this->b()/255.0d;

        // Calculate values used to computation
        float max = std::max(std::max(fr, fg), fb);
        float min = std::min(std::min(fr, fg), fb);
        float delta = max - min;

        // Luminance
        hsl.l = (max+min)/2.0f;

        // Saturation
        if (delta == 0.0f) {
            hsl.s = 0.0f;
            hsl.h = 0.0f;
            return hsl;

        } else {
            hsl.s = delta/(1.0f-std::abs((2.0f*hsl.l)-1.0f));
        }

        // Hue
        if (max == fr) {
            hsl.h = 60.0f * std::fmod(((fg - fb) / delta), 6.0f);
        } else if (max == fg) {
            hsl.h = 60.0f * (((fb - fr)/delta) + 2.0f);
        } else if (max == fb) {
            hsl.h = 60.0f * (((fr - fg)/delta) + 4.0f);
        }

        // Ensure hue is positive
        if (hsl.h < 0.0f) {
            hsl.h += 360.0f;
        }

        return hsl;
    }

    void Colour::setA(int a) {
        this->value &= ~(0xff << 24);
        this->value |= (a & 0xff) << 24;
    }

    void Colour::setR(int r) {
        this->value &= ~(0xff << 16);
        this->value |= (r & 0xff) << 16;
    }

    void Colour::setG(int g) {
        this->value &= ~(0xff << 8);
        this->value |= (g & 0xff) << 8;
    }

    void Colour::setB(int b) {
        this->value &= ~(0xff);
        this->value |= (b & 0xff);
    }

    void Colour::setRaw(unsigned int raw) {
        this->value = raw;
    }

    std::string Colour::toString() {
        return "[RGB: #" + Utils::intToHexString(this->raw()) + "]";
    }
};