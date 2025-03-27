#include"../includes/ModelLoader.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include"../includes/ModelLoader.h"

void loadOBJ(const std::string& filename, std::vector<Point3D>& vertices, std::vector<std::array<int, 3>>& faces) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: File not found: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string type;
        ss >> type;
        if (type == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            vertices.push_back(Point3D(x, y, z));
        }
        else if (type == "f") {
            std::vector<int> faceVertices;
            std::string vertexStr;

            while (ss >> vertexStr) {
                size_t pos = vertexStr.find('/');
                if (pos != std::string::npos) {
                    vertexStr = vertexStr.substr(0, pos);
                }
                int idx = std::stoi(vertexStr) - 1;
                if (idx >= 0 && idx < static_cast<int>(vertices.size())) {
                    faceVertices.push_back(idx);
                }
            }
            if (faceVertices.size() >= 3) {
                for (size_t i = 1; i < faceVertices.size() - 1; ++i) {
                    faces.push_back({
                        faceVertices[0],
                        faceVertices[i],
                        faceVertices[i + 1]
                    });
                }
            }
        }
    }
}