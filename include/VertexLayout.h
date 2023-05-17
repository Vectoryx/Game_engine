#pragma once

#include "Debugging.h"
#include <GL/glew.h>
#include <vector>

struct VertexAttribute {

	unsigned int  type;
	unsigned int  count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type) {

		switch (type) {

		case GL_FLOAT:
			return 4;
		case GL_UNSIGNED_INT:
			return 4;
		case GL_UNSIGNED_BYTE:
			return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexLayout {
private:
	std::vector<VertexAttribute> m_Elements;
	unsigned int				 m_Stride;

public:
	VertexLayout()
		: m_Stride(0) {}

	void Push(unsigned int count, unsigned int size_type) {
		m_Elements.push_back({size_type, count, GL_FALSE});
		m_Stride += count * VertexAttribute::GetSizeOfType(size_type);
		//static_assert(false);
	}
	/*
	template<>
	void Push<float>(unsigned int count) {
		m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
		m_Stride += count * VertexAttribute::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count) {
		m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
		m_Stride += count * VertexAttribute::GetSizeOfType(GL_UNSIGNED_INT);

	}

	template<>
	void Push<unsigned char>(unsigned int count) {
		m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
		m_Stride += count * VertexAttribute::GetSizeOfType(GL_UNSIGNED_BYTE);
	}
*/
	inline const std::vector<VertexAttribute> GetElements() const {
		return m_Elements;
	}

	inline unsigned int GetStride() const {
		return m_Stride;
	}
};