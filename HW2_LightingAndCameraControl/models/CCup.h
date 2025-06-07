// CCup.h
#pragma once

#include "CShape.h"

class CCup : public CShape {
public:
    // bottomR: �M���b�| (�w�] 0.3)
    // topR:   �M�f�b�| (�w�] 0.5)
    // height: �M��     (�w�] 1.0)
    // radialSegs: ���V���q (�w�] 36)
    // heightSegs: �a�V���q (�w�] 1)
    CCup(float bottomR = 0.3f, float topR = 0.5f, float height = 1.0f,
        unsigned int radialSegs = 36, unsigned int heightSegs = 1, GLuint pattern = 1);
    virtual ~CCup();

    virtual void draw()    override;
    virtual void drawRaw() override;
    virtual void update(float dt) override;

protected:
    // �ͦ��M���ΪM��
    void generateCup(float bottomR, float topR, float height,
        unsigned int radialSegs, unsigned int heightSegs, GLuint pattern);
};
