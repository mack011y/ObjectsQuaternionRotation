#include"../includes/ModelLoader.h"
#include<iostream>
#include<fstream>
#include<sstream>

bool loadOBJ(const std::string& filename, std::vector<Point3D>& vertices, std::vector<std::array<int, 3> >& faces) {
    vertices.clear();
    faces.clear();
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: File not found: " << filename << std::endl;
        return false;
    }

    std::string line;
    int lineNum = 0;
    try {
        while (std::getline(file, line)) {
            lineNum++;
            std::stringstream ss(line);
            std::string type;
            ss >> type;
            
            if (type == "v") {
                float x, y, z;
                if (!(ss >> x >> y >> z)) {
                    std::cerr << "Error: Invalid vertex data at line " << lineNum << std::endl;
                    return false;
                }
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
                    try {
                        int idx = std::stoi(vertexStr) - 1;
                        if (idx >= 0 && idx < static_cast<int>(vertices.size())) {
                            faceVertices.push_back(idx);
                        } else {
                            std::cerr << "Error: Invalid vertex index at line " << lineNum << std::endl;
                            return false;
                        }
                    } catch (const std::exception& e) {
                        std::cerr << "Error: Invalid face format at line " << lineNum << std::endl;
                        return false;
                    }
                }
                
                if (faceVertices.size() < 3) {
                    std::cerr << "Error: Face has less than 3 vertices at line " << lineNum << std::endl;
                    return false;
                }
                
                // Triangulate the face
                for (size_t i = 1; i < faceVertices.size() - 1; ++i) {
                    std::array<int, 3> triangle = {faceVertices[0], faceVertices[i], faceVertices[i + 1]};
                    faces.push_back(triangle);
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Failed to parse file at line " << lineNum << ": " << e.what() << std::endl;
        return false;
    }

    if (vertices.empty() || faces.empty()) {
        std::cerr << "Error: No valid vertices or faces found in file" << std::endl;
        return false;
    }

    std::cout << "Successfully loaded model with " << vertices.size() << " vertices and " 
              << faces.size() << " triangulated faces" << std::endl;
    return true;
}