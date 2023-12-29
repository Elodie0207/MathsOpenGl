#include <iostream>

#include <GLFW/glfw3.h>
#include <GL/freeglut.h>


enum {
    MENU_OPTION_1,
    MENU_OPTION_2,
    MENU_OPTION_3,
    MENU_OPTION_4,
    MENU_OPTION_5,
    MENU_OPTION_EXIT
};

void menu(int value) {
    switch (value) {
        case MENU_OPTION_1:
            // a faire
            break;
        case MENU_OPTION_2:
            // a faire
            break;
        case MENU_OPTION_3:
            // a faire
            break;
        case MENU_OPTION_4:
            // a faire
            break;
        case MENU_OPTION_5:
            // a faire
            break;
        case MENU_OPTION_EXIT:
            exit(0); // Quitter le programme

    }
    glutPostRedisplay(); // Demande à GLUT de redessiner la fenêtre
}

void createMenu() {
    glutCreateMenu(menu); // Création du menu

    // Ajout des entrées dans le menu
    glutAddMenuEntry("couleurs", MENU_OPTION_1);
    glutAddMenuEntry("decoupage", MENU_OPTION_2);
    glutAddMenuEntry("trace fenetre", MENU_OPTION_3);
    glutAddMenuEntry("fenetrage", MENU_OPTION_4);
    glutAddMenuEntry("remplissage", MENU_OPTION_5);
    glutAddMenuEntry("Exit", MENU_OPTION_EXIT);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(400, 300);
    glutCreateWindow("Dropdown Menu");

    
    glutDisplayFunc(display);
    createMenu();

    glutMainLoop();
    return 0;
}