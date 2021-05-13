#pragma once

#include "VertexBuffer.h"
#include "VertexLayout.h"

class VertexArray {
public:
	unsigned int m_RendererID;

	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexLayout& layout);

};
