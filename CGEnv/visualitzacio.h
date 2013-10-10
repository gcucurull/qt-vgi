//******** PRACTICA GRAFICS PER COMPUTADOR 2
//******** Entorn bàsic amb interfície Qt i OpenGL (PFC)
//******** Carme Julià, Débora Gil, Ferran Poveda, Enric Martí (Juny 2012)
// visualitzacio.h : Interface de visualitzacio.cpp
//

#ifndef GLVIS_H
#define GLVIS_H

/* ------------------------------------------------------------------------- */
/*                                Funcions                                   */
/* ------------------------------------------------------------------------- */

// G2: Il.luminació
void Iluminacio(char ilumin,bool ifix,LLUM lumin,bool textur,char obj,bool bc_lin);

// G2: Projecció Ortografica;
void Projeccio_Orto(int,int,int,int);
void Ortografica(int prj,GLfloat R,CColor col_fons,CColor col_object,char objecte,bool oculta,bool testv,
				 bool bck_ln,char iluminacio,LLUM lumi,bool textur,bool textur_map,bool ifix,bool eix);

// G2: Projecció Perspectiva
void Projeccio_Perspectiva(int,int,GLsizei,GLsizei,float);
void Perspectiva(CEsfe3D opv,char VPol,bool pant,CPunt3D tr,CPunt3D trF,
                 CColor col_fons,CColor col_object,char objecte,bool oculta,bool testv,
				 bool bck_ln,char iluminacio,LLUM lumi,bool textur,bool textur_map,bool ifix,bool eix);

void PerspectivaN(CPunt3D pv,GLfloat n[3],GLfloat v[3], bool pant,CPunt3D tr,CPunt3D trF,
                  CColor col_fons,CColor col_object,char objecte,bool oculta,bool testv,
				 bool bck_ln,char iluminacio,LLUM lumi,bool textur,bool textur_map,bool ifix,bool eix);

void instancia(bool TR,INSTANCIA tg,INSTANCIA tgF);

// G2: Dibuix dels eixos de coordenades
void deixos(void);

// G2: Esborrats de pantalla a fons variable (Fons), negre (Fons) o a blanc (FonsW)
void Fons(CColor int_fons);
void FonsN(void);
void FonsB(void);

// G2: Carregar fitxer imatge de formats admesos per la llibreria DevIL/OpenIL 
//		com a textura
bool loadIMA(char* filename, int texID);

// Textures Objecte Truck
void Init_Textures();

#endif
