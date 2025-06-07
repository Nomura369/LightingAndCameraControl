#include "CObjLoader.h"

CObjLoader::CObjLoader() {
    // �Y�ϨS���S�O��l�ƪF��A�٬O�n�g�@�U
}

bool CObjLoader::loadObj(const std::string& filename, std::vector<Triangle>& triangles) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "�L�k�}���ɮסG" << filename << std::endl;
        return false;
    }

    std::vector<glm::vec3> vertices; // �ȦsŪ�i�Ӫ����I��m
    std::vector<glm::vec3> normals; // �ȦsŪ�i�Ӫ����I�k�V�q
    std::vector<glm::vec3> textures; // �ȦsŪ�i�Ӫ����I�K�Ϯy��
    std::string line; // obj �ɤ����C�@����

    while (std::getline(file, line)) {
        // �ѪR�r��
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        // Ū�J���I��m�B�K�Ϯy�лP�k�V�q
        if (prefix == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            vertices.emplace_back(x, y, z);
        }
        else if (prefix == "vt") {
            float x, y, z;
            iss >> x >> y >> z;
            textures.emplace_back(x, y, z);
        }
        else if (prefix == "vn") {
            float x, y, z;
            iss >> x >> y >> z;
            normals.emplace_back(x, y, z);
        }
        // Ū�J���]�䴩�T�����^
        else if (prefix == "f") {
            // f ���榡�� v/ vt/ vn�A�T��@��
            int vIdx[3] = { -1, -1, -1 };
            int vnIdx[3] = { -1, -1, -1 };
            int vtIdx[3] = { -1, -1, -1 };

            for (int i = 0; i < 3; ++i) {
                // �ѪR�r��]���]���S���L�ĸ�ơ^
                std::string vertStr;
                iss >> vertStr;

                std::istringstream viss(vertStr);
                std::string idxStr;
                
                // Ū���I���ޡ]v�^
                if (std::getline(viss, idxStr, '/')) {
                    if (!idxStr.empty())
                        vIdx[i] = std::stoi(idxStr) - 1; // obj �ɪ����ޱq 1 �}�l
                }

                // Ū�K�Ϯy�Я��ޡ]vt�^
                if (std::getline(viss, idxStr, '/')) {
                    if (!idxStr.empty())
                        vtIdx[i] = std::stoi(idxStr) - 1;
                }

                // Ū�k�V�q���ޡ]vn�^
                if (std::getline(viss, idxStr, '/')) {
                    if (!idxStr.empty())
                        vnIdx[i] = std::stoi(idxStr) - 1;
                }
            }

            Triangle tri;
            tri.v0 = vertices[vIdx[0]];
            tri.v1 = vertices[vIdx[1]];
            tri.v2 = vertices[vIdx[2]];
            tri.vt0 = (vtIdx[0] >= 0 && vtIdx[0] < textures.size()) ? textures[vtIdx[0]] : glm::vec3(0.0f);
            tri.vt1 = (vtIdx[1] >= 0 && vtIdx[1] < textures.size()) ? textures[vtIdx[1]] : glm::vec3(0.0f);
            tri.vt2 = (vtIdx[2] >= 0 && vtIdx[2] < textures.size()) ? textures[vtIdx[2]] : glm::vec3(0.0f);
            tri.vn0 = (vnIdx[0] >= 0 && vnIdx[0] < normals.size()) ? normals[vnIdx[0]] : glm::vec3(0.0f);
            tri.vn1 = (vnIdx[1] >= 0 && vnIdx[1] < normals.size()) ? normals[vnIdx[1]] : glm::vec3(0.0f);
            tri.vn2 = (vnIdx[2] >= 0 && vnIdx[2] < normals.size()) ? normals[vnIdx[2]] : glm::vec3(0.0f);

            triangles.push_back(tri);
        }
    }

    file.close();
    return true;
}