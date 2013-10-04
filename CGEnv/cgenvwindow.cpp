//******** PRACTICA GRAFICS PER COMPUTADOR 2
//******** Entorn basic amb interficie Qt i OpenGL (PFC)
//******** Ferran Poveda, Enric Marti (Juny 2012)
// cgenvwindow.cpp: Gestor de menús de la finestra principal (mainwindow)

#include <QtGui/QMouseEvent>
#include <QLabel>
#include "cgenvwindow.h"
#include "ui_cgenvwindow.h"

CGEnvWindow::CGEnvWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CGEnvWindow)
{
    ui->setupUi(this);

// Entorn PFC: Variables de control per Menu Bar: ToolBar, StatusBar, FullScreen
   TB=true;			ui->actionToolBar->setChecked(true);
   STB=true;			ui->actionStatusBar->setChecked(true);
   fullScreen=false;	ui->actionFull_Screen->setChecked(false);

// Entorn PFC: Variables de control per Menu Vista: canvi PV interactiu, Zoom i dibuixar eixos
//             i Definicio dels "Checked" per a les opcions del Menu de Persianes
    mobil=true;		ui->actionMobil->setChecked(true);
    zzoom=true;		ui->actionZoom->setChecked(true);
    pan=false;
    navega=false;
    eixos=true;		ui->actionEixos->setChecked(true);
    ui->actionPolars_Eix_Z->setChecked(true);
    ui->actionPolars_Eix_Y->setChecked(false);
    ui->actionPolars_Eix_X->setChecked(false);

// Entorn PFC: Variables de control del men Transforma
    trasl=false;	ui->actionTraslation->setChecked(false);
    rota=false;		ui->actionRotation->setChecked(false);
    escal=false;	ui->actionScale->setChecked(false);
    transX=false;	ui->actionMobil_Eix_X->setChecked(false);
    transY=false;	ui->actionMobil_Eix_Y->setChecked(false);
    transZ=false;	ui->actionMobil_Eix_Z->setChecked(false);

// Entorn PFC: Variables de control per les opcions de men Ocultacions
    oculta=false;		ui->actionZ_Buffer->setChecked(false);
    test_vis=false;		ui->actionBack_Face_Culling->setChecked(false);
    back_line=false;	ui->actionBack_Line->setChecked(false);

// Entorn PFC: Variables de control del men Iluminaci
    ifixe=false;		ui->actionLight_Fixed->setChecked(false);
    ui->actionWireframe->setChecked(true); ui->actionFlat->setChecked(false); ui->actionGouraud->setChecked(false);
    textura=false;		ui->actionTexture->setChecked(false);
    textura_map=true;	ui->actionModulate->setChecked(true);	ui->actionDecal->setChecked(false);

// Activacio Event teclat (keyPressed)
//	ui->contextGL->setFocusPolicy(Qt::StrongFocus);
    this->setFocusPolicy(Qt::WheelFocus);

// Activacio event Timer
//  QTimer *timer = new QTimer(this);
//  timer->stop();

}

CGEnvWindow::~CGEnvWindow()
{
    delete ui;
}

// Barra_Estat: Actualitza la barra d'estat (Status Bar) de l'aplicaci amb els
//      valors R,A,B,PVx,PVy,PVz en Visualitzaci Interactiva.
void CGEnvWindow::Barra_Estat()
{
    bool sw_color,transf,fonsR,fonsG,fonsB,trasl,rota,escal;
    QString buffer;
    //char * nom;
    //int  tam,i,decimal,sign;
    //char car;
    char projeccio,Vis_Polar;
    CEsfe3D OPV,OPVAux;
    CPunt3D opvN,tr_cpv;
    float PVx,PVy,PVz;
    //GLfloat color;
    CColor c_fons,col_obj;
    INSTANCIA TG;
    //QLabel *FileLabel;
    QLabel *RTLabel,*RLabel,*alphaTLabel,*alphaLabel,*betaTLabel,*betaLabel;
    QLabel *PVxTLabel,*PVxLabel,*PVyTLabel,*PVyLabel,*PVzTLabel,*PVzLabel,*FonsLabel,*FonsTCLabel;
    QLabel *TGxLabel, *TGyLabel, *TGzLabel;

    trasl=false;	rota=false;		escal=false;

// Inicialitza StatusBar
    this->setStatusBar(0);

// Obt Status Bar de Finestra Principal
    QStatusBar *statusBar = this->statusBar();

    QString message = tr("Aplicacio grafica OpenGL amb interficie Qt");
    statusBar->showMessage(message);

    projeccio=ui->contextGL->GetProjeccio();
    OPV=ui->contextGL->GetOPV();

// Clcul dels valors per l'opci Vista->Navega
    if (projeccio!=CAP && projeccio!=ORTO) {
        OPV=ui->contextGL->GetOPV();
        if (navega)
            {	opvN=ui->contextGL->GetOPVN();
                OPVAux.R=sqrt(opvN.x*opvN.x+opvN.y*opvN.y+opvN.z*opvN.z);
                OPVAux.alfa=(asin(opvN.z/OPVAux.R)*180)/pi;
                OPVAux.beta=(atan(opvN.y/opvN.x))*180/pi;
            }
            else {	OPVAux.R=OPV.R; OPVAux.alfa=OPV.alfa; OPVAux.beta=OPV.beta; //Raux=R; angv=anglev; angh=angleh;
                }
        }
    else {	OPVAux.R=OPV.R; OPVAux.alfa=OPV.alfa; OPVAux.beta=OPV.beta; //Raux=R; angv=anglev; angh=angleh;
        }

// Status Bar "R=<num>"
    RTLabel = new QLabel(" R=     ");
    RTLabel->setText(tr( " R= "));  //StatusBar->SetPaneText(0, nom);
    RTLabel->setAlignment(Qt::AlignHCenter);
    RTLabel->setMinimumSize(RTLabel->sizeHint());
    statusBar->insertPermanentWidget(0,RTLabel);

    RLabel = new QLabel(" R=     ");
    if (projeccio==CAP) RLabel->setText(tr( "       "));
        else if (projeccio==ORTO) RLabel->setText(tr( " ORTO  "));
            else if (navega) RLabel->setText(tr( " NAV  "));
                else RLabel->setNum(OPVAux.R);
    RLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    RLabel->setMinimumSize(RLabel->sizeHint());
    statusBar->insertPermanentWidget(1,RLabel);

// Status Bar "a=<num>"
    alphaTLabel = new QLabel(" a=     ");
    alphaTLabel->setText(tr( " a= "));
    alphaTLabel->setAlignment(Qt::AlignHCenter);
    alphaTLabel->setMinimumSize(alphaTLabel->sizeHint());
    statusBar->insertPermanentWidget(2,alphaTLabel);

    alphaLabel = new QLabel(" a=     ");
    if (projeccio==CAP) alphaLabel->setText(tr( "       "));
        else if (projeccio==ORTO) alphaLabel->setText(tr( " ORTO  "));
            else if (navega) alphaLabel->setText(tr( " NAV  "));
                else alphaLabel->setNum(OPVAux.alfa);
    alphaLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    alphaLabel->setMinimumSize(alphaLabel->sizeHint());
    statusBar->insertPermanentWidget(3,alphaLabel);

// Status Bar "=<num>"
    betaTLabel = new QLabel(" =     ");
    betaTLabel->setText(tr( " = "));
    betaTLabel->setAlignment(Qt::AlignHCenter);
    betaTLabel->setMinimumSize(betaTLabel->sizeHint());
    statusBar->insertPermanentWidget(4,betaTLabel);

    betaLabel = new QLabel(" =     ");
    if (projeccio==CAP) betaLabel->setText(tr( "       "));
        else if (projeccio==ORTO) betaLabel->setText(tr( " ORTO  "));
            else if (navega) betaLabel->setText(tr( " NAV  "));
                else betaLabel->setNum(OPVAux.beta);
    betaLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    betaLabel->setMinimumSize(betaLabel->sizeHint());
    statusBar->insertPermanentWidget(5,betaLabel);


    Vis_Polar=ui->contextGL->GetVisPolar();
    if (pan) tr_cpv=ui->contextGL->GetTR_CPV();
    sw_color=ui->contextGL->GetSwColor();

// Transformaci PV de Coord. esfriques (R,anglev,angleh) --> Coord. cartesianes (PVx,PVy,PVz)
    if (navega) {PVx=opvN.x; PVy=opvN.y; PVz=opvN.z;}
/*	else {	PVx=OPVAux.R*cos(OPVAux.beta*pi/180)*cos(OPVAux.alfa*pi/180);
            PVy=OPVAux.R*sin(OPVAux.beta*pi/180)*cos(OPVAux.alfa*pi/180);
            PVz=OPVAux.R*sin(OPVAux.alfa*pi/180);
            } */

    else {if (Vis_Polar==POLARZ) { PVx=OPVAux.R*cos(OPVAux.beta*pi/180)*cos(OPVAux.alfa*pi/180);
                                   PVy=OPVAux.R*sin(OPVAux.beta*pi/180)*cos(OPVAux.alfa*pi/180);
                                   PVz=OPVAux.R*sin(OPVAux.alfa*pi/180);	}
        else if (Vis_Polar==POLARY) {	PVx=OPVAux.R*sin(OPVAux.beta*pi/180)*cos(OPVAux.alfa*pi/180);
                                        PVy=OPVAux.R*sin(OPVAux.alfa*pi/180);
                                        PVz=OPVAux.R*cos(OPVAux.beta*pi/180)*cos(OPVAux.alfa*pi/180);}
            else {	PVx=OPVAux.R*sin(OPVAux.alfa*pi/180);
                    PVy=OPVAux.R*cos(OPVAux.beta*pi/180)*cos(OPVAux.alfa*pi/180);
                    PVz=OPVAux.R*sin(OPVAux.beta*pi/180)*cos(OPVAux.alfa*pi/180);	}
        }


// Refrescar posici PVx Status Bar
    PVxTLabel = new QLabel(" PVx=     ");
    PVxTLabel->setText(tr( " PVx= "));
    PVxTLabel->setAlignment(Qt::AlignHCenter);
    PVxTLabel->setMinimumSize(PVxTLabel->sizeHint());
    statusBar->insertPermanentWidget(6,PVxTLabel);

    PVxLabel = new QLabel(" PVx=     ");
    if (projeccio==CAP) PVxLabel->setText(tr( "       "));
        else if (projeccio==ORTO) PVxLabel->setText(tr( " ORTO  "));
            else if (navega) PVxLabel->setText(tr( " NAV  "));
                else if (pan) PVxLabel->setNum(tr_cpv.x);
                    else PVxLabel->setNum(PVx);
    PVxLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    PVxLabel->setMinimumSize(PVxLabel->sizeHint());
    statusBar->insertPermanentWidget(7,PVxLabel);

// Refrescar posici PVy Status Bar
    PVyTLabel = new QLabel(" PVy=     ");
    PVyTLabel->setText(tr( " PVy= "));
    PVyTLabel->setAlignment(Qt::AlignHCenter);
    PVyTLabel->setMinimumSize(PVyTLabel->sizeHint());
    statusBar->insertPermanentWidget(8,PVyTLabel);

    PVyLabel = new QLabel(" PVy=     ");
    if (projeccio==CAP) PVyLabel->setText(tr( "       "));
        else if (projeccio==ORTO) PVyLabel->setText(tr( " ORTO  "));
            else if (navega) PVyLabel->setText(tr( " NAV  "));
                else if (pan) PVyLabel->setNum(tr_cpv.y);
                    else PVyLabel->setNum(PVy);
    PVyLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    PVyLabel->setMinimumSize(PVyLabel->sizeHint());
    statusBar->insertPermanentWidget(9,PVyLabel);

// Refrescar posici PVz Status Bar
    PVzTLabel = new QLabel(" PVz=     ");
    PVzTLabel->setText(tr( " PVz= "));
    PVzTLabel->setAlignment(Qt::AlignHCenter);
    PVzTLabel->setMinimumSize(PVzTLabel->sizeHint());
    statusBar->insertPermanentWidget(10,PVzTLabel);

    PVzLabel = new QLabel(" PVy=     ");
    if (projeccio==CAP) PVyLabel->setText(tr( "       "));
        else if (projeccio==ORTO) PVzLabel->setText(tr( " ORTO  "));
            else if (navega) PVzLabel->setText(tr( " NAV  "));
                else if (pan) PVzLabel->setNum(tr_cpv.z);
                    else PVzLabel->setNum(PVz);
    PVzLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    PVzLabel->setMinimumSize(PVzLabel->sizeHint());
    statusBar->insertPermanentWidget(11,PVzLabel);

// Status Bar per indicar el modus de canvi de color (FONS o OBJECTE)
    FonsLabel = new QLabel(" FONS ");
    if (pan) FonsLabel->setText(tr( " PAN "));
        else if (sw_color) FonsLabel->setText(tr( " OBJ "));
            else FonsLabel->setText(tr( " FONS "));
    FonsLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    FonsLabel->setMinimumSize(FonsLabel->sizeHint());
    statusBar->insertPermanentWidget(12,FonsLabel);

// Status Bar per indicar tipus de Transformaci (TRAS, ROT, ESC)
    transf=ui->contextGL->GetTransf();
    FonsTCLabel = new QLabel(" FONS ");
    if (transf) {
        rota=ui->contextGL->GetRota();
        trasl=ui->contextGL->GetTrasl();
        escal=ui->contextGL->GetEscal();
        if (rota) FonsTCLabel->setText(tr( " ROT "));
            else if (trasl) FonsTCLabel->setText(tr( " TRA "));
                    else if (escal) FonsTCLabel->setText(tr( " ESC "));
        }
    else {
            fonsR=ui->contextGL->GetFonsR();
            fonsG=ui->contextGL->GetFonsG();
            fonsB=ui->contextGL->GetFonsB();
// Components d'intensitat de fons que varien per teclat
           if ((fonsR) && (fonsG) && (fonsB)) FonsTCLabel->setText(tr( " RGB "));
            else if ((fonsR) && (fonsG)) FonsTCLabel->setText(tr( " RG  "));
                    else if ((fonsR) && (fonsB)) FonsTCLabel->setText(tr( " R B "));
                        else if ((fonsG) && (fonsB)) FonsTCLabel->setText(tr( "  GB "));
                            else if (fonsR) FonsTCLabel->setText(tr( " R   "));
                                else if (fonsG) FonsTCLabel->setText(tr( "  G  "));
                                    else if (fonsB) FonsTCLabel->setText(tr( "   B "));
        }
    FonsTCLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    FonsTCLabel->setMinimumSize(FonsTCLabel->sizeHint());
    statusBar->insertPermanentWidget(13,FonsTCLabel);

// Status Bar dels parmetres de Transformaci, Color i posicions de Robot i Cama
    trasl=ui->contextGL->GetTrasl();
    rota=ui->contextGL->GetRota();
    escal=ui->contextGL->GetEscal();
    TG=ui->contextGL->GetTG();			// Obtenir TG
    sw_color=ui->contextGL->GetSWColor();	// Obtenir sw_color

    if (transf)
         { if (rota)
                {
                    TGxLabel = new QLabel("      ");
                    TGxLabel->setNum(TG.VRota.x);
                    TGxLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
                    TGxLabel->setMinimumSize(TGxLabel->sizeHint());
                    statusBar->insertPermanentWidget(14,TGxLabel);

                    TGyLabel = new QLabel("      ");
                    TGyLabel->setNum(TG.VRota.y);
                    TGyLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
                    TGyLabel->setMinimumSize(TGyLabel->sizeHint());
                    statusBar->insertPermanentWidget(15,TGyLabel);

                    TGzLabel = new QLabel("      ");
                    TGzLabel->setNum(TG.VRota.z);
                    TGzLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
                    TGzLabel->setMinimumSize(TGzLabel->sizeHint());
                    statusBar->insertPermanentWidget(16,TGzLabel);
                }

            else if (trasl)
                {
                    TGxLabel = new QLabel("      ");
                    TGxLabel->setNum(TG.VTras.x);
                    TGxLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
                    TGxLabel->setMinimumSize(TGxLabel->sizeHint());
                    statusBar->insertPermanentWidget(14,TGxLabel);

                    TGyLabel = new QLabel("      ");
                    TGyLabel->setNum(TG.VTras.y);
                    TGyLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
                    TGyLabel->setMinimumSize(TGyLabel->sizeHint());
                    statusBar->insertPermanentWidget(15,TGyLabel);

                    TGzLabel = new QLabel("      ");
                    TGzLabel->setNum(TG.VTras.z);
                    TGzLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
                    TGzLabel->setMinimumSize(TGzLabel->sizeHint());
                    statusBar->insertPermanentWidget(16,TGzLabel);

                }
                else if (escal)
                    {

                    TGxLabel = new QLabel("      ");
                    TGxLabel->setNum(TG.VScal.x);
                    TGxLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
                    TGxLabel->setMinimumSize(TGxLabel->sizeHint());
                    statusBar->insertPermanentWidget(14,TGxLabel);

                    TGyLabel = new QLabel("      ");
                    TGyLabel->setNum(TG.VScal.y);
                    TGyLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
                    TGyLabel->setMinimumSize(TGyLabel->sizeHint());
                    statusBar->insertPermanentWidget(15,TGyLabel);

                    TGzLabel = new QLabel("      ");
                    TGzLabel->setNum(TG.VScal.z);
                    TGzLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
                    TGzLabel->setMinimumSize(TGzLabel->sizeHint());
                    statusBar->insertPermanentWidget(16,TGzLabel);

                    }
        }
            else { if (!sw_color)
                    {
                    // Obtenir c_fons
                    c_fons=ui->contextGL->GetCFons();

                    TGxLabel = new QLabel("      ");
                    TGxLabel->setNum(c_fons.r);
                    TGxLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
                    TGxLabel->setMinimumSize(TGxLabel->sizeHint());
                    statusBar->insertPermanentWidget(14,TGxLabel);

                    TGyLabel = new QLabel("      ");
                    TGyLabel->setNum(c_fons.g);
                    TGyLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
                    TGyLabel->setMinimumSize(TGyLabel->sizeHint());
                    statusBar->insertPermanentWidget(15,TGyLabel);

                    TGzLabel = new QLabel("      ");
                    TGzLabel->setNum(c_fons.b);
                    TGzLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
                    TGzLabel->setMinimumSize(TGzLabel->sizeHint());
                    statusBar->insertPermanentWidget(16,TGzLabel);
                    }
                    else
                    {

                    // Obtenir col_obj
                    col_obj=ui->contextGL->GetColObj();

                    TGxLabel = new QLabel("      ");
                    TGxLabel->setNum(col_obj.r);
                    TGxLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
                    TGxLabel->setMinimumSize(TGxLabel->sizeHint());
                    statusBar->insertPermanentWidget(14,TGxLabel);

                    TGyLabel = new QLabel("      ");
                    TGyLabel->setNum(col_obj.g);
                    TGyLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
                    TGyLabel->setMinimumSize(TGyLabel->sizeHint());
                    statusBar->insertPermanentWidget(15,TGyLabel);

                    TGzLabel = new QLabel("      ");
                    TGzLabel->setNum(col_obj.b);
                    TGzLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
                    TGzLabel->setMinimumSize(TGzLabel->sizeHint());
                    statusBar->insertPermanentWidget(16,TGzLabel);
                    }
                }

// Refrescar posici PVz Status Bar
//	StatusBar->SetPaneText(9, sss);

// Status Bar per indicar el pas del Fractal
//	sss=" ";

// Refrescar posici Transformacions en Status Bar
//	StatusBar->SetPaneText(10, sss);

/*
    RTLabel=NULL;
    RLabel=NULL;
    alphaTLabel=NULL;
    alphaLabel=NULL;
    betaTLabel=NULL;
    betaLabel=NULL; */
//	delete RTLabel;
//	delete RLabel;
//	delete alphaTLabel;
//	delete alphaLabel;
//	delete betaTLabel;
//	delete betaLabel;
}

/* ------------------------------------------------------------------------- */
/*                           CONTROL DEL RATOLI                              */
/* ------------------------------------------------------------------------- */

// mousePressEvent: Funci que es crida quan es mou el mouse. La utilitzem per la
//				  Visualitzaci Interactiva amb les tecles del mouse apretades per
//				  modificar els parmetres de P.V. (R,angleh,anglev) segons els
//				  moviments del mouse.
//      PARAMETRES: - event: Estructura (x,y) que dna la posici del mouse
//							 (coord. pantalla) quan el bot s'ha apretat.


void CGEnvWindow::mousePressEvent(QMouseEvent *event) {

// Crida a la funcio 'mousePressEvent' dins contextGL per a gestionar l'event mouse
    ui->contextGL->mousePressEvent(event);

// Refrescar Status Bar
    Barra_Estat();
}


void CGEnvWindow::mouseReleaseEvent( QMouseEvent * event ) {

// Crida a la funcio 'mouseReleaseEvent' dins contextGL per a gestionar l'event mouse
    ui->contextGL->mouseReleaseEvent(event);

// Refrescar Status Bar
    Barra_Estat();
}


void CGEnvWindow::mouseMoveEvent(QMouseEvent *event) {

//	event->accept();
    // Crida a la funcio 'mouseMoveEvent' dins contextGL per a gestionar l'event mouse
    ui->contextGL->mouseMoveEvent(event);

// Refrescar Status Bar
    Barra_Estat();
}

void CGEnvWindow::wheelEvent(QWheelEvent * event ){

//	event->accept();
// Crida a la funcio 'wheelEvent' dins contextGL per a gestionar l'event mouse
    ui->contextGL->wheelEvent(event);

// Refrescar Status Bar
    Barra_Estat();
}


/* ------------------------------------------------------------------------- */
/*                           CONTROL DEL TECLAT                              */
/* ------------------------------------------------------------------------- */

// keyPressEvent: Funci de tractament de teclat (funci que es crida quan es prem una tecla)
//   PARMETRES:
//    - event: Estructura que cont informaci de l'event
void CGEnvWindow::keyPressEvent(QKeyEvent* event) {

// Crida a la funcio 'keyPressEvent' dins contextGL per a gestionar l'event mouse
    ui->contextGL->keyPressEvent(event);

// Refrescar Status Bar
    Barra_Estat();
}

/* ------------------------------------------------------------------------- */
/*   RECURSOS DE MENU (persianes) DE L'APLICACIO:                            */
/*					1. ARXIUS												 */
/*					2. VEURE: ToolBar, StatusBar, Pantalla Completa			 */
/*					3. VISTA: Punt de Vista, Zoom, Pan Navega i Eixos	     */
/*					4. PROJECCIO                                             */
/*					5. OBJECTE					                             */
/*					6. TRANSFORMA											 */
/*					7. OCULTACIONS											 */
/*					8. IL.LUMINACIO											 */
/*					9. TIMER (ANIMACIO)									     */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/*					1. ARXIUS 												 */
/* ------------------------------------------------------------------------- */

// FILE->New
void CGEnvWindow::OnFNew()
{
// TODO: Add your command handler code here
//	objecte=CAP;
//	projeccio=PERSPECT;

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnFNew();

// Refrescar Status Bar
    Barra_Estat();
}

// Obrir fitxer
void CGEnvWindow::OnFileOpen()
{

/*  Obrir dileg de lectura de fitxer (fitxers (*.MNT)
    CFileDialog openMunt (TRUE, NULL, NULL,
        OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ,
        _T("MNT Files(*.mnt)|*.mnt|Error Files (*.err)|*err|All Files (*.*)|*.*||"));;

    if (openMunt.DoModal() != IDOK)
        return;                 // stay with old data file
    else
       {nom=openMunt.GetPathName();}

// Variable de tipus CString 'nom' cont el nom del fitxer seleccionat

*/

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnFileOpen();
}

// Obrir fitxer en format grfic 3DS
void CGEnvWindow::OnFileOpen3ds()
{
/// TODO: Add your command handler code here

// Desactivar opcions persiana Objecte
    ui->actionCube->setChecked(false);
    ui->actionSphere->setChecked(false);
    ui->actionTeapot->setChecked(false);
    ui->actionTruck->setChecked(false);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnFileOpen3ds();

// Refrescar Status Bar
    Barra_Estat();
}

// OnFileOpenObj: Obrir fitxer en format grfic OBJ
void CGEnvWindow::OnFileOpenObj()
{
// TODO: Add your command handler code here

// Desactivar opcions persiana Objecte
   ui->actionCube->setChecked(false);
   ui->actionSphere->setChecked(false);
   ui->actionTeapot->setChecked(false);
   ui->actionTruck->setChecked(false);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnFileOpenObj();

// Refrescar Status Bar
    Barra_Estat();
}

// FILE->New
void CGEnvWindow::OnExit()
{

// Destructor de la classe
    this->~CGEnvWindow();

// Finalitzar programa
    exit(0);
}

/* ------------------------------------------------------------------------- */
/*					2. VEURE: ToolBar, StatusBar, Pantalla Completa         */
/* ------------------------------------------------------------------------- */

// TOOLBAR: Mostrar/Amagar ToolBar
void CGEnvWindow::OnToolBar()
{
// TODO: Add your command handler code here

//	ui->removeToolBar(mainToolBar);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
//	ui->contextGL->OnFullScreen();
}

// STATUSBAR: Mostrar/Amagar StatusBar
void CGEnvWindow::OnStatusBar()
{
// TODO: Add your command handler code here
    STB=!STB;

    if (!STB)	{
                // Obt Status Bar de Finestra Principal
                //QStatusBar *statusBar = this->statusBar();
                // Inicialitza StatusBar
                setStatusBar(0);
                }
    else 	Barra_Estat();	// Refrescar Status Bar
}


// PANTALLA COMPLETA: opci booleana
void CGEnvWindow::OnFullScreen()
{
// TODO: Add your command handler code here
    fullScreen=!fullScreen;

// Passar el valor del flag mobil a contextGL
    ui->contextGL->SetFullScreen(fullScreen);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnFullScreen();

// Refrescar Status Bar
    Barra_Estat();
}


/* ------------------------------------------------------------------------- */
/*					3. VISTA: Punt de Vista, Zoom, Pan i Eixos				 */
/* ------------------------------------------------------------------------- */

// VISTA: Mobil. Punt de Vista Interactiu (opci booleana)
void CGEnvWindow::OnMobil()
{
    mobil=!mobil;

// Activar/desactivar la senyal "Checked" de l'opcio segons el valor del flag
    if (mobil) ui->actionMobil->setChecked(true);
        else ui->actionMobil->setChecked(false);

// Passar el valor del flag mobil a contextGL
    ui->contextGL->SetMobil(mobil);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnMobil();

// Refrescar Status Bar
    Barra_Estat();
}

// VISTA: Zoom. Zoom Interactiu (opci booleana)
void CGEnvWindow::OnZoom()
{
    zzoom=!zzoom;

// Activar/desactivar la senyal "Checked" de l'opcio segons el valor del flag
    if (zzoom) ui->actionZoom->setChecked(true);
        else ui->actionZoom->setChecked(false);

// Passar el valor del flag zzoom a contextGL
    ui->contextGL->SetZoom(zzoom);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnZoom();

// Refrescar Status Bar
    Barra_Estat();
}


// VISTA: Polars Eix Z cap amunt per a Visialitzaci Interactiva
void CGEnvWindow::OnPolarZ()
{
// TODO: Add your command handler code here
//	if ((projeccio!=CAP) && (!navega)) Vis_Polar=POLARZ;

// Activar/desactivar les senyals "Checked" adients a l'opci de men
//	ui->actionPolars_Eix_Z->setChecked(true);
//	ui->actionPolars_Eix_Y->setChecked(false);
//	ui->actionPolars_Eix_X->setChecked(false);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    if (ui->contextGL->OnPolarZ()) {
//									Activar/desactivar les senyals "Checked" adients a l'opci de men
                                    ui->actionPolars_Eix_Z->setChecked(true);
                                    ui->actionPolars_Eix_Y->setChecked(false);
                                    ui->actionPolars_Eix_X->setChecked(false);
                                    }
    else ui->actionPolars_Eix_Z->setChecked(false);

// Refrescar Status Bar
    Barra_Estat();


}

// VISTA: Polars Eix Y cap amunt per a Visialitzaci Interactiva
void CGEnvWindow::OnPolarY()
{
// TODO: Add your command handler code here
//	if ((projeccio!=CAP) && (!navega)) Vis_Polar=POLARY;

// Activar/desactivar les senyals "Checked" adients a l'opci de men
//	ui->actionPolars_Eix_Z->setChecked(false);
//	ui->actionPolars_Eix_Y->setChecked(true);
//	ui->actionPolars_Eix_X->setChecked(false);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    if (ui->contextGL->OnPolarY()) {
//									Activar/desactivar les senyals "Checked" adients a l'opci de men
                                    ui->actionPolars_Eix_Z->setChecked(false);
                                    ui->actionPolars_Eix_Y->setChecked(true);
                                    ui->actionPolars_Eix_X->setChecked(false);
                                }
    else ui->actionPolars_Eix_Y->setChecked(false);

// Refrescar Status Bar
    Barra_Estat();
}


// VISTA: Polars Eix X cap amunt per a Visialitzaci Interactiva
void CGEnvWindow::OnPolarX()
{
// TODO: Add your command handler code here
//	if ((projeccio!=CAP) && (!navega)) Vis_Polar=POLARX;

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    if (ui->contextGL->OnPolarX()){
                                    // Activar/desactivar les senyals "Checked" adients a l'opci de men
                                    ui->actionPolars_Eix_Z->setChecked(false);
                                    ui->actionPolars_Eix_Y->setChecked(false);
                                    ui->actionPolars_Eix_X->setChecked(true);
                                }
    else ui->actionPolars_Eix_X->setChecked(false);

// Refrescar Status Bar
    Barra_Estat();
}


// VISTA: PAN. Desplaament del Centre de l'esfera de visualitzaci (opci booleana)
void CGEnvWindow::OnPan()
{

    pan=!pan;
// Activar/desactivar la senyal "Checked" de l'opcio segons el valor del flag
    if (pan) {	ui->actionMobil->setChecked(true);	ui->actionZoom->setChecked(true),

                ui->actionNavega->setChecked(false);
                ui->actionPan->setChecked(true);
            }
        else ui->actionPan->setChecked(false);

// Passar el valor del flag mobil a contextGL
    ui->contextGL->SetPan(pan);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnPan();

// Refrescar Status Bar
    Barra_Estat();
}


// VISTA: INIPAN. Origen (inicialitzar) Pan
void CGEnvWindow::OnInipan()
{
/* TODO: Add your command handler code here
    if (pan) {	fact_pan=1;
                tr_cpv[0]=0;	tr_cpv[1]=0;	tr_cpv[2]=0;
                }
*/
// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnInipan();

// Refrescar Status Bar
    Barra_Estat();
}


// VISTA: NAVEGA. Opci Navega (opci booleana)
void CGEnvWindow::OnNavega()
{

    navega=!navega;
    if (navega) {	ui->actionMobil->setChecked(false);	ui->actionZoom->setChecked(false);
                    ui->actionPan->setChecked(false);
                    ui->actionNavega->setChecked(true);
                }
        else ui->actionNavega->setChecked(false);

// Passar el valor del flag mobil a contextGL
    ui->contextGL->SetNavega(navega);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnNavega();

// Refrescar Status Bar
    Barra_Estat();
}


// VISTA: Origen Navegaci
void CGEnvWindow::OnIninav()
{

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnIninav();

// Refrescar Status Bar
    Barra_Estat();
}


// VISTA: Dibuixar els eixos (opci booleana)
void CGEnvWindow::OnEixos()
{
    eixos=!eixos;

// Activar/desactivar la senyal "Checked" de l'opcio segons el valor del flag
    if (eixos) ui->actionEixos->setChecked(true);
        else ui->actionEixos->setChecked(false);

// Passar el valor del flag eixos a contextGL
    ui->contextGL->SetEixos(eixos);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnEixos();

// Refrescar Status Bar
    Barra_Estat();
}


/* ------------------------------------------------------------------------- */
/*					4. PROJECCIO                                             */
/* ------------------------------------------------------------------------- */

// PROJECCIO: Perspectiva
void CGEnvWindow::OnPerspectiva()
{
// TODO: Add your command handler code here
//	projeccio=PERSPECT;

// Definici pestanyes "Checked" de les opcions de la persiana View
    mobil=true;		ui->actionMobil->setChecked(true);
    zzoom=true;		ui->actionZoom->setChecked(true);

// Passar el valor dels flags mobil i zzoom a contextGL
    ui->contextGL->SetMobil(mobil);
    ui->contextGL->SetZoom(zzoom);

// Definici pestanyes "Checked" de les opcions de la persiana Projection
    ui->actionOrthographic->setChecked(false);
    ui->actionPerspective->setChecked(true);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnPerspectiva();

// Refrescar Status Bar
    Barra_Estat();
}


/* ------------------------------------------------------------------------- */
/*					5. OBJECTE					                             */
/* ------------------------------------------------------------------------- */

// OBJECTE Cub
void CGEnvWindow::OnCube()
{
// Definici pestanyes "Checked" de les opcions de la persiana Objecte
    ui->actionCube->setChecked(true);
    ui->actionSphere->setChecked(false);
    ui->actionTeapot->setChecked(false);
    ui->actionTruck->setChecked(false);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnCube();

// Refrescar Status Bar
    Barra_Estat();
}


// OBJECTE Esfera
void CGEnvWindow::OnSphere()
{

// Definici pestanyes "Checked" de les opcions de la persiana Objecte
    ui->actionCube->setChecked(false);
    ui->actionSphere->setChecked(true);
    ui->actionTeapot->setChecked(false);
    ui->actionTruck->setChecked(false);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnSphere();

// Refrescar Status Bar
    Barra_Estat();
}


// OBJECTE Tetera
void CGEnvWindow::OnTeapot()
{

// Definici pestanyes "Checked" de les opcions de la persiana Objecte
    ui->actionCube->setChecked(false);
    ui->actionSphere->setChecked(false);
    ui->actionTeapot->setChecked(true);
    ui->actionTruck->setChecked(false);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnTeapot();

// Refrescar Status Bar
    Barra_Estat();
}


// OBJECTE Truck
void CGEnvWindow::OnTruck()
{

// Definici pestanyes "Checked" de les opcions de la persiana Objecte
    ui->actionCube->setChecked(false);
    ui->actionSphere->setChecked(false);
    ui->actionTeapot->setChecked(false);
    ui->actionTruck->setChecked(true);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnTruck();

// Refrescar Status Bar
    Barra_Estat();
}


/* ------------------------------------------------------------------------- */
/*					6. TRANSFORMA											 */
/* ------------------------------------------------------------------------- */

// TRANSFORMA: TRASLACIO
void CGEnvWindow::OnTraslacio()
{

    trasl=!trasl;
    if (trasl) {	ui->actionTraslation->setChecked(true);
                    rota=false;	ui->actionRotation->setChecked(false);
                    escal=true;	ui->actionScale->setChecked(true);
                }
        else  ui->actionTraslation->setChecked(false);

// Passar el valor dels flags eixos a contextGL
    ui->contextGL->SetTrasl(trasl);
    ui->contextGL->SetRota(rota);
    ui->contextGL->SetEscal(escal);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnTraslacio();

// Refrescar Status Bar
    Barra_Estat();
}


// Reinicialitza a l'origen la Traslaci
void CGEnvWindow::OnInitras()
{
/* TODO: Add your command handler code here
    if (trasl)
        {	fact_Tras=1;
            TG.VTras.x=0.0;	TG.VTras.y=0.0;	TG.VTras.z=0;
        }
*/

// Crida a la funci 'On*' dins contextGL per a gestionar l'opci
    ui->contextGL->OnInitras();

// Refrescar Status Bar
    Barra_Estat();
}

// TRANSFORMA: ROTACIO
void CGEnvWindow::OnRotacio()
{

    rota=!rota;
    if (rota) {	ui->actionRotation->setChecked(true);
                trasl=false;	ui->actionTraslation->setChecked(false);
                escal=true;		ui->actionScale->setChecked(true);
                }
        else  ui->actionRotation->setChecked(false);

// Passar el valor dels flags eixos a contextGL
    ui->contextGL->SetTrasl(trasl);
    ui->contextGL->SetRota(rota);
    ui->contextGL->SetEscal(escal);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnRotacio();

// Refrescar Status Bar
    Barra_Estat();
}


// Reinicialitza a l'origen la Rotaci
void CGEnvWindow::OnInirot()
{

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnInirot();

// Refrescar Status Bar
    Barra_Estat();
}


// TRANSFORMA: ESCALATGE
void CGEnvWindow::OnEscalatge()
{

    if ((!rota) && (!trasl)) escal=!escal;
    if (escal) ui->actionScale->setChecked(true);
        else ui->actionScale->setChecked(false);


// Passar el valor del flag escal a contextGL
    ui->contextGL->SetEscal(escal);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnEscalatge();

// Refrescar Status Bar
    Barra_Estat();
}


// Reinicialitza a l'origen l'Escalatge
void CGEnvWindow::OnIniescal()
{
// TODO: Add your command handler code here

// Crida a la funci 'On*' dins contextGL per a gestionar l'opci
    ui->contextGL->OnIniescal();

// Refrescar Status Bar
    Barra_Estat();
}

// TRANSFORMA: Transformacions Geomtriques per l'eix X movent el mouse (booleana)
void CGEnvWindow::OnMobilX()
{
    if (trasl || rota || escal)
        {	transX=!transX;
            if (transX) {	mobil=false;	ui->actionMobil->setChecked(false);
                            zzoom=false;	ui->actionZoom->setChecked(false);
                            pan=false;		ui->actionPan->setChecked(false);
                            navega=false;	ui->actionNavega->setChecked(false);
                            ui->contextGL->SetPan(pan);
                            ui->contextGL->SetNavega(navega);

                        }
                else if ((!transY) && (!transZ)){	mobil=true;	ui->actionMobil->setChecked(true);
                                                    zzoom=true;	ui->actionZoom->setChecked(true);

                                                }
            ui->contextGL->SetMobil(mobil);
            ui->contextGL->SetZoom(zzoom);
        }

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnMobilX();

// Refrescar Status Bar
    Barra_Estat();
}

// TRANSFORMA: Transformacions Geomtriques per l'eix Y movent el mouse (booleana)
void CGEnvWindow::OnMobilY()
{
// TODO: Add your command handler code here
    if (trasl || rota || escal)
        {	transY=!transY;
            if (transY) {	mobil=false;	ui->actionMobil->setChecked(false);
                            zzoom=false;	ui->actionZoom->setChecked(false);
                            pan=false;		ui->actionPan->setChecked(false);
                            navega=false;	ui->actionNavega->setChecked(false);
                            ui->contextGL->SetPan(pan);
                            ui->contextGL->SetNavega(navega);
                        }
                else if ((!transX) && (!transZ)){	mobil=true;	ui->actionMobil->setChecked(true);
                                                    zzoom=true;	ui->actionZoom->setChecked(true);
                                                }
            ui->contextGL->SetMobil(mobil);
            ui->contextGL->SetZoom(zzoom);
        }

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnMobilY();

// Refrescar Status Bar
    Barra_Estat();
}

// TRANSFORMA: Transformacions Geomtriques per l'eix X movent el mouse (booleana)
void CGEnvWindow::OnMobilZ()
{
// TODO: Add your command handler code here
    if (trasl || rota || escal)
        {	transZ=!transZ;
            if (transZ) {	mobil=false;	ui->actionMobil->setChecked(false);
                            zzoom=false;	ui->actionZoom->setChecked(false);
                            pan=false;		ui->actionPan->setChecked(false);
                            navega=false;	ui->actionNavega->setChecked(false);
                            ui->contextGL->SetPan(pan);
                            ui->contextGL->SetNavega(navega);
                        }
                else if ((!transX) && (!transY)) {	mobil=true;	ui->actionMobil->setChecked(true);
                                                    zzoom=true;	ui->actionZoom->setChecked(true);
                                                }
            ui->contextGL->SetMobil(mobil);
            ui->contextGL->SetZoom(zzoom);
        }

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnMobilZ();

// Refrescar Status Bar
    Barra_Estat();
}


/* ------------------------------------------------------------------------- */
/*					7. OCULTACIONS											 */
/* ------------------------------------------------------------------------- */

// OCULTACIONS: Test de Visibilitat (BACK FACE CULLING) (opci booleana)
void CGEnvWindow::OnTest()
{
// TODO: Add your command handler code here
    test_vis=!test_vis;
    if (test_vis) ui->actionBack_Face_Culling->setChecked(true);
        else ui->actionBack_Face_Culling->setChecked(false);

// Passar el valor del flag test_vis a contextGL
    ui->contextGL->SetTestVis(test_vis);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnTest();

// Refrescar Status Bar
    Barra_Estat();
}

// OCULTACIONS: Algorisme d'ocultacions Z-buffer (opci booleana)
void CGEnvWindow::OnZBuffer()
{
// TODO: Add your command handler code here
    oculta=!oculta;
    if (oculta) ui->actionZ_Buffer->setChecked(true);
        else ui->actionZ_Buffer->setChecked(false);

// Passar el valor del flag oculta a contextGL
    ui->contextGL->SetOculta(oculta);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnZBuffer();

// Refrescar Status Bar
    Barra_Estat();
}


// OCULTACIONS: Dibuix cares darrera com a lnies (opci booleana)
void CGEnvWindow::OnBackLine()
{
// TODO: Add your command handler code here
    back_line=!back_line;
    if (back_line) ui->actionBack_Line->setChecked(true);
        else ui->actionBack_Line->setChecked(false);

// Passar el valor del flag back_line a contextGL
    ui->contextGL->SetBackLine(back_line);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnBackLine();

// Refrescar Status Bar
    Barra_Estat();
}


/* ------------------------------------------------------------------------- */
/*					8. IL.LUMINACIO											 */
/* ------------------------------------------------------------------------- */

// IL.LUMINACIO Font de llum fixe? (opcio booleana).
void CGEnvWindow::OnIFixe()
{
// TODO: Add your command handler code here
    ifixe=!ifixe;

    if (ifixe) ui->actionLight_Fixed->setChecked(true);
        else ui->actionLight_Fixed->setChecked(false);

// Passar el valor del flag ifixe a contextGL
    ui->contextGL->SetIFixe(ifixe);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnIFixe();

// Refrescar Status Bar
    Barra_Estat();
}


// IL.LUMINACIO Filferros
void CGEnvWindow::OnFilferros()
{
// TODO: Add your command handler code here
//	ilumina=FILFERROS;
// Definici pestanyes "Checked" de les opcions de la persiana Ocultacions
    test_vis=false;		ui->actionBack_Face_Culling->setChecked(false);
    oculta=false;		ui->actionZ_Buffer->setChecked(false);

// Passar els valors del flags text_vis i oculta a contextGL
    ui->contextGL->SetTestVis(test_vis);
    ui->contextGL->SetOculta(oculta);

// Definici pestanyes "Checked" de les opcions de la persiana Lighting
    ui->actionWireframe->setChecked(true);
    ui->actionFlat->setChecked(false);
    ui->actionGouraud->setChecked(false);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnFilferros();

// Refrescar Status Bar
    Barra_Estat();
}


// IL.LUMINACIO Plana
void CGEnvWindow::OnFlat()
{
// TODO: Add your command handler code here
//	ilumina=PLANA;
// Definici pestanyes "Checked" de les opcions de la persiana Ocultacions
    test_vis=false;		ui->actionBack_Face_Culling->setChecked(false);
    oculta=true;		ui->actionZ_Buffer->setChecked(true);

// Passar els valors del flags text_vis i oculta a contextGL
    ui->contextGL->SetTestVis(test_vis);
    ui->contextGL->SetOculta(oculta);

// Definici pestanyes "Checked" de les opcions de la persiana Lighting
    ui->actionWireframe->setChecked(false);
    ui->actionFlat->setChecked(true);
    ui->actionGouraud->setChecked(false);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnFlat();

// Refrescar Status Bar
    Barra_Estat();
}


// IL.LUMINACIO Suau
void CGEnvWindow::OnGouraud()
{
// TODO: Add your command handler code here
//	ilumina=SUAU;
// Definici pestanyes "Checked" de les opcions de la persiana Ocultacions
    test_vis=false;		ui->actionBack_Face_Culling->setChecked(false);
    oculta=true;		ui->actionZ_Buffer->setChecked(true);

// Passar els valors del flags text_vis i oculta a contextGL
    ui->contextGL->SetTestVis(test_vis);
    ui->contextGL->SetOculta(oculta);

// Definici pestanyes "Checked" de les opcions de la persiana Lighting
    ui->actionWireframe->setChecked(false);
    ui->actionFlat->setChecked(false);
    ui->actionGouraud->setChecked(true);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnGouraud();

// Refrescar Status Bar
    Barra_Estat();
}

// IL.LUMINACIO->TEXTURA Activaci i desactivaci de textura
void CGEnvWindow::OnTexture()
{
// TODO: Add your command handler code here
    textura=!textura;

// Definici pestanyes "Checked" de les opcions de la persiana textura Map
    if (textura) ui->actionTexture->setChecked(true);
        else ui->actionTexture->setChecked(false);

// Passar el valor del flag back_line a contextGL
    ui->contextGL->SetTextura(textura);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnTexture();

// Refrescar Status Bar
    Barra_Estat();
}


void CGEnvWindow::OnTexturima()
{
// TODO: Add your command handler code here

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnTexturima();

// Refrescar Status Bar
    Barra_Estat();
}


// TEXTURA -> Mapping Textura Calcat (textura sobreposada a la llum)
void CGEnvWindow::OnTdecal()
{
// TODO: Add your command handler code here
    if (textura) textura_map=false;

// Definici pestanyes "Checked" de les opcions de la persiana Textura Map
    ui->actionDecal->setChecked(true);
    ui->actionModulate->setChecked(false);

// Passar el valor del flag textura_map a contextGL
    ui->contextGL->SetTexturaMap(textura_map);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnTdecal();

// Refrescar Status Bar
    Barra_Estat();
}



// TEXTURA -> Mapping Textura Modulat (textura barrejada amb la llum)
void CGEnvWindow::OnTmodulate()
{
// TODO: Add your command handler code here
    if (textura) textura_map=true;

// Definici pestanyes "Checked" de les opcions de la persiana textura Map
    ui->actionDecal->setChecked(false);
    ui->actionModulate->setChecked(true);

// Passar el valor del flag textura_map a contextGL
    ui->contextGL->SetTexturaMap(textura_map);

// Crida a la funcio 'On*' dins contextGL per a gestionar l'opcio
    ui->contextGL->OnTmodulate();

// Refrescar Status Bar
    Barra_Estat();
}

