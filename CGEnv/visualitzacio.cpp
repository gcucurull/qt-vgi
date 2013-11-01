//******** PRACTICA GRAFICS PER COMPUTADOR 2
//******** Entorn bàsic amb interfície Qt i OpenGL (PFC)
//******** Carme Julià, Débora Gil, Ferran Poveda, Enric Martí (Juny 2012)
// visualitzacio.cpp : Funcions de visualització i pantalla
// FUNCIONS:	- Iluminació (Ilumina)
//				- Projecció Ortogràfica (ProjeccioOrto i Ortografica)
//				- Projecció Perspectiva (ProjeccioPerspectiva,Perspectiva)
//				- Dibuixar els eixos (deixos)
//				- Dibuixar fons: blanc (FonsW) i negre (Fons)
//				- Carregar imatges textura (loadBMP)
//				- Carregar imatges de formats compatibles amb DevIL/OpenIL com a textura (loadIMA)
//				- Inicialitzar imatges textura per l'objecte Truck
//

#include "Util.h"
#include "visualitzacio.h"
#include "GLWidget.h"

// TEXTURES: Vector de noms de textura
GLuint textures[NUM_MAX_TEXTURES]={0,1,2,3,4,5,6,7,8,9};

// Iluminació: Configurar iluminació de l'escena
void Iluminacio(char ilumin,bool ifix,LLUM lumin,bool textur,bool textur_map,char obj,bool bc_lin)
{   
	
	bool ll_amb=true;
	GLfloat angv,angh;

// Configuració de la font de llum LIGHT0
	GLfloat position[]={0.0,0.0,200.0,1.0};
	GLfloat especular[]={0.0,0.0,0.0,1.0};
    GLfloat ambientg[]={.5,.5,.5,1.0};

// Definició de llum ambient segons booleana ll_amb
	if (ll_amb) glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientg);
		else glLightModelfv(GL_LIGHT_MODEL_AMBIENT,especular);

	if (lumin.encesa) {	if (!ifix) glLightfv(GL_LIGHT0,GL_POSITION,position);
							else {	// Conversió angles graus -> radians
									angv=lumin.posicio.alfa*pi/180;
									angh=lumin.posicio.beta*pi/180;
		
									// Conversió Coord. esfèriques -> Coord. cartesianes
									position[0]=lumin.posicio.R*cos(angh)*cos(angv);	
									position[1]=lumin.posicio.R*sin(angh)*cos(angv);	
									position[2]=lumin.posicio.R*sin(angv);
									position[3]=1.0;
									glLightfv(GL_LIGHT0,GL_POSITION,position);			
								}
						glLightfv(GL_LIGHT0,GL_DIFFUSE,lumin.difusa);
						glLightfv(GL_LIGHT0,GL_SPECULAR,lumin.especular);

						// Coeficients factor atenuació f_att=1/(ad2+bd+c)
						glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,lumin.atenuacio.c);
						glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,lumin.atenuacio.b);
						glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,lumin.atenuacio.a);
						if (lumin.restringida) {
							glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,lumin.spotdirection);
							glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,lumin.cutoff);
							glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,lumin.exponent);
							}
						glEnable(GL_LIGHT0);
					}
		else glDisable(GL_LIGHT0);

	glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);

// Selecció del model d'iluminació segons variable ilumin
	switch(ilumin)
	{
	case FILFERROS:
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glDisable(GL_COLOR_MATERIAL);

// Desactivació de l'ocultació de cares
		glDisable(GL_DEPTH_TEST);

// Desactivació de la il-luminació
		glDisable(GL_LIGHTING);
//		glDisable(GL_LIGHT0);
		break;

    case PLANA:
// Càlcul de les normals a les cares si l'objecte és un fractal
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
// Dibuix de les cares back com a línies en Il.luminacio PLANA i SUAU
		if (bc_lin) glPolygonMode(GL_BACK,GL_LINE);
		
		glEnable(GL_COLOR_MATERIAL);

// Ocultació de cares
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_NORMALIZE);    
// Il.luminació per cares planes
		glShadeModel(GL_FLAT); 

// Activació de la llum
		glEnable(GL_LIGHTING);
//		glEnable(GL_LIGHT0);
		break;
	case SUAU:

// Càlcul de les normals als vertexs si l'objecte és un fractal
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
// Dibuix de les cares back com a línies en Il.luminacio PLANA i SUAU
		if (bc_lin) glPolygonMode(GL_BACK,GL_LINE);
		
		glEnable(GL_COLOR_MATERIAL);

// Ocultació de cares
    	glEnable(GL_DEPTH_TEST);

		glEnable(GL_NORMALIZE); 
		
// Il.luminació suau 
        glShadeModel(GL_SMOOTH); 

// Activació de la llum
    	glEnable(GL_LIGHTING);
//		glEnable(GL_LIGHT0);
		break;
	
	}

// Configuració de les textures.
	if ((textur) && !((obj==OBJ3DS) || (obj==OBJOBJ)))
	{

// Activació de la textura 0.
	  glBindTexture(GL_TEXTURE_2D,textures[0]) ;
//		escena.Activa_Textura(0);

// Pregunta 8 enunciat
//	  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);	
//	  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);	
	 
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
  		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

// Pregunta 9 enunciat
	  if (textur_map) glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE) ;
	  else glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

// Pregunta 7 enunciat
//	  glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
//	  glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);

		glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
		glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);

// Generació coordenades textura.
		GLfloat sPlane[4] = { -1.0f, 1.0f, 0.0f, 1.0f };
		GLfloat tPlane[4] = { -1.0f, 0.0f, 1.0f, 1.0f };

		glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
		glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);

	  glEnable(GL_TEXTURE_2D) ;
	}
	else { glDisable(GL_TEXTURE_2D);
	       glDisable(GL_TEXTURE_GEN_S);
	       glDisable(GL_TEXTURE_GEN_T);
		}

// Creació de la llista que dibuixarà els eixos
//   funció on està codi per dibuixar eixos
	glNewList(EIXOS,GL_COMPILE);
// Dibuix dels eixos sense il.luminació
//	if (ilumin!=FILFERROS) 
		glDisable(GL_LIGHTING);           	
// Dibuixar eixos sense textures
//	if (textura) 
		glDisable(GL_TEXTURE_2D);
	deixos();
	if (ilumin!=FILFERROS) glEnable(GL_LIGHTING);   
	if (textur) glEnable(GL_TEXTURE_2D);
	glEndList();

}

// PROJECCIO ORTOGRAFICA
// Projeccio_Orto: Definició Viewport i glOrtho
void Projeccio_Orto(int minx,int miny,int maxx,int maxy)
{
    //	----GC2: ESPECIFICACIO DELS PARÀMETRES DE PROJECCIÓ ORTOGRÀFICA
    //			QUE ES CARREGUEN A LA MATRIU DE PROJECCIÓ GL_PROJECTION

    // Desactivació del retall de pantalla

    glViewport(minx,miny,maxx,maxy);
    glScissor(minx,miny,maxx,maxy);


    // Activació i inicialització matriu PROJECTION
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        //glOrtho(-10,10,-10,10,p_near,p_far);

        float aspect = (float)maxx/maxy;

        // Aspect es w/h. Si es <1 vol dir que w ´es m´es petit que h
        if (aspect<1){
            glOrtho(-10,10,-10/aspect,10/aspect,p_near,p_far);
        }else{
            glOrtho(-10*aspect,10*aspect,-10,10,p_near,p_far);
        }

    // Activació matriu MODELVIEW (tancar matriu PROJECTION)
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
}

// Ortogràfica: Ilumina i dibuixa l'escena
void Ortografica(int prj, GLfloat R,CColor col_fons,CColor col_object,char objecte,bool oculta,bool testv,
				 bool bck_ln,char iluminacio,LLUM lumi,bool textur,bool textur_map,bool ifix,bool eix)
{
//	int i,j;

	GLfloat cam[3],up[3];
	bool sw_iso=false;

// Iluminacio movent-se amb la camara (abans gluLookAt)
    if (!ifix) Iluminacio(iluminacio,ifix,lumi,textur,textur_map,objecte,bck_ln);

// Implementació de planta,alçada,perfil i isomètrica
// ESPECIFICACIO DEL PUNT DE VISTA
// Cal definir el punt de vista (gluLookAt) en funció del
//     tipus de projecció definit a la variable prj.

    switch(prj){
            case 0:{
            // Superior Dret
            // Especificació del punt de vista
               gluLookAt(0,-10,0,0,0,0,0,0,1);
                break;
            }
            case 1:{
            // Inferior Esq
                // Especificació del punt de vista
               gluLookAt(0,0,10,0,0,0,-1,0,0);
                break;
            }
            case 2:{
            // Superior Esquerra
                // Especificació del punt de vista
               gluLookAt(10,0,0,0,0,0,0,0,1);
                break;
            }
            case 3:{
            // Inferior dreta
                // Especificació del punt de vista
                gluLookAt(10,10,10,0,0,0,0,0,1);
                break;
            }
        }



// Neteja dels buffers de color i profunditat
	Fons(col_fons);

// Iluminacio fixe respecte la camara (després gluLookAt)
    if (ifix) Iluminacio(iluminacio,ifix,lumi,textur,textur_map,objecte,bck_ln);

// Test de Visibilitat
	if (testv) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);

// Ocultacions (Z-buffer)
	if (oculta) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);

// Dibuix de les cares back com a línies en Il.luminacio PLANA i SUAU
    if (bck_ln) glPolygonMode(GL_BACK,GL_LINE);

// Dibuix dels eixos
	if (eix) glCallList(EIXOS);

// Color base de l'objecte
	glColor3f(col_object.r,col_object.g,col_object.b);

}

// PROJECCIO PERSPECTIVA
// Projeccio_Perspectiva: Definició Viewport i gluPerspective
void Projeccio_Perspectiva(int minx,int miny,GLsizei w,GLsizei h,float zoom)
{   

// Desactivació del retall de pantalla
    glDisable(GL_SCISSOR_TEST);

// Definició Viewport
	glViewport(minx,miny,w,h);
	if (h==0) h=1;

// Activació i inicialització matriu PROJECTION
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

// PROJECCIO PERSPECTIVA.Definim volum de visualització adaptant-lo 
//	 a les mides actuals de la finestra windows	

// Amb gluPerspective
	if (w>=h) 
	gluPerspective(60.0,1.0*w/h,p_near,p_far+zoom);
	else gluPerspective(60.0*h/w,1.0*w/h,p_near,p_far+zoom);

// Amb glFrustum (no actualitzar R)
//	if (w>=h) glFrustum(-rang*w/h,rang*w/h,-rang,rang,p_near,p_far+zoom);
//	else glFrustum(-rang,rang,-rang*h/w,rang*h/w,p_near,p_far+zoom);

// Activació matriu MODELVIEW (tancar matriu PROJECTION)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
   	
}

// Perspectiva: Definició gluLookAt amb possibilitat de moure 
//				el punt de vista interactivament amb el ratolí, 
//				ilumina i dibuixa l'escena
void Perspectiva(CEsfe3D opv,char VPol,bool pant,CPunt3D tr,CPunt3D trF,
                 CColor col_fons,CColor col_object,char objecte,bool oculta,bool testv,
				 bool bck_ln,char iluminacio,LLUM lumi,bool textur,bool textur_map,bool ifix,bool eix)
{    
	GLfloat cam[3],up[3];

// Conversió angles radians -> graus
   	opv.alfa=opv.alfa*pi/180;
    opv.beta=opv.beta*pi/180;

	if(opv.R<1.0) opv.R=1.0;

// Neteja dels buffers de color i profunditat
	Fons(col_fons);

// Posició càmera i vector cap amunt
	if (VPol==POLARZ) { cam[0]=opv.R*cos(opv.beta)*cos(opv.alfa);
						cam[1]=opv.R*sin(opv.beta)*cos(opv.alfa);
						cam[2]=opv.R*sin(opv.alfa);		
						up[0]=-cos(opv.beta)*sin(opv.alfa);
						up[1]=-sin(opv.beta)*sin(opv.alfa);
						up[2]=cos(opv.alfa);	}
		else if (VPol==POLARY) {	cam[0]=opv.R*sin(opv.beta)*cos(opv.alfa);
									cam[1]=opv.R*sin(opv.alfa);
									cam[2]=opv.R*cos(opv.beta)*cos(opv.alfa);		
									up[0]=-sin(opv.beta)*sin(opv.alfa);
									up[1]=cos(opv.alfa);
									up[2]=-cos(opv.beta)*sin(opv.alfa);		}
			else {	cam[0]=opv.R*sin(opv.alfa);
					cam[1]=opv.R*cos(opv.beta)*cos(opv.alfa);
					cam[2]=opv.R*sin(opv.beta)*cos(opv.alfa);		
					up[0]=cos(opv.alfa);
					up[1]=-cos(opv.beta)*sin(opv.alfa);
					up[2]=-sin(opv.beta)*sin(opv.alfa);		}

// Iluminacio movent-se amb la camara (abans glLookAt)
    if (!ifix) Iluminacio(iluminacio,ifix,lumi,textur,textur_map,objecte,bck_ln);

// Opció pan: desplaçament del Centre de l'esfera (pant=1)
	if (pant) glTranslatef(tr.x,tr.y,tr.z);
	glTranslatef(trF.x,trF.y,trF.z);	// Traslació fixada amb la INSERT dins l'opció pan

// Especificació del punt de vista
   gluLookAt(cam[0],cam[1],cam[2],0.,0.,0.,
		 up[0],up[1],up[2]);

// Iluminacio fixe respecte la camara (després glLookAt)
    if (ifix) Iluminacio(iluminacio,ifix,lumi,textur,textur_map,objecte,bck_ln);

// Test de Visibilitat
	if (testv) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);

// Ocultacions (Z-buffer)
	if (oculta) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);

// Dibuix de les cares back com a línies en Il.luminacio PLANA i SUAU
    if (bck_ln) glPolygonMode(GL_BACK,GL_LINE);

//  Dibuix dels eixos
	if (eix) glCallList(EIXOS);

// Color base de l'objecte
	glColor3f(col_object.r,col_object.g,col_object.b);

}


//PerspectivaN: Definició gluLookAt directament per paràmetre, sense esfèriques.
//              amb possibilitat de moure el punt de vista interactivament amb les
//				tecles de cursor per poder navegar.
void PerspectivaN(CPunt3D pv,GLfloat n[3],GLfloat v[3],bool pant,CPunt3D tr,CPunt3D trF,
                  CColor col_fons,CColor col_object,char objecte,bool oculta,bool testv,
				 bool bck_ln,char iluminacio,LLUM lumi,bool textur,bool textur_map,bool ifix,bool eix)
{    

// Neteja dels buffers de color i profunditat
	Fons(col_fons);

// Test de Visibilitat
	if (testv) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);

// Ocultacions (Z-buffer)
	if (oculta) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);


// Iluminacio movent-se amb la camara (abans glLookAt)
    if (!ifix) Iluminacio(iluminacio,ifix,lumi,textur,textur_map,objecte,bck_ln);

// Opció pan: desplaçament del Centre de l'esfera (pant=true)
	if (pant) glTranslatef(tr.x,tr.y,tr.z);
	glTranslatef(trF.x,trF.y,trF.z);	// Traslació fixada amb la INSERT dins l'opció pan

// Especificació del punt de vista
	gluLookAt(pv.x,pv.y,pv.z,n[0],n[1],n[2],v[0],v[1],v[2]);

// Iluminacio fixe respecte la camara (després glLookAt)
    if (ifix) Iluminacio(iluminacio,ifix,lumi,textur,textur_map,objecte,bck_ln);

// Dibuix dels eixos
	if (eix) glCallList(EIXOS);

// Color base de l'objecte
	glColor3f(col_object.r,col_object.g,col_object.b);

}



// instancia: Aplica Transformacions Geomètriques d'instanciació, segons els paràmetres 
//            definits a la persiana Transformacions
void instancia(bool TR,INSTANCIA tg,INSTANCIA tgF)
{    

	// Transformacions geomètriques sobre objecte (Traslació, Rotacions i Escalatge)
	if (TR) glTranslatef(tg.VTras.x,tg.VTras.y,tg.VTras.z);
	glTranslatef(tgF.VTras.x,tgF.VTras.y,tgF.VTras.z);	// Traslació fixada amb la INSERT dins l'opció Transformacions
	if (TR) glRotatef(tg.VRota.x,1,0,0);
	glRotatef(tgF.VRota.x,1,0,0);					// Rotació X fixada amb la INSERT dins l'opció Transformacions
	if (TR) glRotatef(tg.VRota.y,0,1,0);
	glRotatef(tgF.VRota.y,0,1,0);					// Rotació Y fixada amb la INSERT dins l'opció Transformacions
	if (TR) glRotatef(tg.VRota.z,0,0,1);
	glRotatef(tgF.VRota.z,0,0,1);					// Rotació Z fixada amb la INSERT dins l'opció Transformacions
	if (TR) glScalef(tg.VScal.x,tg.VScal.y,tg.VScal.z);
	glScalef(tgF.VScal.x,tgF.VScal.y,tgF.VScal.z);	// Escalatge fixada amb la INSERT dins l'opció Transformacions


}


// deixos: Dibuix dels eixos coordenats
void deixos()
{

// Eix X (vermell)
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_LINES);
	 glVertex3f(0.0,0.0,0.0);
	 glVertex3f(300.0,0.0,0.0);
	glEnd();

// Eix Y (verd)
    glColor3f(0.0,1.0,0.0);
	glBegin(GL_LINES);
	 glVertex3f(0.0,0.0,0.0);
	 glVertex3f(0.0,300.0,0.0);
	glEnd();

// Eix Z (blau)
	glColor3f(0.0,1.0,1.0);
	glBegin(GL_LINES);
	 glVertex3f(0.0,0.0,0.0);
	 glVertex3f(0.0,0.0,300.0);
	glEnd();

}

// Fons: Dibuixa el fons variable sefons el color int_fons
void Fons(CColor int_fons)
{
	glClearColor(int_fons.r,int_fons.g,int_fons.b,1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.f,0.f,0.f,1.f);
	glClear(GL_DEPTH_BUFFER_BIT);
   glFlush();

}

// FonsN: Dibuixa el fons negre
void FonsN()
{
	glClearColor(0.f,0.f,0.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glFlush();

}

// FonsB: Dibuixa el fons blanc
void FonsB()
{
	glClearColor(1.f,1.f,1.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.f,0.f,0.f,1.f);
	glClear(GL_DEPTH_BUFFER_BIT);
   glFlush();

}


// TEXTURES------------------------------------------------------
// loadIMA: This function receives as input the image filename and an 
// integer identifier (0 for the first texture) and creates an OpenGL 
// texture which is stored in the global array 'textures'
// Paràmetres:
//		- filename: Fitxer que conté la imatge de qualsevol format:
//					BMP, JPG, TIFF, TGA, GIF, i d'altres suportats per OpenIL
//		- texID: Identificador dins la taula textures on volem
//                assignar la imatge
bool loadIMA(char* filename, int texID)				
{
	FILE *file=NULL;
//	int errno;
	QImage t;
	QImage b;
	
// Open the image file for reading
#ifdef __APPLE__
    file=fopen(filename,"r");					// Funció Visual Studio 6.0 i MAC
    if (file == NULL)	{
    printf("Could not open file '%s'.\n",filename) ;
    return false ;
    }
#else
#ifdef _WIN32
    errno=fopen_s(&file,filename,"r");			// Funció Visual 2005 (Win)
    if (errno!=0)	{
    printf("Could not open file '%s'.\n",filename) ;
     return false ;
    }
#endif
#endif

// Close the image file
	fclose(file);

	b.load(filename);
	t=QGLWidget::convertToGLFormat(b);
//	glGenTextures(1,&texID);
	glBindTexture(GL_TEXTURE_2D,texID);
	gluBuild2DMipmaps(GL_TEXTURE_2D,3,t.width(),t.height(),GL_RGBA,GL_UNSIGNED_BYTE,t.bits());
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

// If execution arrives here it means that all went well. Return true
	return true;

}


