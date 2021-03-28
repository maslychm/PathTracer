#include "rtcommon.h"

// My additions
#include "image.h"
#include "scene.h"

#include <chrono>
#include <iostream>
#include <ctime>
#include <string>

#include "external/thread_pool.h"

color ray_color(const ray& r, const color& background, const hittable& world, int depth) {
	hit_record rec;

	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0)
		return color(0, 0, 0);

	// If the ray hits nothing, return the background color.
	if (!world.hit(r, 0.001, infinity, rec))
		return background;

	ray scattered;
	color attenuation;
	color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

	if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		return emitted;

	return emitted + attenuation * ray_color(scattered, background, world, depth - 1);
}

color render_pixel(
	shared_ptr<camera> cam, 
	color& background,
	hittable_list& world,
	const int max_depth,
	const int samples_per_pixel, 
	const int image_height, 
	const int image_width,
	const int j,
	const int i) {
	color pixel_color(0, 0, 0);

	for (int s = 0; s < samples_per_pixel; ++s) {
		auto u = (i + random_double()) / (image_width - 1);
		auto v = (j + random_double()) / (image_height - 1);
		ray r = cam->get_ray(u, v);
		pixel_color += ray_color(r, background, world, max_depth);
	}

	return pixel_color;
}

void render_line(
	shared_ptr<camera> cam,
	color& background,
	hittable_list& world,
	image* img,
	const int max_depth,
	const int samples_per_pixel,
	const int line,
	const int image_height,
	const int image_width) {
	for (int i = 0; i < image_width; i++) {
		color pixel_color = render_pixel(cam, background, world, max_depth, samples_per_pixel, image_height, image_width, line, i);
		img->set_color(image_height - line - 1, i, pixel_color);
	}
}


class renderer {
public: 
	renderer() {};

	void render() {
		finished = false;

		double aspect_ratio = 16.0 / 9.0;
		int image_width = 400;
		int samples_per_pixel = 200;
		int max_depth = 50;

		scene render_scene;
		hittable_list world;

		point3 lookfrom;
		point3 lookat;
		auto vfov = 40.0;
		auto aperture = 0.0;
		color background(0, 0, 0);

		// Create the scene
		render_scene = avatar_scene();
		//render_scene = avatar_enhanced_scene();
		//render_scene = random_scene();
		//render_scene = two_perlin_spheres_scene();
		//render_scene = earth_scene();
		//render_scene = simple_light_scene();
		//render_scene = cornell_box_scene();
		//render_scene = cornell_smoke_scene();
		//render_scene = final_scene();

		// Get the scene's objects
		world = render_scene.world;

		// Get the scene's custom settings
		image_width = render_scene.image_width;
		aspect_ratio = render_scene.aspect_ratio;
		samples_per_pixel = render_scene.samples_per_pixel;
		max_depth = render_scene.max_depth;

		background = render_scene.background;
		lookfrom = render_scene.lookfrom;
		lookat = render_scene.lookat;
		vfov = render_scene.vfov;

		// Camera
		vec3 vup = render_scene.vup;
		auto dist_to_focus = render_scene.dist_to_focus;
		int image_height = static_cast<int>(image_width / aspect_ratio);

		shared_ptr<camera> cam = make_shared<camera>(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

		// Render
		img = new image(image_width, image_height, samples_per_pixel);

		int num_threads = std::thread::hardware_concurrency() - 2;
		thread_pool pool(num_threads);

		std::cout << "Rendering on " << num_threads << " threads\n";
		std::cout << "W: " << image_width << " H: " << image_height << "\n";
		std::cout << "Samples per pixel: " << samples_per_pixel << std::endl;

		// Cheat, because can't pass a field pointer to lambda
		image* img2 = img;

		// split by lines
		std::vector<std::future<void>> results;
		for (int j = 0; j < image_height; j++) {
			results.emplace_back(
				pool.enqueue([&cam, &background, &world, img2, max_depth, samples_per_pixel, j, image_height, image_width]{
					render_line(
						cam,
						background,
						world,
						img2,
						max_depth,
						samples_per_pixel,
						j,
						image_height,
						image_width);
					})
			);
		}

		for (auto&& result : results)
			result.get();
		
		save_image();
		finished = true;
		return;
	}

	float approx_completion_ratio()
	{
		return img->approx_completion();
	}

	void save_image()
	{
		img->write_image("final.ppm");
	}

	void generate_preview()
	{ 
		img->write_image("preview.ppm");
	}

	void display_status()
	{
		img->print_progress();
	}

	// Separate rendering thread to capture input in main
	void start_rendering() 
	{ 
		render_thread = std::thread([this] { render(); } ); 
	}

	// Finish by joining the thread
	void finish_rendering() 
	{
		render_thread.join(); 
	}

private:
	std::thread render_thread;
	image* img;

public:
	bool finished;
};


int main()
{
	auto start = std::chrono::high_resolution_clock::now();

	renderer rend = renderer();
	rend.start_rendering();
	
	std::cout << "/// enter p to generate a preview\n" << std::endl;
	std::cout << "/// enter s to display rendering status" << std::endl;
	std::cout << "/// enter t to estimate the remaining time" << std::endl;

	// Capture input until rendering is done
	std::string inp;
	while (!rend.finished) {
		std::cin >> inp;

		if (rend.finished)
			break;

		if (inp.find("p") != std::string::npos)
		{
			rend.generate_preview();
		}
		else if (inp.find("s") != std::string::npos)
		{
			rend.display_status();
		}
		else if (inp.find("t") != std::string::npos)
		{
			// Estimate time remaining
			auto now = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
			float time_remaining = (float) duration.count() / rend.approx_completion_ratio() 
				/ 1000 /*ms to sec*/
				/ 60; /*sec to min*/
			std::cout << "time remaining: " << time_remaining << " minutes" << std::endl;
		}
	}

	rend.finish_rendering();

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
	std::cout << "\nTook " << (double)duration.count() << " seconds to finish\n" << std::endl;

	system("pause");
	return 0;
}