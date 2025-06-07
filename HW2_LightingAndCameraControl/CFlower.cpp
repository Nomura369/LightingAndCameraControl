#include "CFlower.h"

//CFlower();
//virtual ~CFlower();
//virtual void draw() override;
//virtual void drawRaw() override;
//virtual void reset() override;
//virtual void update(float dt) override;
//void generateFlower();
//std::vector<Triangle> tris; // 構成該模型的所有三角形

CFlower()::CFlower() {
	CObjLocator::getInstance("krokus.obj", _tris); // 會從工作目錄底下找符合的 obj 檔
	
}