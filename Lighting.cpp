#include "Lighting.h"

void CLight::InitLight(LightType type,RGBA c_ambient,RGBA c_diffuse,RGBA c_specluar, 
					   const POINT3D& light_pos/* =ZeroVector */,
					   const Vector4D& light_dir/* =ZeroVector */,
					   float k_const /* = 0.0 */,float k_linear /* =0.0 */,
					   float k_quadratic/* =0.0 */, float inner_angle/* =0.0 */,
					   float outer_angle/* =0.0 */, float pf/* =0.0 */)
{
	SetLigthType(type);
	m_amblient = c_ambient;
	m_diffuse = c_diffuse;
	m_specular = c_specluar;
	m_lightpos = light_pos;
	m_lightdir = light_dir;
	m_kconst   = k_const;
	m_klinear  = k_linear;
	m_kquadratic = k_quadratic;
	m_innerangle = inner_angle;
	m_outerangle = outer_angle;
	m_pf         = pf;
	
	m_lightdir.Normalize();
}