#include "Globals.h"
#include "Projection.h"
#include "Transform.h"
#include "Basicgraphic.h"
#include "Lighting.h"
#include <algorithm>
inline Matrix& CCamera::GetWorldToCameraMatrix()
{
  return m_w2camMatrix;
}
inline Matrix& CCamera::GetCameraToProjectionMatrix()
{
  return m_c2prjMatrix;
}

inline Matrix& CCamera::GetProjectionToScreenMatrix()
{
  return m_p2ScrMatrix;
}
void CCamera::Init(const POINT3D &pos,const Vector4D& dir,float fov,float nearz,
		  float farz,float viewport_width,float viewport_height )
{

	m_pos = pos;
	m_dir = dir;
	m_fov = fov;
	m_nearclipz = nearz;
	m_farclipz  = farz;
	m_viewport_width = viewport_width;
	m_viewport_height = viewport_height;
	m_aspectratio = m_viewport_width/m_viewport_height;
	m_imageplannar_width = 2;
	m_imageplannar_height = 2/m_aspectratio;
	m_imageplannar_viewdist = 1/tan(DEG2RAD *(m_fov/2));



	m_w2camMatrix.Identity();
	m_c2prjMatrix.Identity();
	m_p2ScrMatrix.Identity();

	SetWorldToCameraMatrix();
	SetCameraToProjectionMatrix();
	SetProjectionToScreenMatrix();
	
}

void CCamera::SetWorldToCameraMatrix()
{
	//set the matrix for the camera
	Matrix& mat = GetWorldToCameraMatrix();
	Matrix translatemat;
	Matrix rotmat;
	BuildTranslateMatrix(translatemat,-m_pos.x,-m_pos.y,-m_pos.z);
	Bulid_XYZ_RotationMatrix(rotmat,0.0,-m_dir.y,0.0);//y
	Bulid_XYZ_RotationMatrix(rotmat,-m_dir.x,0.0,0.0);//x
	Bulid_XYZ_RotationMatrix(rotmat,0.0,0.0,-m_dir.z);//z

	mat = translatemat * rotmat;

}
void CCamera::SetCameraToProjectionMatrix()
{
  
   m_c2prjMatrix._11 =1;
   m_c2prjMatrix._34 =1/m_imageplannar_viewdist;
   m_c2prjMatrix._44 = 0;
}

void CCamera::SetProjectionToScreenMatrix()
{
	float a = (float)(m_viewport_width-1)/2;
	float b = (float)(m_viewport_height-1)/2;
	//
	// x' = a + x*a
	// y  = b - y*b
	//Matrix m =| a 0  0 0|
	//          | 0 -b 0 0|
	//          | 0 0  1 0|
	//          | a b  0 1|
	//so the perspective matix
	Matrix &mat = GetProjectionToScreenMatrix();
	mat._11  = a;
	mat ._22 = -b;
	mat._41 = a;
	mat._42  = b;
}
////////////////////////////////////////////////////////////////////

void    CRenderObjectsManager::SetLightManager(CLightManager *pLightingManager)
{
	m_pLightingManager = pLightingManager;
}
CLightManager*  CRenderObjectsManager::GetLightManager()
{
	return m_pLightingManager;
}

void CRenderObjectsManager::Model2World()
{

	for(int i = 0; i < m_RenderList.size();++i)
	{
		CRenderObject *pObj = m_RenderList[i];
		VERTICESLIST & vertices = pObj->m_verticesList;
#ifdef _DEBUG
		int isize = pObj->m_translateverticesList.size();
#endif
		for(int j = 0; j < vertices.size();++j)
		{
            POINT3D translate(vertices[j].vertex);
			Translate(translate,pObj->world_pos.x,pObj->world_pos.y,
				      pObj->world_pos.z);
			pObj->m_translateverticesList[j].vertex = translate;
		}
	}
	

}
void CRenderObjectsManager::World2Camera()
{
      Matrix & mat = m_pCam->GetWorldToCameraMatrix();

	  for(int i = 0; i < m_RenderList.size();++i)
	  {
		  CRenderObject *pObj = m_RenderList[i];
		  VERTICESLIST & Translatedvertices = pObj->m_translateverticesList;
		  for(int j = 0; j < Translatedvertices.size();++j)
		  {
			  Translatedvertices[j].vertex *= mat;
		  }
	  }

}
void CRenderObjectsManager::Camera2Perspective()
{
	Matrix &mat = m_pCam->GetCameraToProjectionMatrix();
	for(int i = 0; i < m_RenderList.size();++i)
	{
		CRenderObject *pObj = m_RenderList[i];
		VERTICESLIST & Translatedvertices = pObj->m_translateverticesList;
		for(int j = 0; j < Translatedvertices.size();++j)
		{
			Translatedvertices[j].vertex *= mat;
		}
	}


}
void CRenderObjectsManager::Perspective2Screen()
{
	Matrix &mat = m_pCam->GetProjectionToScreenMatrix();

	for(int i = 0; i < m_RenderList.size();++i)
	{
		CRenderObject *pObj = m_RenderList[i];
		VERTICESLIST & Translatedvertices = pObj->m_translateverticesList;
		for(int j = 0; j < Translatedvertices.size();++j)
		{
			Translatedvertices[j].vertex *= mat;
		}
	}
   
	

}
class CSortByWorldPosition
{
public:
	bool operator()(const CRenderObject *pobj1,const CRenderObject* pobj2)const
	{
		return pobj1->world_pos.z > pobj2->world_pos.z;
	}
};

 void CRenderObjectsManager::Add(CRenderObject* pObject)
{
	m_RenderList.push_back(pObject);
	std::sort(m_RenderList.begin(),m_RenderList.end(),CSortByWorldPosition());
	
	
}

inline void CRenderObjectsManager::DrawSolidPolygon(const POINT3D& p1,const POINT3D& p2,
												 const POINT3D& p3,COLORREF clr)
{

 Triangle_v2(p1.x,p1.y,p1.z,p2.x,p2.y,p2.z,p3.x,p3.y,p3.z,clr);

}


//if usezbuffer = false. we use 1/z buffer
void CRenderObjectsManager:: Render(bool bLighting,bool bbackfaceremove,bool usezbuffer)
{
	//convert to world coordinate system
	Model2World();
	

	int i = 0;
	int j = 0;
	////back face removal
	{
		for( i = 0; i < m_RenderList.size();++i)
		{
			CRenderObject *pObj = m_RenderList[i];
			POLYGONLIST &polygonlist = pObj->m_PolyGonList;
			int polygonsize = polygonlist.size();
			for( j = 0; j < polygonsize;++j)
			{
				POLYGON &polygon = polygonlist[j];
				if(bbackfaceremove)
				{
					VERTICESLIST &pointlist = pObj->m_translateverticesList;
					Vector4D p0p1(pointlist[polygon.v[1]].vertex-pointlist[polygon.v[0]].vertex);
					Vector4D p0p2(pointlist[polygon.v[2]].vertex-pointlist[polygon.v[0]].vertex);
					Vector4D normal = p0p1.CrossProduct(p0p2);
					Vector4D viewvec(m_pCam->m_pos-pointlist[polygon.v[0]].vertex);
					float dp = viewvec.DotProduct(normal);
					if(dp <0)
					{
						polygon.state |= POLYGON_BACKFACEREMOVE;
					}
				}
				else
				{
					polygon.state &= ~POLYGON_BACKFACEREMOVE;
				}
				
			}
		}

	}
	
	
	//Lighting handle
	for( i = 0; i < m_RenderList.size();++i)
	{
			CRenderObject *pObj = m_RenderList[i];
			POLYGONLIST &polygonlist = pObj->m_PolyGonList;
			int polygonsize = polygonlist.size();
			for( j = 0; j < polygonsize;++j)
			{
				POLYGON &polygon = polygonlist[j];
				if(polygon.state & POLYGON_BACKFACEREMOVE) continue;
				if(bLighting)
				 polygon.state |= POLYGON_LIGHTING_PROCESSED;
				else
				{
					polygon.state &= ~POLYGON_LIGHTING_PROCESSED;
					continue;
				}
				VERTICESLIST &pointlist = pObj->m_translateverticesList;
				ProcessLighting(pObj,polygon);
			}
	}
	//convert them to the view,screen
	Matrix mat = m_pCam->GetWorldToCameraMatrix();
	mat *= m_pCam->GetCameraToProjectionMatrix();
	mat *=m_pCam->GetProjectionToScreenMatrix();
	//4d   coordinate .need to convert it to 3d
	
	for( i = 0; i < m_RenderList.size();++i)
	{
		CRenderObject *pObj = m_RenderList[i];
		VERTICESLIST & Translatedvertices = pObj->m_translateverticesList;
		for( j = 0; j < Translatedvertices.size();++j)
		{
			float z  = Translatedvertices[j].vertex.z;
			Translatedvertices[j].vertex *= mat;
			Translatedvertices[j].vertex *= 1/Translatedvertices[j].vertex.w ;
			Translatedvertices[j].vertex.z =z;
		}
	}
	//sort them


	for( i = 0; i < m_RenderList.size();++i)
	{
		CRenderObject *pObj = m_RenderList[i];
		POLYGONLIST &polygonlist = pObj->m_PolyGonList;
		int polygonsize = polygonlist.size();
		for( j = 0; j < polygonsize;++j)
		{
			POLYGON &polygon = polygonlist[j];
			if(polygon.state & POLYGON_BACKFACEREMOVE) continue;
			VERTICESLIST &pointlist = pObj->m_translateverticesList;
			




			/************************************************************************/
			/* Draw wireframe polygon                                                                    */
			/************************************************************************/ 
			/*line(pointlist[polygon.v[0]].x,pointlist[polygon.v[0]].y,
				pointlist[polygon.v[1]].x,pointlist[polygon.v[1]].y,
				RGB(0,255,0));
			line(pointlist[polygon.v[1]].x,pointlist[polygon.v[1]].y,
				pointlist[polygon.v[2]].x,pointlist[polygon.v[2]].y,
				RGB(0,255,0));
			line(pointlist[polygon.v[2]].x,pointlist[polygon.v[2]].y,
				pointlist[polygon.v[0]].x,pointlist[polygon.v[0]].y,
				RGB(0,255,0));*/
			//or
			//DrawWireFramePolygon(pointlist[polygon.v[0]],pointlist[polygon.v[1]],
		    //                pointlist[polygon.v[2]],RGB(0,0,0));
			/************************************************************************/
			/* End wireframe polygon                                                                    */
			/************************************************************************/


			/************************************************************************/
			/* Solid polygon                                                                    */
			/************************************************************************/
			//DrawSolidPolygon(pointlist[polygon.v[0]],pointlist[polygon.v[1]],
			       //       pointlist[polygon.v[2]],RGB(0,255,0));
			/************************************************************************/
			/* End Solid Polygon                                                                    */
			/************************************************************************/

			/************************************************************************/
			/* Lighting processing                                                                    */
			/************************************************************************/
			   if((polygon.state & SHADE_MODEL_CONSTANT) && !(polygon.state & OBJECT_HAS_TEXTURE))
				DrawSolidPolygon(pointlist[polygon.v[0]].vertex,
				                 pointlist[polygon.v[1]].vertex,
					             pointlist[polygon.v[2]].vertex,
						         polygon.state & POLYGON_LIGHTING_PROCESSED ?
								 polygon.ret_color.rgba:polygon.color.rgba);
			   else if( polygon.state & OBJECT_HAS_TEXTURE)
			   {
				   if(usezbuffer)
				     RenderTextureTriangle(pObj,polygon);
				   else
					 DrawTextureTriangle_invz(polygon,pObj);
			   }
			   else
			   {
				   DrawWireFramePolygon(pointlist[polygon.v[0]].vertex,pointlist[polygon.v[1]].vertex,
					   pointlist[polygon.v[2]].vertex,polygon.color.rgba);
			   }
		}
	}


}


/////////////////////////////////////////////////////////////////////////////////////////
//draw wire frame 3d triangle
inline void CRenderObjectsManager::DrawWireFramePolygon(int x1,int y1,int x2,int y2,int x3,int y3,
												 COLORREF clr)
{
	line(x1,y1,x2,y2,clr);
	line(x2,y2,x3,y3,clr);
	line(x3,y3,x1,y1,clr);
}
inline void CRenderObjectsManager::DrawWireFramePolygon(const POINT3D& p1,const POINT3D& p2,
												 const POINT3D& p3,COLORREF clr)
{
	DrawWireFramePolygon(p1.x,p1.y,p2.x,p2.y,p3.x,p3.y,clr);

}

inline void CRenderObjectsManager::RenderTextureTriangle(CRenderObject *pObject,POLYGON& poly)
{
	DrawTextureTriangle(poly,pObject);
}
void CRenderObjectsManager::ProcessLighting(CRenderObject* pobj,POLYGON& poly)
{
	VERTICESLIST &pointlist = pobj->m_translateverticesList;
	int v_index1 = poly.v[0];
	int v_index2 = poly.v[1];
	int v_index3 = poly.v[2];
	
	//Get the original r,g,b,a
	bool bHasTexture = poly.state & OBJECT_HAS_TEXTURE;
	UCHAR r_base = bHasTexture ?255:poly.color.r;
	UCHAR g_base = bHasTexture ? 255:poly.color.g;
	UCHAR b_base = bHasTexture?255: poly.color.b;
	UCHAR a_base = bHasTexture?0:poly.color.a;//to do.modify here

	int r_sum=0,g_sum = 0,b_sum = 0;

	int ilightsize = m_pLightingManager->GetLightsCount();
	const LIGHTSLIST& lightslist = m_pLightingManager->GetLights();
	CLight *light = NULL;

	//Get the normal direction
	Vector4D P0P1 = pointlist[v_index2].vertex - pointlist[v_index1].vertex;
	Vector4D P1P2 = pointlist[v_index3].vertex - pointlist[v_index1].vertex;
	Vector4D N = P0P1.CrossProduct(P1P2);
	N.Normalize();

	for(int i = 0; i < ilightsize;++i)
	{
		light = lightslist[i];
		if(!light->IsEnable()) continue;
		
		//Get the cos value of the angle between the light and normal
        Vector4D lightdir = light->m_lightdir;
		float coslight2normal = N.DotProduct(-1*lightdir);

		const CLight::LightType lighttype = light->GetLightType();

		if(lighttype == CLight::kAmbientLight)
		{
             r_sum += ((light->m_amblient.r * r_base)/256);
			 g_sum += ((light->m_amblient.g * g_base)/256);
			 b_sum += ((light->m_amblient.b * b_base)/256);
		}
		if(lighttype == CLight::kInfiniteLigtht)
		{
			if(coslight2normal>0)
			{
				r_sum += ((light->m_diffuse.r * r_base *coslight2normal)/256);
				g_sum += ((light->m_diffuse.g * g_base*coslight2normal)/256);
				b_sum += ((light->m_diffuse.b * b_base*coslight2normal)/256);
			}
		
		}
		if(lighttype == CLight::kPointLight)
		{
			// c = c_diffuse * I/(kc+kl*d+kq*d^2);
			//
			Vector4D vertex2lightpos = Vector4D(light->m_lightpos- pointlist[v_index1].vertex);
			float d = vertex2lightpos.GetLength();
			float l = vertex2lightpos.DotProduct(N);
			float cosv = N.DotProduct(vertex2lightpos);
			if(cosv >0)
			{
                cosv = cosv/d;
				float k = light->m_kconst + light->m_klinear * d + light->m_kquadratic*d*d;
				float fDiv = k*256;//k*256
				r_sum += ((light->m_diffuse.r * r_base *cosv)/fDiv);
				g_sum += ((light->m_diffuse.g * g_base*cosv)/fDiv);
				b_sum += ((light->m_diffuse.b * b_base*cosv)/fDiv);
				

			}
		
		}
		if(lighttype == CLight::kSpotLight)
		{
           //use this model
		   // c = c_diffuse * I*MAX(COSa,0)^pf/(kc+kl*d+kq*d^2)
			float l = N.DotProduct(-1*lightdir);
			if(l >0)
			{
				Vector4D vertex2lightpos = Vector4D(light->m_lightpos-pointlist[v_index1].vertex );
				float d = vertex2lightpos.GetLength();

				//calculate the cos value of light direction and vertext->lightpos

				float l = N.DotProduct(vertex2lightpos);
				if(l > 0)
				{
					float cosv = l/d;
					float dl = cosv;
					for(int i = 1; i < light->m_pf;++i)
						dl *= cosv;

					float k = light->m_kconst + light->m_klinear * d + light->m_kquadratic*d*d;
					float fDiv = k*256;//k*256

					r_sum += ((light->m_diffuse.r * r_base *dl)/fDiv);
					g_sum += ((light->m_diffuse.g * g_base*dl)/fDiv);
					b_sum += ((light->m_diffuse.b * b_base*dl)/fDiv);

				}

			}
		
		}


	}
	if(r_sum >255) r_sum = 255;
	if(g_sum > 255) g_sum = 255;
	if(b_sum > 255) b_sum = 255;
    poly.ret_color = RGBA(r_sum,g_sum,b_sum,0.0);

}