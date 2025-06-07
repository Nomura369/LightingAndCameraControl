// CCapsule.h
#pragma once

#include "CShape.h"

class CCapsule : public CShape {
public:
    // radius: �b�y�b�| (�w�] 0.5)
    // height: ��W���� (�w�] 1.0)
    // sectors: ��P���q (�w�] 36)
    // stacks: �b�y���q (�w�] 18)
    // pattern: �C���� (1�V4)
    CCapsule(float radius = 0.5f, float height = 1.0f, GLuint sectors = 36, GLuint stacks = 18, GLuint pattern = 1);
    virtual ~CCapsule();

    virtual void draw() override;
    virtual void drawRaw() override;
    virtual void reset() override;
    virtual void update(float dt) override;

protected:
    void generateCapsule(float radius,  float height, GLuint sectors,  GLuint stacks,  GLuint pattern);
};
