#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexLayout &layout) {
	//	| attribute0(2)| attribute1(2)				      |
	//	| pos.x, pos.y | texture coord.x, texture coord.y |
	//	|			   |component 1 ,		 component 2  |
	//  ------------------The Entire Vertex----------------

	Renderer::BindVertexArray(*this);
	Renderer::BindVertexBuffer(vb);
	const auto & elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int attributeIndex = 0; attributeIndex < elements.size(); attributeIndex++) {
		const auto &element = elements[attributeIndex];
		GLCall(glEnableVertexAttribArray(attributeIndex));
		// | which attribute | how many components | data type of component | if must be normalized | size of the vertex in bytes | offset from the start of the vertex to the start of the attribute
		GLCall(glVertexAttribPointer(attributeIndex, element.count, element.type, element.normalized, layout.GetStride(), (const void *)(offset));
			   offset += element.count * VertexAttribute::GetSizeOfType(element.type));
	}
}