#include "CFlower.h"

//CFlower();
//virtual ~CFlower();
//virtual void draw() override;
//virtual void drawRaw() override;
//virtual void reset() override;
//virtual void update(float dt) override;
//void generateFlower();
//std::vector<Triangle> tris; // �c���Ӽҫ����Ҧ��T����

CFlower()::CFlower() {
	CObjLocator::getInstance("krokus.obj", _tris); // �|�q�u�@�ؿ����U��ŦX�� obj ��
	
}