// This file tests the Bitmap class
#include "catch.hpp"
#include "splash/Splash.hpp"

using namespace Splash;

TEST_CASE("Bitmap: A default bitmap should always be invalid", "[bitmap]") {
    Bitmap b = Bitmap();
    REQUIRE(!b.isValid());

    b.setWidth(100);
    b.setHeight(100);
    REQUIRE(!b.isValid());
}

TEST_CASE("Bitmap: A correctly sized bitmap is created", "[bitmap]") {
    Bitmap b = Bitmap(55, 123);
    REQUIRE(b.getWidth() == 55);
    REQUIRE(b.getHeight() == 123);
}

TEST_CASE("Bitmap: A new bitmap is filled with white", "[bitmap]") {
    Bitmap b = Bitmap(50, 50);

    bool good = true;
    std::vector<Colour> pixels = b.getPixels(0, 0, 50, 50);
    for (size_t i = 0; i < pixels.size(); i++) {
        if (pixels[i].raw() != (0xffffffff)) {
            good = false;
            break;
        }
    }
    REQUIRE(good);
}

TEST_CASE("Bitmap: Reading a single pixel outside of the bitmap returns white", "[bitmap]") {
    Bitmap b = Bitmap(10, 10);

    REQUIRE(b.getPixel(20, 20).raw() == (0xffffffff));
}

TEST_CASE("Bitmap: Reading multiple pixels outside the bitmap skips to next pixel", "[bitmap]") {
    Bitmap b = Bitmap(10, 10);

    SECTION("Reading partially outside doesn't crash") {
        std::vector<Colour> p = b.getPixels(8, 8, 10, 10);
        REQUIRE(p.size() == 4);
    }

    SECTION("Reading completely outside returns no pixels") {
        std::vector<Colour> p = b.getPixels(100, 50, 230, 23);
        REQUIRE(p.size() == 0);
    }
}

TEST_CASE("Bitmap: Can read/write set of pixels", "[bitmap]") {
    std::vector<Colour> colours = {Colour(0, 255, 255, 0), Colour(0, 0, 255, 30), Colour(0, 255, 40, 255), Colour(0, 25, 55, 200)};
    Bitmap b = Bitmap(2, 2);
    b.setPixels(colours, 0, 0, 2, 2);

    std::vector<Colour> cols = b.getPixels(0, 0, 2, 2);
    REQUIRE(cols[0].raw() == colours[0].raw());
    REQUIRE(cols[1].raw() == colours[1].raw());
    REQUIRE(cols[2].raw() == colours[2].raw());
    REQUIRE(cols[3].raw() == colours[3].raw());
}

TEST_CASE("Bitmap: Can read/write individual pixels", "[bitmap]") {
    Bitmap b = Bitmap(20, 20);
    Colour c = Colour(10, 255, 0, 20);

    SECTION("Within range should change") {
        b.setPixel(c, 15, 15);
        REQUIRE(b.getPixel(15, 15).raw() == (0x0aff0014));
    }

    SECTION("Outside range should do nothing") {
        b.setPixel(c, 1000, 230);
    }
}

TEST_CASE("Bitmap: Resizes correctly", "[bitmap]") {
    Bitmap b = Bitmap(30, 100);
    b.setWidth(1000);
    REQUIRE(b.getWidth() == 1000);
    b.setWidth(10);
    REQUIRE(b.getWidth() == 10);
    b.setHeight(565);
    REQUIRE(b.getHeight() == 565);
    b.setHeight(2);
    REQUIRE(b.getHeight() == 2);
}

TEST_CASE("Bitmap: Returns a nearest-neighbour scaled bitmap", "[bitmap]") {
    Colour colours[4] = {Colour(0, 255, 255, 0), Colour(0, 0, 255, 30), Colour(0, 255, 40, 255), Colour(0, 25, 55, 200)};

    // We'll use a simple 4 colour bitmap here
    Bitmap b = Bitmap(4, 4);
    std::vector<Colour> pixels;
    for (size_t y = 0; y < 4; y++) {
        for (size_t x = 0; x < 4; x++) {
            size_t idx = (x/2 + (2*(y/2)));
            pixels.push_back(colours[idx]);
        }
    }
    b.setPixels(pixels, 0, 0, 4, 4);

    SECTION("Scaled down") {
        Bitmap b2 = b.createScaledBitmap(2, 2);
        std::vector<Colour> cols = b2.getPixels(0, 0, 2, 2);
        REQUIRE(cols[0].raw() == colours[0].raw());
        REQUIRE(cols[1].raw() == colours[1].raw());
        REQUIRE(cols[2].raw() == colours[2].raw());
        REQUIRE(cols[3].raw() == colours[3].raw());
    }

    SECTION("Scaled up") {
        Bitmap b2 = b.createScaledBitmap(8, 8);
        std::vector<Colour> cols = b2.getPixels(0, 0, 8, 8);

        bool good = true;
        for (size_t y = 0; y < 8; y++) {
            for (size_t x = 0; x < 8; x++) {
                size_t idx = (x/4 + (2*(y/4)));
                if (cols[(y*8) + x].raw() != colours[idx].raw()) {
                    good = false;
                    break;
                }
            }

            if (!good) {
                break;
            }
        }
        REQUIRE(good);
    }
}