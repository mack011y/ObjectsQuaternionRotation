#include <iostream>
#include "../includes/Render.h"

bool compareFaceDepth(const FaceDepth &a, const FaceDepth &b) {
    return a.depth > b.depth;
}

void normalizeModel(std::vector<Point3D>& vertices) {
    if (vertices.empty()) return;
    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::min();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::min();

    for (const auto& vertex : vertices) {
        minX = std::min(minX, vertex.x);
        maxX = std::max(maxX, vertex.x);
        minY = std::min(minY, vertex.y);
        maxY = std::max(maxY, vertex.y);
        minZ = std::min(minZ, vertex.z);
        maxZ = std::max(maxZ, vertex.z);
    }

    float centerX = (minX + maxX) / 2.0f;
    float centerY = (minY + maxY) / 2.0f;
    float centerZ = (minZ + maxZ) / 2.0f;

    float scaleX = maxX - minX;
    float scaleY = maxY - minY;
    float scaleZ = maxZ - minZ;
    float scale = std::max({scaleX, scaleY, scaleZ});

    for (auto& vertex : vertices) {
        vertex.x = (vertex.x - centerX) / scale;
        vertex.y = (vertex.y - centerY) / scale;
        vertex.z = (vertex.z - centerZ) / scale;
    }
}

void sortFaces(std::vector<FaceDepth> &faceDepths) {
    std::sort(faceDepths.begin(), faceDepths.end(), compareFaceDepth);
}

void drawPolygon(std::vector<Point3D> vertices, std::vector<std::array<int, 3>> faces) {
    std::vector<Point3D> rotatedVertices(vertices.size());
    for(int i = 0; i < vertices.size(); i++) {
        rotatedVertices[i] = vertices[i];
    }
    static float angle = 0.0f;
    static float time = 0.0f;
    float rotationAxis[3] = {
        sin(time * 0.7f) * cos(time * 0.2f),
        cos(time * 0.2f) * sin(time * 0.4f),
        sin(time * 0.3f) * sin(time * 0.1f)
    };
    float len = sqrt(rotationAxis[0] * rotationAxis[0] +
                    rotationAxis[1] * rotationAxis[1] +
                    rotationAxis[2] * rotationAxis[2]);
    rotationAxis[0] /= len;
    rotationAxis[1] /= len;
    rotationAxis[2] /= len;
    Quaternion rotation = Quaternion::fromAxisAngle(angle,
                                                  rotationAxis[0],
                                                  rotationAxis[1],
                                                  rotationAxis[2]);
    angle += 0.05f;
    time += 0.05f;

    for(int i = 0; i < rotatedVertices.size(); i++) {
        rotation.rotatePoint(rotatedVertices[i]);
    }
    std::vector<FaceDepth> faceDepths(faces.size());
    for(int i = 0; i < faces.size(); i++) {
        float avgZ = 0;
        for(int j = 0; j < 3; j++) {
            avgZ += rotatedVertices[faces[i][j]].z;
        }
        faceDepths[i] = {i, avgZ / 3.0f};
    }
    sortFaces(faceDepths);
    for(int i = 0; i < faces.size(); i++) {
        int faceIdx = faceDepths[i].faceIndex;
        float depth = faceDepths[i].depth;
        float intensity = depth;
        float r = 1-intensity;
        float g = 0.7f + intensity * 0.7f;
        float b = 0.5f - intensity;
        glColor3f(r, g, b);
        glBegin(GL_TRIANGLES);
        for(int j = 0; j < 3; j++) {
            glVertex2f(rotatedVertices[faces[faceIdx][j]].x,
                      rotatedVertices[faces[faceIdx][j]].y);
        }
        glEnd();
    }
}

void drawModelFromOBJ(const std::string& filename) {
    std::vector<Point3D> vertices;
    std::vector<std::array<int, 3>> faces;
    loadOBJ(filename, vertices, faces);
    normalizeModel(vertices);
    drawPolygon(vertices, faces);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void drawIcosahedron() {
    const float phi = (1.0f + sqrt(5.0f)) / 2.0f;
    const float a = 0.4f;
    std::vector<Point3D> v = {
        {0, a, a*phi}, {0, a, -a*phi}, {0, -a, a*phi}, {0, -a, -a*phi},
        {a*phi, 0, a}, {a*phi, 0, -a}, {-a*phi, 0, a}, {-a*phi, 0, -a},
        {a, a*phi, 0}, {-a, a*phi, 0}, {a, -a*phi, 0}, {-a, -a*phi, 0}
    };
    std::vector<std::array<int, 3>> f = {
        {0, 8, 4}, {0, 4, 2}, {0, 2, 6}, {0, 6, 9}, {0, 9, 8},
    {3, 11, 7}, {3, 7, 1}, {3, 1, 5}, {3, 5, 10}, {3, 10, 11},
    {4, 8, 5}, {5, 8, 1}, {8, 9, 1}, {1, 9, 7},
    {7, 9, 6}, {7, 6, 11}, {2, 6, 11}, {2, 4, 10}, {10, 4, 5},
    {11, 10, 2}
    };
    drawPolygon(v, f);
}