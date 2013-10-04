//******** PRACTICA GRAFICS PER COMPUTADOR 2
//******** Entorn bàsic amb interfície Qt i OpenGL (PFC)
//******** Ferran Poveda, Enric Martí (Juny 2012)
// Model.h - Interface de Model.cpp

#ifndef _MODEL_H
#define _MODEL_H

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif
#endif

#include "Util.h"

class Model
{
public:
	Model();
	~Model();
    void dibuixa();
    void Init_Textures();
    void instancia();

//	bool loadIMA(char* filename, int texID);
    GLuint loadIMA(char* filename);
    void Activa_Textura(int texID);

    int GetObjecte(void);
    void SetObjecte(char mode);
    int GetTextu(void);
    void SetTextu(bool mode);
    int GetVTextu(void);
    void SetVTextu(GLuint VTextur[NUM_MAX_TEXTURES]);
	
private:
    char obj;
    bool textu;
// TEXTURES: Vector de noms de textura
    GLuint textures[NUM_MAX_TEXTURES];

// Truck
   void truck(bool textu);
   void neumatic(bool textur);
   void llanta(bool textur);
   void sea(void);
};
#endif /* _MODEL_H */
