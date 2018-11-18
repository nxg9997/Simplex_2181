#include "MyOctree.h"
using namespace Simplex;
//  MyOctree

//set the initial values for the static variables
int MyOctree::m_iOctreeCount = 0;
int MyOctree::m_iMaxLevel = 0;
int MyOctree::m_iIdealEntityCount = 0;

//init funciton, initiallizes all other variables and grabs the manager singletons
void MyOctree::Init(void)
{
	m_nData = 0;

	m_iChildren = 0;
	m_fSize = 0.0f;
	m_iID = m_iOctreeCount;
	m_iLevel = 0;

	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pMyEntityMngr = MyEntityManager::GetInstance();

	m_pRoot = nullptr;
	m_pParent = nullptr;

	for (size_t i = 0; i < 8; i++)
	{
		m_pChildren[i] = nullptr;
	}

}

//creates a list of child octants
void Simplex::MyOctree::ConstructList()
{
	for (size_t i = 0; i < m_iChildren; i++)
	{
		m_pChildren[i]->ConstructList();
	}

	if (m_lEntities.size() > 0)
	{
		m_pRoot->m_lChildren.push_back(this);
	}
}

void MyOctree::Swap(MyOctree& other)
{
	std::swap(m_nData, other.m_nData);
	std::swap(m_lData, other.m_lData);
}
float Simplex::MyOctree::GetSize()
{
	return 0.0f;
}

//checks if an object is colliding with the octant
bool Simplex::MyOctree::IsColliding(uint a_iRBIndex)
{
	int objectCount = m_pMyEntityMngr->GetEntityCount();
	if (a_iRBIndex >= objectCount)
	{
		return false;
	}

	MyEntity* entity = m_pMyEntityMngr->GetEntity(a_iRBIndex);
	MyRigidBody* rb = entity->GetRigidBody();
	vector3 min = rb->GetMinGlobal();
	vector3 max = rb->GetMaxGlobal();

	if (m_v3Max.x < min.x)
	{
		return false;
	}
	if (m_v3Min.x > max.x)
	{
		return false;
	}

	if (m_v3Max.y < min.y)
	{
		return false;
	}
	if (m_v3Min.y > max.y)
	{
		return false;
	}

	if (m_v3Max.z < min.z)
	{
		return false;
	}
	if (m_v3Min.z > max.z)
	{
		return false;
	}

	return true;
}

//displays the octree with a wireframe
void Simplex::MyOctree::Display(uint a_iIndex, vector3 a_v3Color)
{
	if (m_iID == a_iIndex)
	{
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
		return;
	}

	for (size_t i = 0; i < m_iChildren; i++)
	{
		m_pChildren[i]->Display(a_v3Color);
	}
}
void Simplex::MyOctree::Display(vector3 a_v3Color)
{
	for (size_t i = 0; i < m_iChildren; i++)
	{
		m_pChildren[i]->Display(a_v3Color);
	}

	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
}

//clears the entity list of all the children of the octant
void Simplex::MyOctree::ClearEntityList()
{
	for (size_t i = 0; i < m_iChildren; i++)
	{
		m_pChildren[i]->ClearEntityList();
	}
	m_lEntities.clear();
}

//divide the octant into 8 more octants
void Simplex::MyOctree::Subdivide()
{
	if (m_iLevel >= m_iMaxLevel) return;
	if (m_iChildren != 0) return;

	m_iChildren = 8;

	float fSize = m_fSize / 4.0f;
	float newfSize = m_fSize / 2.0f;
	vector3 center;

	center = m_v3Center;
	center.x -= fSize;
	center.y -= fSize;
	center.z -= fSize;
	m_pChildren[0] = new MyOctree(center, newfSize);

	center.x += newfSize;
	m_pChildren[1] = new MyOctree(center, newfSize);

	center.z += newfSize;
	m_pChildren[2] = new MyOctree(center, newfSize);

	center.x -= newfSize;
	m_pChildren[3] = new MyOctree(center, newfSize);

	center.y += newfSize;
	m_pChildren[4] = new MyOctree(center, newfSize);

	center.z -= newfSize;
	m_pChildren[5] = new MyOctree(center, newfSize);

	center.x += newfSize;
	m_pChildren[6] = new MyOctree(center, newfSize);

	center.z += newfSize;
	m_pChildren[7] = new MyOctree(center, newfSize);

	for (size_t i = 0; i < 8; i++)
	{
		m_pChildren[i]->m_pRoot = m_pRoot;
		m_pChildren[i]->m_pParent = this;
		m_pChildren[i]->m_iLevel = m_iLevel + 1;
		m_pChildren[i]->m_iIdealEntityCount = m_iIdealEntityCount;
		if (m_pChildren[i]->ContainsMoreThan(m_iIdealEntityCount))
		{
			m_pChildren[i]->Subdivide();
		}
	}

}

//check to see if the octant has more than a given numbers of objects within it
bool Simplex::MyOctree::ContainsMoreThan(uint a_nEntities)
{
	int count = 0;
	int objectCount = m_pMyEntityMngr->GetEntityCount();
	for (size_t i = 0; i < objectCount; i++)
	{
		if (IsColliding(i))
		{
			count++;
		}
		if (count > a_nEntities)
		{
			return true;
		}
	}
	return false;
}

//delete all the octants in the octree
void Simplex::MyOctree::BurnOctree()
{
	for (size_t i = 0; i < m_iChildren; i++)
	{
		m_pChildren[i]->BurnOctree();
		delete m_pChildren[i];
		m_pChildren[i] = nullptr;
	}
	m_iChildren = 0;
}

//build the octree based on the max number of sublevels
void Simplex::MyOctree::ConstructTree(uint a_iMaxLevel)
{
	if (m_iLevel != 0) return;

	m_iMaxLevel = a_iMaxLevel;
	m_iOctreeCount = 1;
	m_lEntities.clear();
	BurnOctree();
	m_lChildren.clear();

	if (ContainsMoreThan(m_iIdealEntityCount))
	{
		Subdivide();
	}

	AssignIDToEntity();

	ConstructList();
}

//give the objects colliding with the octant the dimention id of the octant
void Simplex::MyOctree::AssignIDToEntity()
{
	for (size_t i = 0; i < m_iChildren; i++)
	{
		m_pChildren[i]->AssignIDToEntity();
	}

	if (m_iChildren == 0)
	{
		int entities = m_pMyEntityMngr->GetEntityCount();
		for (size_t i = 0; i < entities; i++)
		{
			if (IsColliding(i))
			{
				m_lEntities.push_back(i);
				m_pMyEntityMngr->AddDimension(i, m_iID);
			}
		}
	}
}

//returns the number of octants in the octree
uint Simplex::MyOctree::GetOctreeCount()
{
	return m_iOctreeCount;
}

//clean up memory, burn the tree
void MyOctree::Release(void)
{
	if (m_iLevel == 0)
	{
		BurnOctree();
	}
	m_iChildren = 0;
	m_fSize = 0.0f;
	m_lChildren.clear();
}

//The big 3
Simplex::MyOctree::MyOctree(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	Init();

	m_iOctreeCount = 0;
	m_iMaxLevel = a_nMaxLevel;
	m_iIdealEntityCount = a_nIdealEntityCount;
	m_iID = m_iOctreeCount;

	m_pRoot = this;
	m_lChildren.clear();

	std::vector<vector3> lMinMax;

	uint iObjects = m_pMyEntityMngr->GetEntityCount();
	for (size_t i = 0; i < iObjects; i++)
	{
		MyEntity* pEntity = m_pMyEntityMngr->GetEntity(i);
		MyRigidBody* pRB = pEntity->GetRigidBody();

		lMinMax.push_back(pRB->GetMinGlobal());
		lMinMax.push_back(pRB->GetMaxGlobal());
	}

	MyRigidBody* pRigidBody = new MyRigidBody(lMinMax);

	vector3 halfWidth = pRigidBody->GetHalfWidth();

	float fMax = halfWidth.x;
	for (size_t i = 0; i < 3; i++)
	{
		if (fMax < halfWidth[i])
		{
			fMax = halfWidth[i];
		}
	}

	vector3 center = pRigidBody->GetCenterLocal();
	lMinMax.clear();
	SafeDelete(pRigidBody);

	m_fSize = fMax * 2.0f;
	m_v3Center = center;
	m_v3Min = m_v3Center - (vector3(fMax));
	m_v3Max = m_v3Center + (vector3(fMax));

	m_iOctreeCount++;

	ConstructTree(m_iMaxLevel);
}
Simplex::MyOctree::MyOctree(vector3 a_v3Center, float a_fSize)
{
	Init();
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;

	m_v3Min = m_v3Center - (vector3(m_fSize) / 2.0f);
	m_v3Max = m_v3Center + (vector3(m_fSize) / 2.0f);

	m_iOctreeCount++;
}
MyOctree::MyOctree(MyOctree const& other)
{
	m_nData = other.m_nData;
	m_lData = other.m_lData;
}
MyOctree& MyOctree::operator=(MyOctree const& other)
{
	if(this != &other)
	{
		Release();
		Init();
		MyOctree temp(other);
		Swap(temp);
	}
	return *this;
}
MyOctree::~MyOctree(){Release();};
//Accessors
void MyOctree::SetData(int a_nData){ m_nData = a_nData; }
int MyOctree::GetData(void){ return m_nData; }
void MyOctree::SetDataOnVector(int a_nData){ m_lData.push_back(a_nData);}
int& MyOctree::GetDataOnVector(int a_nIndex)
{
	int nIndex = static_cast<int>(m_lData.size());
	assert(a_nIndex >= 0 && a_nIndex < nIndex);
	return m_lData[a_nIndex];
}
//--- Non Standard Singleton Methods

