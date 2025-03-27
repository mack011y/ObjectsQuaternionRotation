//
// Created by Matvey Zhilyaev on 23.03.2025.
//

#ifndef POINT3D_H
#define POINT3D_H

#pragma once

#include <cmath>
#include <iostream>

struct Face3D {
    int p1, p2, p3;
    Face3D(int _p1, int _p2, int _p3) : p1(_p1), p2(_p2), p3(_p3) {}
};

class Point3D {
public:
    float x, y, z;

    Point3D(float, float, float);
    Point3D();
    bool operator==(const Point3D&) const;
    bool operator!=(const Point3D&) const;
    Point3D operator+(const Point3D&) const;
    friend std::ostream& operator<<(std::ostream&, const Point3D&);
    float distanceTo(const Point3D&) const;
    float distanceToPlane(const Point3D&, const Point3D&, const Point3D&) const;
    int orientationToPlane(const Point3D&, const Point3D&, const Point3D&) const;
    bool isInsideTetrahedron(const Point3D&, const Point3D&, const Point3D&, const Point3D&) const;
};

#endif //POINT3D_H
