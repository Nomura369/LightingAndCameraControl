#version 330 core

in vec3 vColor;
in vec3 vNormal;      // ���I���k�V�q (N)
in vec3 vLight;       // ���I���V������ Light(L) �V�q
in vec3 vView;        // ���I���V view �� view(V) �V�q
in vec3 v3Pos;

uniform int  uShadingMode;     // 1=���I��, 2=�����, 3=Per-Pixel
uniform vec4 ui4Color;         // �ҫ��C��

struct LightSource {
    vec3 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float constant;
    float linear;
    float quadratic;
    // �Y�� Spot Light�A�i�[�H�U
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float exponent;
};
uniform LightSource uLight;

struct Material {
    vec4 ambient;   // ka
    vec4 diffuse;   // kd
    vec4 specular;  // ks
    float shininess;
};
uniform Material uMaterial;
out vec4 FragColor;

void main() {
    // �Ҧ������G1 = ���I�C��B2 = �ҫ��C��A3 = Per Pixel Lighting
    if( uShadingMode == 1) { FragColor = vec4(vColor, 1.0);  return; }
    if( uShadingMode == 2 ){ FragColor = ui4Color; return; }

    // 3 = Per-Pixel Phong
    vec3 N = normalize(vNormal);
    vec3 L = normalize(vLight);
    vec3 V = normalize(vView);
    vec3 R = reflect(-L, N);

    // Ambient
    vec4 ambient = uLight.ambient * uMaterial.ambient;

    // Diffuse
    float diff = max(dot(N, L), 0.0);
    vec4 diffuse = uLight.diffuse * diff * uMaterial.diffuse;

    // Specular
    float spec = pow(max(dot(R, V), 0.0), uMaterial.shininess);
    vec4 specular = uLight.specular * spec * uMaterial.specular;

    // Attenuation
    float dist = length(uLight.position - v3Pos);
    float atten = 1.0 / (uLight.constant
                       + uLight.linear   * dist
                       + uLight.quadratic* dist * dist);

    // Spot Light ���ܡ]�p���]�w cutOff > 0�^
    if(uLight.cutOff > 0.0) {
        float theta = dot(L, normalize(-uLight.direction));
        float intensity = clamp(
            (theta - uLight.outerCutOff) /
            (uLight.cutOff - uLight.outerCutOff),
            0.0, 1.0
        );
         if( uLight.exponent == 1.0f ) atten *= intensity;
         else { atten *= pow(intensity, uLight.exponent); }
    }

    vec4 color = (ambient + diffuse + specular) * atten;
    color.w = 1.0f;
    FragColor  = color;
}
