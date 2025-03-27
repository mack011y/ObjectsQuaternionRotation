#include <iostream>
#include <vector>
#include <string>
#include "../includes/Render.h"

void clear_console() {
    system("clear");
}

int main() {
    clear_console();
    std::vector<std::string> examples = {
        "/Users/mack011y/CLionProjects/rotatingFig/assets/Batarang-bl.obj",
        "/Users/mack011y/CLionProjects/rotatingFig/assets/Christ the Redeemer-bl.obj",
        "/Users/mack011y/CLionProjects/rotatingFig/assets/Skeleton With Futuristic Minigun-bl.obj",
        "/Users/mack011y/CLionProjects/rotatingFig/assets/Hand-bl.obj",
        "/Users/mack011y/CLionProjects/rotatingFig/assets/Velociraptor.obj"
    };
    std::cout << "0: Batarang\n"
                 "1: Christ the Redeemer\n"
                 "2: Skeleton With Futuristic Minigu\n"
                 "3: Hand\n"
                 "4: Velociraptor\n"
                 "5: Icosahedron\n";
    int idExample; std::cin >> idExample;
    if (idExample > 5) {
        std::cout << "INVALID ID" << std::endl;
        return 0;
    }
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1000, 1000, "Rotating Polygon", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewInit();

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glColor3f(1.0f, 1.0f, 1.0f);
        if (idExample == 5) {
            drawIcosahedron();
        } else {
            drawModelFromOBJ(examples[idExample]);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}