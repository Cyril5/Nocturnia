#ifndef CAMERA_H
#define CAMERA_H

#include <glm.hpp>

#include <gtc/matrix_transform.hpp>

#include <vector>

namespace NocturniaEngine {
	namespace Render {

		class Camera
		{
			public:
				Camera(glm::vec3 position);

				glm::mat4 getViewMatrix() const;
				float getFOV() const;


			private:
				glm::vec3 m_Position;
				glm::vec3 m_Up;
				glm::vec3 m_Forward;

				float m_FOV = 45.0f;

		};
	}
}

#endif
