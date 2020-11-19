#ifndef WINDOW3D_H
#define WINDOW3D_H

namespace NocturniaEngine { namespace Render {

	class Window3D
	{
		public:
			Window3D(const char* name, int width, int height);
			~Window3D(); // destructeur
			void update() const; // boucle de rendu 3d

		private:
			const char* m_Name;
			unsigned int m_Width, m_Height;
	};
}}



#endif