#pragma  once 
#include "ObjectDef.h"


class CLight
{
public:
	CLight():m_bEnableLight(false){ }
	typedef enum
	{
		kAmbientLight  = 0x0,
		kInfiniteLigtht=0x1,//infinite
		kPointLight,
		kSpotLight,
	}LightType;
	bool m_bEnableLight ;

	RGBA m_amblient,m_diffuse,m_specular;
	POINT3D m_lightpos;
	Vector4D m_lightdir;
	float m_kconst,m_klinear,m_kquadratic;
	float m_innerangle,m_outerangle,m_pf;

	const LightType GetLightType()const{ return m_LightType;}
	void      SetLigthType(LightType type){ m_LightType = type;}
	void InitLight(LightType type,RGBA c_ambient,RGBA c_diffuse,RGBA c_specluar,
		           const POINT3D& light_pos=ZeroVector,const Vector4D& light_dir=ZeroVector,
				   float k_const = 0.0,float k_linear =0.0,float k_quadratic=0.0,
				   float inner_angle=0.0,float outer_angle=0.0,
				   float pf=0.0);//the last 3 parameter is for spot light

	void EnableLight() { m_bEnableLight = true;  }
	void DisableLight(){ m_bEnableLight = false; }
	bool IsEnable() const   { return m_bEnableLight;  }

private:
	LightType m_LightType;
	

};

typedef std::vector<CLight*> LIGHTSLIST;
class CLightManager
{
public:
	void AddLight(CLight *pLight){ m_lights.push_back(pLight);}
	CLight* GetLight(int idx){ return m_lights[idx]; }
	const LIGHTSLIST& GetLights(){ return m_lights;}
	int   GetLightsCount(){ return m_lights.size();}
private:
	LIGHTSLIST m_lights;
};