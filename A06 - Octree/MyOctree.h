/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2017/06
----------------------------------------------*/
#ifndef __MYOCTREE_H_
#define __MYOCTREE_H_

#include "Simplex\Simplex.h"
#include "MyEntityManager.h"

namespace Simplex
{

//System Class
class MyOctree
{
	int m_nData = 0; //Number of elements in the list of elements
	std::vector<int> m_lData; //list of elements

	static int m_iOctreeCount;
	static int m_iMaxLevel;
	static int m_iIdealEntityCount;

	int m_iID;
	int m_iLevel;
	int m_iChildren;

	float m_fSize = 0.0f;

	MeshManager* m_pMeshMngr = nullptr;
	MyEntityManager* m_pMyEntityMngr = nullptr;

	vector3 m_v3Center = vector3(0.0f);
	vector3 m_v3Min = vector3(0.0f);
	vector3 m_v3Max = vector3(0.0f);

	MyOctree* m_pRoot = nullptr;
	MyOctree* m_pParent = nullptr;
	MyOctree* m_pChildren[8];
	std::vector<MyOctree*> m_lChildren;
	std::vector<int> m_lEntities;

public:
	/*
	Usage: Constructor
	Arguments: ---
	Output: class object instance
	*/
	MyOctree(uint a_nMaxLevel, uint a_nIdealEntityCount);
	/*
	Usage: Constructor
	Arguments: ---
	Output: class object instance
	*/
	MyOctree(vector3 a_v3Center, float a_fSize);
	/*
	Usage: Copy Constructor
	Arguments: class object to copy
	Output: class object instance
	*/
	MyOctree(MyOctree const& other);
	/*
	Usage: Copy Assignment Operator
	Arguments: class object to copy
	Output: ---
	*/
	MyOctree& operator=(MyOctree const& other);
	/*
	Usage: Destructor
	Arguments: ---
	Output: ---
	*/
	~MyOctree(void);

	/*
	Usage: Changes object contents for other object's
	Arguments: other -> object to swap content from
	Output: ---
	*/
	void Swap(MyOctree& other);

	float GetSize();

	bool IsColliding(uint a_iRBIndex);

	void Display(uint a_iIndex, vector3 a_v3Color = C_YELLOW);

	void Display(vector3 a_v3Color);

	void ClearEntityList();

	void Subdivide();

	bool ContainsMoreThan(uint a_nEntities);

	void BurnOctree();

	void ConstructTree(uint a_iMaxLevel);

	void AssignIDToEntity();

	uint GetOctreeCount();

	/*
	Usage: Gets data member
	Arguments: ---
	Output: data
	*/
	int GetData(void);
	/*
	Usage: Sets data member
	Arguments: int a_nData = 1 -> data to set
	Output: ---
	*/
	void SetData(int a_nData = 1);
	/* Property */
	//__declspec(property(get = GetData, put = SetData)) int Data;

	/*
	Usage: adds data on vector
	Arguments: int a_nData -> data to add
	Output: ---
	*/
	void SetDataOnVector(int a_nData);
	/*
	Usage: gets data on vector at the specified entry
	Arguments: int a_nIndex -> entry index 
	Output: entry's content
	*/
	int& GetDataOnVector(int a_nIndex);

private:
	/*
	Usage: Deallocates member fields
	Arguments: ---
	Output: ---
	*/
	void Release(void);
	/*
	Usage: Allocates member fields
	Arguments: ---
	Output: ---
	*/
	void Init(void);

	void ConstructList();
};//class

} //namespace Simplex

#endif //__MyOctree_H__

/*
USAGE:
ARGUMENTS: ---
OUTPUT: ---
*/