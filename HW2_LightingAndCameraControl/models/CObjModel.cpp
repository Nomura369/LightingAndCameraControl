#include "CObjModel.h"

#include <glm/gtc/type_ptr.hpp>

//virtual void draw() override;
//virtual void drawRaw() override;
//virtual void reset() override;
//virtual void update(float dt) override;

CObjModel::CObjModel() {
    generateCObjModel();
}

CObjModel::~CObjModel() {
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
    glDeleteVertexArrays(1, &_vao);
    if (_points) delete[] _points;
    if (_idx)    delete[] _idx;
}

void CObjModel::draw() {
    glUseProgram(_shaderProg);
    updateMatrix();
    glBindVertexArray(_vao);
    glUniform1i(_shadingModeLoc, _uShadingMode);
    if (_bObjColor) glUniform4fv(_colorLoc, 1, glm::value_ptr(_color));
    glDrawElements(GL_TRIANGLES, _idxCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void CObjModel::drawRaw() {
    updateMatrix();
    glBindVertexArray(_vao);
    glUniform1i(_shadingModeLoc, _uShadingMode);
    if (_bObjColor) glUniform4fv(_colorLoc, 1, glm::value_ptr(_color));
    glDrawElements(GL_TRIANGLES, _idxCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void CObjModel::reset() {
    CShape::reset();
}

void CObjModel::update(float dt) {
    // �i���ʵe�Φ���
}

void CObjModel::generateCObjModel() {
    // �b�u�@�ؿ����U�M���ɮ�
    CObjLoader::getInstance().loadObj("chair.obj", _tris); 

    // �Q�� obj �ɪ����e�إ߼ҫ����
    _vtxCount = _tris.size() * 3;
    _vtxAttrCount = 11; 
    _idxCount = _tris.size() * 3;

    _points = new GLfloat[_vtxCount * _vtxAttrCount];
    _idx = new GLuint[_idxCount];

    int i, j, p;  i = j = p = 0;
    for (const Triangle& tri : _tris) {
        glm::vec3 pos[3] = { tri.v0, tri.v1, tri.v2 };
        glm::vec3 nor[3] = { tri.vn0, tri.vn1, tri.vn2 };
        glm::vec3 tex[3] = { tri.vt0, tri.vt1, tri.vt2 };

        for (int j = 0; j < 3; ++j) {
            // ���I��m
            _points[p++] = pos[j].x;
            _points[p++] = pos[j].y;
            _points[p++] = pos[j].z;

            // �C��]�w�]�զ�A�i�ۦ�վ�^
            _points[p++] = 1.0f;
            _points[p++] = 1.0f;
            _points[p++] = 1.0f;

            // �k�V�q
            _points[p++] = nor[j].x;
            _points[p++] = nor[j].y;
            _points[p++] = nor[j].z;

            // �K�Ϯy�С]���M dog.obj �̬O vec3�A����Ū�e��ӴN�n�^
            _points[p++] = tex[j].x;
            _points[p++] = tex[j].y;

            _idx[i] = i;
            ++i;
        }
    }
}