#ifndef QUATERNION_H
#define QUATERNION_H

#include <cmath>
#include <iostream>
#include "Point3D.h"

class Quaternion {
private:
    float w, x, y, z;

public:
    Quaternion(float w = 1.0f, float x = 0.0f, float y = 0.0f, float z = 0.0f) : w(w), x(x), y(y), z(z) {};
    Quaternion inverse() const;
    void normalize();
    static Quaternion fromAxisAngle(float angle, float x, float y, float z);
    void rotatePoint(Point3D& th);
    Quaternion operator*(const Quaternion& q) const;
};

#endif // QUATERNION_H
