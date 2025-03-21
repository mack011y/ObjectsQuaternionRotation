#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

class Quaternion {
private:
    float w, x, y, z;

    void normalize() {
        float len = sqrt(w * w + x * x + y * y + z * z);
        w /= len;
        x /= len;
        y /= len;
        z /= len;
    }

public:
    Quaternion() : w(1), x(0), y(0), z(0) {}
    Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

    Quaternion operator*(const Quaternion& q) const {
        return Quaternion(
            w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w
        );
    }

    Quaternion conjugate() const {
        return Quaternion(w, -x, -y, -z);
    }

    void rotatePoint(float& px, float& py, float& pz) const {
        Quaternion p(0, px, py, pz);
        Quaternion rotated = (*this) * p * this->conjugate();
        px = rotated.x;
        py = rotated.y;
        pz = rotated.z;
    }

    Quaternion operator+(const Quaternion& q) const {
        return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
    }

    Quaternion operator-(const Quaternion& q) const {
        return Quaternion(w - q.w, x - q.x, y - q.y, z - q.z);
    }

    Quaternion operator*(float scalar) const {
        return Quaternion(w * scalar, x * scalar, y * scalar, z * scalar);
    }

    static Quaternion fromAxisAngle(float angle, float ax, float ay, float az) {
        float halfAngle = angle * 0.5f;
        float s = sin(halfAngle);
        float length = sqrt(ax * ax + ay * ay + az * az);
        if (length == 0) return Quaternion();
        ax = ax / length * s;
        ay = ay / length * s;
        az = az / length * s;
        return Quaternion(cos(halfAngle), ax, ay, az);
    }
};

struct FaceDepth {
    int faceIndex;
    float depth;
};

void drawIcosahedron() {
    const float phi = (1.0f + sqrt(5.0f)) / 2.0f;
    const float a = 0.4f;

    static float vertices[12][3] = {
        {0, a, a*phi}, {0, a, -a*phi}, {0, -a, a*phi}, {0, -a, -a*phi},
        {a*phi, 0, a}, {a*phi, 0, -a}, {-a*phi, 0, a}, {-a*phi, 0, -a},
        {a, a*phi, 0}, {-a, a*phi, 0}, {a, -a*phi, 0}, {-a, -a*phi, 0}
    };

    static float rotatedVertices[12][3];
    
    for(int i = 0; i < 12; i++) {
        for(int j = 0; j < 3; j++) {
            rotatedVertices[i][j] = vertices[i][j];
        }
    }

    static float angle = 0.0f;
    static float time = 0.0f;
    
    float rotationAxis[3] = {
        sin(time * 0.3f) * cos(time * 0.2f),
        cos(time * 0.4f) * sin(time * 0.5f),
        sin(time * 0.6f) * sin(time * 0.7f)
    };
    
    float len = sqrt(rotationAxis[0] * rotationAxis[0] +
                    rotationAxis[1] * rotationAxis[1] + 
                    rotationAxis[2] * rotationAxis[2]);
    rotationAxis[0] /= len;
    rotationAxis[1] /= len;
    rotationAxis[2] /= len;

    Quaternion rotation = Quaternion::fromAxisAngle(angle, 
                                                  rotationAxis[0], 
                                                  rotationAxis[1], 
                                                  rotationAxis[2]);
    angle += 0.01f;
    time += 0.01f;  // Увеличиваем время для изменения оси вращения

    for(int i = 0; i < 12; i++) {
        rotation.rotatePoint(rotatedVertices[i][0], rotatedVertices[i][1], rotatedVertices[i][2]);
    }

     int faces[20][3] = {
        {0, 8, 4}, {0, 4, 2}, {0, 2, 6}, {0, 6, 9}, {0, 9, 8},
    {3, 11, 7}, {3, 7, 1}, {3, 1, 5}, {3, 5, 10}, {3, 10, 11},
    {4, 8, 5}, {5, 8, 1}, {8, 9, 1}, {1, 9, 7},
    {7, 9, 6}, {7, 6, 11}, {2, 6, 11}, {2, 4, 10}, {10, 4, 5},
    {11, 10, 2}
    };

    FaceDepth faceDepths[20];
    for(int i = 0; i < 20; i++) {
        float avgZ = 0;
        for(int j = 0; j < 3; j++) {
            avgZ += rotatedVertices[faces[i][j]][2];
        }
        faceDepths[i] = {i, avgZ / 3.0f};
    }

    for(int i = 0; i < 20 - 1; i++) {
        for(int j = 0; j < 20 - 1 - i; j++) {
            if(faceDepths[j].depth > faceDepths[j + 1].depth) {
                FaceDepth temp = faceDepths[j];
                faceDepths[j] = faceDepths[j + 1];
                faceDepths[j + 1] = temp;
            }
        }
    }

    for(int i = 0; i < 20; i++) {
        int faceIdx = faceDepths[i].faceIndex;
        float depth = faceDepths[i].depth;
        
        // Базовый синий цвет
        float intensity = 0.1f + 0.8f * (depth);
        glColor3f(0.0f + 0.2f * intensity,  // Красный компонент
                 0.0f + 0.2f * intensity,  // Зелёный компонент
                 0.4f + 0.6f * intensity); // Синий компонент - доминирующий

        glBegin(GL_TRIANGLES);
        for(int j = 0; j < 3; j++) {
            glVertex2f(rotatedVertices[faces[faceIdx][j]][0], 
                      rotatedVertices[faces[faceIdx][j]][1]);
        }
        glEnd();
    }
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 800, "Rotating Icosahedron", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewInit();

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glColor3f(1.0f, 1.0f, 1.0f);
        drawIcosahedron();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}