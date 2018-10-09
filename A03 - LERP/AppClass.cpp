#include "AppClass.h"
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";
	
	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUpward(AXIS_Z * 20.0f, ZERO_V3, AXIS_Y);

	//if the light position is zero move it
	if (m_pLightMngr->GetPosition(1) == ZERO_V3)
		m_pLightMngr->SetPosition(vector3(0.0f, 0.0f, 3.0f));

	//if the background is cornflowerblue change it to black (its easier to see)
	if (vector3(m_v4ClearColor) == C_BLUE_CORNFLOWER)
	{
		m_v4ClearColor = vector4(ZERO_V3, 1.0f);
	}
	
	//if there are no segments create 7
	if(m_uOrbits < 1)
		m_uOrbits = 7;

	float fSize = 1.0f; //initial size of orbits

	//creating a color using the spectrum 
	uint uColor = 650; //650 is Red
	//prevent division by 0
	float decrements = 250.0f / (m_uOrbits > 1? static_cast<float>(m_uOrbits - 1) : 1.0f); //decrement until you get to 400 (which is violet)
	/*
		This part will create the orbits, it start at 3 because that is the minimum subdivisions a torus can have
	*/
	uint uSides = 3; //start with the minimal 3 sides
	for (uint i = uSides; i < m_uOrbits + uSides; i++)
	{
		vector3 v3Color = WaveLengthToRGB(uColor); //calculate color based on wavelength
		m_shapeList.push_back(m_pMeshMngr->GenerateTorus(fSize, fSize - 0.1f, 3, i, v3Color)); //generate a custom torus and add it to the meshmanager
		fSize += 0.5f; //increment the size for the next orbit
		uColor -= static_cast<uint>(decrements); //decrease the wavelength

		//generates a path for the current orbit
		std::vector<vector3> path;
		int stops = m_shapeList[i - uSides] - 10; //each orbit has X number of vertices equal to the current shapeList value - 10
		float theta = glm::radians(360.0f / (float)stops); //rotation of each point in the orbit from the prvious point
		for (int j = 0; j < stops; j++)
		{
			vector3 point = vector3(fSize - 0.5f, 0.0f, 0.0f);
			point = point * glm::angleAxis(theta * j, AXIS_Z);
			path.push_back(point);
		}
		//push new path to the pathList & add values to other member vectors
		pathList.push_back(path);
		float t = 0.0f;
		timeList.push_back(t);
		currStopList.push_back(0);
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

	matrix4 m4View = m_pCameraMngr->GetViewMatrix(); //view Matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix(); //Projection Matrix
	matrix4 m4Offset = IDENTITY_M4; //offset of the orbits, starts as the global coordinate system
	/*
		The following offset will orient the orbits as in the demo, start without it to make your life easier.
	*/
	//m4Offset = glm::rotate(IDENTITY_M4, 1.5708f, AXIS_Z);

	// draw a shapes
	for (uint i = 0; i < m_uOrbits; ++i)
	{
		m_pMeshMngr->AddMeshToRenderList(m_shapeList[i], glm::rotate(m4Offset, 1.5708f, AXIS_X));

		//current position is determined by lerping between the previous and next point in each orbit
		vector3 v3CurrentPos;
		if (currStopList[i] == pathList[i].size() - 1)//prevent the orbit position from going out of bounds of the vector size; at the end of the orbit path lerp b/t the last and first points
		{

			v3CurrentPos = glm::lerp(pathList[i][currStopList[i]], pathList[i][0], timeList[i]);
		}
		else//lerp b/t the current and next orbit point
			v3CurrentPos = glm::lerp(pathList[i][currStopList[i]], pathList[i][currStopList[i] + 1], timeList[i]);

		matrix4 m4Model = glm::translate(m4Offset, v3CurrentPos);

		m_pMeshMngr->AddSphereToRenderList(m4Model * glm::scale(vector3(0.1)), C_WHITE);

		timeList[i] += 0.01f;//update current time step
		if (timeList[i] >= 1.0f)//if the timeStep is above 1, reset it and increase the current orbit point
		{
			timeList[i] = 0.0f;
			currStopList[i]++;
			if (currStopList[i] > pathList[i].size() - 1) //if the current orbit point is the last, reset to the beginning of the path
			{
				currStopList[i] = 0;
			}
		}
		/*m_pMeshMngr->AddMeshToRenderList(m_shapeList[i], glm::rotate(m4Offset, 1.5708f, AXIS_X));

		//calculate the current position
		vector3 v3CurrentPos = ZERO_V3;
		matrix4 m4Model = glm::translate(m4Offset, v3CurrentPos);

		//draw spheres
		m_pMeshMngr->AddSphereToRenderList(m4Model * glm::scale(vector3(0.1)), C_WHITE);*/
	}

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
	//release GUI
	ShutdownGUI();
}