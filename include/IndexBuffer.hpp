#pragma once

class IndexBuffer {
public:
	// the unique ID of the buffer
	unsigned int m_RendererID;
	unsigned int m_Count;
	IndexBuffer(const unsigned int* data, int count);
	~IndexBuffer();

	inline unsigned int GetCount() const {
		return m_Count;
	}
};
