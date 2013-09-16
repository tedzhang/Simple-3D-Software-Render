#include "ObjectDef.h"

#include <algorithm>

const RGBA Black_Color(0.0,0.0,0.0,1.0);
const RGBA White_Color(1.0,1.0,1.0,1.0);


void CRenderObject::Transform(const Matrix &mat)
{
	for(int j = 0; j < m_verticesList.size();++j)
	{
		m_verticesList[j].vertex *= mat;
	}

}

void CRenderObject::Sort()
{
	std::sort(m_PolyGonList.begin(),m_PolyGonList.end(),CSortByAverage());
}