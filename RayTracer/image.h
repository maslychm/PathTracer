#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <iostream>
#include <fstream>

#include "rtcommon.h"
#include "color.h"

class image {
public:
	image() = delete;

	image(const unsigned int width, const unsigned int height, const unsigned int samples_per_pixel)
		: width(width)
		, height(height)
		, samples_per_pixel(samples_per_pixel)
	{
		pixels = new color[width * height];
	}

	~image() {
		delete[] pixels;
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
			//auto scale = 1.0 / samples_so_far;
			auto scale = 1.0;
			r = sqrt(scale * r);
			g = sqrt(scale * g);
			b = sqrt(scale * b);

			// Write the translated [0,255] value of each color component
			file << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
				<< static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
				<< static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
		}

		file.close();
	}

public:
	color *pixels;
	const unsigned int height, width, samples_per_pixel;
};

#endif // !IMAGE_H
