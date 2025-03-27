#include <cmath>
#include"../includes/Point3D.h"
#include"../includes/Quaternion.h"

Quaternion Quaternion::inverse() const {
    float norm = w * w + x * x + y * y + z * z;
    if (norm == 0.0f) return Quaternion(1, 0, 0, 0);
    return Quaternion(w / norm, -x / norm, -y / norm, -z / norm);
}

void Quaternion::normalize() {
    float length = sqrt(w * w + x * x + y * y + z * z);
    if (length > 0.0f) {
        w /= length;
        x /= length;
        y /= length;
        z /= length;
    }
}

Quaternion Quaternion::fromAxisAngle(float angle, float x, float y, float z) {
    float halfAngle = angle / 2.0f;
    float sinHalfAngle = sin(halfAngle);
    Quaternion q(cos(halfAngle),
                 x * sinHalfAngle,
                 y * sinHalfAngle,
                 z * sinHalfAngle);
    q.normalize();
    return q;
}

void Quaternion::rotatePoint(Point3D& th) {
     Quaternion p(0.0f, th.x, th.y, th.z);
     const Quaternion qInv = this->inverse();
     Quaternion rotated = *this * p * qInv;
     th.x = rotated.x;
     th.y = rotated.y;
     th.z = rotated.z;
}

Quaternion Quaternion::operator*(const Quaternion& q) const {
    return Quaternion(
        w * q.w - x * q.x - y * q.y - z * q.z,
        w * q.x + x * q.w + y * q.z - z * q.y,
        w * q.y - x * q.z + y * q.w + z * q.x,
        w * q.z + x * q.y - y * q.x + z * q.w
    );
}
