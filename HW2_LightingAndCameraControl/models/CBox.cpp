#include <glew/include/GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "CBox.h"

CBox::CBox() : CShape() 
{
	_vtxCount = 24; _vtxAttrCount = 11; _idxCount = 36;

	_points = new GLfloat[_vtxCount * _vtxAttrCount]{
		// Front face (�X�M��, z = +0.5, �k�V�q (0,0,1))
		-0.5f, -0.5f,  0.5f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,  // 0
		 0.5f, -0.5f,  0.5f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,  // 1
		 0.5f,  0.5f,  0.5f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,  // 2
		-0.5f,  0.5f,  0.5f,  1.0f, 0.6f, 0.6f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,  // 3

		// Back face (�X�M��, z = -0.5, �k�V�q (0,0,-1))
		 0.5f, -0.5f, -0.5f,  0.6f, 1.0f, 0.6f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,  // 4
		-0.5f, -0.5f, -0.5f,  0.6f, 1.0f, 0.6f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // 5
		-0.5f,  0.5f, -0.5f,  0.6f, 1.0f, 0.6f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,  // 6
		 0.5f,  0.5f, -0.5f,  0.6f, 1.0f, 0.6f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  // 7

		 // Left face (�X�M��, x = -0.5, �k�V�q (-1,0,0))
		-0.5f, -0.5f, -0.5f,  0.6f, 0.6f, 1.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // 8
		-0.5f, -0.5f,  0.5f,  0.6f, 0.6f, 1.0f, 1.0f, 0.0f, 0.0f,  1.0f, 0.0f,  // 9
		-0.5f,  0.5f,  0.5f,  0.6f, 0.6f, 1.0f, 1.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // 10
		-0.5f,  0.5f, -0.5f,  0.6f, 0.6f, 1.0f, 1.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // 11

		// Right face (�X�M��, x = 0.5, �k�V�q (1,0,0))
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.6f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // 12
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.6f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,  // 13
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.6f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // 14
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.6f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // 15

		 // Top face (�X�M�~��, y = 0.5, �k�V�q (0,1,0))
		-0.5f,  0.5f,  0.5f,  1.0f, 0.6f, 1.0f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,  // 16
		 0.5f,  0.5f,  0.5f,  1.0f, 0.6f, 1.0f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f,  // 17
		 0.5f,  0.5f, -0.5f,  1.0f, 0.6f, 1.0f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f,  // 18
		-0.5f,  0.5f, -0.5f,  1.0f, 0.6f, 1.0f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f,  // 19

		// Bottom face (�X�M�C, y = -0.5, �k�V�q (0,-1,0))
		-0.5f, -0.5f, -0.5f,  0.6f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,  // 20
		 0.5f, -0.5f, -0.5f,  0.6f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  // 21
		 0.5f, -0.5f,  0.5f,  0.6f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,  // 22
		-0.5f, -0.5f,  0.5f,  0.6f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f   // 23
	};
	
	_idx = new GLuint[_idxCount]{ 
		// Front face
		0, 1, 2, 2, 3, 0,
		// Back face
		4, 5, 6, 6, 7, 4,
		// Left face
		8, 9, 10, 10, 11, 8,
		// Right face
		12, 13, 14, 14, 15, 12,
		// Top face
		16, 17, 18, 18, 19, 16,
		// Bottom face
		20, 21, 22, 22, 23, 20
	};

}

CBox::~CBox()
{
	glDeleteBuffers(1, &_vbo);  //������ VBO �P EBO
	glDeleteBuffers(1, &_ebo);
	glDeleteVertexArrays(1, &_vao); //�A���� VAO
	if (_points != NULL) delete[] _points;
	if (_idx != NULL) delete[] _idx;
}

void CBox::draw()
{
	glUseProgram(_shaderProg);
	updateMatrix();
	glBindVertexArray(_vao);
	glUniform1i(_shadingModeLoc, _uShadingMode);
	if ( _bObjColor ) glUniform4fv(_colorLoc, 1, glm::value_ptr(_color));
	glDrawElements(GL_TRIANGLES, _idxCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void CBox::drawRaw()
{
	updateMatrix();
	glBindVertexArray(_vao);
	glUniform1i(_shadingModeLoc, _uShadingMode);
	if (_bObjColor) glUniform4fv(_colorLoc, 1, glm::value_ptr(_color));
	glDrawElements(GL_TRIANGLES, _idxCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void CBox::update(float dt)
{

}

void CBox::reset() {
	CShape::reset();
	// �p���ݭn�A�i�[�J��L�S�w��|��Ϊ����]�ʧ@
}

