#pragma once
#include "Point3D.h"
#include <vector>
#include <array>
#include <string>
#include <cmath>
#include <fstream>

class SeashellSurface {
public:
    struct Parameters {
        double alpha = 0.1;
        double beta = 0.1;
        int u_steps = 50;
        int v_steps = 150;
    };

    static bool generate(std::vector<Point3D>& vertices, 
                         std::vector<std::array<int, 3>>& faces, 
                         const Parameters& params);
    
    static bool generateAndSave(const std::string& filename, const Parameters& params);
    
private:
    static std::vector<Point3D> generatePoints(const Parameters& params);
    static std::vector<std::array<int, 3>> generateFaces(int u_steps, int v_steps);
    static bool saveToOBJ(const std::vector<Point3D>& vertices, 
                          const std::vector<std::array<int, 3>>& faces, 
                          const std::string& filename);
}; 