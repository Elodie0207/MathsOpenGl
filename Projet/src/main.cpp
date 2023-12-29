#include <iostream>

#include "Application.hpp"


int main(int argc, char** argv) {
    Log::Init();

    Application* application = new Application(ApplicationSpecification(argc, argv));

    application->Run();

    delete application;
    return 0;
}