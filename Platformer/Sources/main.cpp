#include <iostream>

#include "display.h"
#include "controls.h"
#include "scene.h"


Scene scene1;

int CALLBACK_TIME = 10;

static void timer_callback(int)
{
    glutTimerFunc(CALLBACK_TIME, timer_callback, 0);

    if(scene1.refresh() == true)
        scene1 = Scene();

    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    //Initialisation de l'affichage
    display_init(argc, argv);
    //Fonctions de gestion du clavier et souris
    control_init();
    //Fonction d'appel d'affichage en chaine
    glutTimerFunc(CALLBACK_TIME, timer_callback, 0);
    //Initialisation des fonctions OpenGL
    glewInit();

    //Fonction de gestion du clavier
    control_init();

    //Lancement de la boucle principale
    scene1 = Scene(true);
    glutMainLoop();

    return 0;
}

