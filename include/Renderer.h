#pragma once

#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"

class Renderer {
public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& sh);

	// select / bind specific buffers
	static void BindIndexBuffer(const IndexBuffer& IBuffer);
	static void BindVertexBuffer(const VertexBuffer& VBuffer);
	static void BindVertexArray(const VertexArray& VArray);
	static void BindTexture(const Texture& Texture2D, unsigned int slot);
	static void BindShaderProgram(const Shader& ShaderProgram);

	// deselect / unbind specific buffers 
	static void UnBindIndexBuffer();
	static void UnBindVertexBuffer();
	static void UnBindVertexArray();
	static void UnBindTexture();
	static void UnBindShaderProgram();
};