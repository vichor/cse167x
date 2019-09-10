// Transform.cpp: implementation of the Transform class.

#include "Transform.h"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const vec3& vec)
{
	os << "vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return os;
}



//Please implement the following functions:

// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;
	float phi = pi * degrees / 180.0f;
	float cosphi = cos(phi);
	float sinphi = sin(phi);

	// Rodrigues formula, first term: Cosphi * Identity(3x3)
	mat3 term1 = mat3(cosphi, 0, 0, 0, cosphi, 0, 0, 0, cosphi);

	// Rodrigues formula, secod term: 1-Cosphi * axis*axisTransposed
	mat3 term2 = (1-cosphi) * mat3(x*x, x*y, x*z, y*x, y*y, y*z, z*x, z*y, z*z);
		
	// Rodrigues formula, third term: Sinphi * axisStar
	mat3 term3 = sinphi * mat3(0, z, -y, -z, 0, x, y, -x, 0);

 	return term1 + term2 + term3;
}

// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {
	eye = rotate(degrees, up) * eye;
}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {
	//std::cout << "eye: " << eye << "\tup: " << up;
	vec3 rotAxis = glm::normalize(glm::cross(up, eye));
	//std::cout << "\trotAxis: " << rotAxis;
	eye = rotate(-degrees, rotAxis) * eye;
	up = glm::normalize(glm::cross(eye, rotAxis));
	//std::cout << "=>\t newEye: " << eye << "\tnewUp:" << up << std::endl;
}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {

	// Coordinate frame definiton
	vec3 w = glm::normalize(eye);
	vec3 u = glm::normalize(glm::cross(up, w));
	vec3 v = glm::cross(w, u);

	// Rotation matrix
	mat3 rotationMatrix = mat3(u.x, v.x, w.x, u.y, v.y, w.y, u.z, v.z, w.z);

	// Translation
	vec3 translationMatrix = vec3(-glm::dot(u, eye), -glm::dot(v, eye), -glm::dot(w, eye));

	// Final view matrix is a matrix as follows:
	//		view = (
	//			R00	R01	R02	Tx
	//			R10	R11	R12	Ty
	//			R20	R21	R22	Tz
	//			0	0	0	1
	//			)
	// Applying column major access in glm we have:
	//		view = mat4(R00, R10, R20, 0, R01, R11, R21, 0, R02, R12, R22, 0, Tx, Ty, Tz, 1)
	//
	// or put in different lines
	//		view = mat4(
	//			R00, R10, R20, 0, 
	//			R01, R11, R21, 0, 
	//			R02, R12, R22, 0, 
	//			Tx, Ty, Tz, 1)
	//
	// And access to Rotation matrix is as follows:
	//		R00 = rotationMatrix[0][0]
	//		R01 = rotationMatrix[1][0]
	//		R02 = rotationMatrix[2][0]
	//		R10 = rotationMatrix[0][1]
	//		R11 = rotationMatrix[1][1]
	//		R12 = rotationMatrix[2][1]
	//		R20 = rotationMatrix[0][2]
	//		R21 = rotationMatrix[1][2]
	//		R22 = rotationMatrix[2][2]
	// So we get below view matrix definition
	mat4 view = mat4(
		rotationMatrix[0][0], rotationMatrix[0][1], rotationMatrix[0][2], 0,
		rotationMatrix[1][0], rotationMatrix[1][1], rotationMatrix[1][2], 0,
		rotationMatrix[2][0], rotationMatrix[2][1], rotationMatrix[2][2], 0,
		translationMatrix.x,  translationMatrix.y,  translationMatrix.z,  1);

    return view;
}

Transform::Transform()
{

}

Transform::~Transform()
{

}