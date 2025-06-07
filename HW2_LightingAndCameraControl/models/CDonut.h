#pragma once
#include "CShape.h"

class CDonut : public CShape {
public:
    // majorRadius: �j�b�|, minorRadius: �p�b�|
    // segments: ���V���q, slices: ��I�����q, pattern: �C���� (1~4)
    CDonut(float majorRadius = 0.5f, float minorRadius = 0.2f, GLuint segments = 15, GLuint slices = 15, GLuint pattern = 1);
    virtual ~CDonut();

    virtual void draw() override;
    virtual void drawRaw() override;
    virtual void reset() override;
    virtual void update(float dt) override;

protected:
    void generateDonut(float majorRadius, float minorRadius, GLuint segments, GLuint slices, GLuint pattern);
};