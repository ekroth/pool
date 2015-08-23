/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Graphics/Camera.hpp"
#include "Graphics/GLManager.hpp"
#include "Events/EventManager.hpp"

#include "Input/InputManager.hpp"
#include "GameTime.hpp"

#include <glm/gtc/matrix_transform.hpp>


using namespace kke;

using glm::vec2;
using glm::vec3;
using glm::mat4;
using glm::ortho;
using glm::translate;
using glm::scale;
using glm::rotate;

Camera* Camera::instance = nullptr;

Camera& Camera::Instance()
{
	if (instance == nullptr)
	{
		instance = new Camera();
	}
	
	return *instance;
}

void Camera::Terminate()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

Camera::Camera() :
	viewportX(0), viewportY(0), viewportW(0), viewportH(0),
	perspMatrixChanged(true), viewMatrixChanged(true), fullMatrixChanged(true), autoPerspUpdate(true), autoPortUpdate(true),
	perspMatrix(mat4()), viewMatrix(mat4()), fullMatrix(mat4()),
	winResizeListen(this, &Camera::ProcessEvent, EventHasher("WindowResize"))
{
	viewStack.push(ViewValue(vec2(0, 0), vec2(1, 1), 0.0f));
}

Camera::~Camera()
{ }

void Camera::HandleInput (InputManager& inputManager, GameTime& gameTime)
{
	const float moveSpeed = 1024.f;
	vec2 deltaM(0);
	
	const float rotateSpeed = 64.0f;
	float deltaR = 0.0f;
	
	const float zoomSpeed = 10.0f;
	vec2 deltaZ(0);
	
	if (inputManager.KeyDown(GLFW_KEY_KP_4))
		deltaM += vec2(-1, 0) * moveSpeed;
	
	if (inputManager.KeyDown(GLFW_KEY_KP_6))
		deltaM += vec2(1, 0) * moveSpeed;
	
	if (inputManager.KeyDown(GLFW_KEY_KP_8))
		deltaM += vec2(0, 1) * moveSpeed;
	
	if (inputManager.KeyDown(GLFW_KEY_KP_2))
		deltaM += vec2(0, -1) * moveSpeed;
	
	if (inputManager.KeyDown(GLFW_KEY_KP_7))
		deltaR += -rotateSpeed;
	
	if (inputManager.KeyDown(GLFW_KEY_KP_9))
		deltaR += rotateSpeed;
	
	if (inputManager.KeyDown(GLFW_KEY_KP_ADD))
		deltaZ += zoomSpeed;
	
	if (inputManager.KeyDown(GLFW_KEY_KP_SUBTRACT))
		deltaZ += -zoomSpeed;
	
	Move(deltaM * vec2(gameTime.Elapsed()));
	Rotate(deltaR * gameTime.Elapsed());
	Scale(deltaZ * vec2(gameTime.Elapsed()));
}

void Camera::PushView()
{
	viewMatrixChange();
	viewStack.push(ViewValue(vec2(0, 0), vec2(1, 1), 0.0f));
}

void Camera::PopView()
{
	viewMatrixChange();
	viewStack.pop();
}

const mat4& Camera::GetPersp()
{
	updatePerspMatrix();
	return perspMatrix;
}

const float* Camera::GetPerspPtr()
{
	updatePerspMatrix();
	return &perspMatrix[0][0];
}

const mat4& Camera::GetView()
{
	updateViewMatrix();
	return viewMatrix;
}

const float* Camera::GetViewPtr()
{
	updateViewMatrix();
	return &viewMatrix[0][0];
}

const mat4& Camera::GetFull()
{
	updateFullMatrix();
	return fullMatrix;
}

const float* Camera::GetFullPtr()
{
	updateFullMatrix();
	return &fullMatrix[0][0];
}

void Camera::SetPosition (const vec2& position)
{
	viewMatrixChange();
	viewStack.top().Position = position;
}

const vec2& Camera::GetPosition() const
{
	return viewStack.top().Position;
}

void Camera::Move (const vec2& delta)
{
	viewMatrixChange();
	viewStack.top().Position += delta;
}

void Camera::SetRotation (float rotation)
{
	viewMatrixChange();
	viewStack.top().Rotation = rotation;
}

float Camera::GetRotation() const
{
	return viewStack.top().Rotation;
}

void Camera::Rotate (float delta)
{
	viewMatrixChange();
	viewStack.top().Rotation += delta;
}

void Camera::SetScale (const vec2& scale)
{
	viewMatrixChange();
	viewStack.top().Scale = scale;
}

const vec2& Camera::GetScale() const
{
	return viewStack.top().Scale;
}

void Camera::Scale (const vec2& delta)
{
	viewMatrixChange();
	viewStack.top().Scale += delta;
}

void Camera::SetupViewport()
{
// 	SetupViewport(0, 0, GLManager::Instance().GetWindowWidth(), GLManager::Instance().GetWindowHeight());
}

void Camera::SetupViewport (uint x, uint y, uint width, uint height)
{
	if (x != viewportX || y != viewportY || width != viewportW || height != viewportH)
	{
		glViewport(x, y, width, height);
		viewportX = x;
		viewportY = y;
		viewportW = width;
		viewportH = height;
	}
}

void Camera::ProcessEvent (EventData& event)
{
	if (event.GetType() == EventHasher("WindowResize"))
	{
		WindowResizeEventData& data = (WindowResizeEventData&)event;
		
		if (autoPerspUpdate)
			perspMatrixChange();
		
		if (autoPortUpdate)
			SetupViewport(0, 0, data.GetWidth(), data.GetHeight());
	}
}

void Camera::updatePerspMatrix()
{
	if (perspMatrixChanged)
	{
// 		perspMatrix = ortho<float>(0, GLManager::Instance().GetWindowWidth(), 0, GLManager::Instance().GetWindowHeight(), -1, 1);
		perspMatrixChanged = false;
	}
}

void Camera::updateViewMatrix()
{
	if (viewMatrixChanged)
	{
		updatePerspMatrix();
		{
			const ViewValue& value = viewStack.top();
// 			viewMatrix = translate(
// 							rotate(
// 								scale(mat4(),
// 								vec3(value.Scale.x, value.Scale.y, 0.0f)),
// 							value.Rotation, vec3(0.0f, 0.0f, 1.0f)), 
// 						vec3(-value.Position.x, -value.Position.y, 0.0f));
			
			viewMatrix = mat4();
// 			viewMatrix = translate(viewMatrix, vec3(GLManager::Instance().GetWindowWidth() / 2, GLManager::Instance().GetWindowHeight() / 2, 0));
			viewMatrix = scale(viewMatrix, vec3(value.Scale, 0));
			viewMatrix = rotate(viewMatrix, value.Rotation, vec3(0.0f, 0.0f, 1.0f));
// 			viewMatrix = translate(viewMatrix, -vec3(GLManager::Instance().GetWindowWidth() / 2, GLManager::Instance().GetWindowHeight() / 2, 0));
			viewMatrix = translate(viewMatrix, -vec3(value.Position, 0));
			
			
			
//  			viewMatrix = translate(viewMatrix, -vec3(GLManager::Instance().GetWindowWidth() / 2, GLManager::Instance().GetWindowHeight() / 2, 0));
		}
		viewMatrixChanged = false;
	}
}

void Camera::updateFullMatrix()
{
	if (fullMatrixChanged)
	{
		updatePerspMatrix();
		updateViewMatrix();
		fullMatrix = perspMatrix * viewMatrix;
		fullMatrixChanged = false;
	}
}

void Camera::perspMatrixChange()
{
	perspMatrixChanged = true;
	viewMatrixChange();
}

void Camera::viewMatrixChange()
{
	viewMatrixChanged = true;
	fullMatrixChanged = true;
}

// Static
void Camera::onSizeChange (void* user)
{
	Camera* camera = static_cast<Camera*>(user);
	
	if (camera->autoPerspUpdate)
		camera->perspMatrixChange();
	
	if (camera->autoPortUpdate)
		camera->SetupViewport();
}
