#include "blinn_phong_shading.h"
// Hint:
#include "first_hit.h"
#include <iostream>

// Given a ray and its hit in the scene, return the Blinn-Phong shading
// contribution over all _visible_ light sources (e.g., take into account
// shadows). Use a hard-coded value of ia=0.1 for ambient light.
// 
// Inputs:
//   ray  incoming ray
//   hit_id  index into objects of the object just hit by ray
//   t  _parametric_ distance along ray to hit
//   n  unit surface normal at hit
//   objects  list of objects in the scene
//   lights  list of lights in the scene
// Returns shaded color collected by this ray as rgb 3-vector

Eigen::Vector3d blinn_phong_shading(
	const Ray& ray,
	const int& hit_id,
	const double& t,
	const Eigen::Vector3d& n,
	const std::vector< std::shared_ptr<Object> >& objects,
	const std::vector<std::shared_ptr<Light> >& lights)
{
	//////////////////////////////////////////////////////////////////////////////
	Eigen::Vector3d rgb = Eigen::Vector3d(0, 0, 0);

	//This function is called, so object with hit_id index is intersecting with viewing ray ray
	// Add Ambient light then
	rgb += (objects[hit_id]->material->ka) * 0.1;
	//Determine point of intersection
	Eigen::Vector3d point = ray.origin + t * ray.direction;
	Eigen::Vector3d kd = objects[hit_id]->material->kd;
	Eigen::Vector3d ks = objects[hit_id]->material->ks;
	double phongExp = objects[hit_id]->material->phong_exponent;

	//Need to loop through all lights, and see what effect they have on the point of intersection
	for (int i = 0; i < lights.size(); i++) {
		//Get the light vector l to the light source and t distance to source
		Eigen::Vector3d l;
		double light_t;
		lights[i]->direction(point, l, light_t);
		Ray lightRay;
		lightRay.direction = l;
		lightRay.origin = point;
		//Check if the light ray is hit anything, i.e it is in the shadow of another object
		int tempHitID;
		Eigen::Vector3d tempn;
		double tempt;
		if (!first_hit(lightRay, 0.00001, objects, tempHitID, tempt, tempn) || tempt > light_t) {
			//if the light vector doesnt hit anything or something that is larger than the distance to the light source
			// i.e something that is on the other side of the light source
			// calculate the h vector
			Eigen::Vector3d I = lights[i]->I;

			Eigen::Vector3d h = (l.normalized() + (-ray.direction).normalized()).normalized();
			rgb += ((kd.array() * I.array() * fmax(0.0, n.dot(l))) + (ks.array() * I.array() * pow(fmax(n.dot(h), 0), phongExp))).matrix();
		}

	}

	return rgb;
	//////////////////////////////////////////////////////////////////////////////
}
