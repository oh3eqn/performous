#pragma once


#include "color.hh"
#include "glmath.hh"
#include <epoxy/gl.h>
#include <string>
#include <iostream>
#include <vector>

namespace glutil {

	// Note: if you reorder or otherwise change the contents of this, VertexArray::Draw() must be modified accordingly
	struct VertexInfo {
		glmath::vec3 position{};
		glmath::vec2 texCoord{};
		glmath::vec3 normal{};
		glmath::vec4 color{ 1.0, 1.0, 1.0, 1.0 };
	};

	/// Handy vertex array capable of drawing itself
	class VertexArray {
	private:
		std::vector<VertexInfo> m_vertices;
		VertexInfo m_vert;
		GLuint m_vbo = 0, m_vao = 0;
	public:
		VertexArray();
		~VertexArray();

		VertexArray& vertex(float x, float y, float z = 0.0f) {
			return vertex(glmath::vec3(x, y, z));
		}

		VertexArray& vertex(glmath::vec3 const& v) {
			m_vert.position = v;
			m_vertices.push_back(m_vert);
			m_vert = VertexInfo();
			return *this;
		}

		VertexArray& normal(float x, float y, float z) {
			return normal(glmath::vec3(x, y, z));
		}

		VertexArray& normal(glmath::vec3 const& v) {
			m_vert.normal = v;
			return *this;
		}

		VertexArray& texCoord(float s, float t) {
			return texCoord(glmath::vec2(s, t));
		}

		VertexArray& texCoord(glmath::vec2 const& v) {
			m_vert.texCoord = v;
			return *this;
		}

		VertexArray& color(glmath::vec4 const& v) {
			m_vert.color = v;
			return *this;
		}

		void draw(GLint mode = GL_TRIANGLE_STRIP);

		bool empty() const {
			return m_vertices.empty();
		}

		unsigned size() const {
			return m_vertices.size();
		}

		void clear();
	};

	/// Wrapper struct for RAII
	struct UseDepthTest {
		/// enable depth test (for 3d objects)
		UseDepthTest() {
			glClear(GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
		}
		~UseDepthTest() {
			glDisable(GL_DEPTH_TEST);
		}
	};

	/// Checks for OpenGL error and displays it with given location info
	class GLErrorChecker {
		static thread_local std::vector<std::string> stack;
		std::string info;
		void setWhat(std::string what);
	public:
		GLErrorChecker(std::string const& info);
		~GLErrorChecker();
		void check(std::string const& what = "check()");  ///< An error-check milestone; will log and clear any active GL errors
		static void reset() { glGetError(); }  ///< Ignore any existing error
		static std::string msg(GLenum err);
	};
}


