# Splash Example

This example program reads a 24-bit bitmap image and prints out the generated RGB values on the command line. While this loads a bitmap, any image type can be used in other implementations provided a `Splash::Bitmap` is populated with the correct pixel data.

## Usage

```bash
./example <path-to-image>
```

If `<path-to-image>` is not specified the program will use `images/mountain.bmp`. **This is a relative path!**
