    #ifndef MOBIUSSTRIP_H
#define MOBIUSSTRIP_H

#include <vector>
#include <string>
#include <cmath>
#include "Point3D.h"

class MobiusStrip {
public:
    struct Parameters {
        Parameters() : 
            alpha_min(1.0), alpha_max(3.0), alpha_step(0.1),
            beta_min(0.0), beta_max(4.0), beta_step(0.1),
            u_steps(250), v_steps(200) {}
            
        double alpha_min;
        double alpha_max;
        double alpha_step;
        double beta_min;
        double beta_max;
        double beta_step;
        int u_steps;
        int v_steps;
    };

    static bool generateAndSave(const std::string& filename, const Parameters& params = Parameters());
    static bool generate(std::vector<Point3D>& vertices, std::vector<std::array<int, 3>>& faces, const Parameters& params = Parameters());

private:
    static std::vector<Point3D> generatePoints(const Parameters& params);
    static std::vector<std::array<int, 3>> generateFaces(int u_steps, int v_steps);
    static bool saveToOBJ(const std::vector<Point3D>& vertices, const std::vector<std::array<int, 3>>& faces, const std::string& filename);
};

#endif // MOBIUSSTRIP_H 