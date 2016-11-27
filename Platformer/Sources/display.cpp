#include "display.h"

GLuint shader_program_id;

int SCREEN_W=1920, SCREEN_H=1080;

void display_init(int argc, char** argv)
{
    glutInit(&argc, argv);

    //Paramétrage de la fenêtre
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_W, SCREEN_H);
    glutInitWindowPosition(920,50);
    glutCreateWindow("Platformer");
    glutFullScreen();
    glutSetCursor(GLUT_CURSOR_NONE);

    //Activation de la transparence
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Lancement de la boucle d'affichage
    glutDisplayFunc(display_callback);
}

void shaders_init(std::string vertex_file, std::string fragment_file)
{
    // Chargement du shader
    shader_program_id = read_shader(vertex_file, fragment_file);PRINT_OPENGL_ERROR();
}

void display_callback()
{
    //Effacement
    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    //Configuration 2D
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //Camera
    camera cam = scene1.view;
    glScaled(cam.get_zoom(),cam.get_zoom(),0.0);
    glRotated(cam.get_angle(), 0.0, 0.0, 1.0);
    glTranslated(1-2*cam.get_pos().x/SCREEN_W, -1+2*cam.get_pos().y/SCREEN_H, 0.0);
    glOrtho(0, SCREEN_W, SCREEN_H, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);

    //Affichage de la scène
    scene1.display();

    glFlush();
    //Changement du buffer d'affichage
    glutSwapBuffers();
}







