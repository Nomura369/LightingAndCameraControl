#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class CCamera
{
public:
	enum class Type {
		PERSPECTIVE = 1,
		ORTHOGRAPHIC = 2
	};

	// Singleton Accessor
	static CCamera& getInstance();

	// Camera Update Methods
	void updatePerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
	void updateOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane);
	void updateViewCenter(const glm::vec3& view, const glm::vec3& center);
	void updateView(const glm::vec3& view);
	void updateCenter(const glm::vec3& center);
	void updateRadius(float delta);

	void processMouseMovement(float deltaX, float deltaY, float sens);
	void processMouseScroll(float deltaScroll, float zoomSensitivity);

	// Camera Getters
	const glm::vec3& getViewLocation();
	const glm::mat4& getProjectionMatrix();
	const glm::mat4& getViewMatrix();
	const glm::mat4& getViewProjectionMatrix() const;
	CCamera::Type getProjectionType() const;

protected:
	// Constructor & Destructor
	CCamera();
	~CCamera() = default;

	// Delete copy/assign (Singleton-safe)
	CCamera(const CCamera&) = delete;   // �T��ϥΫ����غc�l
	CCamera& operator=(const CCamera&) = delete; // �T��ϥΫ��w�B��l (=)

	// Internals
	void updateViewMatrix();
	void updateViewMatrix(float theta, float phi);

	mutable glm::mat4 _mxView;
	mutable glm::mat4 _mxProj;
	mutable glm::mat4 _mxViewProj;

	CCamera::Type _type;
	mutable bool _bviewUpdate;
	mutable bool _bprojUpdate;

	// Camera data
	glm::vec3 _view;
	glm::vec3 _center;
	glm::vec3 _up;

	// �ƹ����Y���ʪ�����
	float _theta;  // ��������
	float _phi;    // ��������
	float _radius; // eye �P center �����Z��
};