#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <fstream>
#include <string>

#include "rtcommon.h"
#include "color.h"

class image {
public:
	image(const unsigned int image_width, const unsigned int image_height, const unsigned int samples_per_pixel)
		: width(image_width)
		, height(image_height)
		, samples_per_pixel(samples_per_pixel)
		, num_pixels_total(width * height)
	{
		pixels = new color[width * height];
		pixels_completed = 0;
	}

	~image() {
		delete[] pixels;
	}

	void set_color(int y, int x, color c)
	{
		pixels[width * y + x] = c;
		pixels_completed += 1;
		
		if (pixels_completed % 1000 == 0)
			print_progress();
	}

	void write_image(std::string filename) {
		std::ofstream file;
		file.open(filename);

		file << "P3\n" << this->width << " " << this->height << "\n255\n";

		for (unsigned i = 0; i < this->width * this->height; i++) {
			auto pixel_color = pixels[i];
			auto r = pixel_color.x();
			auto g = pixel_color.y();
			auto b = pixel_color.z();

			// Divide the color by the number of samples and gamma correct for gamma=2.0
			auto scale = 1.0 / samples_per_pixel;
			//auto scale = 1.0;
			r = sqrt(scale * r);
			g = sqrt(scale * g);
			b = sqrt(scale * b);

			// Write the translated [0,255] value of each color component
			file << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
				<< static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
				<< static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
		}

		file.close();

		std::cout << "\nsaved as " << filename << std::endl;
	}

	float approx_completion()
	{
		return (float)pixels_completed / num_pixels_total;
	}


	void print_progress()
	{
		std::cerr << "\r" << approx_completion() * 100 << "%" << "(" << pixels_completed << " of " << num_pixels_total << ")" << std::flush;
	}

public:
	color *pixels;
	const unsigned int height, width, samples_per_pixel, num_pixels_total;
	int pixels_completed;
};

#endif // !IMAGE_H
