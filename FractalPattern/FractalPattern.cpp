#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h" // Library for saving images in formats like PNG.
#include <iostream> // Standard C++ library for input/output operations.
#include <vector> // Library for using dynamic arrays (vectors).
#include <complex> // Library for handling complex numbers.

/// Function to calculate Mandelbrot set membership.
/// @param real Real part of the complex number.
/// @param imag Imaginary part of the complex number.
/// @param maxIterations Maximum number of iterations to check membership.
/// @return The number of iterations before the magnitude of z exceeds 2.
int mandelbrot(double real, double imag, int maxIterations) {
    std::complex<double> c(real, imag); // Construct the complex number c = real + imag * i.
    std::complex<double> z = 0; // Start with z = 0.
    int iterations = 0; // Counter for the number of iterations.

    // Iterate until z's magnitude exceeds 2 or the max number of iterations is reached.
    while (std::abs(z) < 2.0 && iterations < maxIterations) {
        z = z * z + c; // Mandelbrot formula: z = z^2 + c.
        iterations++; // Increment the iteration counter.
    }

    return iterations; // Return the number of iterations.
}

/// Function to map Mandelbrot iterations to RGB color values.
/// @param iterations Number of iterations for the point.
/// @param maxIterations Maximum iterations allowed.
/// @param r Reference to red color value (output).
/// @param g Reference to green color value (output).
/// @param b Reference to blue color value (output).
void getColor(int iterations, int maxIterations, unsigned char& r, unsigned char& g, unsigned char& b) {
    double t = (double)iterations / maxIterations; // Normalize iterations to [0, 1].

    if (iterations == maxIterations) {
        // If max iterations are reached, set color to black.
        r = g = b = 0;
    }
    else {
        // Calculate a smooth gradient color.
        r = (unsigned char)(9 * (1 - t) * t * t * t * 255 * 1.5);
        g = (unsigned char)(15 * (1 - t) * (1 - t) * t * t * 255 * 1.5);
        b = (unsigned char)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255 * 1.5);

        // Clamp color values to the range [0, 255].
        r = std::min(r, (unsigned char)255);
        g = std::min(g, (unsigned char)255);
        b = std::min(b, (unsigned char)255);
    }
}

/// Main function to generate and save the Mandelbrot fractal image.
int main() {
    // Image dimensions.
    int width = 800;
    int height = 800;
    int maxIterations = 1000; // Maximum iterations for Mandelbrot calculations.

    // Mandelbrot coordinate bounds.
    double minReal = -2.0, maxReal = 1.0;
    double minImag = -1.5, maxImag = 1.5;

    // Image buffer to store pixel RGB values (3 bytes per pixel).
    std::vector<unsigned char> image(width * height * 3);

    std::cout << "Generating Mandelbrot..." << std::endl;

    // Loop through each pixel in the image.
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            // Map pixel coordinates to Mandelbrot coordinates.
            double real = minReal + (x / (double)width) * (maxReal - minReal);
            double imag = minImag + (y / (double)height) * (maxImag - minImag);

            // Calculate the Mandelbrot iterations for this coordinate.
            int iterations = mandelbrot(real, imag, maxIterations);

            // Convert iterations to a color.
            unsigned char r, g, b;
            getColor(iterations, maxIterations, r, g, b);

            // Store the RGB values in the image buffer.
            int index = (y * width + x) * 3; // Calculate buffer index for the pixel.
            image[index] = r;       // Red channel.
            image[index + 1] = g;   // Green channel.
            image[index + 2] = b;   // Blue channel.
        }
    }

    std::cout << "Mandelbrot generation complete." << std::endl;

    // Display the size of the image buffer.
    std::cout << "Image buffer size: " << image.size() << std::endl;

    // Save the image buffer to a PNG file.
    if (!stbi_write_png("mandelbrot_fractal_pattern.png", width, height, 3, image.data(), width * 3)) {
        std::cerr << "Failed to save the image!" << std::endl;
        return 1; // Return an error code if saving fails.
    }

    std::cout << "Image saved successfully!" << std::endl;
    return 0; // Return 0 to indicate successful execution.
}
