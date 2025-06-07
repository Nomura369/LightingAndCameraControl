#pragma once
#include <glm/glm.hpp>
#include <glew/include/GL/glew.h>
#include "../common/CMaterial.h"

class CShape
{
public:
	CShape();
	virtual ~CShape();
	virtual void draw();
	virtual void drawRaw(); //���ϥ� shader
	virtual void reset();
	virtual void update(float dt);
	void setupVertexAttributes();
	void setShaderID(GLuint shaderID, int shadeingmode = 1); // �w�]�ϥζǤJ�� vertex color
	void setColor(glm::vec4 vColor); // �]�w�ҫ����C��
	void setScale(glm::vec3 vScale); // �]�w�ҫ����Y���
	void setPos(glm::vec3 vPt); // �]�w�ҫ�����m
	void setRotate(float angle, const glm::vec3& axis); // �]�w�ҫ������ਤ�׻P����b
	void setTransformMatrix(glm::mat4 mxMatrix);
	void updateMatrix();
	glm::vec3 getPos(); // ���o�ҫ�����m
	glm::mat4 getModelMatrix();
	glm::mat4 getTransMatrix();
	GLuint getShaderProgram();

	// ����޲z
	void setMaterial(const CMaterial& material);
	void uploadMaterial();

protected:
	int _vtxCount, _vtxAttrCount, _idxCount; // ���I��, ���I�ݩʼ�,�I�����ޭȼ�
	GLfloat* _points;
	GLuint* _idx;
	GLuint _vao, _vbo, _ebo;
	GLuint _shaderProg;
	GLint _modelMxLoc;
	GLint _shadingModeLoc, _uShadingMode; //�W��Ҧ����i�J�I, �W��Ҧ�
	GLint _colorLoc; // �W��Ҧ����i�J�I
	bool _bRotation, _bScale, _bPos, _bObjColor;
	bool _bTransform, _bOnTransform;	
	// _bTransform : true �N���]�w�s���ഫ�x�}
	// _bOnTransform : true �N���g�]�w�L�ഫ�x�}�A�Ω�P�_�O�_�ݭn��s model matrix
	glm::vec4 _color; // �ϥ� RGBA
	glm::vec3 _scale; // �ҫ����Y���
	glm::vec3 _pos;  // �ҫ�����m

	// �]�w�ҫ�����l�P�B�ʤU���Ѽ�
	GLfloat   _rotAngle; // �ҫ������ਤ��
	glm::vec3 _rotAxis; // �ҫ�������b
	glm::mat4 _mxRotation; // �ҫ��ثe������x�}
	glm::mat4 _mxScale, _mxTrans, _mxTRS; // �ҫ����Y��B�첾�P�Y�����첾����X�x�}
	glm::mat4 _mxTransform, _mxFinal; // �B�~�W�[���ഫ�x�}�P�̲ת��ҫ��x�}

	// ����
	CMaterial _material;
};