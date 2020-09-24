#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtcommon.h"

struct hit_record;

class material {
public:
	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const = 0;
};

class lambertian : public material {
public:
	lambertian(const color& a) : albedo(a) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override {
		//point3 target;
		//target = rec.p + rec.normal + random_unit_vector();
		//target = rec.p + rec.normal + random_in_unit_sphere();
		//target = rec.p + random_in_hemisphere(rec.normal);
		vec3 scattered_direction = rec.normal + random_unit_vector();
		scattered = ray(rec.p, scattered_direction);
		attenuation = albedo;
		return true;
	}

public:
	color albedo;
};

class metal : public material {
public:
	metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

public:
	color albedo;
	double fuzz;
};

#endif // !MATERIAL_H
