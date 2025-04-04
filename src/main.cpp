#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <GLUT/glut.h>
#include <cmath>
#include "../includes/Point3D.h"
#include "../includes/ModelLoader.h"
#include "../includes/Render.h"
#include "../includes/MobiusStrip.h"
#include "../includes/SpiralSurface.h"
#include "../includes/HelicalSurface.h"
#include "../includes/SeashellSurface.h"
#include "../includes/WaveSurface.h"

// Глобальные переменные для хранения модели
std::vector<Point3D> g_vertices;
std::vector<Point3D> g_originalVertices;
std::vector<Point3D> g_rotatedVertices; // Буфер для повернутых вершин
std::vector<std::array<int, 3> > g_faces;

// Точки для осей координат
std::vector<Point3D> g_axisPoints = {
    Point3D(0.0f, 0.0f, 0.0f),  // Начало координат
    Point3D(2.0f, 0.0f, 0.0f),  // Конец оси X
    Point3D(0.0f, 2.0f, 0.0f),  // Конец оси Y
    Point3D(0.0f, 0.0f, 2.0f)   // Конец оси Z
};
std::vector<Point3D> g_originalAxisPoints;
std::vector<Point3D> g_rotatedAxisPoints; // Буфер для повернутых осей

// Параметры сцены
const Point3D LIGHT_POSITION(3.0f, 2.0f, 5.0f);   // Свет сбоку и сверху для лучших теней
// Нормализованная ось вращения для равномерной скорости
const Point3D ROTATION_AXIS(0.45f, 0.89f, 0.09f); // Нормализованный вектор (примерно то же направление)

float g_angle = 0.0f;
// Контролируем скорость вращения
const float ROTATION_SPEED = 0.01f;


void clear_console() {
    system("clear");
}


void drawRotatedAxes(const std::vector<Point3D>& axisPoints) {
    glLineWidth(4.0f);
    // Ось X
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(axisPoints[0].x, axisPoints[0].y);
    glVertex2f(axisPoints[1].x, axisPoints[1].y);
    glEnd();
    // Ось Y
    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(axisPoints[0].x, axisPoints[0].y);
    glVertex2f(axisPoints[2].x, axisPoints[2].y);
    glEnd();
    // Ось Z (только для наглядности)
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(axisPoints[0].x, axisPoints[0].y);
    glVertex2f(axisPoints[3].x, axisPoints[3].y);
    glEnd();
}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Копируем исходные данные в буферы только при первом вызове
    if (g_rotatedVertices.empty()) {
        g_rotatedVertices = g_originalVertices;
        g_rotatedAxisPoints = g_originalAxisPoints;
    } else {
        // В последующих вызовах копируем исходные данные
        std::copy(g_originalVertices.begin(), g_originalVertices.end(), g_rotatedVertices.begin());
        std::copy(g_originalAxisPoints.begin(), g_originalAxisPoints.end(), g_rotatedAxisPoints.begin());
    }

    // Поворачиваем буферы
    rotatePoints(g_rotatedAxisPoints, ROTATION_AXIS, g_angle);
    rotatePoints(g_rotatedVertices, ROTATION_AXIS, g_angle);
    
    // Отображаем данные из буферов
    drawRotatedAxes(g_rotatedAxisPoints);
    display(g_rotatedVertices, g_faces, LIGHT_POSITION);
    
    g_angle += ROTATION_SPEED;
    // Сбрасываем угол после полного оборота, чтобы предотвратить замедление
    if (g_angle > 2.0f * M_PI) {
        g_angle -= 2.0f * M_PI;
    }
    
    glutSwapBuffers();
}

void update(int value) {
    glutPostRedisplay();
    glutTimerFunc(10, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    float aspect = (float)w / (float)h;
    float size = 2.0f;
    
    if (w <= h) {
        gluOrtho2D(-size, size, -size/aspect, size/aspect);
    } else {
        gluOrtho2D(-size*aspect, size*aspect, -size, size);
    }
    
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    clear_console();
    std::vector<std::string> examples;
    examples.push_back("/Users/mack011y/CLionProjects/rotatingFig/assets/Batarang-bl.obj");
    examples.push_back("/Users/mack011y/CLionProjects/rotatingFig/assets/Christ the Redeemer-bl.obj");
    examples.push_back("/Users/mack011y/CLionProjects/rotatingFig/assets/Skeleton With Futuristic Minigun-bl.obj");
    examples.push_back("/Users/mack011y/CLionProjects/rotatingFig/assets/Hand-bl.obj");
    examples.push_back("/Users/mack011y/CLionProjects/rotatingFig/assets/Velociraptor.obj");
    examples.push_back("/Users/mack011y/CLionProjects/rotatingFig/assets/cat.obj");
    examples.push_back("mobius");
    examples.push_back("spiral");
    examples.push_back("helical");
    examples.push_back("seashell");
    examples.push_back("wave");
    
    std::cout << "Available models:\n"
              << "0: Batarang\n"
              << "1: Christ the Redeemer\n"
              << "2: Skeleton With Futuristic Minigun\n"
              << "3: Hand\n"
              << "4: Velociraptor\n"
              << "5: Cat\n"
              << "6: Mobius Strip\n"
              << "7: Spiral Surface\n"
              << "8: Helical Surface\n"
              << "9: Seashell Surface\n"
              << "10: Wave Surface\n"
              << "Enter model number: ";
              
    int idExample; 
    std::cin >> idExample;
    
    if (idExample < 0 || idExample >= examples.size()) {
        std::cerr << "Error: Invalid model ID" << std::endl;
        return 1;
    }

    if (idExample == 6) {
        MobiusStrip::Parameters params;
        if (!MobiusStrip::generate(g_vertices, g_faces, params)) {
            std::cerr << "Failed to generate Mobius strip" << std::endl;
            return 1;
        }
    } else if (idExample == 7) {
        SpiralSurface::Parameters params;
        if (!SpiralSurface::generate(g_vertices, g_faces, params)) {
            std::cerr << "Failed to generate Spiral surface" << std::endl;
            return 1;
        }
    } else if (idExample == 8) {
        HelicalSurface::Parameters params;
        if (!HelicalSurface::generate(g_vertices, g_faces, params)) {
            std::cerr << "Failed to generate Helical surface" << std::endl;
            return 1;
        }
    } else if (idExample == 9) {
        SeashellSurface::Parameters params;
        if (!SeashellSurface::generate(g_vertices, g_faces, params)) {
            std::cerr << "Failed to generate Seashell surface" << std::endl;
            return 1;
        }
    } else if (idExample == 10) {
        WaveSurface::Parameters params;
        if (!WaveSurface::generate(g_vertices, g_faces, params)) {
            std::cerr << "Failed to generate Wave surface" << std::endl;
            return 1;
        }
    } else {
        std::cout << "Loading model: " << examples[idExample] << std::endl;
        if (!loadOBJ(examples[idExample], g_vertices, g_faces)) {
            std::cerr << "Failed to load model" << std::endl;
            return 1;
        }
    }
    
    if (g_vertices.empty() || g_faces.empty()) {
        std::cerr << "Error: Model has no vertices or faces" << std::endl;
        return 1;
    }
    
    std::cout << "Model loaded successfully. Vertices: " << g_vertices.size() 
              << ", Faces: " << g_faces.size() << std::endl;

    if (!normalizeModel(g_vertices)) {
        std::cerr << "Failed to normalize model" << std::endl;
        return 1;
    }
    g_originalAxisPoints = g_axisPoints;
    g_originalVertices = g_vertices;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1920, 1080);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("3D Model Viewer");
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glutDisplayFunc(renderScene);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);
    std::cout << "\nPress ESC to exit\n";
    glutMainLoop();
    return 0;
}