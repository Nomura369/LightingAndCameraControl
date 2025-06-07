#pragma once
#include <glm/glm.hpp>
#include <glew/include/GL/glew.h>

// �w�]���Ѫ���W��A�����ѳ��I�W��
// �ݭn���I�W�����A�A�Ѧ� CShape ���O
class CSprite2D
{
public:
	CSprite2D();
	virtual ~CSprite2D();
	virtual void draw();
	virtual void drawRaw(); //���ϥ� shader
	virtual void reset();
	virtual void update(float dt);
	void setupVertexAttributes();
	void setShaderID(GLuint shaderID);
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

protected:
	int _vtxCount, _vtxAttrCount, _idxCount; // ���I��, ���I�ݩʼ�,�I�����ޭȼ�
	GLfloat* _points;
	GLuint* _idx;
	GLuint _vao, _vbo, _ebo;
	GLuint _shaderProg;
	GLint _modelMxLoc;
	GLint _coloringModeLoc, _coloringMode; //�W��Ҧ����i�J�I, �W��Ҧ�
	GLint _colorLoc; // �W��Ҧ����i�J�I
	bool _bRotation, _bScale, _bPos, _bObjColor;
	bool _bOnTransform, _bTransform;
	glm::vec4 _color; // �ϥ� RGBA
	glm::vec3 _scale; // �ҫ����Y���
	glm::vec3 _pos;  // �ҫ�����m

	// �]�w�ҫ�����l�P�B�ʤU���Ѽ�
	GLfloat   _rotAngle; // �ҫ������ਤ��
	glm::vec3 _rotAxis; // �ҫ�������b
	glm::mat4 _mxRotation; // �ҫ��ثe������x�}
	glm::mat4 _mxScale, _mxTrans, _mxTRS; // �ҫ����Y��B�첾�P�Y�����첾����X�x�}
	glm::mat4 _mxFinal; // �̲ת��ҫ��x�}
	glm::mat4 _mxTransform; // �B�~�W�[���ഫ�x�}
};