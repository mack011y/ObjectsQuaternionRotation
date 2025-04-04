#pragma once
#include "Point3D.h"
#include <vector>
#include <array>
#include <string>
#include <cmath>
#include <fstream>

class HelicalSurface {
public:
    struct Parameters {
        double alpha = 1.0;
        double beta = 1.0;
        int u_steps = 200;
        int v_steps = 200;
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