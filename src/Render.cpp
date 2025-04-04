#include "../includes/Render.h"
#include "../includes/ModelLoader.h"
#include <algorithm>
#include <OpenGL/gl.h>

bool compareFaceDepth(const FaceDepth& a, const FaceDepth& b) {
    return a.depth < b.depth;
}

void sortFaces(std::vector<FaceDepth>& faceDepths) {
    std::sort(faceDepths.begin(), faceDepths.end(), compareFaceDepth);
}

void drawPoint(float x, float y) {
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

void drawLine(float x1, float y1, float x2, float y2) {
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void drawTriangle(const Point3D& v1, const Point3D& v2, const Point3D& v3, float r, float g, float b, float brightness) {
    glColor3f(r*brightness, g*brightness, b*brightness);
    glBegin(GL_TRIANGLES);
    glVertex2f(v1.x, v1.y);
    glVertex2f(v2.x, v2.y);
    glVertex2f(v3.x, v3.y);
    glEnd();
}

void rotatePoint(Point3D& point, const Point3D& axis, float angle) {
    if (std::abs(axis.x) < 1e-6 && std::abs(axis.y) < 1e-6 && std::abs(axis.z) < 1e-6) {
        return; 
    }
    
    Point3D normalizedAxis = axis;
    float length = sqrt(axis.x*axis.x + axis.y*axis.y + axis.z*axis.z);
    if (std::abs(length - 1.0f) > 1e-6) {
        normalizedAxis.x /= length;
        normalizedAxis.y /= length;
        normalizedAxis.z /= length;
    }
    
    Quaternion rotation = Quaternion::fromAxisAngle(angle, normalizedAxis.x, normalizedAxis.y, normalizedAxis.z);
    rotation.rotatePoint(point);
}

void rotatePoints(std::vector<Point3D>& vertices, const Point3D& axis, float angle) {
    if (vertices.empty()) return;
    for (auto& vertex : vertices) {
        rotatePoint(vertex, axis, angle);
    }
}

bool normalizeModel(std::vector<Point3D>& vertices) {
    if (vertices.empty()) return false;
    Point3D min = vertices[0];
    Point3D max = vertices[0];
    for (const auto& vertex : vertices) {
        min.x = std::min(min.x, vertex.x);
        min.y = std::min(min.y, vertex.y);
        min.z = std::min(min.z, vertex.z);
        max.x = std::max(max.x, vertex.x);
        max.y = std::max(max.y, vertex.y);
        max.z = std::max(max.z, vertex.z);
    }
    float size = std::max({max.x - min.x, max.y - min.y, max.z - min.z});
    if (size < 1e-6) return false;
    Point3D center{
        (min.x + max.x) / 2.0f,
        (min.y + max.y) / 2.0f,
        (min.z + max.z) / 2.0f
    };
    float scale = 3.0f / size;
    for (auto& vertex : vertices) {
        vertex.x = (vertex.x - center.x) * scale;
        vertex.y = (vertex.y - center.y) * scale;
        vertex.z = (vertex.z - center.z) * scale;
    }
    return true;
}

class Camera {
public:
    Point3D position;
    Point3D direction;
    Camera(const Point3D& pos, const Point3D& dir) : position(pos), direction(dir) {}

    void setPosition(const Point3D& pos) { position = pos; }

    void setDirection(const Point3D& dir) { direction = dir; }
};

float calculateDepthToLight(const Point3D& point, const Point3D& light) {
    return point.distanceTo(light);
}

std::vector<FaceDepth> g_faceDepths;

void display(std::vector<Point3D>& vertices, const std::vector<std::array<int, 3> >& faces, const Point3D& light) {
    if (vertices.empty() || faces.empty()) return;
    
    if (g_faceDepths.size() != faces.size()) {
        g_faceDepths.resize(faces.size());
    }
    
    for (size_t i = 0; i < faces.size(); ++i) {
        const auto& face = faces[i];
        float avgZ = (vertices[face[0]].z + vertices[face[1]].z + vertices[face[2]].z) / 3.0f;
        
        g_faceDepths[i].faceIndex = i;
        g_faceDepths[i].depth = avgZ;
    }
    
    std::sort(g_faceDepths.begin(), g_faceDepths.end(), 
        [](const FaceDepth& a, const FaceDepth& b) { return a.depth < b.depth; });
    
    Point3D viewDir(0.0f, 0.0f, 1.0f);
    
    for (const auto& fd : g_faceDepths) {
        const auto& face = faces[fd.faceIndex];
        
        Point3D v1 = vertices[face[0]];
        Point3D v2 = vertices[face[1]];
        Point3D v3 = vertices[face[2]];
        
        Point3D edge1(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
        Point3D edge2(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);
        
        Point3D normal(
            edge1.y * edge2.z - edge1.z * edge2.y,
            edge1.z * edge2.x - edge1.x * edge2.z,
            edge1.x * edge2.y - edge1.y * edge2.x
        );
        
        float length = sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
        if (length > 1e-6) {
            normal.x /= length;
            normal.y /= length;
            normal.z /= length;
        }
        
        Point3D center((v1.x + v2.x + v3.x) / 3.0f, 
                       (v1.y + v2.y + v3.y) / 3.0f,
                       (v1.z + v2.z + v3.z) / 3.0f);
                       
        // Убедимся, что нормаль направлена к камере для правильного затенения
        // Проверка, направлена ли нормаль от камеры
        float dotWithViewDir = normal.x * viewDir.x + normal.y * viewDir.y + normal.z * viewDir.z;
        if (dotWithViewDir < 0) {
            // Если нормаль смотрит от камеры, инвертируем её
            normal.x = -normal.x;
            normal.y = -normal.y;
            normal.z = -normal.z;
        }
        
        // Направление к источнику света
        Point3D toLight(light.x - center.x, light.y - center.y, light.z - center.z);
        
        // Нормализуем вектор к свету
        length = sqrt(toLight.x*toLight.x + toLight.y*toLight.y + toLight.z*toLight.z);
        if (length > 1e-6) {
            toLight.x /= length;
            toLight.y /= length;
            toLight.z /= length;
        }
        
        // Скалярное произведение для косинуса угла между нормалью и направлением к свету
        float dot = normal.x * toLight.x + normal.y * toLight.y + normal.z * toLight.z;
        
        // Ограничиваем значение от 0 до 1
        float diffuse = std::max(0.0f, dot);
        
        // Определяем амбиентное освещение (фоновое)
        float ambient = 0.1f;
        
        // Яркость с учетом амбиентного и диффузного освещения
        float brightness = ambient + (1.0f - ambient) * diffuse;
        
        // Базовый цвет (можно менять в зависимости от модели)
        float r = 0.8f;
        float g = 0.4f;
        float b = 0.7f;
        
        // Добавляем блики для более глянцевых поверхностей
        if (diffuse > 0.7f) {
            // Усиливаем яркость для областей, которые больше повернуты к свету
            float specular = pow(diffuse, 8.0f) * 0.5f;
            r = std::min(1.0f, r + specular);
            g = std::min(1.0f, g + specular);
            b = std::min(1.0f, b + specular);
        }
        
        drawTriangle(v1, v2, v3, r, g, b, brightness);
    }
}