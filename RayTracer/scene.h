#ifndef SCENE_H
#define SCENE_H

#include "rtcommon.h"
#include "camera.h"
#include "hittable_list.h"
#include "texture.h"
#include "sphere.h"
#include "material.h"
#include "bvh.h"
#include "aarect.h"
#include "box.h"
#include "moving_sphere.h"
#include "constant_medium.h"

class scene {
public:
	void set_image_defaults()
	{
		aspect_ratio = 16.0 / 9.0;
		image_width = 400;
		samples_per_pixel = 50;
		max_depth = 50;
		vup = vec3(0, 1, 0);
		vfov = 40.0;
		aperture = 0.1;
		dist_to_focus = 10.0;
		t0 = 0;
		t1 = 1;
		background = color(0, 0, 0);
	}

	virtual void set_custom_image_settings() {};

public:
	// Image settings.
	int image_width;
	double aspect_ratio;
	
	// number of light bounces
	int max_depth;

	// Random rays per pixel
	int samples_per_pixel;
	
	// Background color
	color background;

	// Camera location
	point3 lookfrom;

	// Center of the scene location
	point3 lookat;

	// Vector up (usually 0,1,0)
	vec3 vup;
	
	// Vertical field of view
	double vfov;

	// Size of opening -> "brightness"
	double aperture;

	// Where defocus blur begins
	double dist_to_focus;
	
	// Begin and end time of the simulation
	double t0;
	double t1;

	// Objects in this scene.
	hittable_list world;
};


class avatar_scene : public scene {
public:
	avatar_scene()
	{
		hittable_list objects;

		auto avamat = make_shared<lambertian>(make_shared<image_texture>("../resources/untiLARGE.png"));
		auto avasphere = make_shared<sphere>(point3(0, 0, 3.73), 1.8, avamat);
		objects.add(make_shared<rotate_y>(avasphere, 0));

		auto earthmat = make_shared<lambertian>(make_shared<image_texture>("../resources/earthmap.jpg"));
		objects.add(make_shared<sphere>(point3(1.88, .4, 1.61), .18, earthmat));

		auto light = make_shared<diffuse_light>(color(1.2, 1.2, .8));
		objects.add(make_shared<xy_rect>(-20, 100, -40, 100, -20, light));

		world = objects;

		set_image_defaults();
		set_custom_image_settings();
	}

	void set_custom_image_settings() override {
		image_width = 800;
		aspect_ratio = 1.0;
		samples_per_pixel = 100;
		max_depth = 10;
		background = color(0.0235, .0078, .0);
		lookfrom = point3(2.46, 1.61, -3.98);
		lookat = point3(0, 0, 3.73);
		vfov = 60.0;
	}
};


class avatar_enhanced_scene : public scene {
public:
	avatar_enhanced_scene()
	{
		hittable_list objects;

		auto avamat = make_shared<lambertian>(make_shared<image_texture>("../resources/untiLARGE.png"));
		auto avasphere = make_shared<sphere>(point3(0, 0, 3.73), 1.8, avamat);
		objects.add(make_shared<rotate_y>(avasphere, 0));

		auto earthmat = make_shared<lambertian>(make_shared<image_texture>("../resources/earthmap.jpg"));
		objects.add(make_shared<sphere>(point3(1.88, .4, 1.61), .18, earthmat));

		auto light = make_shared<diffuse_light>(color(1.5, 1.5, 1));
		objects.add(make_shared<xy_rect>(-20, 100, -40, 100, -20, light));


		// "from us forward" z increases from us
		// "right to left" x increases to left
		// Boxes under the scene
		auto ground = make_shared<lambertian>(color(0.0, 0.0, 60.0 / 255.0));

		hittable_list boxes1;
		hittable_list boxes2;
		hittable_list boxes3;
		hittable_list boxes4;
		hittable_list boxes5;

		boxes1.add(make_shared<box>(point3(-5, -6.25, 4), point3(-3, -4.25, 6), ground));
		boxes1.add(make_shared<box>(point3(-3, -6.5, 4), point3(-1, -4.5, 6), ground));
		boxes1.add(make_shared<box>(point3(-1, -6, 4), point3(1, -4, 6), ground));
		boxes1.add(make_shared<box>(point3(1, -6.4, 4), point3(3, -4.4, 6), ground));
		boxes1.add(make_shared<box>(point3(3, -6.1, 4), point3(5, -4.1, 6), ground));

		boxes2.add(make_shared<box>(point3(-5, -6.1, 2), point3(-3, -4.1, 4), ground));
		boxes2.add(make_shared<box>(point3(-3, -6, 2), point3(-1, -4, 4), ground));
		boxes2.add(make_shared<box>(point3(-1, -6.2, 2), point3(1, -4.2, 4), ground));
		boxes2.add(make_shared<box>(point3(1, -6.45, 2), point3(3, -4.45, 4), ground));
		boxes2.add(make_shared<box>(point3(3, -6.32, 2), point3(5, -4.32, 4), ground));

		boxes3.add(make_shared<box>(point3(-5, -6.26, 0), point3(-3, -4.26, 2), ground));
		boxes3.add(make_shared<box>(point3(-3, -6.35, 0), point3(-1, -4.35, 2), ground));
		boxes3.add(make_shared<box>(point3(-1, -6.1, 0), point3(1, -4.1, 2), ground));
		boxes3.add(make_shared<box>(point3(1, -6.5, 0), point3(3, -4.5, 2), ground));
		boxes3.add(make_shared<box>(point3(3, -6.21, 0), point3(5, -4.21, 2), ground));

		boxes4.add(make_shared<box>(point3(-5, -6.3, 6), point3(-3, -4.3, 8), ground));
		boxes4.add(make_shared<box>(point3(-3, -6.35, 6), point3(-1, -4.35, 8), ground));
		boxes4.add(make_shared<box>(point3(-1, -6.1, 6), point3(1, -4.1, 8), ground));
		boxes4.add(make_shared<box>(point3(1, -6.5, 6), point3(3, -4.5, 8), ground));
		boxes4.add(make_shared<box>(point3(3, -6.21, 6), point3(5, -4.21, 8), ground));

		boxes5.add(make_shared<box>(point3(-5, -6.1, 8), point3(-3, -4.1, 10), ground));
		boxes5.add(make_shared<box>(point3(-3, -6, 8), point3(-1, -4, 10), ground));
		boxes5.add(make_shared<box>(point3(-1, -6.2, 8), point3(1, -4.2, 10), ground));
		boxes5.add(make_shared<box>(point3(1, -6.45, 8), point3(3, -4.45, 10), ground));
		boxes5.add(make_shared<box>(point3(3, -6.32, 8), point3(5, -4.32, 10), ground));

		objects.add(make_shared<bvh_node>(boxes1, 0, 1));
		objects.add(make_shared<bvh_node>(boxes2, 0, 1));
		objects.add(make_shared<bvh_node>(boxes3, 0, 1));
		objects.add(make_shared<bvh_node>(boxes4, 0, 1));
		objects.add(make_shared<bvh_node>(boxes5, 0, 1));

		world = objects;

		set_image_defaults();
		set_custom_image_settings();
	}

	void set_custom_image_settings() override {
		image_width = 600;
		aspect_ratio = 1.0;
		samples_per_pixel = 50;
		max_depth = 20;
		background = color(0.0235, .0078, .0);
		lookfrom = point3(2.46, 1.61, -3.98);
		lookat = point3(0, 0, 3.73);
		vfov = 60.0;
	}
};


class earth_scene : public scene {
public:
	earth_scene() {
		auto earth_texture = make_shared<image_texture>("../resources/earthmap.jpg");
		auto earth_surface = make_shared<lambertian>(earth_texture);
		auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

		world = hittable_list(globe);

		set_image_defaults();
		set_custom_image_settings();
	}

	void set_custom_image_settings() override {
		background = color(0.70, 0.80, 1.00);
		lookfrom = point3(13, 2, 3);
		lookat = point3(0, 0, 0);
		vfov = 20.0;
	}
};


class two_perlin_spheres_scene : public scene {
public:
	two_perlin_spheres_scene() {
		hittable_list objects;

		auto pertext = make_shared<noise_texture>(5);
		objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
		objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

		world = objects;

		set_image_defaults();
		set_custom_image_settings();
	}

	void set_custom_image_settings() override {
		background = color(0.70, 0.80, 1.00);
		lookfrom = point3(13, 2, 3);
		lookat = point3(0, 0, 0);
		vfov = 20.0;
	}
};


class random_scene : public scene {
public:
	random_scene() {
		hittable_list objects;

		auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
		objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

		hittable_list smaller_spheres;

		for (int a = -11; a < 11; a++) {
			for (int b = -11; b < 11; b++) {
				auto choose_mat = random_double();
				point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

				if ((center - point3(4, 0.2, 0)).length() > 0.9) {
					shared_ptr<material> sphere_material;

					if (choose_mat < 0.8) {
						// diffuse
						auto albedo = color::random() * color::random();
						sphere_material = make_shared<lambertian>(albedo);
						smaller_spheres.add(make_shared<sphere>(center, 0.2, sphere_material));
					}
					else if (choose_mat < 0.95) {
						// metal
						auto albedo = color::random(0.5, 1);
						auto fuzz = random_double(0, 0.5);
						sphere_material = make_shared<metal>(albedo, fuzz);
						smaller_spheres.add(make_shared<sphere>(center, 0.2, sphere_material));
					}
					else {
						// glass
						sphere_material = make_shared<dielectric>(1.5);
						smaller_spheres.add(make_shared<sphere>(center, 0.2, sphere_material));
					}
				}
			}
		}

		objects.add(make_shared<bvh_node>(smaller_spheres, 0.0, 1.0));

		auto material1 = make_shared<dielectric>(1.5);
		objects.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

		auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
		objects.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

		auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
		objects.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

		world = objects;
		
		set_image_defaults();
		set_custom_image_settings();
	}

	void set_custom_image_settings() override {
		image_width = 800;
		samples_per_pixel = 500;
		max_depth = 20;
		aspect_ratio = 16.0 / 9.0;
		background = color(0.70, 0.80, 1.00);
		lookfrom = point3(13, 2, 3);
		lookat = point3(0, 0, 0);
		vfov = 20.0;
		aperture = 0.1;
	}
};


class simple_light_scene : public scene {
public:
	simple_light_scene() {
		hittable_list objects;

		auto pertext = make_shared<noise_texture>(4);
		objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
		objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

		auto difflight = make_shared<diffuse_light>(color(4, 4, 4));
		objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

		objects.add(make_shared<sphere>(point3(0, 7, 0), 2, difflight));

		world = objects;
		
		set_image_defaults();
		set_custom_image_settings();
	}

	void set_custom_image_settings() override {
		samples_per_pixel = 400;
		background = color(0, 0, 0);
		lookfrom = point3(26, 3, 6);
		lookat = point3(0, 2, 0);
		vfov = 20.0;
	}
};


class cornell_box_scene : public scene {
public:
	cornell_box_scene() {
		hittable_list objects;

		auto red = make_shared<lambertian>(color(.65, .05, .05));
		auto white = make_shared<lambertian>(color(.73, .73, .73));
		auto green = make_shared<lambertian>(color(.12, .45, .15));
		auto light = make_shared<diffuse_light>(color(15, 15, 15));

		objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
		objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
		objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
		objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
		objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
		objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

		shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
		box1 = make_shared<rotate_y>(box1, 15);
		box1 = make_shared<translate>(box1, vec3(265, 0, 295));
		objects.add(box1);

		shared_ptr<hittable> box2 = make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
		box2 = make_shared<rotate_y>(box2, -18);
		box2 = make_shared<translate>(box2, vec3(130, 0, 65));
		objects.add(box2);

		world = objects;

		set_image_defaults();
		set_custom_image_settings();
	}

	void set_custom_image_settings() override {
		aspect_ratio = 1.0;
		image_width = 600;
		samples_per_pixel = 1000;
		background = color(0, 0, 0);
		lookfrom = point3(278, 278, -800);
		lookat = point3(278, 278, 0);
		vfov = 40.0;
	}
};


class cornell_smoke_scene : public scene {
public :
	cornell_smoke_scene() {
		hittable_list objects;

		auto red = make_shared<lambertian>(color(.65, .05, .05));
		auto white = make_shared<lambertian>(color(.73, .73, .73));
		auto green = make_shared<lambertian>(color(.12, .45, .15));
		auto light = make_shared<diffuse_light>(color(7, 7, 7));

		objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
		objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
		objects.add(make_shared<xz_rect>(113, 443, 127, 432, 554, light));
		objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
		objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
		objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

		shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
		box1 = make_shared<rotate_y>(box1, 15);
		box1 = make_shared<translate>(box1, vec3(265, 0, 295));

		shared_ptr<hittable> box2 = make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
		box2 = make_shared<rotate_y>(box2, -18);
		box2 = make_shared<translate>(box2, vec3(130, 0, 65));

		objects.add(make_shared<constant_medium>(box1, 0.01, color(0, 0, 0)));
		objects.add(make_shared<constant_medium>(box2, 0.01, color(1, 1, 1)));

		world = objects;

		set_image_defaults();
		set_custom_image_settings();
	}

	void set_custom_image_settings() override {
		aspect_ratio = 1.0;
		image_width = 600;
		samples_per_pixel = 200;
		lookfrom = point3(278, 278, -800);
		lookat = point3(278, 278, 0);
		vfov = 40.0;
	}
};


class final_scene : public scene {
public:
	final_scene() {
		hittable_list boxes1;
		auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

		const int boxes_per_side = 20;
		for (int i = 0; i < boxes_per_side; i++) {
			for (int j = 0; j < boxes_per_side; j++) {
				auto w = 100.0;
				auto x0 = -1000.0 + i * w;
				auto z0 = -1000.0 + j * w;
				auto y0 = 0.0;
				auto x1 = x0 + w;
				auto y1 = random_double(1, 101);
				auto z1 = z0 + w;

				boxes1.add(make_shared<box>(point3(x0, y0, z0), point3(x1, y1, z1), ground));
			}
		}

		hittable_list objects;

		objects.add(make_shared<bvh_node>(boxes1, 0, 1));

		auto light = make_shared<diffuse_light>(color(7, 7, 7));
		objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));

		auto center1 = point3(400, 400, 200);
		auto center2 = center1 + vec3(30, 0, 0);
		auto moving_sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
		objects.add(make_shared<moving_sphere>(center1, center2, 0, 1, 50, moving_sphere_material));

		objects.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
		objects.add(make_shared<sphere>(
			point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
			));

		auto boundary = make_shared<sphere>(point3(360, 150, 145), 70, make_shared<dielectric>(1.5));
		objects.add(boundary);
		objects.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
		boundary = make_shared<sphere>(point3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
		objects.add(make_shared<constant_medium>(boundary, .0001, color(1, 1, 1)));

		auto emat = make_shared<lambertian>(make_shared<image_texture>("../../resources/earthmap.jpg"));
		objects.add(make_shared<sphere>(point3(400, 200, 400), 100, emat));
		auto pertext = make_shared<noise_texture>(0.1);
		objects.add(make_shared<sphere>(point3(220, 280, 300), 80, make_shared<lambertian>(pertext)));

		hittable_list boxes2;
		auto white = make_shared<lambertian>(color(.73, .73, .73));
		int ns = 1000;
		for (int j = 0; j < ns; j++) {
			boxes2.add(make_shared<sphere>(point3::random(0, 165), 10, white));
		}

		objects.add(make_shared<translate>(
			make_shared<rotate_y>(
				make_shared<bvh_node>(boxes2, 0.0, 1.0), 15),
			vec3(-100, 270, 395)
			)
		);

		world = objects;

		set_image_defaults();
		set_custom_image_settings();
	}

	void set_custom_image_settings() override {
		aspect_ratio = 1.0;
		image_width = 600;
		samples_per_pixel = 5000;
		background = color(0, 0, 0);
		lookfrom = point3(478, 278, -600);
		lookat = point3(278, 278, 0);
		vfov = 40.0;
	}
};


#endif // !SCENE_H