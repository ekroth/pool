/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_CAMERA_HPP
#define KKE_CAMERA_HPP

#include "Interface/IInputable.hpp"
#include "Interface/INoncopyable.hpp"
#include "Events/ScopedListener.hpp"

#include <glm/glm.hpp>
#include <stack>

// TODO: Shouldn't be Singleton..

namespace kke
{
    class InputManager;
    class GameTime;
    
	/**
	 * @brief Manages the matrices and viewport.
	 **/
	class Camera : public INoncopyable
	{
	public:
		static Camera& Instance();
		static void Terminate();
		
	public:
		void HandleInput(InputManager& inputManager, GameTime& gameTime);
		
		/**
		 * @brief Perspective matrix.
		 *
		 * @return :core::type::mat4&
		 **/
		const glm::mat4& GetPersp();
		
		/**
		 * @brief Perspective matrix float pointer.
		 *
		  * @return const float*
		 **/
		const float* GetPerspPtr();
		
		/**
		 * @brief View matrix.
		 *
		 * @return :core::type::mat4&
		 **/
		const glm::mat4& GetView();
		
		/**
		 * @brief View matrix float pointer.
		 *
		 * @return const float*
		 **/
		const float* GetViewPtr();
		
		/**
		 * @brief Fully translated matrix (perspective * view).
		 *
		 * @return :core::type::mat4&
		 **/
		const glm::mat4& GetFull();
		
		/**
		 * @brief Full matrix float pointer.
		 *
		 * @return const float*
		 **/
		const float* GetFullPtr();
		
		/**
		 * @brief Pop view value stack.
		 *
		 * @return void
		 **/
		void PopView();
		
		/**
		 * @brief Push view value stack.
		 *
		 * @return void
		 **/
		void PushView();
		
		void SetPosition(const glm::vec2& position);
		const glm::vec2& GetPosition() const;
		void Move(const glm::vec2& delta);
		
		void SetRotation(float rotation);
		float GetRotation() const;
		void Rotate(float delta);
		
		void SetScale(const glm::vec2& scale);
		const glm::vec2& GetScale() const;
		void Scale(const glm::vec2& delta);

		
// 		void SetupPersp();
// 		void SetupPersp(uint width, uint height);
		/**
		 * @brief Setup viewport with window values.
		 *
		 * @return void
		 **/
		void SetupViewport();
		
		/**
		 * @brief Setup viewport with custom values.
		 * Note that viewport will change automatically when window resizes,
		 * if @autoPortChange is enabled (true by default).
		 * @return void
		 **/
		void SetupViewport(uint x, uint y, uint width, uint height);
		
		void ProcessEvent (EventData& event);
		
	private:
		static Camera* instance;
		static void onSizeChange(void* user);
		
	private:
		Camera();
		~Camera();
		
		struct ViewValue
		{
			ViewValue(const glm::vec2& pos, const glm::vec2& scale, float rotation) : 
				Position(pos), Scale(scale), Rotation(rotation)
			{ }
			glm::vec2 Position;
			glm::vec2 Scale;
			float Rotation;
		};
		
		void updatePerspMatrix();
		void perspMatrixChange();
		
		void updateViewMatrix();
		void viewMatrixChange();
		
		void updateFullMatrix();
		
		uint viewportX, viewportY, viewportW, viewportH;
		bool perspMatrixChanged, viewMatrixChanged, fullMatrixChanged, autoPerspUpdate, autoPortUpdate;
		glm::mat4 perspMatrix, viewMatrix, fullMatrix;
		std::stack<ViewValue> viewStack;
        ScopedEMListener winResizeListen;
	};
}

#endif // KKE_CAMERA_HPP