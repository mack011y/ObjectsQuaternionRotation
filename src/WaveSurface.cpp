#include "../includes/WaveSurface.h"
#include <fstream>

bool WaveSurface::generate(std::vector<Point3D>& vertices, 
                           std::vector<std::array<int, 3>>& faces, 
                           const Parameters& params) {
    vertices = generatePoints(params);
    if (vertices.empty()) return false;
    
    faces = generateFaces(params.x_steps, params.y_steps);
    return true;
}

bool WaveSurface::generateAndSave(const std::string& filename, const Parameters& params) {
    std::vector<Point3D> vertices;
    std::vector<std::array<int, 3>> faces;
    
    vertices = generatePoints(params);
    if (vertices.empty()) return false;
    
    faces = generateFaces(params.x_steps, params.y_steps);
    return saveToOBJ(vertices, faces, filename);
}

std::vector<Point3D> WaveSurface::generatePoints(const Parameters& params) {
    std::vector<Point3D> points;
    const double PI = 3.14159265358979323846;
    
    // Создаем сетку точек
    for (int i = 0; i <= params.x_steps; ++i) {
        double x = -2.0 + 4.0 * i / params.x_steps; 
        
        for (int j = 0; j <= params.y_steps; ++j) {
            double y = -2.0 + 4.0 * j / params.y_steps;
            
            // Основная волна: сумма синусоид по X и Y с разными фазами
            double wave1 = sin(x * params.frequency_x * PI);
            double wave2 = cos(y * params.frequency_y * PI + params.phase_shift);
            
            // Интерференция волн: сложная волновая картина
            double wave3 = sin((x + y) * params.frequency_x * 0.7);
            double wave4 = cos((x - y) * params.frequency_y * 0.7 + params.phase_shift);
            
            // Комбинируем волны с затуханием к краям
            double edge_damping = (1.0 - 0.3 * (x*x + y*y));
            double z = params.amplitude * (wave1 * 0.4 + wave2 * 0.3 + wave3 * 0.2 + wave4 * 0.1) * edge_damping;
            
            points.push_back(Point3D(x, y, z));
        }
    }
    
    return points;
}

std::vector<std::array<int, 3>> WaveSurface::generateFaces(int x_steps, int y_steps) {
    std::vector<std::array<int, 3>> faces;
    int cols = y_steps + 1;
    
    for (int i = 0; i < x_steps; ++i) {
        for (int j = 0; j < y_steps; ++j) {
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

bool WaveSurface::saveToOBJ(const std::vector<Point3D>& vertices, 
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