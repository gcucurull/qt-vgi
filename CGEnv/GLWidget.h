//******** PRACTICA GRAFICS PER COMPUTADOR 2
//******** Entorn basic amb interficie Qt i OpenGL (PFC)
//******** Ferran Poveda, Enric Marti (Juny 2012)
// GLWidget.h: Interficia de GLWidget
//          - Definicio funcions d'event de dispositius i menús
//          - Variables globals entorn

#ifndef _GLWIDGET_H
#define _GLWIDGET_H

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <QtOpenGL/QGLWidget>
#include <QGLShaderProgram>
#include <QTimer>
#include "Model.h"
#include "Util.h"

// OBJECTE 3DS: Include per la definició de l'objecte Obj_3DS
#include "Obj3DS.h"

// OBJECTE OBJ: Include per la definició de l'objecte Obj_OBJ
#include "objLoader.h"

class GLWidget : public QGLWidget {

    Q_OBJECT // must include this if you use Qt signals/slots

public slots:
// Event Timer
    void OnTimer();

public:
    GLWidget(QWidget *parent = NULL);

// Events Mouse
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent * event );

// Event Teclat
    void keyPressEvent(QKeyEvent *event);

//	1. FILES
    void OnFNew();
    void OnFileOpen();
    void OnFileOpen3ds();
    void OnFileOpenObj();
//	2. BARS
     void OnFullScreen();
//	3. VEURE
    void OnMobil();
    void OnZoom();
    bool OnPolarZ();
    bool OnPolarY();
    bool OnPolarX();
    void OnPan();
    void OnInipan();
    void OnNavega();
    void OnIninav();
    void OnEixos();
//	4. PROJECCIÓ
    void OnPerspectiva();
    void OnOrtografica();
//	5. OBJECTE
    void OnCube();
    void OnSphere();
    void OnTeapot();
    void OnTruck();
    void OnCamio();
//	6. TRANSFORMA
    void OnTraslacio();
    void OnInitras();
    void OnRotacio();
    void OnInirot();
    void OnEscalatge();
    void OnIniescal();
    void OnMobilX();
    void OnMobilY();
    void OnMobilZ();
//	7. OCULTACIONS
    void OnTest();
    void OnZBuffer();
    void OnBackLine();
//	8. IL.LUMINACIO
    void OnIFixe();
    void OnFilferros();
    void OnFlat();
    void OnGouraud();
    void OnTexture();
    void OnTexturima();
    void OnWood();
    void OnMarble();
    void OnMetal();
    void OnTdecal();
    void OnTmodulate();

// SETTERS & GETTERS
    int GetModeVisualitzacio(void);
    void SetModeVisualitzacio(int mode);
    void SetMobil(bool mode);
    void SetZoom(bool mode);
    void SetPan(bool mode);
    void SetNavega(bool mode);
    void SetEixos(bool mode);
    void SetTrasl(bool mode);
    void SetRota(bool mode);
    void SetEscal(bool mode);
    void SetTestVis(bool mode);
    void SetOculta(bool mode);
    void SetBackLine(bool mode);
    void SetIFixe(bool mode);
    void SetTextura(bool mode);
    void SetTexturaMap(bool mode);
    void SetFullScreen(bool mode);
    char *GetNom();
    char GetProjeccio();
    CEsfe3D GetOPV();
    CPunt3D GetOPVN();
    char GetVisPolar();
    CPunt3D GetTR_CPV();
    bool GetSwColor();
    bool GetTransf();
    bool GetFonsR();
    bool GetFonsG();
    bool GetFonsB();
    bool GetTrasl();
    bool GetRota();
    bool GetEscal();
    INSTANCIA GetTG();
    bool GetSWColor();
    CColor GetCFons();
    CColor GetColObj();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
/*
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent * event );
    void keyPressEvent(QKeyEvent *event);
*/
    int Log2(int num);

private:
    QGLShaderProgram m_shader;

    int modeVisualitzacio;
    Model escena;
    QWidget *pare;	//SOLUCIO 1: Cridar a cgenv des de contextGL

//------------------- Entorn PFC: Variables globals de l'aplicació CGEnv
// Entorn PFC: Variables de control per Menú Veure->Pantalla Completa
    bool fullScreen;	// Opció booleana per Pantalla Completal [0:NO,1:SI]

// Entorn PFC: Variables de control per Menú Vista: canvi PV interactiu, Zoom i dibuixar eixos
    bool mobil;	 // Opció canvi de Punt de Vista interactiu (mobil) [0:NO,1:SI]
    bool zzoom;	 // Opció Zoom interactiu (zoom) [0:NO,1:SI]
    bool pan;    // Opcio Pan. Desplaçament del centre de l'esfera de Vista [0:NO,1:SI]
    bool navega; // Opció Navega [F:NO,T:SI].
    bool eixos;	 // Opció per dibuixar els eixos X,Y,Z (Eixos?) [0:NO,1:SI]

// Entorn PFC: Variables de l'opció Vista->Pan
    float fact_pan;		 // Factor de desplaçament de la càmara (opció pan).
    CPunt3D tr_cpv;   // Vector de Traslació del Centre del Punt de Vista actiu dins opció pan.
    CPunt3D tr_cpvF;  // Vector de Traslació del Centre del Punt de Vista (fixat amb INSERT dins opció pan)
                         //       i actiu fora l'opció pan.

// Entorn PFC: Variables de control de l'opció Vista->Navega?
    GLfloat n[3];   // Punt cap on mira.
    CPunt3D opvN;   // Definició Orígen Punt de Vista (en coordenades món)
    float angleZ;   // angle de rotació de la trajectòria.

// Entorn PFC: Variables de control per les opcions de menú Projecció, Objecte
    char projeccio;	// Menú Projecció: Tipus de projeccio
    char objecte;	// Menú Objecte: Tipus d'Objecte

// Entorn PFC: Variables de control del menú Transforma
    bool transf;	// Booleana per activar Transformacions (true) o no (false).
    bool trasl;		// Booleana d'activació de la Traslació (true) o no (false).
    bool rota;		// Booleana d'activació de la Rotació (true) o no (false).
    bool escal;		// Booleana d'activació de l'Escalatge (true) o no (false).
    float fact_Tras, fact_Rota;	// Increments de Traslació i Rotació.
    INSTANCIA TG;	// Estructura que conté TG del menú Transforma actiu dins qualsevol opció de Transforma
                    //      (Traslació Rotació i Escalatge).
    INSTANCIA TGF;	// Estructura que conté TG del menú Transforma fixat amb INSERT dins les opcions de Transforma
                    //     i actiu fora l'opció Transforma.
    bool transX;	// Opció Mobil Eix X?: TG interactives per l'eix X via mouse [F:NO,T:SI].
    bool transY;	// Opció Mobil Eix Y?: TG interactives per l'eix Y via mouse [F:NO,T:SI].
    bool transZ;	// Opció Mobil Eix Z?: TG interactives per l'eix Z via mouse [F:NO,T:SI].

// Entorn PFC: Variables de control per les opcions de menú Ocultacions
    bool test_vis;  // Menú ocultacions: Activa [true] o desactiva [false] Test Visibilitat.
    bool oculta;    // Menú ocultacions: Activa [true] o desactiva [false] Ocultacions (Z-buffer).
    bool back_line; // Menú ocultacions: Activa [true] o desactiva [false] dibuix cares back com a línies.

// Entorn PFC: Variables de control del menú Iluminació
    bool ifixe;          // Iluminació fixe independent del PV (ifixe=1)
                        // o depenent (mobil) respecte el PV (casc miner) (ifixe=0)
    char ilumina;		// Tipus d'il.luminació [FILFERROS 'f',PLANA 'c',SUAU 'c']
    bool textura;       // Control de textures desactivades [0:NO] o activades [1:SI]
    char t_textura;		// Tipus de textures (predefinides o per fitxer)
    bool textura_map;	// Mapping de textura modulat amb la llum [TRUE] o calcat [FALSE]
    bool llum_ambient;	// Booleana que controla la llum ambient (SI/NO).
    LLUM llumGL;		// Llum 0

// Entorn PFC: Variables butons de mouse
    CPoint m_PosEAvall,m_PosDAvall; //Coordenades del cursor quan el
                                    //boto esquerre(E) o dret(D) del
                                    //mouse ha estat clicat
    bool m_ButoEAvall,m_ButoDAvall; //TRUE si el boto esquerre(E) o dret(D)
                                    //del mouse esta clicat

// Entorn PFC: Variables que controlen paràmetres visualització: Mides finestra Windows i PV
    int w,h;				// Mides de la finestra Windows (w-amplada,h-alçada)
    CEsfe3D OPV;			// Paràmetres camera en coord. esfèriques (R,alfa,beta)
    char Vis_Polar;			// Variable que controla orientació dels eixos en Visualització Interactiva (POLARZ,POLARY,POLARX)

// Entorn PFC: Ortogràfica Iteractiva
    CEsfe3D OPVPl;			// Camara en coord. esfèriques en Planta (R,alfa,beta)
    CEsfe3D OPVPr;			// Camara en coord. esfèriques en Perfil (R,alfa,beta)
    CEsfe3D OPVA;			// Camara en coord. esfèriques en Alçat (R,alfa,beta)
    CEsfe3D OPVI;			// Camara en coord. esfèriques en Isomètrica (R,alfa,beta)
    int scrO;				// Detecció quina pantalla Ortogràfica seleccionem
                            //  (0-Alçat,1-Perfil,2-Planta,3-Isomètrica)
    float VVOrto;			// Escala del Volum de Visualització en Ortogràfica (Zoom Ortogràfic).

// Entorn PFC: Color de fons i de l'objecte
    bool fonsR,fonsG,fonsB;	// Booleanes per controlar variació de color per teclat.
    CColor c_fons;			// Intensitat de color de fons.
    bool sw_color;			// Booleana que controla el modus de configurar color de l'objecte per teclat [TRUE] o no [FALSE]
    CColor col_obj;			// Color de l'objecte simple.

// Entorn PFC: Objecte 3DS:
    Obj_3DS *Ob3DS;			// Variable d'objecte 3D Studio (*.3DS)

// Entorn PFC: Objecte OBJ:
    COBJModel *ObOBJ;		// Variable d'objecte format OBJ (*.OBJ)

// Entorn PFC: Variables del Timer
    float t;		// Paràmetre t pel Timer.
    bool anima;		// Booleana que controla si l'animació és activa (TRUE) o no (FALSE)
                    //    dins la funció de control del rellotge OnTimer.
    QTimer *timer;  // Estructura Qt que controla l'event Timer

// Entorn PFC: Altres variables
    int mida;		// Factor d'escala per calcular Volum de Visualització de l'objecte que encaixi.
    int pas;		// Resolució del fractal inicial (pas_ini) i de visualització (pas).
    char *nom;	// Nom de fitxer.

};

#endif  /* _GLWIDGET_H */
