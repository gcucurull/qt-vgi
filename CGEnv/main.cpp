//******** PRACTICA GRAFICS PER COMPUTADOR 2
//******** Entorn basic amb interficie Qt i OpenGL (PFC)
//******** Ferran Poveda, Enric Marti (Juny 2012)
// main.cpp: Funció main que arrenca l'aplicació

#include <QApplication>
#include "cgenvwindow.h"

#ifdef __linux__
#include <GL/glut.h>
#endif

int main(int argc, char *argv[])
{
    #ifdef __linux__
    glutInit(&argc,argv);
    #endif

    QApplication a(argc, argv);
    CGEnvWindow w;
    w.show();

    return a.exec();
}
