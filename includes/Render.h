//
// Created by Matvey Zhilyaev on 27.03.2025.
//

#ifndef RENDER_H
#define RENDER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Point3D.h"
#include "Quaternion.h"
#include "ModelLoader.h"


struct FaceDepth {
    int faceIndex;
    float depth;
};

bool compareFaceDepth(const FaceDepth &a, const FaceDepth &b);
void normalizeModel(std::vector<Point3D>& vertices);
void sortFaces(std::vector<FaceDepth> &faceDepths);
void drawPolygon(std::vector<Point3D> vertices, std::vector<std::array<int, 3>> faces);
void drawModelFromOBJ(const std::string& filename);
void processInput(GLFWwindow* window);
void drawIcosahedron();

#endif //RENDER_H
