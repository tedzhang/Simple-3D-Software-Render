#pragma  once 
#include "vector.h"
#include "Matrix.h"
#include <vector>
#include "Globals.h"
#include "Texture.h" //for bitmap_file struct definition
class Vertex2D 
{
public:
	int x,y;
	Vertex2D(int _x,int _y):x(_x),y(_y){ }
	Vertex2D(){ }
};
typedef Vertex2D POINT2D,VECTOR2D;

struct RGBA
{
	union
	{
		struct {
			UCHAR r;
			UCHAR g;
			UCHAR b;
			UCHAR a;
		};
		UCHAR M[4];
		int rgba;//int value
	};
	RGBA(UCHAR _r,UCHAR _g,UCHAR _b,UCHAR _a=0.0):r(_r),g(_g),b(_b),a(_a){}
	RGBA(){ }
	RGBA(COLORREF clr):rgba(clr)
	{
	}
};

extern const RGBA Black_Color;
extern const RGBA White_Color;


typedef struct  tagVertex
{
	POINT3D vertex;
	Vector4D normal;
	Vertex2D texture;
	tagVertex(POINT3D pt,Vector4D nor=ZeroVector,Vertex2D tex=POINT2D(0,0))
		:vertex(pt),normal(nor),texture(tex)
	{
	}
	tagVertex(){ }

}Vertex;

typedef std::vector<Vertex> VERTICESLIST;
typedef std::vector<POINT2D> POINT2DLIST;//as texture

#define SHADE_MODEL_CONSTANT 0X1
#define SHADE_MODEL_GOURAUD  0x2
#define OBJECT_HAS_TEXTURE   0x4
#define POLYGON_BACKFACEREMOVE 0x8
#define POLYGON_LIGHTING_PROCESSED 0x10

typedef struct tagPolygon
{
	int state;
	int  v[3];
	VERTICESLIST* pointlist;
	RGBA color;
	RGBA ret_color;//result color.after lighting processing
	int lit_color[3];//3 vertex color through lighting
	BITMAP_FILE_PTR texture;
	POINT2DLIST* texturecoordinatelist;
	int text[3];
	tagPolygon():state(0){}
	float GetAveragez()const
	{ 
		return (float)(pointlist->at(v[0]).vertex.z +pointlist->at(v[1]).vertex.z +pointlist->at(v[2]).vertex.z)/3;
	}

}POLYGON;

typedef std::vector<POLYGON> POLYGONLIST;

class CSortByAverage
{
public:
     bool operator () (const POLYGON& lhs,const POLYGON& rhs) const
	{
		float av1 = lhs.GetAveragez();
		float  av2 = rhs.GetAveragez();
		return av1 > av2;//we need to sort them in descending order

	}
};

class  CRenderObject 
{
public:
	int state ;
	POLYGONLIST m_PolyGonList;
	VERTICESLIST m_verticesList; 
	VERTICESLIST m_translateverticesList;
	POINT2DLIST  m_texturecoordinatesList;
	POINT3D world_pos;
	//local transform
	void Transform(const Matrix &mat);
	void Sort();


};