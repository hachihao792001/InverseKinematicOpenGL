#pragma once
class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size, bool isStatic = true);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	void UpdateData(const void* data, unsigned int size);
};

