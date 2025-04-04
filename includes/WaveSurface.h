#pragma once
#include "Point3D.h"
#include <vector>
#include <array>
#include <string>
#include <cmath>
#include <fstream>

class WaveSurface {
public:
    struct Parameters {
        double amplitude = 1.0;    // Высота волн
        double frequency_x = 2.0;  // Частота по оси X
        double frequency_y = 2.0;  // Частота по оси Y
        double phase_shift = 0.0;  // Сдвиг фаз между волнами
        int x_steps = 100;         // Количество шагов по X
        int y_steps = 100;         // Количество шагов по Y
    };

    static bool generate(std::vector<Point3D>& vertices, 
                         std::vector<std::array<int, 3>>& faces, 
                         const Parameters& params);
    
    static bool generateAndSave(const std::string& filename, const Parameters& params);
    
private:
    static std::vector<Point3D> generatePoints(const Parameters& params);
    static std::vector<std::array<int, 3>> generateFaces(int x_steps, int y_steps);
    static bool saveToOBJ(const std::vector<Point3D>& vertices, 
                          const std::vector<std::array<int, 3>>& faces, 
                          const std::string& filename);
}; 