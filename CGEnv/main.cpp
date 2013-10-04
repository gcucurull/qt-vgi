//******** PRACTICA GRAFICS PER COMPUTADOR 2
//******** Entorn basic amb interficie Qt i OpenGL (PFC)
//******** Ferran Poveda, Enric Marti (Juny 2012)
// main.cpp: Funció main que arrenca l'aplicació

#include <QApplication>
#include "cgenvwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CGEnvWindow w;
    w.show();
    
    return a.exec();
}
