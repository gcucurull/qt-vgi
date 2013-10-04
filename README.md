qt-vgi
======

Linux (Ubuntu 12.04 32-bits): 
Al compilar el projecte desde Qt Creator es necesita afegir uns paràmetres per linkar correctament les llibreries GLEW y GLUT:
- Obrir CGEnv.pro amb un editor de text
- Afegir la línea "QMAKE_LFLAGS = -lGLEW -lGLU -lGL -lglut" al fitxer. Jo l'he afegit abans de "SOURCES"
- Guardar (no cal dir-ho, però una llista amb només 2 punts queda molt cutre)
