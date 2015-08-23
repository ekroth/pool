/* Copyright (c) 2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Graphics/VertexArray.hpp"

using namespace kke;

VertexArray::VertexArray (bool generate)
{
	if (generate)
		Generate();
}

VertexArray::~VertexArray()
{
	Delete();
}

bool VertexArray::Generate()
{
	if (handle == 0)
		glGenVertexArrays(1, &handle);
    
    return true;
}

bool VertexArray::Delete()
{
	if (handle != 0)
	{
		glDeleteVertexArrays(1, &handle);
		handle = 0;
	}
	
	return true;
}

// Non-member
void GenerateAOs(VertexArray** data, int amount)
{
	GLuint handles[amount];
	glGenVertexArrays(amount, handles);
	
	for (int i = 0; i < amount; i++)
	{
		data[i] = new VertexArray(false);
		data[i]->SetHandle(handles[i]);
	}
}

// Non-member
void DeleteAOs(VertexArray** data, int amount)
{
	for (int i = 0; i < amount; i++)
		delete data[i];
}