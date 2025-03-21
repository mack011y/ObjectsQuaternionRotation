//
// Created by Matvey Zhilyaev on 20.03.2025.
//

#ifndef QUATERNIONS_H
#define QUATERNIONS_H

class Quaternion {
public:
    Quaternion();
    Quaternion(float, float, float, float);
    float* toMatrix() const;
    Quaternion operator*(const Quaternion&) const;
    Quaternion& operator*=(const Quaternion&);
    Quaternion QforAngle(float, float*);
    Quaternion QforVector(float*);
private:
    float x, y, z, w;
};

Quaternion::Quaternion() {
   x = y = z = w = 0;
}

#endif //QUATERNIONS_H
