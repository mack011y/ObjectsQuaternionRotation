#include <iostream>
#include "../includes/Point3D.h"

Point3D::Point3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

Point3D::Point3D() : x(0), y(0), z(0) {}

bool Point3D::operator==(const Point3D& other) const {
    return other.x == x && other.y == y && other.z == z;
}

bool Point3D::operator!=(const Point3D& other) const {
    return other.x != x || other.y != y || other.z != z;
}

Point3D Point3D::operator+(const Point3D& other) const {
    return Point3D(x + other.x, y + other.y, z + other.z);
}

Point3D Point3D::operator*(const float other) const {
    return Point3D(x * other, y * other, z * other);
}
Point3D Point3D::operator-(const Point3D& other) const {
    return Point3D(x - other.x, y - other.y, z - other.z);
}

std::ostream& operator<<(std::ostream& os, const Point3D& p) {
    os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    return os;
}

float Point3D::distanceTo(const Point3D& other) const {
    float dx = x - other.x, dy = y - other.y, dz = z - other.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

float Point3D::distanceToPlane(const Point3D& p1, const Point3D& p2, const Point3D& p3) const {
    float nx = (p2.y - p1.y) * (p3.z - p1.z) - (p2.z - p1.z) * (p3.y - p1.y);
    float ny = (p2.z - p1.z) * (p3.x - p1.x) - (p2.x - p1.x) * (p3.z - p1.z);
    float nz = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
    float len = std::sqrt(nx * nx + ny * ny + nz * nz);
    if (len == 0) return 0.0f;
    float d = -(nx * p1.x + ny * p1.y + nz * p1.z);
    return std::abs(nx * x + ny * y + nz * z + d) / len;
}

int Point3D::orientationToPlane(const Point3D& p1, const Point3D& p2, const Point3D& p3) const {
    float nx = (p2.y - p1.y) * (p3.z - p1.z) - (p2.z - p1.z) * (p3.y - p1.y);
    float ny = (p2.z - p1.z) * (p3.x - p1.x) - (p2.x - p1.x) * (p3.z - p1.z);
    float nz = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
    float len = std::sqrt(nx * nx + ny * ny + nz * nz);
    if (len < 1e-6) return 0;
    float val = nx * (x - p1.x) + ny * (y - p1.y) + nz * (z - p1.z);
    if (std::abs(val) < 1e-6) return 0;
    return (val > 0) ? 1 : -1;
}

void Point3D::normalize() {
    float len = sqrt(x * x + y * y + z * z);
    x /= len;
    y /= len;
    z /= len;
}