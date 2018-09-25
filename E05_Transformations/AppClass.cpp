#include "AppClass.h"
void Application::InitVariables(void)
{
	////Make MyMesh object
	//m_pMesh = new MyMesh();
	//m_pMesh->GenerateCube(2.0f, C_BROWN);

	////Make MyMesh object
	//m_pMesh1 = new MyMesh();
	//m_pMesh1->GenerateCube(1.0f, C_WHITE);
	for (size_t i = 0; i < 46; i++)
	{
		MyMesh* newMesh = new MyMesh();
		newMesh->GenerateCube(1.0f, C_BLACK);
		m_vMeshes.push_back(newMesh);
	}
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	/*
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), ToMatrix4(m_qArcBall));
	m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3( 3.0f, 0.0f, 0.0f)));*/
	static float time = -22.0f;
	time += 0.02f;
	if (time > 22.0f)
	{
		time = -22.0f;
	}
	for (size_t i = 0; i < 46; i++)
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		if (i < 3)
		{
			x = -5;
			y = -(float)i;
		}
		if (i >= 3 && i < 5)
		{
			x = -4;
			y = (i-3);
		}
		if (i >= 5 && i < 11)
		{
			x = -3;
			y = ((float)i - 7);
			if ((float)i - 7 == 3)
			{
				y = 4;
			}
		}
		if (i >= 11 && i < 16)
		{
			x = -2;
			y = ((float)i - 12);
			if (y < 2)
			{
				y--;
			}
			if (y < -1)
			{
				y--;
			}
		}
		if (i >= 16 && i < 21)
		{
			x = -1;
			y = ((float)i - 18);
			if (y < -1)
			{
				y--;
			}
		}
		if (i >= 21 && i < 25)
		{
			y = ((float)i - 22);
		}
		if (i >= 25 && i < 30)
		{
			x = 1;
			y = ((float)i - 27);
			if (y < -1)
			{
				y--;
			}
		}
		if (i >= 30 && i < 35)
		{
			x = 2;
			y = ((float)i - 31);
			if (y < 2)
			{
				y--;
			}
			if (y < -1)
			{
				y--;
			}
		}
		if (i >= 35 && i < 41)
		{
			x = 3;
			y = ((float)i - 37);
			if ((float)i - 37 == 3)
			{
				y = 4;
			}
		}
		if (i >= 41 && i < 43)
		{
			x = 4;
			y = (((float)i - 38) - 3);
		}
		if(i >= 43)
		{
			x = 5;
			y = -(float)i + 43;
		}

		vector3 translation = vector3(x + time,y,z);
		m_vMeshes[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(translation));
	}
		
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	SafeDelete(m_pMesh1);
	for (size_t i = 0; i < 46; i++)
	{
		SafeDelete(m_vMeshes[i]);
	}
	//release GUI
	ShutdownGUI();
}