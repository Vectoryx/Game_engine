#pragma once

class VertexBuffer {
public:
	// the unique ID of the buffer
	unsigned int m_RendererID;

	VertexBuffer(const void* data, int size);
	~VertexBuffer();

};
