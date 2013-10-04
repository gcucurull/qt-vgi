//******** PRACTICA GRAFICS PER COMPUTADOR 2
//******** Entorn basic amb interfi­cie Qt i OpenGL (PFC)
//******** Ferran Poveda, Enric Marti (Juny 2012)
// util.h : Definiciio de constants utils per l'entorn

#ifndef _UTIL_H
#define _UTIL_H

/* ------------------------------------------------------------------------- */
/*                                INCLUDES                                   */
/* ------------------------------------------------------------------------- */

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glew.h>
//#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#include <QtOpenGL/QGLWidget>

// Entorn PFC: Includes per lectura fitxers, funcions trigonomÃštriques i nombres aleatoris.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <QTimer>

// Entorn PFC: Includes de Qt
#include <QtGlobal>
#include <QFileDialog>

/* ------------------------------------------------------------------------- */
/*                                CONSTANTS                                  */
/* ------------------------------------------------------------------------- */

//--------------- Entorn PFC: Tipus de Projeccio
#define CAP ' '
#define ORTO 'O'
#define PERSPECT 'P'

//--------------- Entorn PFC: Tipus de Polars (per la Visualitzacio Interactiva)
#define POLARZ 'Z'
#define POLARY 'Y'
#define POLARX 'X'

//-------------- Entorn PFC: Tipus d'Objectes
#define CUB 'c'
#define ESFERA 'e'
#define MALLA 'M'
#define TETERA 't'
#define TRUCK 'T'
#define OBJ3DS '3'		// Objecte format 3DS
#define OBJOBJ '4'		// Objecte format OBJ

//-------------- Entorn PFC: Tipus d'Iluminacio
#define FILFERROS 'f'
#define PLANA 'p'
#define SUAU 's'

//-------------- Entorn PFC: Objectes precompilats OpenGL amb glNewList
#define EIXOS 1			// Eixos
#define FRACTAL 2		// Fractal
#define OBJECTE3DS 3	// Objecte 3DS sense textures
#define OBJECTE3DS_T 4	// Objecte 3DS amb textures
#define OBJECTEOBJ 5	// Objecte OBJ sense textura
#define OBJECTEOBJ_T 6	// Objecte OBJ amb textura

// DefiniciÃ³ dels valors del pla near i far del Volum de
// VisualitzaciÃ³ en Perspectiva
const int p_near=1;
const int p_far=500000;

// -------------- Entorn PFC: TEXTURES: Nombre maxim de textures
#define NUM_MAX_TEXTURES 10

// -------------- Entorn PFC: TEXTURES: Tipus de textures
#define CAP ' '
#define FUSTA 'F'
#define MARBRE 'M'
#define METALL 'E'
#define FITXERBMP 'f'
#define FITXERIMA 'I'

//--------------- Entorn PFC: Valor constant de pi
const double pi=3.14159;

// ------TRUCK: ResoluciÃ³ de les rodes (Nombre de divisions del cilindre i discos)
#define RESOLUCIO_RODA 16

// --------------- Entorn PFC: Estructura coordenada 2D
struct CPoint
{   GLint x;
    GLint y;
};

// --------------- Entorn PFC: Estructura coordenada 3D
struct CPunt3D
{   GLfloat x;
    GLfloat y;
    GLfloat z;
};

// --------------- Entorn PFC: Estructura de color R,G,B
struct CColor
{   GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
};

// --------------- Entorn PFC: Estructura coordenada Esferica 3D
struct CEsfe3D
{   GLfloat R;
    GLfloat alfa;
    GLfloat beta;
};

// --------------- Entorn PFC: INSTANCIA (TG d'instanciacio d'un objecte)
struct INSTANCIA
{	CPunt3D VTras;	// Vector de Traslacio
    CPunt3D VScal;	// Vector d'Escalatge
    CPunt3D VRota;	// Vector de Rotacio
};

// --------------- Entorn PFC: Coeficients equacio d'atenuacio de la llum fatt=1/(ad2+bd+c)
struct CAtenua
{   GLfloat a;
    GLfloat b;
    GLfloat c;
};

// --------------- Entorn PFC: Estructura font de llum
struct LLUM
{	bool encesa;		// Booleana que controla si la llum es encesa [TRUE] o no [FALSE]
    CEsfe3D posicio;	// Posicio d ela foont de llum en coordenades esferiques.
    GLfloat difusa[4];	// Intensitat difusa de la font de llum (r,g,b,a)
    GLfloat especular[4];	// Intensitat especular de la font de llum (r,g,b,a)
    CAtenua atenuacio;		// Coeficients de l'equacio d'atenuacio de la llum fatt=1/(ad2+bd+c)
    bool restringida;		// Booleana que indica si la font de llum es restringida [TRUE] i per tant son valids els coeficients posteriors o no [FALSE].
    GLfloat spotdirection[3];	// vector de direccio de la font de llum restringida (x,y,z).
    GLfloat cutoff;			// Angle d'obertura de la font de llum restringida.
    GLfloat exponent;		// Exponent que indica l'atenuacio de la font del centre de l'eix a l'exterior, segons model de Warn.
};

#endif  /* _UTIL_H */
