//
// Created by Matvey Zhilyaev on 27.03.2025.
//

#ifndef RENDER_H
#define RENDER_H

#include <vector>
#include <array>
#include "Point3D.h"
#include "Quaternion.h"

// Структура для хранения глубины грани
struct FaceDepth {
    int faceIndex;
    float depth;
};

// Базовые функции рисования (используют только базовый OpenGL)
void drawPoint(float x, float y);
void drawLine(float x1, float y1, float x2, float y2);
void fillTriangle(float x1, float y1, float x2, float y2, float x3, float y3);

// Функции рисования объектов
void drawTriangle(const Point3D& v1, const Point3D& v2, const Point3D& v3, float r, float g, float b, float brightness);
void drawFaces(const std::vector<Point3D>& vertices, const std::vector<std::array<int, 3> >& faces);

// Функции для работы с точками
void rotatePoint(Point3D& point, const Point3D& axis, float angle);
void rotatePoints(std::vector<Point3D>& vertices, const Point3D& axis, float angle);
bool normalizeModel(std::vector<Point3D>& vertices);

// Вспомогательные функции
float calculateFaceDepth(const std::vector<Point3D>& vertices, const std::array<int, 3>& face);

// Основная функция отрисовки
void display(std::vector<Point3D>& vertices, const std::vector<std::array<int, 3> >& faces, const Point3D& light);

#endif //RENDER_H
