#include "bitmap_image.hpp"
#include "splash/Splash.hpp"

// Path to default image
#define DEFAULT_IMAGE "images/mountain.bmp"

int main(int argc, char * argv[]) {
    // Read the default image if a path is not specified
    std::string path = DEFAULT_IMAGE;
    if (argc > 1) {
        path = std::string(argv[1]);
    } else {
        std::cout << "NOTE: Using the default image '" + path + "'. Pass the path to a 24-bit .bmp to use a custom image." << std::endl;
        std::cout << std::endl;
    }
    bitmap_image rawImage(path);
    if (!rawImage) {
        std::cerr << "There was an error reading the image. Please ensure it exists and is a 24-bit bitmap." << std::endl;
        return -1;
    }

    // Convert to a Splash::Bitmap which is used for generating Palettes
    // The initial bitmap is filled opaque white
    Splash::Bitmap bitmap = Splash::Bitmap(rawImage.width(), rawImage.height());
    for (size_t x = 0; x < rawImage.width(); x++) {
        for (size_t y = 0; y < rawImage.height(); y++) {
            // Get pixel from bitmap_image
            rgb_t pixel;
            rawImage.get_pixel(x, y, pixel);

            // Create colour for pixel and set in bitmap
            // Colour constructor takes (alpha, red, green, blue) where all are between 0 and 255 inclusive
            Splash::Colour colour = Splash::Colour(255, pixel.red, pixel.green, pixel.blue);
            bitmap.setPixel(colour, x, y);
        }
    }

    // Generate a palette using the default settings
    // We first acquire a Palette builder which is used to adjust the creation settings
    // Calling .generate() on the builder returns the generated palette
    // See splash/Palette.hpp or the Android documentation for more builder methods
    Splash::Palette::Builder builder = Splash::Palette::from(bitmap);
    std::shared_ptr<Splash::Palette> palette = builder.generate();

    // Print each Swatch
    // Sometimes the library will fail to find a fitting swatch and thus will return an 'invalid' swatch
    // It is worth checking if the swatch is marked as valid, otherwise the colour values contained are undefined!
    std::cout << "==== Palette Swatches ====" << std::endl;
    std::cout << "Light Vibrant: " << (palette->getLightVibrantSwatch().isValid() ? palette->getLightVibrantSwatch().toString() : "No suitable swatch found.") << std::endl;
    std::cout << "Vibrant:       " << (palette->getVibrantSwatch().isValid() ? palette->getVibrantSwatch().toString() : "No suitable swatch found.") << std::endl;
    std::cout << "Dark Vibrant:  " << (palette->getDarkVibrantSwatch().isValid() ? palette->getDarkVibrantSwatch().toString() : "No suitable swatch found.") << std::endl;
    std::cout << "Light Muted:   " << (palette->getLightMutedSwatch().isValid() ? palette->getLightMutedSwatch().toString() : "No suitable swatch found.") << std::endl;
    std::cout << "Muted:         " << (palette->getMutedSwatch().isValid() ? palette->getMutedSwatch().toString() : "No suitable swatch found.") << std::endl;
    std::cout << "Dark Muted:    " << (palette->getDarkMutedSwatch().isValid() ? palette->getDarkMutedSwatch().toString() : "No suitable swatch found.") << std::endl;
    std::cout << "Dominant:      " << (palette->getDominantSwatch().isValid() ? palette->getDominantSwatch().toString() : "No suitable swatch found.") << std::endl;
    std::cout << std::endl;

    // Create the MediaStyle object to extract suitable colours for a background and text
    Splash::MediaStyle style = Splash::MediaStyle(bitmap);

    // Print out the generated colours
    // Note that a colour is never 'invalid' and thus these colours should always be readable
    std::cout << "==== MediaStyle Colours ====" << std::endl;
    std::cout << "Background:            " << (style.isLight() ? "Light" : "Dark") << std::endl;
    std::cout << "Background Colour:     " << style.getBackgroundColour().toString() << std::endl;
    std::cout << "Primary Text Colour:   " << style.getPrimaryTextColour().toString() << std::endl;
    std::cout << "Secondary Text Colour: " << style.getSecondaryTextColour().toString() << std::endl;

    return 0;
}
