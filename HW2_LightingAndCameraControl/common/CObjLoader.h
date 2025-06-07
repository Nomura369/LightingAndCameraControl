#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>

struct Triangle { // �N��c���ҫ����C�ӤT����
	glm::vec3 v0, v1, v2; // �T���Ϊ��T�ӳ��I
	glm::vec3 vn0, vn1, vn2; // �T�ӳ��I���k�V�q
	glm::vec3 vt0, vt1, vt2; // �T�ӳ��I���K�Ϯy��
};

class CObjLoader // Singleton 
{
public:
    static CObjLoader& getInstance() {
        static CObjLoader instance;
        return instance;
    }

	bool loadObj(const std::string& filename, std::vector<Triangle>& triangles);

private:
    CObjLoader();
};
