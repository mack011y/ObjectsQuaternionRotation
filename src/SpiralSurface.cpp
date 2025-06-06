#include "../includes/SpiralSurface.h"
#include <fstream>

bool SpiralSurface::generate(std::vector<Point3D>& vertices, 
                            std::vector<std::array<int, 3>>& faces, 
                            const Parameters& params) {
    vertices = generatePoints(params);
    if (vertices.empty()) return false;
    
    faces = generateFaces(params.u_steps, params.v_steps);
    return true;
}

bool SpiralSurface::generateAndSave(const std::string& filename, const Parameters& params) {
    std::vector<Point3D> vertices;
    std::vector<std::array<int, 3>> faces;
    
    vertices = generatePoints(params);
    if (vertices.empty()) return false;
    
    faces = generateFaces(params.u_steps, params.v_steps);
    return saveToOBJ(vertices, faces, filename);
}

std::vector<Point3D> SpiralSurface::generatePoints(const Parameters& params) {
    std::vector<Point3D> points;
    const double PI = 3.14159265358979323846;
    
    for (int i = 0; i <= params.u_steps; ++i) {
        double u = 10 * PI * i / params.u_steps;
        for (int j = 0; j <= params.v_steps; ++j) {
            double v = 2 * PI * j / params.v_steps;
            
            double x = (params.alpha + params.beta * cos(v)) * cos(u);
            double y = (params.alpha + params.beta * cos(v)) * sin(u);
            double z = params.beta * sin(v) + params.alpha * u / (4 * PI);
            
            points.push_back(Point3D(x, y, z));
        }
    }
    return points;
}

std::vector<std::array<int, 3>> SpiralSurface::generateFaces(int u_steps, int v_steps) {
    std::vector<std::array<int, 3>> faces;
    int rows = u_steps + 1;
    int cols = v_steps + 1;
    
    for (int i = 0; i < u_steps; ++i) {
        for (int j = 0; j < v_steps; ++j) {
            int p1 = i * cols + j;
            int p2 = (i + 1) * cols + j;
            int p3 = (i + 1) * cols + (j + 1);
            int p4 = i * cols + (j + 1);
            
            faces.push_back({p1, p2, p3});
            faces.push_back({p1, p3, p4});
        }
    }
    
    return faces;
}

bool SpiralSurface::saveToOBJ(const std::vector<Point3D>& vertices, 
                             const std::vector<std::array<int, 3>>& faces, 
                             const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    
    // Write vertices
    for (const auto& v : vertices) {
        file << "v " << v.x << " " << v.y << " " << v.z << "\n";
    }
    
    // Write faces (OBJ uses 1-indexed vertices)
    for (const auto& f : faces) {
        file << "f " << (f[0] + 1) << " " << (f[1] + 1) << " " << (f[2] + 1) << "\n";
    }
    
    file.close();
    return true;
} 