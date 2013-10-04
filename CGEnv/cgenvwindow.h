//******** PRACTICA GRAFICS PER COMPUTADOR 2
//******** Entorn basic amb interficie Qt i OpenGL (PFC)
//******** Ferran Poveda, Enric Marti (Juny 2012)
// cgenvwindow.h: interface de cgenvwindow

#ifndef CGENVWINDOW_H
#define CGENVWINDOW_H

#include <QMainWindow>

namespace Ui {
class CGEnvWindow;
}

class CGEnvWindow : public QMainWindow
{
    Q_OBJECT

public slots:
//	1. FILES
    void OnFNew();
    void OnFileOpen();
    void OnFileOpen3ds();
    void OnFileOpenObj();
    void OnExit();
//	2. BARS
    void OnToolBar();
    void OnStatusBar();
    void OnFullScreen();
//	3. VEURE
    void OnMobil();
    void OnZoom();
    void OnPolarZ();
    void OnPolarY();
    void OnPolarX();
    void OnPan();
    void OnInipan();
    void OnNavega();
    void OnIninav();
    void OnEixos();
//	4. PROJECCIÃ
    void OnPerspectiva();
//	5. OBJECTE
    void OnCube();
    void OnSphere();
    void OnTeapot();
    void OnTruck();
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
    void OnTdecal();
    void OnTmodulate();

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent * event );

    void keyPressEvent(QKeyEvent *event);

public:
    explicit CGEnvWindow(QWidget *parent = 0);
    ~CGEnvWindow();
    void Barra_Estat();

protected:
    
private:
    Ui::CGEnvWindow *ui;

// Entorn PFC: Variables de control per Menu Bar: ToolBar, StatusBar, FullScreen
    bool TB;			// Booleana controla visibilitat [SI] o no visibilitat del ToolBar.
    bool STB;			// Booleana controla visibilitat [SI] o no visibilitat del StatusBar.
    bool fullScreen;	// Booleana controla visibilitat [SI] o no visibilitat del FullScreen (Pantalla Completa).

// Entorn PFC: Variables de control per MenÃº Vista: canvi PV interactiu, Zoom i dibuixar eixos
    bool mobil;	 // Opcio canvi de Punt de Vista interactiu (mobil) [0:NO,1:SI]
    bool zzoom;	 // Opcio Zoom interactiu (zoom) [0:NO,1:SI]
    bool pan;    // Opcio Pan. DesplaÃ§ament del centre de l'esfera de Vista [0:NO,1:SI]
    bool navega; // Opcio Navega [F:NO,T:SI].
    bool eixos;	 // Opcio per dibuixar els eixos X,Y,Z (Eixos?) [0:NO,1:SI]

// Entorn PFC: Variables de control del menÃº Transforma
    bool trasl;		// Booleana d'activacio de la Traslacio (true) o no (false).
    bool rota;		// Booleana d'activacio de la Rotacio (true) o no (false).
    bool escal;		// Booleana d'activaciÃo de l'Escalatge (true) o no (false).
    bool transX;	// OpciÃ³ Mobil Eix X?: TG interactives per l'eix X via mouse [F:NO,T:SI].
    bool transY;	// OpciÃ³ Mobil Eix Y?: TG interactives per l'eix Y via mouse [F:NO,T:SI].
    bool transZ;	// OpciÃ³ Mobil Eix Z?: TG interactives per l'eix Z via mouse [F:NO,T:SI].

// Entorn PFC: Variables de control per les opcions de menÃº Ocultacions
   bool test_vis;  // MenÃº ocultacions: Activa [true] o desactiva [false] Test Visibilitat.
   bool oculta;    // MenÃº ocultacions: Activa [true] o desactiva [false] Ocultacions (Z-buffer).
   bool back_line; // MenÃº ocultacions: Activa [true] o desactiva [false] dibuix cares back com a lÃ­nies.

// Entorn PFC: Variables de control del menÃº IluminaciÃ³
   bool ifixe;          // IluminaciÃ³ fixe independent del PV (ifixe=1)
                        // o depenent (mobil) respecte el PV (casc miner) (ifixe=0)
   bool textura;       // Control de textures desactivades [0:NO] o activades [1:SI]
   bool textura_map;	// Mapping de textura modulat amb la llum [TRUE] o calcat [FALSE]

};

#endif // CGENVWINDOW_H
