#include <glm/gtc/type_ptr.hpp>

#include "CShape.h"
#include "../common/typedefs.h"

CShape::CShape()
{
	_vtxCount = _vtxAttrCount = _idxCount = 0;
	_vao = 0; _vbo = 0; _ebo = 0;
	_shaderProg = 0;
	_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	_color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	_rotAngle = 0.0f;
	_rotAxis = glm::vec3(0.0f,1.0f,0.0f);
	_bRotation = _bScale = _bPos = _bTransform = _bOnTransform = false;
	_mxScale = glm::mat4(1.0f);
	_mxTrans = glm::mat4(1.0f);
	_mxRotation = glm::mat4(1.0f);
	_mxTRS = glm::mat4(1.0f);
	_mxTransform = glm::mat4(1.0f);
	_mxFinal = glm::mat4(1.0f);
	_colorLoc = _modelMxLoc = 0;
	_points = nullptr; _idx = nullptr;
	_uShadingMode = 1; // �w�]�W��Ҧ��A1 : vertex color, 2: uniform color(object color)
	_bObjColor = false; // �w�]���ϥΪ����C��
	_shadingModeLoc = 0; // �W��Ҧ����i�J�I
}

CShape::~CShape()
{

}

void CShape::setupVertexAttributes()
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

	//�k�V�q�ݩ�
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, _vtxAttrCount * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//�K�Ϯy���ݩ�
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, _vtxAttrCount * sizeof(float), BUFFER_OFFSET(9 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glBindVertexArray(0); // �Ѱ��� VAO ���j�w
}

void CShape::setShaderID(GLuint shaderID, int shadeingmode)
{
	_shaderProg = shaderID;
	_uShadingMode = shadeingmode;
	glUseProgram(_shaderProg);
	_modelMxLoc = glGetUniformLocation(_shaderProg, "mxModel"); 	// ���o mxModel �ܼƪ���m
	glUniformMatrix4fv(_modelMxLoc, 1, GL_FALSE, glm::value_ptr(_mxTRS));
	_shadingModeLoc = glGetUniformLocation(_shaderProg, "uShadingMode"); 	// ���o iColorType �ܼƪ���m
	glUniform1i(_shadingModeLoc, _uShadingMode);
}

void CShape::setColor(glm::vec4 vColor)
{
	// �z�L glUniform �ǤJ�ҫ����C��
	_color = vColor;
	_colorLoc = glGetUniformLocation(_shaderProg, "ui4Color"); 	// ���o ui4Color �ܼƪ���m
	glUniform4fv(_colorLoc, 1, glm::value_ptr(_color));
	_uShadingMode = 2; // �]�w�W��Ҧ��� uniform color
	glUniform1i(_shadingModeLoc, _uShadingMode);
	_bObjColor = true;
}

void CShape::draw() {
	//glUseProgram(_shaderProg);
	//updateMatrix();
	//glBindVertexArray(_vao);
	// �w�]���I�s��ڪ�ø�s���O�A�Ѥl���O�M�w�O�_�����I�s glDrawElements()
}

void CShape::drawRaw()
{

}

void CShape::update(float dt)
{


}

void CShape::setScale(glm::vec3 vScale)
{
	_scale = vScale;
	_bScale = true;
	_mxScale = glm::scale(glm::mat4(1.0f), _scale);
}

void CShape::setPos(glm::vec3 vPt)
{
	_pos = vPt;
	_bPos = true;
	_mxTrans = glm::translate(glm::mat4(1.0f), _pos);
}

glm::vec3 CShape::getPos() { return _pos; }
glm::mat4 CShape::getTransMatrix() { return _mxTrans; }


void CShape::setRotate(float angle, const glm::vec3& axis)
{
	float rad = glm::radians(angle); 	// glm::angleAxis �ݭn����
	_mxRotation = glm::rotate(glm::mat4(1.0f), rad, glm::normalize(axis)); // �p�����b�U���ਤ�שҹ�������x�}
	_bRotation = true;
}

void CShape::updateMatrix()
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

void CShape::setTransformMatrix(glm::mat4 mxMatrix)
{
	_bOnTransform = _bTransform = true;
	_mxTransform = mxMatrix;
}

glm::mat4 CShape::getModelMatrix() { return _mxFinal; }
GLuint CShape::getShaderProgram() { return _shaderProg; }

void CShape::reset()
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
	_uShadingMode = 1; // �w�]�W��Ҧ��A1 : vertex color, 2: uniform color(object color)
	_bObjColor = false; // �w�]���ϥΪ����C��
}


void CShape::setMaterial(const CMaterial& material) {
	_material = material;
}

void CShape::uploadMaterial()  {
	_material.uploadToShader(_shaderProg, "uMaterial");
}
