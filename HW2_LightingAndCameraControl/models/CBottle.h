#pragma once
#include "CShape.h"
#include <vector>

class CBottle : public CShape {
public:
    // radialSegs: ���V���q��
    // heightSegs: ���פ�V���q��
    // pattern: �w�] 1�A���� 1~4 ���C����
    CBottle(unsigned int radialSegs = 36, unsigned int heightSegs = 8, GLuint pattern = 1);
    virtual ~CBottle();

    virtual void draw() override;
    virtual void drawRaw() override;
    virtual void update(float dt) override;

protected:
    // ���Ͳ~���P�����A�ҫ��w�]��V�� Z �b�¤W
    void generateBottle(unsigned int radialSegs, unsigned int heightSegs, GLuint pattern);
    std::vector<glm::vec2> bottleProfile = {
        {0.50f, 0.00f}, {0.48f, 0.05f}, {0.45f, 0.15f}, {0.40f, 0.40f},
        {0.35f, 0.80f}, {0.38f, 1.20f}, {0.45f, 1.60f}, {0.50f, 2.00f}};
};