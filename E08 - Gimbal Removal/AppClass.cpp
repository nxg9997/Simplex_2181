#include "AppClass.h"
void Application::InitVariables(void)
{
	//init the mesh
	m_pMesh = new MyMesh();
	//m_pMesh->GenerateCube(1.0f, C_WHITE);
	m_pMesh->GenerateCone(2.0f, 5.0f, 3, C_WHITE);
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

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();

	

	/*m_m4Model = glm::rotate(IDENTITY_M4, glm::radians(m_v3Rotation.x), vector3(1.0f, 0.0f, 0.0f));
	m_m4Model = glm::rotate(m_m4Model, glm::radians(m_v3Rotation.y), vector3(0.0f, 1.0f, 0.0f));
	m_m4Model = glm::rotate(m_m4Model, glm::radians(m_v3Rotation.z), vector3(0.0f, 0.0f, 1.0f));*/

	quaternion q1 = glm::angleAxis(glm::radians(m_v3Rotation.x), AXIS_X);
	quaternion q2 = glm::angleAxis(glm::radians(m_v3Rotation.y), AXIS_Y);
	quaternion q3 = glm::angleAxis(glm::radians(m_v3Rotation.z), AXIS_Z);

	/*static quaternion q4;
	m_m4Model = m_m4Model * ToMatrix4(q3 * q2 * q1);*/

	/*std::cout << m_v3Rotation.x << " " << m_v3Rotation.y << " " << m_v3Rotation.z << std::endl;

	vector3 axis = vector3(0.0f);

	if (m_bX) axis.x = 1.0f;
	if (m_bY) axis.y = 1.0f;
	if (m_bZ) axis.z = 1.0f;*/

	//m_qOrientation = m_qOrientation * glm::angleAxis(glm::radians(1.0f), axis);

	m_pMesh->Render(m4Projection, m4View, ToMatrix4(m_qOrientation));


	//m_qOrientation = m_qOrientation * glm::angleAxis(glm::radians(1.0f), vector3(1.0f));
	//m_pMesh->Render(m4Projection, m4View, ToMatrix4(m_qOrientation));
	
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
	SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}