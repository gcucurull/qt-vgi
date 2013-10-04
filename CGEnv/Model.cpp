//******** PRACTICA GRAFICS PER COMPUTADOR 2
//******** Entorn bàsic amb interfície Qt i OpenGL (PFC)
//******** Ferran Poveda, Enric Martí (Juny 2012)
// Model.cpp - Classe que conté totes les comandes de dibuix d'objectes OpenGL

#include "Util.h"
#include "Model.h"

Model::Model(){

    obj=CAP;
    textu=false;
// TEXTURES: Vector de noms de textura
    for (int i=0;i<10;i++) textures[i]=i;

}

Model::~Model(){
	
}

void Model::dibuixa(){

    // Dibuixat d'eixos cartesians d'exemple
    switch(obj)
    {
// Cub
    case CUB:
//		glColor3f(1.0,1.0,1.0);
        glPushMatrix();
          glScalef(5.0f,5.0f,5.0f);
          glutSolidCube(1.0);
        glPopMatrix();
        break;

// Esfera
    case ESFERA:
//		glColor3f(1.0,1.0,1.0);
        glPushMatrix();
          glScalef(5.0f,5.0f,5.0f);
          glutSolidSphere(1.0,40,40);
        glPopMatrix();
        break;

// Tetera
    case TETERA:
//		glColor3f(1.0,1.0,1.0);
        glPushMatrix();
          glScalef(5.0f,5.0f,5.0f);
          glutSolidTeapot(1.0);
        glPopMatrix();
        break;

    case TRUCK:
// Dibuix del Truck
        glDisable(GL_TEXTURE_2D);
//		sea();
        truck(textu);
            break;

    case OBJ3DS:
//		glColor3f(col_object.r,col_object.g,col_object.b);
// Dibuix de l'objecte 3DS amb textures (OBJECTE3DST) o sense textures (OBJECTE3DS)
        if (textu) glCallList(OBJECTE3DS_T);
            else glCallList(OBJECTE3DS);
        break;

    case OBJOBJ:
// Objecte OBJ: Dibuix de l'objecte OBJ
        if (textu)  glCallList(OBJECTEOBJ_T);
            else glCallList(OBJECTEOBJ);
        break;

    }

}


/// OBJECTE Truck amb imatges textura si són actives
// Truck: Paràmetres:
//    - textu: Flag de textures activades o no.
//    - VTextu: Vector d'imatges textura.
void Model::truck(bool textu)
{
// Crear quàdriques de dibuix
        GLUquadricObj *eix;
        GLUquadricObj *TubEscapeEsq;
        GLUquadricObj *TubEscapeDrt;
        GLUquadricObj *CisternaDarrera;
        GLUquadricObj *CisternaCentre;
        GLUquadricObj *CisternaDavant;
        GLUquadricObj *CisternaDarreraTapa;
        GLUquadricObj *CisternaDavantTapa;

        eix = gluNewQuadric();
        TubEscapeEsq = gluNewQuadric();
        TubEscapeDrt = gluNewQuadric();
        CisternaDarrera = gluNewQuadric();
        CisternaCentre = gluNewQuadric();
        CisternaDavant = gluNewQuadric();

        CisternaDarreraTapa = gluNewQuadric();
        CisternaDavantTapa = gluNewQuadric();

glPushMatrix();
  glTranslatef(0.0f,0.0f,20.0f);

// MOTOR
// Textura carrosseria
    if (textu)
        {	GLfloat sPlane[4] = { 0.05f, 0.0f, 0.00f, 0.250f };
            GLfloat tPlane[4] = { 0.00f, 0.05f, 0.1f, 0.00f };
// Activar textura planxa
            glBindTexture(GL_TEXTURE_2D,textures[5]) ;
            glEnable(GL_TEXTURE_2D);
            glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
            glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);

            glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
            glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);

        }

// Color carrosseria sense textura
    glColor4f(0.5f,0.7f,1.0f,0.5f);

    glPushMatrix();
        glScalef(2.0f, 4.4f, 2.0f);
        glutSolidCube(10.0f);
    glPopMatrix();

// CABINA
// Cabina inferior
    glPushMatrix();
        glTranslatef(0.0f,32.0f,0.0f);
        glScalef(4.0f, 2.0f, 2.0f);
        glutSolidCube(10.0f);
    glPopMatrix();

// Cabina superior
    glPushMatrix();
        glTranslatef(0.0f,32.0f,15.0f);
        glScalef(2.0f, 2.0f, 1.0f);
        glutSolidCube(10.0f);
    glPopMatrix();

// Cabina trasera
    glPushMatrix();
        glTranslatef(0.0f,49.5f,12.5f);
        glScalef(4.0f, 1.5f, 4.5f);
        glutSolidCube(10.0f);
    glPopMatrix();

// CANTONADES
// Cantonada esquerra
    glPushMatrix() ;
      glTranslatef(10.0f,22.0f,-10.0f);

      glBegin(GL_QUADS);  // Cara de 4 costats
        glNormal3f(200.0f, 200.0f, 0.0f);
        glVertex3f(10.0f ,  0.0f , 0.0f ); //P1
        glVertex3f(10.0f , 0.0f , 20.0f ); //P2
        glVertex3f(0.0f , -10.0f ,20.0f ); //P3
        glVertex3f(0.0f ,  -10.0f ,0.0f ); //P4
      glEnd();

      glBegin(GL_POLYGON);  // Triangle d'abaix
        glNormal3f(0.0f, 0.0f, -200.0f);
        glVertex3f(0.0f ,  0.0f , 0.0f );  // T1
        glVertex3f(10.0f ,  0.0f ,0.0f );  // P1
        glVertex3f(0.0f , -10.0f , 0.0f ); // P4
      glEnd();

      glBegin(GL_POLYGON);  // Triangle de dalt
        glNormal3f(0.0f, 0.0f, 200.0f);
        glVertex3f(0.0f ,  0.0f , 20.0f );	// T2
        glVertex3f(0.0f , -10.0f , 20.0f ); // P3
        glVertex3f(10.0f ,  0.0f, 20.0f );  // P2
      glEnd();
    glPopMatrix();

// Cantonada dreta
    glPushMatrix() ;
      glTranslatef(-10.0f,22.0f,-10.0f);
      glRotatef(-90.0f,0.0f,0.0f,1.0f);

      glBegin(GL_QUADS);  // Cara de 4 costats
        glNormal3f(200.0f, 200.0f, 0.0f);
        glVertex3f(10.0f ,  0.0f , 0.0f ); //P1
        glVertex3f(10.0f , 0.0f , 20.0f ); //P2
        glVertex3f(0.0f , -10.0f ,20.0f ); //P3
        glVertex3f(0.0f ,  -10.0f ,0.0f ); //P4
      glEnd();

      glBegin(GL_POLYGON);  // Triangle d'abaix
        glNormal3f(0.0f, 0.0f, -200.0f);
        glVertex3f(0.0f ,  0.0f , 0.0f );  // T1
        glVertex3f(10.0f ,  0.0f ,0.0f );  // P1
        glVertex3f(0.0f , -10.0f , 0.0f ); // P4
      glEnd();

      glBegin(GL_POLYGON);  // Triangle de dalt
        glNormal3f(0.0f, 0.0f, 200.0f);
        glVertex3f(0.0f ,  0.0f , 20.0f );	// T2
        glVertex3f(0.0f , -10.0f , 20.0f ); // P3
        glVertex3f(10.0f ,  0.0f, 20.0f );  // P2
      glEnd();
    glPopMatrix();


// PROTECCIONS DE RODA
// Textura Proteccions de roda (metall)
//	if (textu)
//		{
//			glBindTexture(GL_TEXTURE_2D,VTextu[5]) ;
//			glEnable(GL_TEXTURE_2D) ;
//			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
//			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
//			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
//			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
//			}

// Color proteccions roda sense textura
    glColor4f(0.5f,0.7f,1.0f,0.5f);

// Protecció de roda esquerra
    glPushMatrix();
        glTranslatef(10.0f, 0.0f, 3.0f);
        glBegin(GL_QUADS);
        //  glNormal3f(200.0f, 200.0f, 0.0f);
            glVertex3f(7.0f , -5.0f , 0.0f );
            glVertex3f(7.0f , 5.0f , 0.0f );
            glVertex3f(0.0f , 5.0f , 0.0f );
            glVertex3f(0.0f , -5.0f , 0.0f );

            glVertex3f(7.0f , -12.0f , -8.0f );
            glVertex3f(7.0f , -5.0f , 0.0f );
            glVertex3f(0.0f , -5.0f , 0.0f );
            glVertex3f(0.0f , -12.0f , -8.0f );

            glVertex3f(0.0f , 15.0f , -10.0f );
            glVertex3f(0.0f , 5.0f , 0.0f );
            glVertex3f(7.0f , 5.0f , 0.0f );
            glVertex3f(7.0f , 15.0f , -10.0f );

            glVertex3f(7.5f , -5.0f , -3.0f );
            glVertex3f(7.5f , 5.0f , -3.0f );
            glVertex3f(7.0f , 5.0f , 0.0f );
            glVertex3f(7.0f , -5.0f , 0.0f );

            glVertex3f(7.5f , -12.0f , -8.0f );
            glVertex3f(7.5f , -5.0f , -3.0f );
            glVertex3f(7.0f , -5.0f , 0.0f );
            glVertex3f(7.0f , -12.0f , -8.0f );

            glVertex3f(7.0f , 15.0f , -10.0f );
            glVertex3f(7.0f , 5.0f , 0.0f );
            glVertex3f(7.5f , 5.0f , -3.0f );
            glVertex3f(7.5f , 15.0f , -10.0f );

        glEnd();
    glPopMatrix();

// Protecció de roda dreta
    glPushMatrix();
        glTranslatef(-10.0f, 0.0f, 3.0f);
        glRotatef(180,0,0,1);
        glBegin(GL_QUADS);
        //  glNormal3f(200.0f, 200.0f, 0.0f);
            glVertex3f(7.0f , -5.0f , 0.0f );
            glVertex3f(7.0f , 5.0f , 0.0f );
            glVertex3f(0.0f , 5.0f , 0.0f );
            glVertex3f(0.0f , -5.0f , 0.0f );

            glVertex3f(7.0f , -12.0f , -8.0f );
            glVertex3f(7.0f , -5.0f , 0.0f );
            glVertex3f(0.0f , -5.0f , 0.0f );
            glVertex3f(0.0f , -12.0f , -8.0f );

            glVertex3f(0.0f , 15.0f , -10.0f );
            glVertex3f(0.0f , 5.0f , 0.0f );
            glVertex3f(7.0f , 5.0f , 0.0f );
            glVertex3f(7.0f , 15.0f , -10.0f );

            glVertex3f(7.5f , -5.0f , -3.0f );
            glVertex3f(7.5f , 5.0f , -3.0f );
            glVertex3f(7.0f , 5.0f , 0.0f );
            glVertex3f(7.0f , -5.0f , 0.0f );

            glVertex3f(7.5f , -12.0f , -8.0f );
            glVertex3f(7.5f , -5.0f , -3.0f );
            glVertex3f(7.0f , -5.0f , 0.0f );
            glVertex3f(7.0f , -12.0f , -8.0f );

            glVertex3f(7.0f , 15.0f , -10.0f );
            glVertex3f(7.0f , 5.0f , 0.0f );
            glVertex3f(7.5f , 5.0f , -3.0f );
            glVertex3f(7.5f , 15.0f , -10.0f );
        glEnd();
    glPopMatrix();

// Textura Reixeta Motor
    if (textu)
        {	GLfloat sPlane[4] = { 0.05f, 0.25f, 0.00f, 0.00f };
            GLfloat tPlane[4] = { 0.00f, 0.25f, 0.05f, 0.00f };
// Activar textura
            glBindTexture(GL_TEXTURE_2D,textures[3]) ;

            glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
            glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);

            glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
            glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);
            glEnable(GL_TEXTURE_2D);
        }

// Color reixeta motor sense textura
    glColor4f(0.5f,0.7f,1.0f,0.5f);

// Reixeta Motor
    glPushMatrix();
        glTranslatef(0.0f,-22.0f,0.0f);
        glBegin(GL_QUADS);
            glNormal3f(0.0f,-1.0f,0.0f);
            glTexCoord2f(0.0f,0.0f);
            glVertex3f(-10.0f , -2.0f , -10.0f );
            glTexCoord2f(1.0f,0.00f);
            glVertex3f(10.0f , -2.0f , -10.0f );
            glTexCoord2f(1.0f,1.0);
            glVertex3f(10.0f , -2.0f , 10.0f );
            glTexCoord2f(0.0f,1.0f);
            glVertex3f(-10.0f , -2.0f , 10.0f );
        glEnd();
    glPopMatrix();

// Desactivar textura
    if (textu) glDisable(GL_TEXTURE_2D);

// Color eix sense textura
    glColor4f(0.1f,0.4f,1.0f,0.5f);

// Eix Davanter
    glPushMatrix();
      glTranslatef(0.0f,0.0f,-10.0f);
      glRotatef(90.0f,0.0f,1.0f,0.0f);
      glTranslatef(0.0f,0.0f,-13.0f);
      gluCylinder(eix, 2,2,26.0f,6,1);
    glPopMatrix();

// Roda Dreta Davantera
    glPushMatrix();
        glTranslatef(-10.0f,0.0f,-10.0f);
        neumatic(textu);
    glPopMatrix();

// Roda Esquerra Davantera
    glPushMatrix();
        glTranslatef(17.0f,0.0f,-10.0f);
        neumatic(textu);
    glPopMatrix();

// Desactivar textura
    if (textu) glDisable(GL_TEXTURE_2D);

// FARS
// Color fars sense textura
    glColor4f(1.0f,0.0f,0.0f,0.5f);

// Far Esquerra
    glPushMatrix();
        glTranslatef(-13.5f,-17.0f,-8.0f);
        glScalef(7.0f, 2.0f, 4.0f);
        glutSolidCube(1.0f);
    glPopMatrix();

// Far dret
    glPushMatrix();
        glTranslatef(13.5f,-17.0f,-8.0f);
        glScalef(7.0f, 2.0f, 4.0f);
        glRotatef(90.0f,1.0f,0.0f,0.0f); // Per a la textura
        glutSolidCube(1.0f);
    glPopMatrix();

// Textura frontal fars
      if (textu)
        {	GLfloat sPlane[4] = { 0.15f, 0.0f, 0.0f, 0.5f };
            GLfloat tPlane[4] = { 0.0f, 0.35f, 0.25f, 0.15f };
// Activar textura
            glBindTexture(GL_TEXTURE_2D,textures[4]) ;

            glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
            glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);

            glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
            glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);
            glEnable(GL_TEXTURE_2D);
            }

// Color frontal fars sense textura
    glColor4f(1.0f,1.0f,1.0f,0.5f);

// Frontal far dret
    glPushMatrix();
        glTranslatef(-13.5f,-16.5f,-8.0f);
        glBegin(GL_QUADS);
            glNormal3f(0.0f,-1.0f,0.0f);
            glTexCoord2f(0.0f,0.0f);
            glVertex3f(-3.5f , -2.0f , -2.0f );
            glTexCoord2f(1.0f,0.00f);
            glVertex3f(3.5f , -2.0f , -2.0f );
            glTexCoord2f(1.0f,1.0);
            glVertex3f(3.5f , -2.0f , 2.0f );
            glTexCoord2f(0.0f,1.0f);
            glVertex3f(-3.5f , -2.0f , 2.0f );
        glEnd();
    glPopMatrix();

// Frontal far esquerre
    glPushMatrix();
        glTranslatef(13.5f,-16.5f,-8.0f);
        glBegin(GL_QUADS);
            glNormal3f(0.0f,-1.0f,0.0f);
            glTexCoord2f(0.0f,0.0f);
            glVertex3f(-3.5f , -2.0f , -2.0f );
            glTexCoord2f(1.0f,0.00f);
            glVertex3f(3.5f , -2.0f , -2.0f );
        glTexCoord2f(1.0f,1.0);
            glVertex3f(3.5f , -2.0f , 2.0f );
            glTexCoord2f(0.0f,1.0f);
            glVertex3f(-3.5f , -2.0f , 2.0f );
        glEnd();
    glPopMatrix();

// Desactivar textura
    if (textu) glDisable(GL_TEXTURE_2D);

// VIDRE CABINA (SENSE TEXTURA)
// Activar transparència
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

// Color vidre cabina sense textura
    glColor4f(0.3f,0.8f,0.7f,0.5f);

// Vidre
    glPushMatrix() ;
      glTranslatef(0.0f,22.0f,10.0f);
      glRotatef(-90.0f,1.0f,0.0f,0.0f);
      glRotatef(90.0f,0.0f,1.0f,0.0f);
      glTranslatef(0.0f,00.0f,-10.0f);

      glBegin(GL_QUADS);  // Cara de 4 costats
        glNormal3f(200.0f, 200.0f, 0.0f);
        glVertex3f(10.0f ,  0.0f , 0.0f ); //P1
        glVertex3f(10.0f , 0.0f , 20.0f ); //P2
        glVertex3f(0.0f , -10.0f ,20.0f ); //P3
        glVertex3f(0.0f ,  -10.0f ,0.0f ); //P4
      glEnd();

      glBegin(GL_POLYGON);  // Triangle d'abaix
        glNormal3f(0.0f, 0.0f, -200.0f);
        glVertex3f(0.0f ,  0.0f , 0.0f );  // T1
        glVertex3f(10.0f ,  0.0f ,0.0f );  // P1
        glVertex3f(0.0f , -10.0f , 0.0f ); // P4
      glEnd();

      glBegin(GL_POLYGON);  // Triangle de dalt
        glNormal3f(0.0f, 0.0f, 200.0f);
        glVertex3f(0.0f ,  0.0f , 20.0f );	// T2
        glVertex3f(0.0f , -10.0f , 20.0f ); // P3
        glVertex3f(10.0f ,  0.0f, 20.0f );  // P2
      glEnd();
    glPopMatrix();

// Desactivar transparència
    glDisable(GL_BLEND);

// TUBS D'ESCAPAMENT (SENSE TEXTURA)
// Color tub d'escapament sense textura
    glColor4f(1.0f,0.0f,0.0f,0.5f);

// Tub d'escapament esquerre
    glPushMatrix();
        glTranslatef(18.0f,42.0f,10.0f);
        gluCylinder(TubEscapeEsq, 2,2,40.0f,12,1);
    glPopMatrix();

// Tub d'escapament dret
    glPushMatrix();
        glTranslatef(-18.0f,42.0f,10.0f);
        gluCylinder(TubEscapeDrt, 2,2,40.0f,12,1);
    glPopMatrix();

// CARROSSERIA
// Textura carrosseria
    if (textu)
        {	GLfloat sPlane[4] = { 0.05f, 0.0f, 0.00f, 0.250f };
            GLfloat tPlane[4] = { 0.00f, 0.05f, 0.1f, 0.00f };

// Activar textura
            glBindTexture(GL_TEXTURE_2D,textures[5]) ;

            glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
            glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);

            glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
            glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);
            glEnable(GL_TEXTURE_2D);
        }

// Color porta-eix motriu sense textura
    glColor4f(0.5f,0.7f,1.0f,0.5f);

// Porta-eix motriu mig
    glPushMatrix();
      glTranslatef(0.0f,82.0f,-5.0f);
      glScalef(1.0f, 5.0f, 1.0f);
      glutSolidCube(10.0f);
    glPopMatrix();

    if (textu) glDisable(GL_TEXTURE_2D);

// Conjunt eixos i rodes eix mig
    glPushMatrix();
// Desplaçament Eix Mig
      glTranslatef(0.0f,72.0f,0.0f);

// Color eix sense textura
      glColor4f(0.1f,0.4f,1.0f,0.5f);

// Eix Mig Davanter
      glPushMatrix();
        glTranslatef(0.0f,0.0f,-10.0f);
        glRotatef(90.0f,0.0f,1.0f,0.0f);
        glTranslatef(0.0f,0.0f,-13.0f);
        gluCylinder(eix, 2,2,26.0f,6,1);
      glPopMatrix();

// Roda Dreta Davantera Eix Mig Davanter
      glPushMatrix();
        glTranslatef(-10.0f,0.0f,-10.0f);
        neumatic(textu);
      glPopMatrix();

// Roda Esquerra Davantera Eix Mig Davanter
      glPushMatrix();
        glTranslatef(17.0f,0.0f,-10.0f);
        neumatic(textu);
      glPopMatrix();

// Desactivar textura
    if (textu) glDisable(GL_TEXTURE_2D);

// Desplaçament entre dos eixos
      glTranslatef(0.0f,25.0f,0.0f);

// Color eix sense textura
      glColor4f(0.1f,0.4f,1.0f,0.5f);

// Eix Mig Darrera
      glPushMatrix();
        glTranslatef(0.0f,0.0f,-10.0f);
        glRotatef(90.0f,0.0f,1.0f,0.0f);
        glTranslatef(0.0f,0.0f,-13.0f);
        gluCylinder(eix, 2,2,26.0f,6,1);
      glPopMatrix();

// Roda Dreta Davantera Eix Mig Darrera
      glPushMatrix();
        glTranslatef(-10.0f,0.0f,-10.0f);
        neumatic(textu);
      glPopMatrix();

// Roda Esquerra Davantera Eix Mig Darrera
      glPushMatrix();
        glTranslatef(17.0f,0.0f,-10.0f);
        neumatic(textu);
      glPopMatrix();

// Fi conjunt d'eixos i rodes eix mig
    glPopMatrix();

// Desactivar textura
    if (textu) glDisable(GL_TEXTURE_2D);



// REMOLC
// Textura carrosseria
    if (textu)
        {	GLfloat sPlane[4] = { 0.05f, 0.0f, 0.00f, 0.250f };
            GLfloat tPlane[4] = { 0.00f, 0.05f, 0.1f, 0.00f };

// Activar textura
            glBindTexture(GL_TEXTURE_2D,textures[5]);

            glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
            glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);

            glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
            glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);
            glEnable(GL_TEXTURE_2D);
        }


// Color plataforma sense textura
    glColor4f(0.5f,0.7f,1.0f,0.5f);

// Plataforma
    glPushMatrix();
        glTranslatef(0.0f,127.0f,4.0f);
        glScalef(3.0f, 14.0f, 0.8f);
        glutSolidCube(10.0f);
    glPopMatrix();

// Textura Cisterna
    if (textu)
        {	glBindTexture(GL_TEXTURE_2D,textures[0]);
            glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
            glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
            glEnable(GL_TEXTURE_2D);
        }

// Color cisterna sense textura
    glColor4f(0.9f,0.9f,1.0f,0.5f);

// Cisterna
    glPushMatrix();
      glTranslatef(0.0f,60.0f,20.0f);
      glScalef(0.75f,1.0f,0.5f);
      glRotatef(270.0f,1.0f,0.0f,0.0f);
// Part de davant
      gluCylinder(CisternaDavant, 20,25,5.0f,25,1);
      glPushMatrix();
        glRotatef(180.0f,1.0f,0.0f,0.0f);
        gluDisk(CisternaDavantTapa, 0.0f, 20.0f, 25, 1);
      glPopMatrix();
// Part Central
      glTranslatef(0.0f,0.0f,5.0f);
      gluCylinder(CisternaCentre, 25,25,125.0f,25,1);
// Part Darrera
      glPushMatrix();
        glTranslatef(0.0f,0.0f,125.0f);
        gluCylinder(CisternaDarrera, 25,20,5.0f,25,1);
        glTranslatef(0.0f,0.0f,5.0f);
        gluDisk(CisternaDarreraTapa, 0.0f, 20.0f, 25, 1);
      glPopMatrix();
    glPopMatrix();

// Desactivar textura
//	if (textu) glDisable(GL_TEXTURE_2D);

// Textura planxa
    if (textu)
        {
            GLfloat sPlane[4] = { 0.05f, 0.0f, 0.00f, 0.250f };
            GLfloat tPlane[4] = { 0.00f, 0.05f, 0.1f, 0.00f };
// Activar textura planxa
            glBindTexture(GL_TEXTURE_2D,textures[5]) ;

            glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
            glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);

            glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
            glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);
            glEnable(GL_TEXTURE_2D);
        }

// Color porta-eix motriu
    glColor4f(0.5f,0.7f,1.0f,0.5f);

// Porta-eix motriu darrera
    glPushMatrix();
      glTranslatef(0.0f,172.0f,-5.0f);
      glScalef(1.0f, 5.0f, 1.0f);
      glutSolidCube(10.0f);
    glPopMatrix();

// Desactivar textura
    if (textu) glDisable(GL_TEXTURE_2D);

// Conjunt eixos i rodes eix darrera
    glPushMatrix();

// Desplaçament Eix Darrera
      glTranslatef(0.0f,162.0f,0.0f);

// Color eix sense textura
      glColor4f(0.1f,0.4f,1.0f,0.5f);

// Eix Darrera Davanter
      glPushMatrix();
        glTranslatef(0.0f,0.0f,-10.0f);
        glRotatef(90.0f,0.0f,1.0f,0.0f);
        glTranslatef(0.0f,0.0f,-13.0f);
        gluCylinder(eix, 2,2,26.0f,6,1);
      glPopMatrix();

// Roda Dreta Davantera Eix Darrera Davanter
      glPushMatrix();
        glTranslatef(-10.0f,0.0f,-10.0f);
        neumatic(textu);
      glPopMatrix();

// Roda Esquerra Davantera Eix Darrera Davanter
      glPushMatrix();
        glTranslatef(17.0f,0.0f,-10.0f);
        neumatic(textu);
      glPopMatrix();

// Desactivar textura
    if (textu) glDisable(GL_TEXTURE_2D);

// Desplaçament entre dos eixos
      glTranslatef(0.0f,25.0f,0.0f);

// Color eix sense textura
      glColor4f(0.1f,0.4f,1.0f,0.5f);

// Eix Darrera Darrera
      glPushMatrix();
        glTranslatef(0.0f,0.0f,-10.0f);
        glRotatef(90.0f,0.0f,1.0f,0.0f);
        glTranslatef(0.0f,0.0f,-13.0f);
        gluCylinder(eix, 2,2,26.0f,6,1);
      glPopMatrix();

// Roda Dreta Davantera Eix Darrera Darrera
      glPushMatrix();
        glTranslatef(-10.0f,0.0f,-10.0f);
        neumatic(textu);
      glPopMatrix();

// Roda Esquerra Davantera Eix Darrera Darrera
      glPushMatrix();
        glTranslatef(17.0f,0.0f,-10.0f);
        neumatic(textu);
      glPopMatrix();
// Fi conjunt eixos i rodes eix darrera
    glPopMatrix();

// Fi dibuix Truck
glPopMatrix();

// Restaurar textura[0]
if (textu)
    {	glBindTexture(GL_TEXTURE_2D,textures[0]);
        glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
        glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
        glEnable(GL_TEXTURE_2D);
    }

// Alliberar memòria quàdriques
    gluDeleteQuadric(CisternaDavantTapa);
    gluDeleteQuadric(CisternaDarreraTapa);
    gluDeleteQuadric(CisternaDavant);
    gluDeleteQuadric(CisternaCentre);
    gluDeleteQuadric(CisternaDarrera);
    gluDeleteQuadric(TubEscapeDrt);
    gluDeleteQuadric(TubEscapeEsq);
    gluDeleteQuadric(eix);
}


// Dibuix de roda
// neumatic: Paràmetres:
//    - textur: Flag de textures activades o no.
//    - VTextur: Vector d'imatges textura.
void Model::neumatic(bool textur)
{

// Crear quàdrigues per la roda
    GLUquadricObj *roda;
    GLUquadricObj *tapareraroda;
    GLUquadricObj *taparoda;

    roda = gluNewQuadric();
    tapareraroda = gluNewQuadric();
    taparoda = gluNewQuadric();

// Textura neumàtic
    if (textur)
        {
// Activar textura neumàtic
            glBindTexture(GL_TEXTURE_2D,textures[1]) ;

            glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
            glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
            glEnable(GL_TEXTURE_2D);
        }

// Color neumàtic sense textura
    glColor4f(0.2f,0.2f,0.2f,0.5f);

    glPushMatrix();
// Tapa inferior neumàtic
      glRotatef(90.0f,0.0f,1.0f,0.0f);
      gluDisk(tapareraroda, 5.0f, 10.0f, RESOLUCIO_RODA, 1);

// Dibuix de la llanta
      llanta(textur);

// Textura neumàtic
      if (textur)
        {
// Activar textura neumàtic
            glBindTexture(GL_TEXTURE_2D,textures[1]) ;
            glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
            glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
            glEnable(GL_TEXTURE_2D);
        }

// Color neumàtic sense textura
    glColor4f(0.2f,0.2f,0.2f,0.5f);

// Gruix del neumàtic
      glTranslatef(0.0f,0.0f,-6.0f);
      gluCylinder(roda, 10,10,6.0f,RESOLUCIO_RODA,1);
// Tapa superior neumàtic
      glRotatef(180.0f,0.0f,1.0f,0.0f);
      gluDisk(taparoda, 5.0f, 10.0f, RESOLUCIO_RODA, 1);
// Dibuix de la llanta
      llanta(textur);
    glPopMatrix();

// Desactivar textures
//	if (textur) glDisable(GL_TEXTURE_2D);

// Alliberar memòria quàdriques
    gluDeleteQuadric(tapareraroda);
    gluDeleteQuadric(taparoda);
    gluDeleteQuadric(roda);
}

// Dibuix de la llanta de la roda
// llanta: Paràmetres:
//    - textur: Flag de textures activades o no.
//    - VTextur: Vector d'imatges textura.
void Model::llanta(bool textur)
{

// Crear quàdrica per la llanta
    GLUquadricObj *tapacub;
    tapacub = gluNewQuadric();

// Textura de la llanta
    if (textur)
        {	GLfloat sPlane[4] = { 0.0f, -0.1f, 0.5f, 0.5f };
            GLfloat tPlane[4] = { 0.1f, 0.0f, 0.5f, 0.5f };
// Activar la textura
            glBindTexture(GL_TEXTURE_2D,textures[2]) ;
            glEnable(GL_TEXTURE_2D) ;

            glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
            glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);

            glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR) ;
            glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR) ;
            glEnable(GL_TEXTURE_GEN_S) ;
            glEnable(GL_TEXTURE_GEN_T) ;
        }

// Dibuix de la llanta (disc de radi 5 de color clar)
    glColor4f(0.7f,0.7f,0.7f,0.5f);
    gluDisk(tapacub, 0.0f, 5.0f, RESOLUCIO_RODA, 1);

// Alliberar quàdrica
    gluDeleteQuadric(tapacub);

}


// Inicialitzar imatges textures pel Truck
void Model::Init_Textures()
{

//	loadIMA("./ima/metall.bmp",0);
    textures[0]=loadIMA("./ima/metall.bmp");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE) ;

//	loadIMA("./ima/vent.bmp",1);
    textures[1]=loadIMA("./ima/vent.bmp");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE) ;

//	loadIMA("./ima/plat.bmp",2);
    textures[2]=loadIMA("./ima/plat.bmp");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE) ;

//	loadIMA("./ima/reixeta.bmp",3);
    textures[3]=loadIMA("./ima/reixeta.bmp");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE) ;

//	loadIMA("./ima/fars.bmp",4);
    textures[4]=loadIMA("./ima/fars.bmp");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE) ;

//	loadIMA("./ima/txapa.bmp",5);
    textures[5]=loadIMA("./ima/txapa.bmp");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE) ;

}

// Mar amb ondulacions
void Model::sea(void)
{
    int i,j,step,it1,it2;
//     GLfloat pi=3.14159;
     float angle,delay;

        //aigua plana
    /*	glColor3f(0.5,0.4,0.9);
        glNormal3f(0.0,0.0,1.0);
        glBegin(GL_POLYGON);
        glVertex3f(-250,-250,0);
        glVertex3f(250,-250,0);
        glVertex3f(250,250,0);
        glVertex3f(-250,250,0);
        glEnd();*/


// Aigua amb ondulacions simulades a partir de normals sinusoidals
    step=5;
    delay=0;
    it1=0;

//   SeleccionaMaterial(MAT_DEFAULT);

     float h=2*pi*step/500;
      for(j=-250;j<250-step;j=j+step)
      {

         delay=1.0*h*it1;

         it2=0;
           glColor3f(0.5f,0.4f,0.9f);
          for(i=-250;i<250-step;i=i+step)
          {
               glBegin(GL_POLYGON);
               angle=1.0*it2*h*15;
               glNormal3f(-cos(angle),0,1);
               glVertex3f(i,j,0);

               angle=1.0*(it2+1)*h*15;
               glNormal3f(-cos(angle),0,1);
               glVertex3f(i+step,j,0);

               angle=1.0*(it2+1)*h*15;
               glNormal3f(-cos(angle),0,1);
               glVertex3f(i+step,j+step,0);
               glEnd();

               glBegin(GL_POLYGON);
               angle=1.0*it2*h*15;
               glNormal3f(-cos(angle),0,1);
               glVertex3f(i,j,0);

               angle=1.0*(it2+1)*h*15;
               glNormal3f(-cos(angle),0,1);
               glVertex3f(i+step,j+step,0);

               angle=1.0*it2*h*15;
               glNormal3f(-cos(angle),0,1);
               glVertex3f(i,j+step,0);
               glEnd();
            it2++;
          }
          it1++;
          }

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
GLuint Model::loadIMA(char* filename)
{
    GLuint texID;
    QImage t;
    QImage b;

    b.load(filename);
    t=QGLWidget::convertToGLFormat(b);
    glGenTextures(1,&texID);
    glBindTexture(GL_TEXTURE_2D,texID);
    gluBuild2DMipmaps(GL_TEXTURE_2D,3,t.width(),t.height(),GL_RGBA,GL_UNSIGNED_BYTE,t.bits());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    return texID;
}

void Model::Activa_Textura(int texID){

    glBindTexture(GL_TEXTURE_2D,texID);
//    glBindTexture(GL_TEXTURE_2D,textures[0]);
}

/* ------------------------------------------------------------------------- */
/*					SETTERS & GETTERS										 */
/* ------------------------------------------------------------------------- */


// VARIABLE obj
int Model::GetObjecte(void){

    return this->obj;
}

void Model::SetObjecte(char mode){

    this->obj=mode;
}

//VARIABLE textu
int Model::GetTextu(void) {

    return this->textu;
}


void Model::SetTextu(bool mode) {

    this->textu=mode;

}

// VARIABLE VTextu
void Model::SetVTextu(GLuint mode[NUM_MAX_TEXTURES]) {

    int i;

    for (i=0;i<NUM_MAX_TEXTURES;i++){
        this->textures[i]=mode[i];
    }

}
