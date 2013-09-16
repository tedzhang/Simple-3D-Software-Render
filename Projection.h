#pragma  once 
#include "Vector.h"
#include "Matrix.h"
#include <Vector>
#include "ObjectDef.h"

class CCamera
{
public:

	POINT3D  m_pos;//camera position
	Vector4D m_dir;//camera direction
	float    m_fov;//the view volum degrees
	float    m_nearclipz;//the near clip plannar
	float    m_farclipz;//the far clip plannar
	float    m_viewport_width;
	float    m_viewport_height;
	float    m_aspectratio;
	float    m_imageplannar_width;
	float    m_imageplannar_height;
	float    m_imageplannar_viewdist;
	CCamera():m_pos(0.0,0.0,0.0),m_dir(0.0,0.0,0.0){ }
    Matrix& GetWorldToCameraMatrix();
	Matrix& GetCameraToProjectionMatrix();
	Matrix& GetProjectionToScreenMatrix();
	void SetWorldToCameraMatrix();
	void SetProjectionToScreenMatrix();
	void SetCameraToProjectionMatrix();
	
	void Init(const POINT3D &pos,const Vector4D& dir,float fov,float nearz,
		      float farz,float viewport_width,float viewport_height );
private:
	
	Matrix m_w2camMatrix;//world->camera
	Matrix m_c2prjMatrix;//camera->projection
	Matrix m_p2ScrMatrix;

};


/////////////////////////////////////////////////////
//Object definition
/////////////////////////////////////////////////////

class CLightManager;
class CRenderObjectsManager
{
public:
	CRenderObjectsManager():m_pCam(NULL),m_pLightingManager(NULL){ };
	CRenderObjectsManager(CCamera *pCam):m_pCam(pCam){ }
	void    SetCamera(CCamera* pCam){m_pCam = pCam;}
	CCamera *GetCamera(){ return m_pCam;}
	void           SetLightManager(CLightManager *pLightingManager);
	CLightManager* GetLightManager();
	void Add(CRenderObject* pObject);
	int RenderObjectSize(){ return m_RenderList.size();}
	const CRenderObject* operator[](int i)const { return m_RenderList[i];}
	CRenderObject* &operator[](int i){ return m_RenderList[i];}
	void Render(bool bLighting,bool bbackfaceremove,bool usezbuffer=true);
private:
	void Model2World();
	void World2Camera();
	void Camera2Perspective();
	void Perspective2Screen();
private:
	typedef std::vector<CRenderObject*> RENDERLIST;
	RENDERLIST m_RenderList;
private:
	void DrawWireFramePolygon(int x1,int y1,int x2,int y2,int x3,int y3,
		COLORREF clr);
	void DrawWireFramePolygon(const POINT3D& p1,const POINT3D& p2,const POINT3D& p3,COLORREF clr);
	void DrawSolidPolygon(const POINT3D& p1,const POINT3D& p2,const POINT3D& p3,COLORREF clr);
	void ProcessLighting(CRenderObject* pobj,POLYGON& poly);
	void RenderTextureTriangle(CRenderObject *pObject,POLYGON& poly);

private:
	CCamera *m_pCam;
	CLightManager *m_pLightingManager;


};