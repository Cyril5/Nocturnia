#include "Camera.h"

namespace NocturniaEngine {
	namespace Render {

		Camera::Camera(glm::vec3 position)
		{
			m_Position = position;
			m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
			m_Forward = glm::vec3(0.0f, 0.0f, -1.0f);
		}


		glm::mat4 Camera::getViewMatrix() const
		{
			return glm::lookAt(m_Position,m_Position+m_Forward,m_Up);
		}

		float Camera::getFOV() const
		{
			return m_FOV;
		}

	}
}