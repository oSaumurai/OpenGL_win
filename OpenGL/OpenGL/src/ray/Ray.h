#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define RAY_T_MIN 0.0001f
#define RAY_T_MAX 1.0e30f

class Ray
{
	glm::vec3 origin;
	glm::vec3 direction;
	float tMax;

	Ray();
	Ray(const Ray& r);

	~Ray();

};

