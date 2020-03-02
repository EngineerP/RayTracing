#include "raycolor.h"
#include "first_hit.h"
#include "blinn_phong_shading.h"
#include "reflect.h"

// Shoot a ray into a lit scene and collect color information.
//
// Inputs:
//   ray  ray along which to search
//   min_t  minimum t value to consider (for viewing rays, this is typically at
//     least the _parametric_ distance of the image plane to the camera)
//   objects  list of objects (shapes) in the scene
//   lights  list of lights in the scene
//   num_recursive_calls  how many times has raycolor been called already
// Outputs:
//   rgb  collected color 
// Returns true iff a hit was found

bool raycolor(
	const Ray& ray,
	const double min_t,
	const std::vector< std::shared_ptr<Object> >& objects,
	const std::vector< std::shared_ptr<Light> >& lights,
	const int num_recursive_calls,
	Eigen::Vector3d& rgb)
{
	////////////////////////////////////////////////////////////////////////////
	// Replace with your code here:
	//Base Case
	//Incoming Viewing Ray, check and see what it hits
	int hitID;
	double t;
	rgb = Eigen::Vector3d(0, 0, 0);
	Eigen::Vector3d n;
	if (first_hit(ray, min_t, objects, hitID, t, n)) {
		//If it hits something, determine the point of intersection
		Eigen::Vector3d p = ray.origin + t * (ray.direction);
		//Compute reflect ray vector from intersection Point
		Ray reflectedRay;
		reflectedRay.direction = reflect(ray.direction.normalized(), n);
		reflectedRay.origin = p;
		Eigen::Vector3d reflectedRGB = Eigen::Vector3d(0,0,0);
		//Now we have to see what color will be brought in from this reflected ray
		//Run RayColor with the reflected ray
		if (num_recursive_calls < 6 && raycolor(reflectedRay, 0.00001, objects, lights, num_recursive_calls + 1, reflectedRGB)) {
			//if the reflected ray hit something, that means some colored light is reflected onto this object
			//Add the reflected rgb value from the Recurse Raycolor call with the Km Factor
			rgb += ((objects[hitID]->material->km.array())*reflectedRGB.array()).matrix();
		}
		//Calculate Blinn Phong and sum it with the reflected light
		rgb += blinn_phong_shading(ray, hitID, t, n, objects, lights);
		return true;
	}
	else {
		return false;
	}
	////////////////////////////////////////////////////////////////////////////
}
