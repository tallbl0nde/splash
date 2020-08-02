// This file tests the Colour class
#include "catch.hpp"

#include "Colour.hpp"
using namespace Splash;

TEST_CASE("Colour: Should default to fully transparent black (0x00000000)", "[colour]") {
    Colour c = Colour();
    REQUIRE(c.raw() == static_cast<int>(0x00000000));
}

TEST_CASE("Colour: Raw value stored in the format ARGB", "[colour]") {
    Colour c = Colour(0xdd, 0xaa, 0x55, 0x12);
    REQUIRE(c.raw() == static_cast<int>(0xddaa5512));
}

TEST_CASE("Colour: Component methods return correct component value", "[colour]") {
    Colour c = Colour(0x22, 0x77, 0x81, 0x18);
    REQUIRE(c.a() == 0x22);
    REQUIRE(c.r() == 0x77);
    REQUIRE(c.g() == 0x81);
    REQUIRE(c.b() == 0x18);
}

TEST_CASE("Colour: Correctly converts RGB value to HSL", "[colour]") {
    HSL hsl;

    // Pure red
    hsl = Colour(0, 255, 0, 0).hsl();
    REQUIRE(hsl.h == 0.0f);
    REQUIRE(hsl.s == 1.0f);
    REQUIRE(hsl.l == 0.5f);

    // Pure green
    hsl = Colour(0, 0, 255, 0).hsl();
    REQUIRE(hsl.h == 120.0f);
    REQUIRE(hsl.s == 1.0f);
    REQUIRE(hsl.l == 0.5f);

    // Pure blue
    hsl = Colour(0, 0, 0, 255).hsl();
    REQUIRE(hsl.h == 240.0f);
    REQUIRE(hsl.s == 1.0f);
    REQUIRE(hsl.l == 0.5f);

    // Other colours for good measure
    hsl = Colour(0, 200, 105, 20).hsl();
    REQUIRE((hsl.h > 27.0f && hsl.h < 29.0f));
    REQUIRE((hsl.s > 0.81f && hsl.s < 0.82f));
    REQUIRE((hsl.l > 0.43f && hsl.l < 0.44f));
    hsl = Colour(0, 10, 10, 200).hsl();
    REQUIRE((hsl.h > 239.0f && hsl.h < 241.0f));
    REQUIRE((hsl.s > 0.90f && hsl.s < 0.91f));
    REQUIRE((hsl.l > 0.41f && hsl.l < 0.42f));
    hsl = Colour(0, 240, 233, 198).hsl();
    REQUIRE((hsl.h > 49.0f && hsl.h < 51.0f));
    REQUIRE((hsl.s > 0.58f && hsl.s < 0.59f));
    REQUIRE((hsl.l > 0.85f && hsl.l < 0.86f));
}

TEST_CASE("Colour: Correctly sets each component", "[colour]") {
    Colour c = Colour(0, 0, 0, 0);

    SECTION("Alpha") {
        c.setA(100);
        REQUIRE(c.a() == 100);
    }

    SECTION("Red") {
        c.setR(219);
        REQUIRE(c.r() == 219);
    }

    SECTION("Green") {
        c.setG(12);
        REQUIRE(c.g() == 12);
    }

    SECTION("Blue") {
        c.setB(255);
        REQUIRE(c.b() == 255);
    }
}

TEST_CASE("Colour: Updates raw value correctly", "[colour]") {
    Colour c = Colour(255, 255, 255, 255);
    REQUIRE(c.raw() == static_cast<int>(0xffffffff));

    c.setRaw(0xabcdef12);
    REQUIRE(c.raw() == static_cast<int>(0xabcdef12));
}