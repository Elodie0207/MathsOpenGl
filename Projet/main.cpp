#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main() {
    // Initialisation de GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configuration GLFW pour utiliser OpenGL Core Profile 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Création d'une fenêtre GLFW
    GLFWwindow* window = glfwCreateWindow(800, 600, "Fenêtre", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Activation du contexte de la fenêtre
    glfwMakeContextCurrent(window);

    // Initialisation de GLEW (après avoir créé le contexte OpenGL)
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Boucle principale
    while (!glfwWindowShouldClose(window)) {
        // Vérification des événements
        glfwPollEvents();

        // Effacement du buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap des buffers
        glfwSwapBuffers(window);
    }

    // Fermeture de la fenêtre
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}