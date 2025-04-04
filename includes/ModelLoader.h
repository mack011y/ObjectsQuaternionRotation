#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <iostream>
#include <vector>
#include <array>
#include "../includes/Point3D.h"

bool loadOBJ(const std::string& filename, std::vector<Point3D>& vertices, std::vector<std::array<int, 3> >& faces);

#endif //MODELLOADER_H
