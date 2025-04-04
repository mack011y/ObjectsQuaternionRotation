#include "../includes/MobiusStrip.h"
#include <fstream>
#include <iostream>

bool MobiusStrip::generateAndSave(const std::string& filename, const Parameters& params) {
    std::vector<Point3D> vertices;
    std::vector<std::array<int, 3>> faces;
    
    if (!generate(vertices, faces, params)) {
        return false;
    }
    
    return saveToOBJ(vertices, faces, filename);
}

bool MobiusStrip::generate(std::vector<Point3D>& vertices, std::vector<std::array<int, 3>>& faces, const Parameters& params) {
    vertices = generatePoints(params);
    if (vertices.empty()) {
        return false;
    }
    
    faces = generateFaces(params.u_steps, params.v_steps);
    return !faces.empty();
}

std::vector<Point3D> MobiusStrip::generatePoints(const Parameters& params) {
    std::vector<Point3D> points;
    const double PI = 3.14159265358979323846;
    
    // Генерируем точки поверхности
    for (int i = 0; i <= params.u_steps; ++i) {
        double u = 2.0 * PI * i / params.u_steps;
        
        for (int j = 0; j <= params.v_steps; ++j) {
            double v = -0.5 + (double)j / params.v_steps;
            
            // Параметрические уравнения ленты Мёбиуса
            double x = (1 + v * cos(u/2)) * cos(u);
            double y = (1 + v * cos(u/2)) * sin(u);
            double z = v * sin(u/2);
            
            // Масштабируем для лучшей видимости
            points.push_back(Point3D(x * 0.5, y * 0.5, z * 0.5));
        }
    }
    
    return points;
}

std::vector<std::array<int, 3>> MobiusStrip::generateFaces(int u_steps, int v_steps) {
    std::vector<std::array<int, 3>> faces;
    
    // Генерируем треугольники
    for (int i = 0; i < u_steps; ++i) {
        for (int j = 0; j < v_steps; ++j) {
            int current = i * (v_steps + 1) + j;
            int next = ((i + 1) % u_steps) * (v_steps + 1) + j;
            
            // Первый треугольник
            faces.push_back({current, next, current + 1});
            
            // Второй треугольник
            faces.push_back({current + 1, next, next + 1});
        }
    }
    return faces;
}

bool MobiusStrip::saveToOBJ(const std::vector<Point3D>& vertices, 
                           const std::vector<std::array<int, 3>>& faces, 
                           const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    // Записываем вершины
    for (const auto& v : vertices) {
        file << "v " << v.x << " " << v.y << " " << v.z << "\n";
    }
    
    // Записываем грани (в OBJ индексы начинаются с 1)
    for (const auto& f : faces) {
        file << "f " << (f[0] + 1) << " " << (f[1] + 1) << " " << (f[2] + 1) << "\n";
    }
    
    return true;
} 