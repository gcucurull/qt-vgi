// OBJModel.h: Inteface of the class COBJModel.
//
//////////////////////////////////////////////////////////////////////

#ifndef OBJLOADER_H
#define OBJLOADER_H

#define OBJLOADER_CLASS_DECL     __declspec(dllexport)

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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <QImage>
#include <QGLWidget>

//Guillem: Nomes Apple??
/*
#ifdef __APPLE__
#define _MAX_PATH 512
#endif
*/

// Proposta:
#define _MAX_PATH 512

// Needed structures

struct Vector3D
{
	float fX;
	float fY;
	float fZ;
};

struct Vector2D
{
	float fX;
	float fY;
};

struct OBJFileInfo
{
	unsigned int iVertexCount;
	unsigned int iTexCoordCount;
	unsigned int iNormalCount;
	unsigned int iFaceCount;
	unsigned int iMaterialCount;
};

struct Face
{
	unsigned int	iNumVertices;
	unsigned int	iMaterialIndex;
	Vector3D		*pVertices;
	Vector3D		*pNormals;
	Vector2D		*pTexCoords;
};

struct Material
{
  char	szName[1024];		 // Name of the material
  char	szTexture[_MAX_PATH];// Filename of the texture
  int	iTextureID;			 // OpenGL name of the texture
  float fDiffuse[3];		 // Diffuse component
  float fAmbient[3];		 // Ambient component
  float fSpecular[3];		 // Specular component
  float fEmmissive[3];		 // Emmissive component
  float fShininess;			 // Specular exponent
};

class COBJModel
{
  public:
      void DrawModel();
//	  bool LoadModel(const char *szFileName[],unsigned int iDisplayList);
      bool LoadModel(const char *szFileName,unsigned int iDisplayList);
      COBJModel();
      virtual ~COBJModel();
      void EliminaLlista(unsigned int iDisplayList);
	  bool texture;
  private:
      void ReadNextString(char szString[], FILE *hStream);
      int LoadTextureQt(const char szFileName[_MAX_PATH]);
      void  UseMaterial(const Material *pMaterial);
      void  GetTokenParameter(char szString[], const unsigned int iStrSize, FILE *hFile);
      void  MakePath(char szFileAndPath[]);
      bool LoadMaterialLib(const char szFileName[], Material *pMaterials,
		  unsigned int *iCurMaterialIndex, char szBasePath[]);
      void  RenderToDisplayList(const Face *pFaces, const unsigned int iFaceCount,
		  const Material *pMaterials);
      void GetFaceNormal(float fNormalOut[3], const Face *pFace);
      void ParseFaceString(char szFaceString[], Face *FaceOut, const Vector3D *pVertices,
		  const Vector3D *pNormals, const Vector2D *pTexCoords, const unsigned int iMaterialIndex);
      void GetFileInfo(FILE *hStream, OBJFileInfo *Stat, const char szConstBasePath[]);
      void GenTexCoords();
	  unsigned int m_iDisplayList;
};


// Callback function for comparing two faces with qsort
static int CompareFaceByMaterial(const void *Arg1, const void *Arg2);

//Returns a newly created COBJModel object. 
COBJModel* InitObject();

//Destroys a COBJModel object
void UnInitObject(COBJModel *obj);

#endif // !defined(AFX_OBJMODEL_H__32C5F722_AD3D_11D1_8F4D_E0B57CC10800__INCLUDED_)
