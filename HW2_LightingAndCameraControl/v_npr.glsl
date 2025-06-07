// v_NPR.glsl
#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aColor;    // ���I�C��Ҧ���
layout(location=2) in vec3 aNormal;
layout(location=3) in vec2 aTex;    // Texture Coordinates

uniform mat4 mxModel;
uniform mat4 mxView;
uniform mat4 mxProj;

uniform vec3 viewPos;   // �ө��p�⥲�������Y��m 
uniform vec3 lightPos;  // �ө��p�⥲����������m 

out vec3 vNormal;      // ���I���k�V�q (N)
out vec3 vLight;       // ���I���V������ Light(L) �V�q
out vec3 vView;        // ���I���V view �� view(V) �V�q
out vec3 vColor;       // ���I�C��
out vec3 v3Pos;        // ���I����m 

void main() {
    vec4 worldPos = mxModel * vec4(aPos, 1.0);
    v3Pos   = worldPos.xyz;
    vNormal = normalize((mat3(mxModel) * aNormal));
    vLight  = normalize(lightPos - v3Pos);
    vView   = normalize(viewPos - v3Pos);
    vColor   = aColor;
    gl_Position = mxProj * mxView * worldPos;
}