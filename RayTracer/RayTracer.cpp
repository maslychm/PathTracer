#include "rtcommon.h"

// My additions
#include "image.h"
#include "scene.h"

#include <thread>
#include <chrono>
#include <iostream>
#include <ctime>

double hit_sphere(const point3& center, double radius, const ray& r) {
	vec3 oc = r.origin() - center;
	auto a = r.direction().length_squared();
	auto half_b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;
	auto discriminant = half_b * half_b - a * c;

	if (discriminant < 0) {
		return -1.0;
	}
	else {
		return (-half_b - sqrt(discriminant)) / a;
	}
}

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
	const int i) 
{
	color pixel_color(0, 0, 0);

	for (int s = 0; s < samples_per_pixel; ++s) {
		auto u = (i + random_double()) / (image_width - 1);
		auto v = (j + random_double()) / (image_height - 1);
		ray r = cam->get_ray(u, v);
		pixel_color += ray_color(r, background, world, max_depth);
	}

	return pixel_color;
}

void render_tile(
	shared_ptr<camera> cam,
	color& background,
	hittable_list& world,
	image& img,
	const int max_depth,
	const int samples_per_pixel,
	const int tile_start,
	const int tile_end,
	const int image_height,
	const int image_width)
{
	for (int j = tile_start; j < tile_end; j++) {
		for (int i = 0; i < image_width; i++) {
			color pixel_color = render_pixel(cam, background, world, max_depth, samples_per_pixel, image_height, image_width, j, i);
			img.set_color(image_height - j - 1, i, pixel_color);
		}
	}
}

int main()
{
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

	switch (0) {
	case -2:
		render_scene = avatar_scene();
		break;

	default:
	case -1:
		render_scene = avatar_enhanced_scene();
		break;
	
	case 1:
		render_scene = random_scene();
		break;
	
	case 3:
		render_scene = two_perlin_spheres_scene();
		break;

	case 4:
		render_scene = earth_scene();
		break;
	
	case 5:
		render_scene = simple_light_scene();
		break;
	
	case 6:
		render_scene = cornell_box_scene();
		break;

	case 7:
		render_scene = cornell_smoke_scene();
		break;

	case 8:
		render_scene = final_scene();
		break;
	}

	world = render_scene.world;

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
	image img(image_width, image_height, samples_per_pixel);
	
	// Spawn threads and distribute work
	// My machine has 8, leave 2 free to reduce context switching :)
	int num_threads = 6;
	std::vector<std::thread> threads;
	int tile_height = static_cast<int>(image_height / num_threads);

	std::cout << "Rendering on " << num_threads << " threads\n";
	std::cout << "W: " << image_width << " H: " << image_height << "\n";
	std::cout << "Samples per pixel: " << samples_per_pixel << std::endl;

	auto start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < num_threads; i++)
	{
		int tile_start = tile_height * i;
		int tile_end = tile_start + tile_height;

		if (i == num_threads - 1)
			tile_end = image_height;
		
		threads.push_back(
			std::thread(
			render_tile, 
			cam,
			std::ref(background),
			std::ref(world), 
			std::ref(img), 
			max_depth, 
			samples_per_pixel, 
			tile_start, 
			tile_end, 
			image_height, 
			image_width)
		);
	}

	for (auto& t : threads)
		if (t.joinable())
			t.join();

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
	std::cout << "\nTook " << (double) duration.count() << " seconds to finish" << std::endl;

	img.write_image("test.ppm");

	system("pause");

	return 0;
}