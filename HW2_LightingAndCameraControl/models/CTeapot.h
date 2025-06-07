// CTeapot.h
#pragma once

#include "CShape.h"

class CTeapot : public CShape {
public:
    // resolution�G�C�� patch �b u�Bv ��V���Ӥ��q�ơA�w�] 10
    CTeapot(GLuint resolution = 5, GLuint pattern = 1);
    virtual ~CTeapot();

    virtual void draw() override;
    virtual void drawRaw() override;
    virtual void reset() override;
    virtual void update(float dt) override;

private:
    void generateTeapot(unsigned int resolution, GLuint pattern);

    // �H�U��Ӱ}�C�Ч���q freeglut �� fg_teapot_data.h �ƻs
    // Patch ���ޡ]32 patches �� 4 �� 4 �����I�^
    static const GLushort teapotPatches[32][4][4];
    // �����I�y�С]306 ���I �� 3 ���^
    static const GLfloat teapotVertices[306][3];
};
