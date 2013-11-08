//******** PRACTICA GRAFICS PER COMPUTADOR 2
//******** Entorn basic amb interficie Qt i OpenGL (PFC)
//******** Ferran Poveda, Enric Marti (Juny 2012)
// GLWidget.cpp: Widget personalitzat d'OpenGL per a QT per a Win64 i Mac OS
// FUNCIONS:		- Control del bucle principal (paintGL)
//					- Control teclat (KeyPressEvent)
//					- Control mouse interactiu i botons mouse
//                          (mousePressEvent,mouseReleaseEvent, mouseMoveEvent, wheelEvent)
//					- Control opcions de menú (On*)
//					- Control de color de fons per teclat (FONS)
//					- Transformacions Geomètriques Interactives via mouse
//
//    Versió 2.0:	- Canvi de color de l'objecte per teclat (OBJ)
//					- Fixar Transformacions Geomètriques desactivant opcions Transforma (INSERT dins opcions Transforma)
//					- Esborrar Transformacions Geomètriques fixades (DELETE dins opcions Transforma)
//					- Fixar Traslació pantalla fora de l'opció Vista->Pan? (INSERT dins opció Vista->Pan?)
//					- Esborrar Traslació pantalla fixada (DELETE dins opció Vista->Pan?)
//

#include <QtGui/QMouseEvent>
#include <QTimer>
#include "Util.h"
#include "visualitzacio.h"
#include "GLWidget.h"

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent) {
    setMouseTracking(true);

    modeVisualitzacio = ORTO;
    pare=parent;	//SOLUCIO 1: Cridar a cgenvWindow des de contextGL

//------ Entorn PFC: Inicialització de les variables globals de CPracticaView

// Entorn PFC: Variables de control per Menú Veure->Pantalla Completa
    fullScreen=false;

// Entorn PFC: Variables de control per Menú Vista: canvi PV interactiu, Zoom i dibuixar eixos
    mobil=true;				zzoom=true;		pan=false;		navega=false;	eixos=true;

// Entorn PFC: Variables opció Vista->Pan
    fact_pan=1;
    tr_cpv.x=0.0;	tr_cpv.y=0.0;	tr_cpv.z=0.0;		tr_cpvF.x=0.0;	tr_cpvF.y=0.0;	tr_cpvF.z=0.0;

// Entorn PFC: Variables de control de l'opció Vista->Navega?
    n[0]=0.0;		n[1]=0.0;		n[2]=0.0;
    opvN.x=10.0;	opvN.y=0.0;		opvN.z=0.0;
    angleZ=0.0;

// Entorn PFC: Variables de control per les opcions de menú Projecció, Objecte
    projeccio=CAP;	objecte=CAP;

// Entorn PFC: Variables de control del menú Transforma
    transf=false;	trasl=false;	rota=false;		escal=false;
    fact_Tras=1;	fact_Rota=90;
    TG.VTras.x=0.0;	TG.VTras.y=0.0;	TG.VTras.z=0;	TGF.VTras.x=0.0;TGF.VTras.y=0.0;TGF.VTras.z=0;
    TG.VRota.x=0;	TG.VRota.y=0;	TG.VRota.z=0;	TGF.VRota.x=0;	TGF.VRota.y=0;	TGF.VRota.z=0;
    TG.VScal.x=1;	TG.VScal.y=1;	TG.VScal.z=1;	TGF.VScal.x=1;	TGF.VScal.y=1;	TGF.VScal.z=1;

    transX=false;	transY=false;	transZ=false;

// Entorn PFC: Variables de control per les opcions de menú Ocultacions
    oculta=false;			test_vis=false;			back_line=false;

// Entorn PFC: Variables de control del menú Iluminació
    ilumina=FILFERROS;		ifixe=false;	textura=false;	t_textura=CAP;		textura_map=true;

    llum_ambient=true;
    llumGL.encesa=true;
    llumGL.difusa[0]=1.0f;		llumGL.difusa[1]=1.0f;		llumGL.difusa[2]=1.0f;		llumGL.difusa[3]=1.0f;
    llumGL.especular[0]=0.0f;	llumGL.especular[1]=0.0f;	llumGL.especular[2]=0.0f;	llumGL.especular[3]=1.0f;

    llumGL.posicio.R=75.0;		llumGL.posicio.alfa=90.0;	llumGL.posicio.beta=0.0;	// Posició llum (x,y,z)=(0,0,75)
    llumGL.atenuacio.a=0.0;		llumGL.atenuacio.b=0.0;		llumGL.atenuacio.c=1.0;		// Llum sense atenuació per distància (a,b,c)=(0,0,1)
    llumGL.restringida=false;
    llumGL.spotdirection[0]=0.0;	llumGL.spotdirection[1]=0.0;					llumGL.spotdirection[2]=0.0;
    llumGL.cutoff=0.0;			llumGL.exponent=0.0;

// Entorn PFC: Variables de control dels botons de mouse
    m_PosEAvall.x=0;		m_PosDAvall.y=0;
    m_PosDAvall.x=0;		m_PosDAvall.y=0;
    m_ButoEAvall=false;		m_ButoDAvall=false;

// Entorn PFC: Variables que controlen paràmetres visualització: Mides finestra Windows i PV
    w=0;			h=0;								// Mides finestra
    OPV.R=15.0;		OPV.alfa=0.0;		OPV.beta=0.0;	// Origen PV en esfèriques
    Vis_Polar=POLARZ;

// GC2: Ortogràfica Iteractiva
    OPVPl.R=15.0;	OPVPl.alfa=0.0;		OPVPl.beta=270.0;	// ORTOGRÀFICA->Planta
    OPVPr.R=15.0;	OPVPr.alfa=0.0;		OPVPr.beta=0.0;		// ORTOGRÀFICA->Perfil
    OPVA.R=15.0;	OPVA.alfa=90.0;		OPVA.beta=0.0;		// ORTOGRÀFICA->Alçat
    OPVI.R=15.0;	OPVI.alfa=45.0;		OPVI.beta=45.0;		// ORTOGRÀFICA->Isomètrica
    scrO=0;			VVOrto=1.0;

// Entorn PFC: Color de fons i de l'objecte
    fonsR=true;		fonsG=true;		fonsB=true;
    c_fons.r=0.0;	c_fons.g=0.0;	c_fons.b=0.0;
    sw_color=false;
    col_obj.r=1.0;		col_obj.g=1.0;		col_obj.b=1.0;		col_obj.a=1.0;

// Entorn PFC: Objecte 3DS:
    Ob3DS = NULL;

// Entorn PFC: Objecte OBJ:
    ObOBJ = NULL;

// variable
// Entorn PFC: Variables del Timer
    t=0;			anima=false;    timer=NULL;

// Entorn PFC: Altres variables
    mida=sqrtf(3);	pas=64;			nom="";

}

void GLWidget::initializeGL() {

// Test per a verificar si s'implementa el doble buffer per fer picking objects
    bool doble_buffer=this->doubleBuffer();

// Automatic GL buffer swapping off (cal fer swapbuffers explicitament (per a picking objects)
// Si on, background and foreground GL buffers will automatically be swapped after each paintGL() call.
    this->setAutoBufferSwap(false);

// Activació del Timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
//  timer->start(100);      // Engegar Timer
//  timer->killTimer();     // Parar Timer


// ***********************************************************
// Test de suport OpenGL 2.0 (necessaris per carregar shaders)
#ifndef __APPLE__
    glewInit();

    if (glewIsSupported("GL_VERSION_2_0"))
    {
        printf("Ready for OpenGL 2.0\n");
    }
    else
    {
        printf("OpenGL 2.0 not supported\n");
        exit(1);
    }
#endif
// ****************************************************


// ****************************************************
// Carrega dels shaders (per a les practiques 2, 3 i 4)
    bool result;
    result = m_shader.addShaderFromSourceFile( QGLShader::Vertex, "simple.vert" );
    if ( !result )
        qDebug() << "Vertex: " << m_shader.log();
    else
        qDebug() << "Vertex shader works!";
    result = m_shader.addShaderFromSourceFile( QGLShader::Fragment, "simple.frag" );
    if ( !result )
        qDebug() << "Fragment: " << m_shader.log();
    else
        qDebug() << "Fragment shader works!";
    result = m_shader.link();
    if ( !result )
        qDebug() << "Link: " << m_shader.log();
    else
        qDebug() << "Shaders link: OK!";
    result = m_shader.bind();
    if ( !result )
        qDebug() << "Bind: " << m_shader.log();
    else
        qDebug() << "Shaders bind: OK!";

    GLuint num_shader=m_shader.programId();

// Eliminar Shaders precarregats (OpenGL 1.1)
    m_shader.release();
//  m_shader.removeAllShaders();
// ****************************************************

}

void GLWidget::resizeGL(int cw, int ch) {

// ATENCIÓ: MODIFICACIÓ OPENGL
// Establim les mides de la finestra actual
   w=cw;
   h=ch;

}

void GLWidget::paintGL() {

    GLfloat vpv[3]={0.0,0.0,1.0};

// Cridem a les funcions de l'escena i la projecció segons s'hagi
// seleccionat una projecció o un altra
    switch(projeccio)
    {
    case ORTO:
// Aquí farem les quatre crides a ProjeccioOrto i Ortografica per obtenir
// les quatre vistes ortogràfiques

//      Activació del retall de pantalla
        glEnable(GL_SCISSOR_TEST);

// Retall
        glScissor(0,0,w,h);
        glViewport(0,0,w,h);

// Fons condicionat al color de fons
        if ((c_fons.r < 0.5) || (c_fons.g < 0.5) || (c_fons.b<0.5))
            FonsB();
        else
            FonsN();

// PLANTA (Superior Dret)
        Projeccio_Orto(w/2+1,h/2+1,w/2-1,h/2-1);
        Ortografica(0,OPV.R,c_fons,col_obj,objecte,oculta,
            test_vis,back_line,ilumina,llumGL,textura,textura_map,ifixe,eixos);
        // Dibuixar Model (escena)
        glPushMatrix();
            instancia(transf,TG,TGF);			// Aplicar Transformacions Geomètriques segons persiana Transformacio
            escena.SetObjecte(objecte);
            escena.SetTextu(textura);
            glScalef(VVOrto, VVOrto, VVOrto);
            escena.dibuixa();					// Dibuix geometria de l'escena.
        glPopMatrix();
// ALÇAT (Inferior Esquerra)
        Projeccio_Orto(0,0,w/2,h/2);
        Ortografica(1,OPV.R,c_fons,col_obj,objecte,oculta,
            test_vis,back_line,ilumina,llumGL,textura,textura_map,ifixe,eixos);
        // Dibuixar Model (escena)
        glPushMatrix();
            instancia(transf,TG,TGF);			// Aplicar Transformacions Geomètriques segons persiana Transformacio
            escena.SetObjecte(objecte);
            escena.SetTextu(textura);
            glScalef(VVOrto, VVOrto, VVOrto);
            escena.dibuixa();					// Dibuix geometria de l'escena.
        glPopMatrix();
// PERFIL (Superior Esquerra)
        Projeccio_Orto(0,h/2+1,w/2-1,h/2-1);
        Ortografica(2,OPV.R,c_fons,col_obj,objecte,oculta,
            test_vis,back_line,ilumina,llumGL,textura,textura_map,ifixe,eixos);
        // Dibuixar Model (escena)
        glPushMatrix();
            instancia(transf,TG,TGF);			// Aplicar Transformacions Geomètriques segons persiana Transformacio
            escena.SetObjecte(objecte);
            escena.SetTextu(textura);
            glScalef(VVOrto, VVOrto, VVOrto);
            escena.dibuixa();					// Dibuix geometria de l'escena.
        glPopMatrix();
// ISOMÈTRICA (Inferior Dret)
        Projeccio_Orto(w/2+1,0,w/2-1,h/2-1);
        Ortografica(3,OPVI.R,c_fons,col_obj,objecte,oculta,
            test_vis,back_line,ilumina,llumGL,textura,textura_map,ifixe,eixos);
        // Dibuixar Model (escena)
        glPushMatrix();
            instancia(transf,TG,TGF);			// Aplicar Transformacions Geomètriques segons persiana Transformacio
            escena.SetObjecte(objecte);
            escena.SetTextu(textura);
            glScalef(VVOrto, VVOrto, VVOrto);
            escena.dibuixa();					// Dibuix geometria de l'escena.
        glPopMatrix();

// Intercanvia l'escena al front de la pantalla
        this->swapBuffers();
        break;

    case PERSPECT:
// Aquí cridarem a ProjeccioPerspectiva i Perspectiva
        glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Set Perspective Calculations To Most Accurate
        Fons(c_fons);
        Projeccio_Perspectiva(0,0,w,h,OPV.R);
        if (navega)
            {	PerspectivaN(opvN,n,vpv,pan,tr_cpv,tr_cpvF,c_fons,col_obj,objecte,oculta,
                    test_vis,back_line,ilumina,llumGL,textura,textura_map,ifixe,eixos);
            }
        else {	n[0]=0;		n[1]=0;		n[2]=0;
            Perspectiva(OPV,Vis_Polar,pan,tr_cpv,tr_cpvF,c_fons,col_obj,objecte,
                oculta,test_vis,back_line,ilumina,llumGL,textura,
                textura_map,ifixe,eixos);
            }
        // Dibuixar Model (escena)
        glPushMatrix();
            instancia(transf,TG,TGF);			// Aplicar Transformacions Geomètriques segons persiana Transformacio
            escena.SetObjecte(objecte);
            escena.SetTextu(textura);
            escena.dibuixa();					// Dibuix geometria de l'escena.
        glPopMatrix();
// Intercanvia l'escena al front de la pantalla
        this->swapBuffers();

        break;
    default:
// Crida a la funció Fons Blanc
        FonsB();

// Intercanvia l'escena al front de la pantalla
        this->swapBuffers();
        break;
    }

// Refrescar Status Bar
//	((CGenv *)pare)->Barra_Estat();		//SOLUCIO 1: Cridar a cgenv des de contextGL

}

/* ------------------------------------------------------------------------- */
/*                           CONTROL DEL RATOLI                              */
/* ------------------------------------------------------------------------- */

// mousePressEvent: Funció que es crida quan es mou el mouse. La utilitzem per la
//				  Visualització Interactiva amb les tecles del mouse apretades per
//				  modificar els paràmetres de P.V. (R,angleh,anglev) segons els
//				  moviments del mouse.
//      PARAMETRES: - event: Estructura (x,y) que dóna la posició del mouse
//							 (coord. pantalla) quan el botó s'ha apretat.

void GLWidget::mousePressEvent(QMouseEvent *event) {

// Detectem si s'ha apretat una tecla del mouse
    if (event->type() == QEvent::MouseButtonPress) {
// Activem flag m_ButoLAvall i detectem en quina posició s'ha apretat el botó
//	esquerra del mouse i ho guardem a la variable m_PosEAvall
        if (event->button() == Qt::LeftButton) {
            m_ButoEAvall = true;
            m_PosEAvall.x = event->x();
            m_PosEAvall.y = event->y();		}
// Activem flag m_ButoEAvall i detectem en quina posició s'ha apretat el botó
//   dret del mouse i ho guardem a la variable m_PosDAvall
            else if (event->button()==Qt::RightButton) {
                m_ButoDAvall = true;
                m_PosDAvall.x = event->x();
                m_PosDAvall.y = event->y();		}
    }
// Detectem si s'ha aixecat una tecla del mouse
    else if (event->type()==QEvent::MouseButtonRelease) {
// Desactivem flag m_ButoEAvall
    if (event->button()==Qt::LeftButton) {
        m_ButoEAvall = false;	}
// Desactivem flag m_ButoDAvall
    else if (event->button()==Qt::RightButton) {
            m_ButoDAvall = false;	}
    }

// Crida a paintGL() per redibuixar l'escena
    updateGL();

// Refrescar Status Bar
//	((CGenv *)pare)->Barra_Estat();	//SOLUCIO 1: Cridar a cgenv des de contextGL
}

void GLWidget::mouseReleaseEvent( QMouseEvent * event ) {


 // Detectem si s'ha aixecat una tecla del mouse
    if (event->type()==QEvent::MouseButtonRelease) {
// Desactivem flag m_ButoEAvall
    if (event->button()==Qt::LeftButton) {
        m_ButoEAvall = false;	}
// Desactivem flag m_ButoDAvall
    else if (event->button()==Qt::RightButton) {
            m_ButoDAvall = false;	}
    }

// Crida a paintGL() per redibuixar l'escena
    updateGL();

// Refrescar Status Bar
//	((CGenv *)pare)->Barra_Estat();	//SOLUCIO 1: Cridar a cgenv des de contextGL
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {

    float modul=0;
    GLfloat vdir[3]={0,0,0};
    CPoint gir,girn,girT,zoomincr,point;

// Agafar la posició actual del mouse
    point.x=event->x();
    point.y=event->y();

// Si botó apretat és l'esquerra
    if (m_ButoEAvall && mobil && projeccio!=CAP)
    {

// Determinació dels angles (en graus) segons l'increment
//  horitzontal i vertical de la posició del mouse
// 		CSize gir = m_PosEAvall - point;
        gir.x = m_PosEAvall.x - point.x;
        gir.y = m_PosEAvall.y - point.y;

        if (projeccio!=ORTO)  {
//		m_PosEAvall = point;
            m_PosEAvall.x = point.x;
            m_PosEAvall.y = point.y;
            OPV.beta=OPV.beta-gir.x/2;
            OPV.alfa=OPV.alfa+gir.y/2;
// Control per evitar el creixement desmesurat dels angles
            if(OPV.alfa>=360)	OPV.alfa=OPV.alfa-360;
            if(OPV.alfa<0)		OPV.alfa=OPV.alfa+360;
            if(OPV.beta>=360)	OPV.beta=OPV.beta-360;
            if(OPV.beta<0)		OPV.beta=OPV.beta+360;
//			InvalidateRect(NULL, false);
        }
        else { if (point.x<w/2) {
                    if (point.y>h/2) { // ALÇAT
                    OPVA.beta = OPVA.beta-gir.x/4;
                    OPVA.alfa = OPVA.alfa+gir.y/4 ;
// Control per evitar el creixement desmesurat dels angles
                    if(OPVA.alfa>=360)	OPVA.alfa=OPVA.alfa-360;
                    if(OPVA.alfa<0)		OPVA.alfa=OPVA.alfa+360;
                    if(OPVA.beta>=360)	OPVA.beta=OPVA.beta-360;
                    if(OPVA.beta<0)		OPVA.beta=OPVA.beta+360;
                    scrO=0;
                    }
                    else { // PERFIL
                        OPVPr.beta = OPVPr.beta-gir.x/4;
                        OPVPr.alfa = OPVPr.alfa+gir.y/4 ;
// Control per evitar el creixement desmesurat dels angles
                        if(OPVPr.alfa>=360)	OPVPr.alfa=OPVPr.alfa-360;
                        if(OPVPr.alfa<0)	OPVPr.alfa=OPVPr.alfa+360;
                        if(OPVPr.beta>=360)	OPVPr.beta=OPVPr.beta-360;
                        if(OPVPr.beta<0)	OPVPr.beta=OPVPr.beta+360;
                        scrO=1;
                        }
                }
                else {	if (point.y<h/2) { // PLANTA
                        OPVPl.beta = OPVPl.beta-gir.x/4;
                        OPVPl.alfa = OPVPl.alfa+gir.y/4 ;
// Control per evitar el creixement desmesurat dels angles
                        if(OPVPl.alfa>=360)	OPVPl.alfa=OPVPl.alfa-360;
                        if(OPVPl.alfa<0)	OPVPl.alfa=OPVPl.alfa+360;
                        if(OPVPl.beta>=360)	OPVPl.beta=OPVPl.beta-360;
                        if(OPVPl.beta<0)	OPVPl.beta=OPVPl.beta+360;
                        scrO=2;
                        }
                        else { // ISOMÈTRICA
                            OPVI.alfa = OPVI.alfa-gir.x/2;
                            OPVI.beta = OPVI.beta+gir.y/2 ;
// Control per evitar el creixement desmesurat dels angles
                            if(OPVI.alfa>=360)	OPVI.alfa=OPVI.alfa-360;
                            if(OPVI.alfa<0)		OPVI.alfa=OPVI.alfa+360;
                            if(OPVI.beta>=360)	OPVI.beta=OPVI.beta-360;
                            if(OPVI.beta<0)		OPVI.beta=OPVI.beta+360;
                            scrO=3;
                            }
                    }
        }
    }
    else if (m_ButoEAvall && navega && (projeccio!=CAP && projeccio!=ORTO)) // Opció Navegació
            {
// Canviar orientació en opció de Navegació
//			CSize girn = m_PosEAvall - point;
            girn.x = m_PosEAvall.x - point.x;
            girn.y = m_PosEAvall.y - point.y;
            angleZ=girn.x/2.0;
// Control per evitar el creixement desmesurat dels angles
            if(angleZ>=360) angleZ=angleZ-360;
            if(angleZ<0)	angleZ=angleZ+360;

            n[0]=n[0]-opvN.x;
            n[1]=n[1]-opvN.y;
            n[0]=n[0]*cos(angleZ*pi/180)-n[1]*sin(angleZ*pi/180);
            n[1]=n[0]*sin(angleZ*pi/180)+n[1]*cos(angleZ*pi/180);
            n[0]=n[0]+opvN.x;
            n[1]=n[1]+opvN.y;

//			m_PosEAvall = point;
            m_PosEAvall.x = point.x;
            m_PosEAvall.y = point.y;
//			InvalidateRect(NULL,false);
            }

// Transformació Geomètrica interactiva pels eixos X,Y boto esquerra del mouse
            else {	bool transE = transX || transY;
                    if (m_ButoEAvall && transE && transf)
                    {
// Calcular increment
//						CSize girT = m_PosEAvall - point;
                        girT.x = m_PosEAvall.x - point.x;
                        girT.y = m_PosEAvall.y - point.y;
                        if (transX)
                            {	long int incrT=girT.x;
                                if (trasl)
                                    { TG.VTras.x+=incrT*fact_Tras;
                                      if (TG.VTras.x<-100000) TG.VTras.x=100000;
                                      if (TG.VTras.x>100000) TG.VTras.x=100000;
                                    }
                                    else if (rota)
                                            {	TG.VRota.x+=incrT*fact_Rota;
                                                while (TG.VRota.x>=360) TG.VRota.x-=360;
                                                while (TG.VRota.x<0) TG.VRota.x+=360;
                                            }
                                            else if (escal)
                                                { if (incrT<0) incrT=-1/incrT;
                                                  TG.VScal.x=TG.VScal.x*incrT;
                                                  if(TG.VScal.x<0.25) TG.VScal.x=0.25;
                                                  if(TG.VScal.x>8192) TG.VScal.x=8192;
                                                }
                            }
                        if (transY)
                            {	long int incrT=girT.y;
                                if (trasl)
                                    {	TG.VTras.y+=incrT*fact_Tras;
                                        if (TG.VTras.y<-100000) TG.VTras.y=100000;
                                        if (TG.VTras.y>100000) TG.VTras.y=100000;
                                    }
                                    else if (rota)
                                        {	TG.VRota.y+=incrT*fact_Rota;
                                            while (TG.VRota.y>=360) TG.VRota.y-=360;
                                            while (TG.VRota.y<0) TG.VRota.y+=360;
                                        }
                                        else if (escal)
                                        {	if (incrT<=0) {	if (incrT>=-2) incrT=-2;
                                                            incrT=1/Log2(-incrT);}
                                                else incrT=Log2(incrT);
                                            TG.VScal.y=TG.VScal.y*incrT;
                                            if(TG.VScal.y<0.25) TG.VScal.y=0.25;
                                            if(TG.VScal.y>8192) TG.VScal.y=8192;
                                            }
                            }
//					m_PosEAvall = point;
                    m_PosEAvall.x = point.x;
                    m_PosEAvall.y = point.y;
//					InvalidateRect(NULL,false);
                    }
                }

// Determinació del desplaçament del pan segons l'increment
//   vertical de la posició del mouse (tecla dreta apretada)
        if (m_ButoDAvall && pan && (projeccio!=CAP && projeccio!=ORTO))
        {
//			CSize zoomincr=m_PosDAvall - point;
            zoomincr.x = m_PosDAvall.x - point.x;
            zoomincr.y = m_PosDAvall.y - point.y;

            long int incrx=zoomincr.x;
            long int incry=zoomincr.y;

            // Desplaçament pan vertical
            tr_cpv.y-=incry*fact_pan;
            if(tr_cpv.y>100000) tr_cpv.y=100000;
            else if(tr_cpv.y<-100000) tr_cpv.y=-100000;

            // Desplaçament pan horitzontal
            tr_cpv.x+=incrx*fact_pan;
            if(tr_cpv.x>100000) tr_cpv.x=100000;
            else if(tr_cpv.x<-100000) tr_cpv.x=-100000;

//			m_PosDAvall=point;
            m_PosDAvall.x = point.x;
            m_PosDAvall.y = point.y;
//			InvalidateRect(NULL,false);
        }
// Determinació del paràmetre R segons l'increment
//   vertical de la posició del mouse (tecla dreta apretada)
        //else if (m_ButoDAvall && zzoom && (projeccio!=CAP && projeccio!=ORTO))
        else if (m_ButoDAvall && zzoom && (projeccio!=CAP && projeccio!=ORTO))
            {
//				CSize zoomincr=m_PosDAvall - point;
                zoomincr.x = m_PosDAvall.x - point.x;
                zoomincr.y = m_PosDAvall.y - point.y;
                long int incr=zoomincr.y/1.0;

//				zoom=zoom+incr;
                OPV.R=OPV.R+incr;
                if (OPV.R<1) OPV.R=1;
//				m_PosDAvall=point;
                m_PosDAvall.x = point.x;
                m_PosDAvall.y = point.y;
//				InvalidateRect(NULL,false);
            }
            else if (m_ButoDAvall && zzoom && projeccio==ORTO)
//				{ if (m_PosDAvall!=point)
                { if ((m_PosDAvall.x!=point.x) &&  (m_PosDAvall.y!=point.y))
                    {	//CSize zoomincr=m_PosDAvall - point;
                        zoomincr.x = m_PosDAvall.x - point.x;
                        zoomincr.y = m_PosDAvall.y - point.y;
                        long int incr=zoomincr.y;
                        if (incr!=0)
                            {	if (mida+incr>0) mida=mida+incr;
                                OPVPl.R=OPVPl.R+incr;	// PLANTA
                                if (OPVPl.R<1) OPVPl.R=1;
                                OPVA.R=	OPVA.R+incr;	// ALÇAT
                                if (OPVA.R<1) OPVA.R=1;
                                OPVPr.R=OPVPr.R+incr;	// PERFIL
                                if (OPVPr.R<1) OPVPr.R=1;
                                OPVI.R=	OPVI.R+incr;	// ISOMÈTRICA
                                if (OPVI.R<1) OPVI.R=1;
                            }
//						m_PosDAvall=point;
                        m_PosDAvall.x = point.x;
                        m_PosDAvall.y = point.y;
//						InvalidateRect(NULL,false);
                    }
                }
                else if (m_ButoDAvall && navega && (projeccio==PERSPECT))
                {
// Avançar en opció de Navegació
//				if (m_PosDAvall!=point)
                if ((m_PosDAvall.x!=point.x) && (m_PosDAvall.y!=point.y))
                    {
//					CSize zoomincr=m_PosDAvall - point;
                    zoomincr.x = m_PosDAvall.x - point.x;
                    zoomincr.y = m_PosDAvall.y - point.y;

                    float incr=zoomincr.y/2;
//					long int incr=zoomincr.cy/2.0;  // Causa assertion en "afx.inl" lin 169
                    vdir[0]=n[0]-opvN.x;
                    vdir[1]=n[1]-opvN.y;
                    vdir[2]=n[2]-opvN.z;
                    modul=sqrt(vdir[0]*vdir[0]+vdir[1]*vdir[1]+vdir[2]*vdir[2]);
                    vdir[0]=vdir[0]/modul;
                    vdir[1]=vdir[1]/modul;
                    vdir[2]=vdir[2]/modul;
                    opvN.x+=incr*vdir[0];
                    opvN.y+=incr*vdir[1];
                    n[0]+=incr*vdir[0];
                    n[1]+=incr*vdir[1];
//					m_PosDAvall=point;
                    m_PosDAvall.x = point.x;
                    m_PosDAvall.y = point.y;
//					InvalidateRect(NULL,false);
                    }
                }

// Transformació Geomètrica interactiva per l'eix Z amb boto dret del mouse
                else if (m_ButoDAvall && transZ && transf)
                        {
// Calcular increment
//							CSize girT = m_PosDAvall - point;
                            girT.x = m_PosDAvall.x - point.x;
                            girT.y = m_PosDAvall.y - point.y;
                            long int incrT=girT.y;
                            if (trasl)
                                { TG.VTras.z+=incrT*fact_Tras;
                                  if (TG.VTras.z<-100000) TG.VTras.z=100000;
                                  if (TG.VTras.z>100000) TG.VTras.z=100000;
                                }
                                else if (rota)
                                        {	incrT=girT.x;
                                            TG.VRota.z+=incrT*fact_Rota;
                                            while (TG.VRota.z>=360) TG.VRota.z-=360;
                                            while (TG.VRota.z<0) TG.VRota.z+=360;
                                        }
                                    else if (escal)
                                        {	if (incrT<=0) {	if (incrT>=-2) incrT=-2;
                                                            incrT=1/Log2(-incrT);}
                                                else incrT=Log2(incrT);
                                            TG.VScal.z=TG.VScal.z*incrT;
                                            if(TG.VScal.z<0.25) TG.VScal.z=0.25;
                                            if(TG.VScal.z>8192) TG.VScal.z=8192;
                                        }

//							m_PosDAvall = point;
                            m_PosDAvall.x = point.x;
                            m_PosDAvall.y = point.y;
//							InvalidateRect(NULL,false);
                        }

// Crida a paintGL() per redibuixar l'escena
    updateGL();


// Refrescar Status Bar
//	((CGenv *)pare)->Barra_Estat();	//SOLUCIO 1: Cridar a cgenv des de contextGL
}

void GLWidget::wheelEvent(QWheelEvent * event )
{

    float modul=0;
    GLfloat vdir[3]={0,0,0};

// Obtenim el desplaçament del rodet del mouse
    short zDelta=event->delta();

// Funció de zoom quan està activada la funció pan o les T. Geomètriques
    if ((zzoom) || (transX) || (transY) || (transZ))
        {	OPV.R=OPV.R+zDelta/4;
            if (OPV.R<1) OPV.R=1;
            // Crida a paintGL() per redibuixar l'escena
            updateGL();
//			Invalidate();
    }
    else if (navega)
        {	vdir[0]=n[0]-opvN.x;
            vdir[1]=n[1]-opvN.y;
            vdir[2]=n[2]-opvN.z;
            modul=sqrt(vdir[0]*vdir[0]+vdir[1]*vdir[1]+vdir[2]*vdir[2]);
            vdir[0]=vdir[0]/modul;
            vdir[1]=vdir[1]/modul;
            vdir[2]=vdir[2]/modul;
            opvN.x+=(zDelta/4)*vdir[0];
            opvN.y+=(zDelta/4)*vdir[1];
            n[0]+=(zDelta/4)*vdir[0];
            n[1]+=(zDelta/4)*vdir[1];

// Crida a paintGL() per redibuixar l'escena
            updateGL();

// Refrescar Status Bar
//			((CGenv *)pare)->Barra_Estat();
        }
}

    /* ------------------------------------------------------------------------- */
    /*                           CONTROL DEL TECLAT                              */
    /* ------------------------------------------------------------------------- */

    // keyPressEvent: Funció de tractament de teclat (funció que es crida quan es prem una tecla)
    //   PARÀMETRES:
    //    - event: Estructura que conté informació de l'event
    void GLWidget::keyPressEvent(QKeyEvent* event) {

        const float incr=0.025f;
        float modul=0;
        GLfloat vdir[3]={0,0,0};
        int nChar,nRepCnt;

        nChar=event->key();
        nRepCnt=event->count();
        if ((!pan) && (!transf) && (!navega))
        { /*1*/ if (!sw_color)
            {/*2*/
    // Canvi de la intensitat de fons per teclat
            if(nChar==Qt::Key_Down) {
                if (fonsR) {
                    c_fons.r-=nRepCnt*incr;
                    if(c_fons.r<0.0) c_fons.r=0.0;
                    }
                if (fonsG) {
                    c_fons.g-=nRepCnt*incr;
                    if(c_fons.g<0.0) c_fons.g=0.0;
                    }
                if (fonsB) {
                    c_fons.b-=nRepCnt*incr;
                    if(c_fons.b<0.0) c_fons.b=0.0;
                    }
                }
                else if(nChar==Qt::Key_Up) {
                        if (fonsR) {
                            c_fons.r+=nRepCnt*incr;
                            if(c_fons.r>1.0) c_fons.r=1.0;
                            }
                        if (fonsG) {
                            c_fons.g+=nRepCnt*incr;
                            if(c_fons.g>1.0) c_fons.g=1.0;
                            }
                        if (fonsB) {
                            c_fons.b+=nRepCnt*incr;
                            if(c_fons.b>1.0) c_fons.b=1.0;
                            }
                        }
                    else if(nChar==Qt::Key_Space) {
                            if ((fonsR) && (fonsG) && (fonsB)) {
                                fonsG=false;
                                fonsB=false;
                                }
                            else if ((fonsR) && (fonsG)) {
                                fonsG=false;
                                fonsB=true;
                                }
                                else if ((fonsR) && (fonsB)) {
                                    fonsR=false;
                                    fonsG=true;
                                    }
                                    else if ((fonsG) && (fonsB)) fonsR=true;
                                        else if (fonsR) {
                                                fonsR=false;
                                                fonsG=true;
                                                }
                                            else if (fonsG) {
                                                    fonsG=false;
                                                    fonsB=true;
                                                    }
                                                else if (fonsB) {
                                                    fonsR=true;
                                                    fonsG=true;
                                                    fonsB=false;
                                                    }
                        }
                        else if ((nChar=='o') || (nChar==Qt::Key_O)) sw_color=true;
                            else if ((nChar=='f') || (nChar==Qt::Key_F)) sw_color=false;
                /*2*/}
            else
            {	/*2*/
    // Canvi de la intensitat de color de l'objecte per teclat
            if(nChar==Qt::Key_Down) {
                if (fonsR) {
                    col_obj.r-=nRepCnt*incr;
                    if(col_obj.r<0.0) col_obj.r=0.0;
                    }
                if (fonsG) {
                    col_obj.g-=nRepCnt*incr;
                    if(col_obj.g<0.0) col_obj.g=0.0;
                    }
                if (fonsB) {
                    col_obj.b-=nRepCnt*incr;
                    if(col_obj.b<0.0) col_obj.b=0.0;
                    }
                }
                else if(nChar==Qt::Key_Up) {
                        if (fonsR) {
                            col_obj.r+=nRepCnt*incr;
                            if(col_obj.r>1.0) col_obj.r=1.0;
                            }
                        if (fonsG) {
                            col_obj.g+=nRepCnt*incr;
                            if(col_obj.g>1.0) col_obj.g=1.0;
                            }
                        if (fonsB) {
                            col_obj.b+=nRepCnt*incr;
                            if(col_obj.b>1.0) col_obj.b=1.0;
                            }
                        }
                    else if(nChar==Qt::Key_Space) {
                            if ((fonsR) && (fonsG) && (fonsB)) {
                                fonsG=false;
                                fonsB=false;
                                }
                            else if ((fonsR) && (fonsG)) {
                                fonsG=false;
                                fonsB=true;
                                }
                                else if ((fonsR) && (fonsB)) {
                                    fonsR=false;
                                    fonsG=true;
                                    }
                                    else if ((fonsG) && (fonsB)) fonsR=true;
                                        else if (fonsR) {
                                                fonsR=false;
                                                fonsG=true;
                                                }
                                            else if (fonsG) {
                                                    fonsG=false;
                                                    fonsB=true;
                                                    }
                                                else if (fonsB) {
                                                    fonsR=true;
                                                    fonsG=true;
                                                    fonsB=false;
                                                    }
                        }
                        else if ((nChar=='o') || (nChar==Qt::Key_O)) sw_color=true;
                            else if ((nChar=='f') || (nChar==Qt::Key_F)) sw_color=false;
                /*2*/}
            /*1*/}
            else {
                if (transf)
                    { if (rota)
    // Modificar vector de rotació per teclat.
                        {switch(nChar)
                            {
                            // Tecla cursor amunt ('8')
                            case Qt::Key_8:
                                TG.VRota.x+=nRepCnt*fact_Rota;
                                if(TG.VRota.x>=360) TG.VRota.x-=360;
                                break;

                            // Tecla cursor avall ('2')
                            case Qt::Key_2:
                                TG.VRota.x-=nRepCnt*fact_Rota;
                                if(TG.VRota.x<1) TG.VRota.x+=360;
                                break;

                            // Tecla cursor esquerra ('4')
                            case Qt::Key_4:
                                TG.VRota.y-=nRepCnt*fact_Rota;
                                if(TG.VRota.y<1) TG.VRota.y+=360;
                                break;

                            // Tecla cursor dret ('6')
                            case Qt::Key_6:
                                TG.VRota.y+=nRepCnt*fact_Rota;
                                if(TG.VRota.y>=360) TG.VRota.y-=360;
                                break;

                            // Tecla HOME ('7')
                            case Qt::Key_7:
                                TG.VRota.z+=nRepCnt*fact_Rota;
                                if(TG.VRota.z>=360) TG.VRota.z-=360;
                                break;

                            // Tecla END ('1')
                            case Qt::Key_1:
                                TG.VRota.z-=nRepCnt*fact_Rota;
                                if(TG.VRota.z<1) TG.VRota.z+=360;
                                break;

                            // Tecla PgUp ('9')
                            case Qt::Key_9:
                                fact_Rota/=2;
                                if (fact_Rota<1) fact_Rota=1;
                                break;

                            // Tecla PgDown ('3')
                            case Qt::Key_3:
                                fact_Rota*=2;
                                if(fact_Rota>90) fact_Rota=90;
                                break;

    // Modificar vector d'Escalatge per teclat (actiu amb Rotació)
                            // Tecla '+' (augmentar escalat)
                            case Qt::Key_Plus:
                                TG.VScal.x=TG.VScal.x*2;
                                if(TG.VScal.x>8192) TG.VScal.x=8192;
                                TG.VScal.y=TG.VScal.y*2;
                                if(TG.VScal.y>8192) TG.VScal.y=8192;
                                TG.VScal.z=TG.VScal.z*2;
                                if(TG.VScal.z>8192) TG.VScal.z=8192;
                                break;

                            // Tecla '-' (disminuir escalat)
                            case Qt::Key_Minus:
                                TG.VScal.x=TG.VScal.x/2;
                                if(TG.VScal.x<0.25) TG.VScal.x=0.25;
                                TG.VScal.y=TG.VScal.y/2;
                                if(TG.VScal.y<0.25) TG.VScal.y=0.25;
                                TG.VScal.z=TG.VScal.z/2;
                                if(TG.VScal.z<0.25) TG.VScal.z=0.25;
                                break;

                            // Tecla Insert: Acumular transformacions Geomètriques (variable TG) i de pan en variables fixes (variable TGF)
                            case Qt::Key_Insert:
                                TGF.VScal.x*=TG.VScal.x;	TGF.VScal.y*=TG.VScal.y;TGF.VScal.z*=TG.VScal.z;
                                if(TGF.VScal.x>8192)		TGF.VScal.x=8192;
                                if(TGF.VScal.y>8192)		TGF.VScal.y=8192;
                                if(TGF.VScal.z>8192)		TGF.VScal.z=8192;
                                TG.VScal.x=1.0;				TG.VScal.y=1.0;			TG.VScal.z=1.0;
                                TGF.VRota.x+=TG.VRota.x;	TGF.VRota.y+=TG.VRota.y;TGF.VRota.z+=TG.VRota.z;
                                if(TGF.VRota.x>=360)		TGF.VRota.x-=360; 		if(TGF.VRota.x<0) TGF.VRota.x+=360;
                                if(TGF.VRota.y>=360)		TGF.VRota.y-=360;		if(TGF.VRota.y<0) TGF.VRota.y+=360;
                                if(TGF.VRota.z>=360)		TGF.VRota.z-=360;		if(TGF.VRota.z<0) TGF.VRota.z+=360;
                                TG.VRota.x=0.0;				TG.VRota.y=0.0;					TG.VRota.z=0.0;
                                TGF.VTras.x+=TG.VTras.x;	TGF.VTras.y+=TG.VTras.y;TGF.VTras.z+=TG.VTras.z;
                                if(TGF.VTras.x<-100000)		TGF.VTras.x=100000;		if(TGF.VTras.x>10000) TGF.VTras.x=100000;
                                if(TGF.VTras.y<-100000)		TGF.VTras.y=100000;		if(TGF.VTras.y>10000) TGF.VTras.y=100000;
                                if(TGF.VTras.z<-100000)		TGF.VTras.z=100000;		if(TGF.VTras.z>10000) TGF.VTras.z=100000;
                                TG.VTras.x=0.0;		TG.VTras.y=0.0;		TG.VTras.z=0.0;
                                break;

                            // Tecla Delete: Esborrar les Transformacions Geomètriques Calculades
                            case Qt::Key_Delete:
    // Inicialitzar els valors de transformacions Geomètriques i de pan en variables fixes.
                                TGF.VScal.x=1.0;	TGF.VScal.y=1.0;;	TGF.VScal.z=1.0;
                                TG.VScal.x=1.0;		TG.VScal.y=1.0;		TG.VScal.z=1.0;
                                TGF.VRota.x=0.0;	TGF.VRota.y=0.0;	TGF.VRota.z=0.0;
                                TG.VRota.x=0.0;		TG.VRota.y=0.0;		TG.VRota.z=0.0;
                                TGF.VTras.x=0.0;	TGF.VTras.y=0.0;	TGF.VTras.z=0.0;
                                TG.VTras.x=0.0;		TG.VTras.y=0.0;		TG.VTras.z=0.0;
                                break;

                            // Tecla Espaiador
                            case Qt::Key_Space:
                                rota=!rota;
                                trasl=!trasl;
                                break;
    //						default:
    //								break;
                            }
                        }
                        else if (trasl)
                            {switch(nChar)
                                {
    // Modificar vector de Traslació per teclat.
                                // Tecla cursor amunt ('8')
                                case Qt::Key_8:
                                    TG.VTras.x-=nRepCnt*fact_Tras;
                                    if(TG.VTras.x<-100000) TG.VTras.x=100000;
                                    break;

                                // Tecla cursor avall ('2')
                                case Qt::Key_2:
                                    TG.VTras.x+=nRepCnt*fact_Tras;
                                    if(TG.VTras.x>10000) TG.VTras.x=100000;
                                    break;

                                // Tecla cursor esquerra ('4')
                                case Qt::Key_4:
                                    TG.VTras.y-=nRepCnt*fact_Tras;
                                    if(TG.VTras.y<-100000) TG.VTras.y=-100000;
                                    break;

                                // Tecla cursor dret ('6')
                                case Qt::Key_6:
                                    TG.VTras.y+=nRepCnt*fact_Tras;
                                    if(TG.VTras.y>100000) TG.VTras.y=100000;
                                    break;

                                // Tecla HOME ('7')
                                case Qt::Key_7:
                                    TG.VTras.z+=nRepCnt*fact_Tras;
                                    if(TG.VTras.z>100000) TG.VTras.z=100000;
                                    break;

                                // Tecla END ('1')
                                case Qt::Key_1:
                                    TG.VTras.z-=nRepCnt*fact_Tras;
                                    if(TG.VTras.z<-100000) TG.VTras.z=-100000;
                                    break;

                                // Tecla PgUp ('9')
                                case Qt::Key_9:
                                    fact_Tras/=2;
                                    if (fact_Tras<1) fact_Tras=1;
                                    break;

                                // Tecla PgDown ('3')
                                case Qt::Key_3:
                                    fact_Tras*=2;
                                    if(fact_Tras>100000) fact_Tras=100000;
                                    break;

    // Modificar vector d'Escalatge per teclat (actiu amb Traslació)
                                // Tecla '+' (augmentar escalat)
                                case Qt::Key_Plus:
                                    TG.VScal.x=TG.VScal.x*2;
                                    if(TG.VScal.x>8192) TG.VScal.x=8192;
                                    TG.VScal.y=TG.VScal.y*2;
                                    if(TG.VScal.y>8192) TG.VScal.y=8192;
                                    TG.VScal.z=TG.VScal.z*2;
                                    if(TG.VScal.z>8192) TG.VScal.z=8192;
                                    break;

                                // Tecla '-' (disminuir escalat)
                                case Qt::Key_Minus:
                                    TG.VScal.x=TG.VScal.x/2;
                                    if(TG.VScal.x<0.25) TG.VScal.x=0.25;
                                    TG.VScal.y=TG.VScal.y/2;
                                    if(TG.VScal.y<0.25) TG.VScal.y=0.25;
                                    TG.VScal.z=TG.VScal.z/2;
                                    if(TG.VScal.z<0.25) TG.VScal.z=0.25;
                                    break;

                                case Qt::Key_Insert:
                                // Acumular transformacions Geomètriques (variable TG) i de pan en variables fixes (variable TGF)
                                    TGF.VScal.x*=TG.VScal.x;	TGF.VScal.y*=TG.VScal.y;TGF.VScal.z*=TG.VScal.z;
                                    if(TGF.VScal.x>8192)		TGF.VScal.x=8192;
                                    if(TGF.VScal.y>8192)		TGF.VScal.y=8192;
                                    if(TGF.VScal.z>8192)		TGF.VScal.z=8192;
                                    TG.VScal.x=1.0;				TG.VScal.y=1.0;			TG.VScal.z=1.0;
                                    TGF.VRota.x+=TG.VRota.x;	TGF.VRota.y+=TG.VRota.y;TGF.VRota.z+=TG.VRota.z;
                                    if(TGF.VRota.x>=360)		TGF.VRota.x-=360; 		if(TGF.VRota.x<0) TGF.VRota.x+=360;
                                    if(TGF.VRota.y>=360)		TGF.VRota.y-=360;		if(TGF.VRota.y<0) TGF.VRota.y+=360;
                                    if(TGF.VRota.z>=360)		TGF.VRota.z-=360;		if(TGF.VRota.z<0) TGF.VRota.z+=360;
                                    TG.VRota.x=0.0;				TG.VRota.y=0.0;					TG.VRota.z=0.0;
                                    TGF.VTras.x+=TG.VTras.x;	TGF.VTras.y+=TG.VTras.y;TGF.VTras.z+=TG.VTras.z;
                                    if(TGF.VTras.x<-100000)		TGF.VTras.x=100000;		if(TGF.VTras.x>10000) TGF.VTras.x=100000;
                                    if(TGF.VTras.y<-100000)		TGF.VTras.y=100000;		if(TGF.VTras.y>10000) TGF.VTras.y=100000;
                                    if(TGF.VTras.z<-100000)		TGF.VTras.z=100000;		if(TGF.VTras.z>10000) TGF.VTras.z=100000;
                                    TG.VTras.x=0.0;		TG.VTras.y=0.0;		TG.VTras.z=0.0;
                                    break;

                            // Tecla Delete: Esborrar les Transformacions Geomètriques Calculades
                                case Qt::Key_Delete:
    // Inicialitzar els valors de transformacions Geomètriques i de pan en variables fixes.
                                    TGF.VScal.x=1.0;		TGF.VScal.y=1.0;;		TGF.VScal.z=1.0;
                                    TG.VScal.x=1.0;		TG.VScal.y=1.0;		TG.VScal.z=1.0;
                                    TGF.VRota.x=0.0;		TGF.VRota.y=0.0;		TGF.VRota.z=0.0;
                                    TG.VRota.x=0.0;		TG.VRota.y=0.0;		TG.VRota.z=0.0;
                                    TGF.VTras.x=0.0;		TGF.VTras.y=0.0;		TGF.VTras.z=0.0;
                                    TG.VTras.x=0.0;		TG.VTras.y=0.0;		TG.VTras.z=0.0;
                                    break;

                                // Tecla Espaiador
                                case Qt::Key_Space:
                                    rota=!rota;
                                    trasl=!trasl;
                                    break;
    //							default:
    //								break;
                                }
                            }
                        else if (escal)
                                {/*3*/switch(nChar)
                                    {/*4*/
    // Modificar vector d'Escalatge per teclat (actiu amb Escalat únicament)
                                    // Tecla '+' (augmentar tot l'escalat)
                                    case Qt::Key_Plus:
                                        TG.VScal.x=TG.VScal.x*2;
                                        if(TG.VScal.x>8192) TG.VScal.x=8192;
                                        TG.VScal.y=TG.VScal.y*2;
                                        if(TG.VScal.y>8192) TG.VScal.y=8192;
                                        TG.VScal.z=TG.VScal.z*2;
                                        if(TG.VScal.z>8192) TG.VScal.z=8192;
                                        break;

                                    // Tecla '-' (disminuir tot l'escalat)
                                    case Qt::Key_Minus:
                                        TG.VScal.x=TG.VScal.x/2;
                                        if(TG.VScal.x<0.25) TG.VScal.x=0.25;
                                        TG.VScal.y=TG.VScal.y/2;
                                        if(TG.VScal.y<0.25) TG.VScal.y=0.25;
                                        TG.VScal.z=TG.VScal.z/2;
                                        if(TG.VScal.z<0.25) TG.VScal.z=0.25;
                                        break;

                                    // Tecla cursor amunt ('8')
                                    case Qt::Key_8:
                                        TG.VScal.x=TG.VScal.x*2;
                                        if(TG.VScal.x>8192) TG.VScal.x=8192;
                                        break;

                                    // Tecla cursor avall ('2')
                                    case Qt::Key_2:
                                        TG.VScal.x=TG.VScal.x/2;
                                        if(TG.VScal.x<0.25) TG.VScal.x=0.25;
                                        break;

                                    // Tecla cursor esquerra ('4')
                                    case Qt::Key_4:
                                        TG.VScal.y=TG.VScal.y/2;
                                        if(TG.VScal.y<0.25) TG.VScal.y=0.25;
                                        break;

                                    // Tecla cursor dret ('6')
                                    case Qt::Key_6:
                                        TG.VScal.y=TG.VScal.y*2;
                                        if(TG.VScal.y>8192) TG.VScal.y=8192;
                                        break;

                                    // Tecla HOME ('7')
                                    case Qt::Key_7:
                                        TG.VScal.z=TG.VScal.z*2;
                                        if(TG.VScal.z>8192) TG.VScal.z=8192;
                                        break;

                                    // Tecla END ('1')
                                    case Qt::Key_1:
                                        TG.VScal.z=TG.VScal.z/2;
                                        if(TG.VScal.z<0.25) TG.VScal.z=0.25;
                                        break;

                                    case Qt::Key_Insert:
                                    // Acumular transformacions Geomètriques (variable TG) i de pan en variables fixes (variable TGF)
                                        TGF.VScal.x*=TG.VScal.x;	TGF.VScal.y*=TG.VScal.y;TGF.VScal.z*=TG.VScal.z;
                                        if(TGF.VScal.x>8192)		TGF.VScal.x=8192;
                                        if(TGF.VScal.y>8192)		TGF.VScal.y=8192;
                                        if(TGF.VScal.z>8192)		TGF.VScal.z=8192;
                                        TG.VScal.x=1.0;				TG.VScal.y=1.0;			TG.VScal.z=1.0;
                                        TGF.VRota.x+=TG.VRota.x;	TGF.VRota.y+=TG.VRota.y;TGF.VRota.z+=TG.VRota.z;
                                        if(TGF.VRota.x>=360)		TGF.VRota.x-=360; 		if(TGF.VRota.x<0) TGF.VRota.x+=360;
                                        if(TGF.VRota.y>=360)		TGF.VRota.y-=360;		if(TGF.VRota.y<0) TGF.VRota.y+=360;
                                        if(TGF.VRota.z>=360)		TGF.VRota.z-=360;		if(TGF.VRota.z<0) TGF.VRota.z+=360;
                                        TG.VRota.x=0.0;				TG.VRota.y=0.0;					TG.VRota.z=0.0;
                                        TGF.VTras.x+=TG.VTras.x;	TGF.VTras.y+=TG.VTras.y;TGF.VTras.z+=TG.VTras.z;
                                        if(TGF.VTras.x<-100000)		TGF.VTras.x=100000;		if(TGF.VTras.x>10000) TGF.VTras.x=100000;
                                        if(TGF.VTras.y<-100000)		TGF.VTras.y=100000;		if(TGF.VTras.y>10000) TGF.VTras.y=100000;
                                        if(TGF.VTras.z<-100000)		TGF.VTras.z=100000;		if(TGF.VTras.z>10000) TGF.VTras.z=100000;
                                        TG.VTras.x=0.0;		TG.VTras.y=0.0;		TG.VTras.z=0.0;
                                        break;

                                // Tecla Delete: Esborrar les Transformacions Geomètriques Calculades
                                    case Qt::Key_Delete:
    // Inicialitzar els valors de transformacions Geomètriques i de pan en variables fixes.
                                        TGF.VScal.x=1.0;		TGF.VScal.y=1.0;;		TGF.VScal.z=1.0;
                                        TG.VScal.x=1.0;		TG.VScal.y=1.0;		TG.VScal.z=1.0;
                                        TGF.VRota.x=0.0;		TGF.VRota.y=0.0;		TGF.VRota.z=0.0;
                                        TG.VRota.x=0.0;		TG.VRota.y=0.0;		TG.VRota.z=0.0;
                                        TGF.VTras.x=0.0;		TGF.VTras.y=0.0;		TGF.VTras.z=0.0;
                                        TG.VTras.x=0.0;		TG.VTras.y=0.0;		TG.VTras.z=0.0;
                                        break;

    //								default:
    //									break;
                                /*4*/}
                            /*3*/}
                            }
    //				}
                    if (pan) {
                        switch(nChar)
                            {
                                // Tecla cursor amunt
                                case Qt::Key_Up:
                                    tr_cpv.y-=nRepCnt*fact_pan;
                                    if(tr_cpv.y<-100000) tr_cpv.y=100000;
                                    break;

                                // Tecla cursor avall
                                case Qt::Key_Down:
                                    tr_cpv.y+=nRepCnt*fact_pan;
                                    if(tr_cpv.y>100000) tr_cpv.y=100000;
                                    break;

                                // Tecla cursor esquerra
                                case Qt::Key_Left:
                                    tr_cpv.x+=nRepCnt*fact_pan;
                                    if(tr_cpv.x>100000) tr_cpv.x=100000;
                                    break;

                                // Tecla cursor dret
                                case Qt::Key_Right:
                                    tr_cpv.x-=nRepCnt*fact_pan;
                                    if(tr_cpv.x<-100000) tr_cpv.x=100000;
                                    break;

                                // Tecla PgUp
                                case Qt::Key_PageUp:
                                    fact_pan/=2;
                                    if(fact_pan<0.125) fact_pan=0.125;
                                    break;

                                // Tecla PgDown
                                case Qt::Key_PageDown:
                                    fact_pan*=2;
                                    if(fact_pan>2048) fact_pan=2048;
                                    break;

                                // Tecla Insert: Fixar el desplaçament de pantalla (pan)
                                case Qt::Key_Insert:
                                // Acumular desplaçaments de pan (tr_cpv) en variables fixes (tr_cpvF).
                                    tr_cpvF.x+=tr_cpv.x;		tr_cpv.x=0.0;
                                    if(tr_cpvF.x>100000) tr_cpvF.x=100000;
                                    tr_cpvF.y+=tr_cpv.y;		tr_cpv.y=0.0;
                                    if(tr_cpvF.y>100000) tr_cpvF.y=100000;
                                    tr_cpvF.z+=tr_cpv.z;		tr_cpv.z=0.0;
                                    if(tr_cpvF.z>100000) tr_cpvF.z=100000;
                                    break;

                                // Tecla Delete: Inicialitzar el desplaçament de pantalla (pan)
                                case Qt::Key_Delete:
                                // Inicialitzar els valors de pan tant de la variable tr_cpv com de la tr_cpvF.
                                    tr_cpv.x=0.0;		tr_cpv.y=0.0;			tr_cpv.z=0.0;
                                    tr_cpvF.x=0.0;		tr_cpvF.y=0.0;			tr_cpvF.z=0.0;
                                    break;

                                default:
                                    break;
                            }
                        }
                        else if (navega)
    // Controls de moviment de navegació
                                { vdir[0]=n[0]-opvN.x;
                                  vdir[1]=n[1]-opvN.y;
                                  vdir[2]=n[2]-opvN.z;
                                  modul=sqrt(vdir[0]*vdir[0]+vdir[1]*vdir[1]+vdir[2]*vdir[2]);
                                  vdir[0]=vdir[0]/modul;
                                  vdir[1]=vdir[1]/modul;
                                  vdir[2]=vdir[2]/modul;
                                  switch(nChar)
                                    {
                                    // Tecla cursor amunt
                                    case Qt::Key_Up:
                                        opvN.x+=nRepCnt*fact_pan*vdir[0];
                                        opvN.y+=nRepCnt*fact_pan*vdir[1];
                                        n[0]+=nRepCnt*fact_pan*vdir[0];
                                        n[1]+=nRepCnt*fact_pan*vdir[1];
                                        break;

                                    // Tecla cursor avall
                                    case Qt::Key_Down:
                                        opvN.x-=nRepCnt*fact_pan*vdir[0];
                                        opvN.y-=nRepCnt*fact_pan*vdir[1];
                                        n[0]-=nRepCnt*fact_pan*vdir[0];
                                        n[1]-=nRepCnt*fact_pan*vdir[1];
                                        break;

                                    // Tecla cursor esquerra
                                    case Qt::Key_Left:
                                        angleZ=+nRepCnt*fact_pan;
                                        n[0]=n[0]-opvN.x;
                                        n[1]=n[1]-opvN.y;
                                        n[0]=n[0]*cos(angleZ*pi/180)-n[1]*sin(angleZ*pi/180);
                                        n[1]=n[0]*sin(angleZ*pi/180)+n[1]*cos(angleZ*pi/180);
                                        n[0]=n[0]+opvN.x;
                                        n[1]=n[1]+opvN.y;
                                        break;

                                    // Tecla cursor dret
                                    case Qt::Key_Right:
                                        angleZ=360-nRepCnt*fact_pan;
                                        n[0]=n[0]-opvN.x;
                                        n[1]=n[1]-opvN.y;
                                        n[0]=n[0]*cos(angleZ*pi/180)-n[1]*sin(angleZ*pi/180);
                                        n[1]=n[0]*sin(angleZ*pi/180)+n[1]*cos(angleZ*pi/180);
                                        n[0]=n[0]+opvN.x;
                                        n[1]=n[1]+opvN.y;
                                        break;

                                    // Tecla Inicio
                                    case Qt::Key_Home:
                                        opvN.z+=nRepCnt*fact_pan;
                                        n[2]+=nRepCnt*fact_pan;
                                        break;

                                    // Tecla Fin
                                    case Qt::Key_End:
                                        opvN.z-=nRepCnt*fact_pan;
                                        n[2]-=nRepCnt*fact_pan;
                                        break;

                                    // Tecla PgUp
                                    case Qt::Key_PageUp:
                                        fact_pan/=2;
                                        if(fact_pan<1) fact_pan=1;
                                        break;

                                    // Tecla PgDown
                                    case Qt::Key_PageDown:
                                        fact_pan*=2;
                                        if(fact_pan>2048) fact_pan=2048;
                                        break;

                                    default:
                                        break;
                                    }
                                }
                    }


    // Crida a paintGL() per redibuixar l'escena
        updateGL();
}

    /* ------------------------------------------------------------------------- */
    /*   RECURSOS DE MENU (persianes) DE L'APLICACIO:                            */
    /*					1. ARXIUS												 */
    /*					2. VEURE: ToolBar, StatusBar, Pantalla Completa			 */
    /*					3. VISTA: Punt de Vista, Zoom, Pan Navega i Eixos	     */
    /*					4. PROJECCIÓ                                             */
    /*					5. OBJECTE					                             */
    /*					6. TRANSFORMA											 */
    /*					7. OCULTACIONS											 */
    /*					8. IL.LUMINACIÓ											 */
    /*					9. TIMER (ANIMACIÓ)									     */
    /* ------------------------------------------------------------------------- */

    /* ------------------------------------------------------------------------- */
    /*					1. ARXIUS 												 */
    /* ------------------------------------------------------------------------- */

    // FILE->New
    void GLWidget::OnFNew()
    {
    // TODO: Add your command handler code here
        objecte=CAP;
        projeccio=PERSPECT;

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }

    // Obrir fitxer
    void GLWidget::OnFileOpen()
    {

    //  Obrir diàleg de lectura de fitxer (fitxers (*.MNT)
    /*	CFileDialog openMunt (TRUE, NULL, NULL,
            OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ,
            _T("MNT Files(*.mnt)|*.mnt|Error Files (*.err)|*err|All Files (*.*)|*.*||"));;

        if (openMunt.DoModal() != IDOK)
            return;                 // stay with old data file
        else
           {nom=openMunt.GetPathName();}
    */
    // Variable de tipus CString 'nom' conté el nom del fitxer seleccionat

    // Crida a paintGL() per redibuixar l'escena
        updateGL();
    }

    // Obrir fitxer en format gràfic 3DS
    void GLWidget::OnFileOpen3ds()
    {
        QString nomfitx;
    // TODO: Add your command handler code here
        //if(Ob3DS!=NULL) delete Ob3DS;

    //	objecte=OBJ3DS;	textura=true;
        nomfitx="";

    // Obrir diàleg de lectura de fitxer
        nomfitx = QFileDialog::getOpenFileName(this, tr("Open 3DS File"),"./",tr("3DS Files (*.3ds)"));

    //	Conversió variable QString (nomfitx) a nomfitxer (const char*), compatible amb la funció carregar3DS
        QByteArray a=nomfitx.toLatin1();
        const char *nomfitxer=a.data();

        if (nomfitx!="") {
            objecte=OBJ3DS;
        // i carreguem
            if (Ob3DS==NULL) Ob3DS = new Obj_3DS;
                else Ob3DS->EliminarMemoria();
            Ob3DS->Inicialitzar();
            Ob3DS->Carregar3DS(nomfitxer);

    // Precompilació de dos objectes nous: OBJECTE3DS: Objecte 3DS sense textures i OBJECTE3DST amb textures,
            Ob3DS->Dibuixa3DS(false,OBJECTE3DS,false);
            Ob3DS->Dibuixa3DS(false,OBJECTE3DS_T,true);
        //Ob3DS->~Obj_3DS();
        //Ob3DS=NULL;
        //delete  Ob3DS;

    //  Modificar R per centrar Vista amb mida de l'objecte
            mida=sqrtf(3.0)*10;
            OPV.R=0.5*mida/sin(30*pi/180)+p_near;
        }

    // Crida a paintGL() per redibuixar l'escena
        updateGL();

    }

    // OnFileOpenObj: Obrir fitxer en format gràfic OBJ
    void GLWidget::OnFileOpenObj()
    {

        QString nomfitx;
    // TODO: Add your command handler code here
    //	if(ObOBJ!=NULL) delete ObOBJ;

        nomfitx="";
    //	objecte=OBJOBJ;

    // Obrir diàleg de lectura de fitxer
        nomfitx = QFileDialog::getOpenFileName(this, tr("Open OBJ File"),"./",tr("OBJ Files (*.obj)"));

    //	Conversió variable QString (nomfitx) a nomfitxer (const char*), compatible amb la funció carregar3DS
        QByteArray a=nomfitx.toLatin1();
        const char *nomfitxer=a.data();

        if (nomfitx!="") {
            objecte=OBJOBJ;
            // i carreguem
            if (ObOBJ==NULL) ObOBJ = new COBJModel;
            ObOBJ->texture=true;
            ObOBJ->LoadModel(nomfitxer,OBJECTEOBJ_T);
            ObOBJ->texture=false;
            ObOBJ->LoadModel(nomfitxer,OBJECTEOBJ);
            // ObOBJ->~Obj_3DS();
            // ObOBJ=NULL;
            // delete  ObOBJ;
            //  Modificar R per centrar Vista amb mida de l'objecte
            mida=sqrtf(3.0)*10;
            OPV.R=0.5*mida/sin(30*pi/180)+p_near;
        }

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();

    }

    /* ------------------------------------------------------------------------- */
    /*					2. VEURE: ToolBar), StatusBar, Pantalla Completa         */
    /* ------------------------------------------------------------------------- */

    // PANTALLA COMPLETA: opció booleana
    void GLWidget::OnFullScreen()
    {
    // TODO: Add your command handler code here
    //	fullscreen=!fullscreen;

        if (fullScreen) this->showFullScreen();
        else this->showNormal();

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }


    /* ------------------------------------------------------------------------- */
    /*					3. VISTA: Punt de Vista, Zoom, Pan i Eixos				 */
    /* ------------------------------------------------------------------------- */

    // VISTA: Mobil. Punt de Vista Interactiu (opció booleana)
    void GLWidget::OnMobil()
    {
    // TODO: Add your command handler code here
    //	if (projeccio!=CAP) mobil=!mobil;
    // Desactivació de Transformacions Geomètriques via mouse
    //	si Visualització Interactiva activada
        if (mobil)	{	transX=false;	transY=false; transZ=false;
                    //actionMobil->setOn(true);
                    }
    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }

    // VISTA: Zoom. Zoom Interactiu (opció booleana)
    void GLWidget::OnZoom()
    {
    // TODO: Add your command handler code here
    //	if (projeccio==PERSPECT || projeccio==ORTO) zzoom=!zzoom;
    // Desactivació de Transformacions Geomètriques via mouse
    //	si Zoom activat
        if (zzoom)	{	transX=false;	transY=false;	transZ=false;
                    }
    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }


    // VISTA: Polars Eix Z cap amunt per a Visialització Interactiva
    bool GLWidget::OnPolarZ()
    {
    // TODO: Add your command handler code here
        if ((projeccio!=CAP) && (!navega)) Vis_Polar=POLARZ;

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();

        if ((projeccio!=CAP) && (!navega)) return true;
            else return false;

    }

    // VISTA: Polars Eix Y cap amunt per a Visialització Interactiva
    bool GLWidget::OnPolarY()
    {
    // TODO: Add your command handler code here
        if ((projeccio!=CAP) && (!navega)) Vis_Polar=POLARY;

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();

        if ((projeccio!=CAP) && (!navega)) return true;
            else return false;

    }


    // VISTA: Polars Eix X cap amunt per a Visialització Interactiva
    bool GLWidget::OnPolarX()
    {
    // TODO: Add your command handler code here
        if ((projeccio!=CAP) && (!navega)) Vis_Polar=POLARX;

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();

        if ((projeccio!=CAP) && (!navega)) return true;
            else return false;

    }


    // VISTA: PAN. Desplaçament del Centre de l'esfera de visualització (opció booleana)
    void GLWidget::OnPan()
    {
    // TODO: Add your command handler code here
    //	if (projeccio==PERSPECT) pan=!pan;
    // Desactivació de Transformacions Geomètriques via mouse i navega si pan activat
        if (pan)	{	mobil=true;		zzoom=true;
                        transX=false;	transY=false;	transZ=false;
                        navega=false;
                    }

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }


    // VISTA: INIPAN. Origen (inicialitzar) Pan
    void GLWidget::OnInipan()
    {
    // TODO: Add your command handler code here
        if (pan) {	fact_pan=1;
                    tr_cpv.x=0;	tr_cpv.y=0;	tr_cpv.z=0;
                    }
    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }


    // VISTA: NAVEGA. Opció Navega (opció booleana)
    void GLWidget::OnNavega()
    {
    // TODO: Add your command handler code here
    //	if (projeccio==PERSPECT) navega=!navega;
    // Desactivació de zoom, mobil, Transformacions Geomètriques via mouse i pan
    //		si navega activat
        if (navega){	mobil=false;	zzoom=false;
                        transX=false;	transY=false;	transZ=false;
                        pan=false;
                        tr_cpv.x=0.0;	tr_cpv.y=0.0;	tr_cpv.z=0.0;	// Inicialitzar a 0 desplaçament de pantalla
                        tr_cpvF.x=0.0;	tr_cpvF.y=0.0;	tr_cpvF.z=0.0; // Inicialitzar a 0 desplaçament de pantalla
                    }
        else {mobil=true;
              zzoom=true;}

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }


    // VISTA: Origen Navegació
    void GLWidget::OnIninav()
    {
    // TODO: Add your command handler code here
        if (navega) {	n[0]=0.0;		n[1]=0.0;		n[2]=0.0;
                        opvN.x=10.0;	opvN.y=0.0;		opvN.z=0.0;
                        angleZ=0.0;
                    }
    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }


    // VISTA: Dibuixar els eixos (opció booleana)
    void GLWidget::OnEixos()
    {
    // TODO: Add your command handler code here
    //	eixos=!eixos;
    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }


    /* ------------------------------------------------------------------------- */
    /*					4. PROJECCIÓ                                             */
    /* ------------------------------------------------------------------------- */

    // PROJECCIÓ: Perspectiva
    void GLWidget::OnPerspectiva()
    {
    // TODO: Add your command handler code here
        projeccio=PERSPECT;
        mobil=true;			zzoom=true;

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }

    // PROJECCIÓ: Perspectiva
    void GLWidget::OnOrtografica()
    {
    // TODO: Add your command handler code here
        projeccio=ORTO;
        mobil=true;			zzoom=true;

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }

    /* ------------------------------------------------------------------------- */
    /*					5. OBJECTE					                             */
    /* ------------------------------------------------------------------------- */

    // OBJECTE Cub
    void GLWidget::OnCube()
    {
    // TODO: Add your command handler code here
        objecte=CUB;

    //  Modificar R per centrar Vista amb mida de l'objecte
        mida=5*sqrtf(3);
        float D = sqrt (5 * 5 + 5 * 5 + 5 * 5); // D = sqrt(rangx*rangx + rangy*rangy + rangz*rangz)
        VVOrto = sqrt(3)/D; // S = 2r/D
        OPV.R=0.5*mida/sin(30*pi/180)+p_near;

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }


    // OBJECTE Esfera
    void GLWidget::OnSphere()
    {
    // TODO: Add your command handler code here
        objecte=ESFERA;

    //  Modificar R per centrar Vista amb mida de l'objecte
        mida=10;
        OPV.R=0.5*mida/sin(30*pi/180)+p_near;

        float D = sqrt (10 * 10 + 10 * 10 + 10 * 10); // D = sqrt(rangx*rangx + rangy*rangy + rangz*rangz)
        VVOrto = (sqrt(3)/D); // S = 2r/D

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }


    // OBJECTE Tetera
    void GLWidget::OnTeapot()
    {
    // TODO: Add your command handler code here
        objecte=TETERA;

    //  Modificar R per centrar Vista amb mida de l'objecte
        mida=sqrtf(3)*7.25;

        OPV.R=0.5*mida/sin(30*pi/180)+p_near;

        float D = sqrt (10 * 10 + 10 * 10 + 10 * 10); // D = sqrt(rangx*rangx + rangy*rangy + rangz*rangz)
        VVOrto = (sqrt(3)/D); // S = 2r/D

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }


    // OBJECTE Truck
    void GLWidget::OnTruck()
    {
    // TODO: Add your command handler code here
        objecte=TRUCK;

    // Inicialitza vector imatges textura pel Truck.
        escena.Init_Textures();

    //  Modificar R per centrar Vista amb mida de l'objecte
        mida=sqrtf(3)*90;
        OPV.R=0.5*mida/sin(30*pi/180)+p_near;

        float D = sqrt (30 * 30 + 210 * 210 + 70 * 70); // D = sqrt(rangx*rangx + rangy*rangy + rangz*rangz)
        VVOrto = (sqrt(3)/D); // S = 2r/D

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }

    // OBJECTE Camio
    void GLWidget::OnCamio()
    {
    // TODO: Add your command handler code here
        objecte=CAMIO;

    //  Modificar R per centrar Vista amb mida de l'objecte
        mida=sqrtf(3)*9;
        OPV.R=0.5*mida/sin(30*pi/180)+p_near;

        float D = sqrt (2 * 2 + 6.5 * 6.5 + 2.1 * 2.1); // D = sqrt(rangx*rangx + rangy*rangy + rangz*rangz)
        VVOrto = (sqrt(3)/D); // S = 2r/D

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }


    /* ------------------------------------------------------------------------- */
    /*					6. TRANSFORMA											 */
    /* ------------------------------------------------------------------------- */

    // TRANSFORMA: TRASLACIÓ
    void GLWidget::OnTraslacio()
    {
    // TODO: Add your command handler code here
    //	trasl=!trasl;
    //	rota=false;
    //	if (trasl) escal=true;
        transf=trasl || rota || escal;

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }


    // Reinicialitza a l'origen la Traslació
    void GLWidget::OnInitras()
    {
    // TODO: Add your command handler code here
        if (trasl)
            {	fact_Tras=1;
                TG.VTras.x=0.0;	TG.VTras.y=0.0;	TG.VTras.z=0;
            }
    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }

    // TRANSFORMA: ROTACIÓ
    void GLWidget::OnRotacio()
    {
    // TODO: Add your command handler code here
    //	rota=!rota;
    //	trasl=false;
    //	if (rota) escal=true;
        transf=trasl || rota || escal;

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }


    // Reinicialitza a l'origen la Rotació
    void GLWidget::OnInirot()
    {
    // TODO: Add your command handler code here
        if (rota)
            {	fact_Rota=90;
                TG.VRota.x=0;		TG.VRota.y=0;		TG.VRota.z=0;
            }
    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }


    // TRANSFORMA: ESCALATGE
    void GLWidget::OnEscalatge()
    {
    // TODO: Add your command handler code here
    //	if ((!rota) && (!trasl)) escal=!escal;
        transf=trasl || rota || escal;

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }


    // Reinicialitza a l'origen l'Escalatge
    void GLWidget::OnIniescal()
    {
    // TODO: Add your command handler code here
        if (escal) {TG.VScal.x=1;	TG.VScal.y=1;	TG.VScal.z=1;}

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }

    // TRANSFORMA: Transformacions Geomètriques per l'eix X movent el mouse (booleana)
    void GLWidget::OnMobilX()
    {
    // TODO: Add your command handler code here
        if (transf)
            {	transX=!transX;
                if (transX) {	mobil=false;			zzoom=false;
                                pan=false;				navega=false;
                            }
                    else if ((!transY) && (!transZ)){	mobil=true;
                                                        zzoom=true;
                                                    }
            }

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }

    // TRANSFORMA: Transformacions Geomètriques per l'eix Y movent el mouse (booleana)
    void GLWidget::OnMobilY()
    {
    // TODO: Add your command handler code here
        if (transf)
            {	transY=!transY;
                if (transY) {	mobil=false;			zzoom=false;
                                pan=false;				navega=false;
                            }
                    else if ((!transX) && (!transZ)){	mobil=true;
                                                        zzoom=true;
                                                    }
            }

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }

    // TRANSFORMA: Transformacions Geomètriques per l'eix X movent el mouse (booleana)
    void GLWidget::OnMobilZ()
    {
    // TODO: Add your command handler code here
        if (transf)
            {	transZ=!transZ;
                if (transZ) {	mobil=false;			zzoom=false;
                                pan=false;				navega=false;
                            }
                    else if ((!transX) && (!transY)) {	mobil=true;
                                                        zzoom=true;
                                                    }
            }

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }


    /* ------------------------------------------------------------------------- */
    /*					7. OCULTACIONS											 */
    /* ------------------------------------------------------------------------- */

    // OCULTACIONS: Test de Visibilitat (BACK FACE CULLING) (opció booleana)
    void GLWidget::OnTest()
    {
    // TODO: Add your command handler code here
        //test_vis=!test_vis;

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }

    // OCULTACIONS: Algorisme d'ocultacions Z-buffer (opció booleana)
    void GLWidget::OnZBuffer()
    {
    // TODO: Add your command handler code here
        //oculta=!oculta;

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }


    // OCULTACIONS: Dibuix cares darrera com a línies (opció booleana)
    void GLWidget::OnBackLine()
    {
    // TODO: Add your command handler code here
        //back_line=!back_line;

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }


    /* ------------------------------------------------------------------------- */
    /*					8. IL.LUMINACIÓ											 */
    /* ------------------------------------------------------------------------- */

    // IL.LUMINACIÓ Font de llum fixe? (opció booleana).
    void GLWidget::OnIFixe()
    {
    // TODO: Add your command handler code here
        //ifixe=!ifixe;

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }


    // IL.LUMINACIÓ Filferros
    void GLWidget::OnFilferros()
    {
    // TODO: Add your command handler code here
        ilumina=FILFERROS;
        //test_vis=false;		oculta=false;

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }


    // IL.LUMINACIÓ plana
    void GLWidget::OnFlat()
    {
    // TODO: Add your command handler code here
        ilumina=PLANA;
        //test_vis=false;		oculta=true;

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }


    // IL.LUMINACIÓ Suau
    void GLWidget::OnGouraud()
    {
    // TODO: Add your command handler code here
        ilumina=SUAU;
        //test_vis=false;		oculta=true;

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }

    // IL.LUMINACIÓ->TEXTURA Activació i desactivació de textura
    void GLWidget::OnTexture()
    {
    // TODO: Add your command handler code here
        //textura=!textura;

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();

    }


    void GLWidget::OnTexturima()
    {
    // TODO: Add your command handler code here

        QString nomtext;
    //	CString nomt;
    //	char* nomt="";

        t_textura=FITXERIMA;

    // Obrir diàleg de lectura de fitxer
        nomtext = QFileDialog::getOpenFileName(this, tr("Open image File"),"./",tr("JPG,BMP,TIF,TGA Files (*.*)"));

    //	Conversió variable QString (nomfitx) a nomfitxer (const char*), compatible amb la funció carregar3DS
        QByteArray b=nomtext.toLatin1();
        char *nomt=b.data();

    // Càrrega del fitxer de textura nomt com a nom de textura 0
    //	escena.loadBMP(nomt,0);
        loadIMA(nomt,0);

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();

    }


    // TEXTURA -> Mapping Textura Calcat (textura sobreposada a la llum)
    void GLWidget::OnTdecal()
    {
    // TODO: Add your command handler code here
        //if (textura) textura_map=false;

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }



    // TEXTURA -> Mapping Textura Modulat (textura barrejada amb la llum)
    void GLWidget::OnTmodulate()
    {
    // TODO: Add your command handler code here
        //if (textura) textura_map=true;

    // Crida a PaintGL() per redibuixar l'escena
        updateGL();
    }

    /* ------------------------------------------------------------------------- */
    /*					9. TIMER (ANIMACIÓ)										 */
    /* ------------------------------------------------------------------------- */

    // Rutina d'interrupció pel crono
    void GLWidget::OnTimer()
    {
    // TODO: Add your message handler code here and/or call default
        if (anima)	{   OPV.beta=OPV.beta+3;
                        if(OPV.beta>=360)	OPV.beta=OPV.beta-360;
    // Codi de tractament de l'animació quan transcorren els ms. del crono.

    // Crida a PaintGL() per redibuixar l'escena
                        updateGL();
                  }
    }


    /* ------------------------------------------------------------------------- */
    /*					SETTERS & GETTERS										 */
    /* ------------------------------------------------------------------------- */

    int GLWidget::GetModeVisualitzacio(void){

        return this->modeVisualitzacio;
    }

    void GLWidget::SetModeVisualitzacio(int mode){

        this->modeVisualitzacio=mode;
    }

    void GLWidget::SetMobil(bool mode){

        this->mobil=mode;
    }

    void GLWidget::SetZoom(bool mode){

        this->zzoom=mode;
    }

    void GLWidget::SetPan(bool mode){

        this->pan=mode;
    }

    void GLWidget::SetNavega(bool mode){

        this->navega=mode;
    }

    void GLWidget::SetEixos(bool mode){

        this->eixos=mode;
    }

    void GLWidget::SetTrasl(bool mode){

        this->trasl=mode;
    }

    void GLWidget::SetRota(bool mode){

        this->rota=mode;
    }

    void GLWidget::SetEscal(bool mode){

        this->escal=mode;
    }

    void GLWidget::SetTestVis(bool mode){

        this->test_vis=mode;
    }

    void GLWidget::SetOculta(bool mode){

        this->oculta=mode;
    }

    void GLWidget::SetBackLine(bool mode){

        this->back_line=mode;
    }

    void GLWidget::SetIFixe(bool mode){

        this->ifixe=mode;
    }

    void GLWidget::SetTextura(bool mode){

        this->textura=mode;
    }

    void GLWidget::SetTexturaMap(bool mode){

        this->textura_map=mode;
    }

    void GLWidget::SetFullScreen(bool mode){

        this->fullScreen=mode;
    }

    char * GLWidget::GetNom(){

        return nom;
    }

    char GLWidget::GetProjeccio(){

        return projeccio;
    }

    CEsfe3D GLWidget::GetOPV(){

        return OPV;
    }

    CPunt3D GLWidget::GetOPVN(){

        return opvN;
    }

    char GLWidget::GetVisPolar(){

        return Vis_Polar;
    }

    CPunt3D GLWidget::GetTR_CPV(){

        return tr_cpv;
    }

    bool GLWidget::GetSwColor(){

        return sw_color;
    }

    bool GLWidget::GetTransf(){

        return transf;
    }

    bool GLWidget::GetFonsR(){

        return fonsR;
    }

    bool GLWidget::GetFonsG(){

        return fonsG;
    }

    bool GLWidget::GetFonsB(){

        return fonsB;
    }

    bool GLWidget::GetTrasl(){

        return trasl;
    }

    bool GLWidget::GetRota(){

        return rota;
    }

    bool GLWidget::GetEscal(){

        return escal;
    }

    INSTANCIA GLWidget::GetTG(){

        return TG;
    }

    bool GLWidget::GetSWColor(){

        return sw_color;
    }

    CColor GLWidget::GetCFons(){

        return c_fons;
    }

    CColor GLWidget::GetColObj(){

        return col_obj;
    }

    // ---------------- Entorn PFC: Funcions locals a GLWidget.cpp

    // Log2: Càlcul del log base 2 de num
    int GLWidget::Log2(int num)
    {
        int tlog;

        if (num>=8192) tlog=13;
        else if (num>=4096) tlog=12;
        else if (num>=2048) tlog=11;
        else if (num>=1024) tlog=10;
        else if (num>=512) tlog=9;
        else if (num>=256) tlog=8;
        else if (num>=128) tlog=7;
        else if (num>=64) tlog=6;
        else if (num>=32) tlog=5;
        else if (num>=16) tlog=4;
        else if (num>=8) tlog=3;
        else if (num>=4) tlog=2;
        else if (num>=2) tlog=1;
        else tlog=0;

        return tlog;
    }
