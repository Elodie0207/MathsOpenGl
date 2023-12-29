#include <iostream>

#include "Application.hpp"

#include <GLFW/glfw3.h>
#include <GL/freeglut.h>


int main(int argc, char** argv) {
    Log::Init();

    Application* application = new Application(ApplicationSpecification(argc, argv));

    application->Run();

    delete application;
    return 0;
}