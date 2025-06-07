#include <glm/gtc/type_ptr.hpp>

#include "CSprite2D.h"
#include "initshader.h"

CSprite2D::CSprite2D()
{
	_vtxCount = _vtxAttrCount = _idxCount = 0;
	_vao = 0; _vbo = 0; _ebo = 0;
	_shaderProg = 0;
	_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	_color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	_rotAngle = 0.0f;
	_rotAxis = glm::vec3(0.0f,1.0f,0.0f);
	_mxScale = glm::mat4(1.0f);
	_mxTrans = glm::mat4(1.0f);
	_mxTransform = glm::mat4(1.0f);
	_mxRotation = glm::mat4(1.0f);
	_mxTRS = glm::mat4(1.0f);
	_mxFinal = glm::mat4(1.0f);
	_colorLoc = _modelMxLoc = 0;
	_bRotation = _bScale = _bPos = _bTransform = _bOnTransform = false;
	_coloringMode = 2;  // �w�]�W��Ҧ��A1 : vertex color, 2: uniform color(object color)
	_bObjColor = false; // �w�]���ϥΪ����C��
	_coloringModeLoc = 0; _coloringMode = 2; //�W��Ҧ����i�J�I, �W��Ҧ�
	_colorLoc = 0; // �W��Ҧ����i�J�I
}

CSprite2D::~CSprite2D()
{

}

void CSprite2D::setupVertexAttributes()
{
	// �]�w VAO�BVBO �P EBO
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(_vao);

	// �]�w VBO
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _vtxCount * _vtxAttrCount * sizeof(float), _points, GL_STATIC_DRAW);

	// �]�w EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _idxCount * sizeof(GLuint), _idx, GL_STATIC_DRAW);

	// ��m�ݩ�
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, _vtxAttrCount * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	// �C���ݩ�
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, _vtxAttrCount * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//�K�Ϯy���ݩ�
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, _vtxAttrCount * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0); // �Ѱ��� VAO ���j�w
}

void CSprite2D::setShaderID(GLuint shaderID)
{
	_shaderProg = shaderID;
	glUseProgram(_shaderProg);
	_modelMxLoc = glGetUniformLocation(_shaderProg, "mxModel"); 	// ���o mxModel �ܼƪ���m
	glUniformMatrix4fv(_modelMxLoc, 1, GL_FALSE, glm::value_ptr(_mxTRS));
	_coloringModeLoc = glGetUniformLocation(_shaderProg, "iColorType"); 	// ���o iColorType �ܼƪ���m
	glUniform1i(_coloringModeLoc, _coloringMode);
}

void CSprite2D::setColor(glm::vec4 vColor)
{
	// �z�L glUniform �ǤJ�ҫ����C��
	_color = vColor;
	_colorLoc = glGetUniformLocation(_shaderProg, "ui4Color"); 	// ���o ui4Color �ܼƪ���m
	glUniform4fv(_colorLoc, 1, glm::value_ptr(_color));
	_coloringMode = 2; // �]�w�W��Ҧ��� uniform color
	glUniform1i(_coloringModeLoc, _coloringMode);
	_bObjColor = true;
}

void CSprite2D::draw() {
	//glUseProgram(_shaderProg);
	//updateMatrix();
	//glBindVertexArray(_vao);
	// �w�]���I�s��ڪ�ø�s���O�A�Ѥl���O�M�w�O�_�����I�s glDrawElements()
}

void CSprite2D::drawRaw()
{

}

void CSprite2D::update(float dt)
{


}

void CSprite2D::setScale(glm::vec3 vScale)
{
	_scale = vScale;
	_bScale = true;
	_mxScale = glm::scale(glm::mat4(1.0f), _scale);
}

void CSprite2D::setPos(glm::vec3 vPt)
{
	_pos = vPt;
	_bPos = true;
	_mxTrans = glm::translate(glm::mat4(1.0f), _pos);
}

glm::vec3 CSprite2D::getPos() { return _pos; }
glm::mat4 CSprite2D::getTransMatrix() { return _mxTrans; }

void CSprite2D::setRotate(float angle, const glm::vec3& axis)
{
	float rad = glm::radians(angle); 	// glm::angleAxis �ݭn����
	_mxRotation = glm::rotate(glm::mat4(1.0f), rad, glm::normalize(axis)); // �p�����b�U���ਤ�שҹ�������x�}
	_bRotation = true;
}

void CSprite2D::setTransformMatrix(glm::mat4 mxMatrix)
{
	_bOnTransform = _bTransform = true;
	_mxTransform = mxMatrix;
}

void CSprite2D::updateMatrix()
{
	if (_bScale || _bPos || _bRotation )
	{
		_mxTRS = _mxTrans * _mxRotation * _mxScale;
		if (_bOnTransform == true) _mxFinal = _mxTransform * _mxTRS;
		else _mxFinal = _mxTRS;
		_bScale = _bPos = _bRotation = false;
	}
	if (_bTransform) {
		_mxFinal = _mxTransform * _mxTRS;
		_bTransform = false;
	}
	// �p�h�Ӽҫ��ϥάۦP�� shader program,�]�C�@�Ӽҫ��� mxTRS �����P�A�ҥH�C��frame���n��s
	glUniformMatrix4fv(_modelMxLoc, 1, GL_FALSE, glm::value_ptr(_mxFinal));
}

glm::mat4 CSprite2D::getModelMatrix() { return _mxFinal; }

GLuint CSprite2D::getShaderProgram() { return _shaderProg; }

void CSprite2D::reset()
{
	_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	_color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	_rotAngle = 0.0f;
	_rotAxis = glm::vec3(0.0f, 1.0f, 0.0f);

	_bRotation = _bScale = _bPos = _bTransform = _bOnTransform = false;
	_mxScale = glm::mat4(1.0f);
	_mxTrans = glm::mat4(1.0f);
	_mxTRS = glm::mat4(1.0f);
	_mxRotation = glm::mat4(1.0f);
	_mxTransform = glm::mat4(1.0f);
	_mxFinal = glm::mat4(1.0f);
	_coloringMode = 1; // �w�]�W��Ҧ��A1 : vertex color, 2: uniform color(object color)
	_bObjColor = false; // �w�]���ϥΪ����C��
}