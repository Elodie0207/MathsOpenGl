#include <iostream>

#include <GLFW/glfw3.h>
#include <GL/freeglut.h>


void display() {
    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer

    // Draw something here
    // Example: Draw a triangle
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0); // Set color to red
    glVertex2f(-0.5, -0.5);
    glColor3f(0.0, 1.0, 0.0); // Set color to green
    glVertex2f(0.5, -0.5);
    glColor3f(0.0, 0.0, 1.0); // Set color to blue
    glVertex2f(0.0, 0.5);
    glEnd();

    glFlush(); // Flush the rendering pipeline
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); // Initialize GLUT
    glutCreateWindow("Simple Window"); // Create a window with the given title
    glutDisplayFunc(display); // Set the display callback function
    glutMainLoop(); // Enter the GLUT event processing loop

    return 0;
}