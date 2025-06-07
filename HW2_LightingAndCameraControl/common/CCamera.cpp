#include "CCamera.h"
#include <glm/gtc/matrix_transform.hpp>  // For glm::lookAt and glm::perspective
#include <iostream>
#include "typedefs.h"

using namespace glm;

// Singleton instance
CCamera& CCamera::getInstance()
{
	static CCamera instance;
	return instance;
}

CCamera::CCamera()
{
	_theta = 0; _phi = 0;	 // ���ץ�  updateViewCenter �ӭp�� 
	_radius = 0.0f; // �b�|��  updateViewCenter �ӭp��

	// Default projection type
	_type = Type::PERSPECTIVE;
	_view = glm::vec3(5.0f, 5.0f, 5.0f); // ���Y��m
	_center = glm::vec3(0.0f, 0.0f, 0.0f); // �ؼ��I
	_up = glm::vec3(0.0f, 1.0f, 0.0f);
	// �w�]���Y�q (5,5,5) �ݦV (0,0,0) �A�Q�� updateViewMatrix �ӭp��
	updateViewCenter(_view, _center);
	_mxProj = glm::perspective(glm::radians(60.0f), 1.0f, 1.0f, 1000.0f);
	_mxViewProj = _mxProj * _mxView;

	_bviewUpdate = true;
	_bprojUpdate = true;
}

void CCamera::updatePerspective(float fovy, float aspect, float zNear, float zFar)
{
	_mxProj = glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
	_type = Type::PERSPECTIVE;
	_bprojUpdate = true;
}

void CCamera::updateOrthographic(float left, float right, float bottom, float top, float zNear, float zFar)
{
	_mxProj = glm::ortho(left, right, bottom, top, zNear, zFar);
	_type = Type::ORTHOGRAPHIC;
	_bprojUpdate = true;
}

void CCamera::processMouseMovement(float deltaX, float deltaY, float sensitivity)
{
	_theta += deltaX * sensitivity;
	_phi -= deltaY * sensitivity;

	// ���� _phi ���קK���� 0 �� pi
	if (_phi < 0.1f) _phi = 0.1f;
	if (_phi > M_PI- 0.1f) _phi = M_PI - 0.1f;

	updateViewMatrix(_theta, _phi);
}

void CCamera::processMouseScroll(float deltaScroll, float zoomSensitivity)
{
	_radius -= deltaScroll * zoomSensitivity;
	if (_radius < 1.0f) _radius = 1.0f;  // ����̤p�Z��
	updateViewMatrix();
}

const glm::vec3& CCamera::getViewLocation()
{
	return _view;
}

void CCamera::updateView(const glm::vec3& view)
{
	_view = view;
	glm::vec3 offset = _view - _center;
	_radius = glm::length(offset);

	if (_radius < 1e-5f) {
		_radius = 1.0f;
		_theta = 0.0f;
		_phi = M_PI / 2.0f;  // ����
	}
	else {
		_theta = -atan2(-offset.z, offset.x);
		_phi = acos(glm::clamp(offset.y / _radius, -1.0f, 1.0f));
	}
	updateViewMatrix();
}

void CCamera::updateCenter(const glm::vec3& center)
{
	_center = center;
	glm::vec3 offset = _view - _center;
	_radius = glm::length(offset);

	// �קK���H 0
	if (_radius < 1e-5f) {
		_radius = 1.0f;
		_theta = 0.0f;
		_phi = M_PI / 2.0f;  // �����¦V
	}
	else {
		// Y �b�¤W�A�y�y�Эp��
		_theta = -atan2(-offset.z, offset.x);
		_phi = acos(glm::clamp(offset.y / _radius, -1.0f, 1.0f));
	}
	updateViewMatrix();
}

void CCamera::updateViewCenter(const glm::vec3& view, const glm::vec3& center)
{
	_view = view; _center = center;

	glm::vec3 offset = _view - _center;
	_radius = glm::length(offset);

	// �קK���H 0
	if (_radius < 1e-5f) {
		_radius = 1.0f;
		_theta = 0.0f;
		_phi = M_PI / 2.0f;  // �����¦V
	}
	else {
		// Y �b�¤W�A�y�y�Эp��
		_theta = -atan2(-offset.z, offset.x);
		_phi = acos(glm::clamp(offset.y / _radius, -1.0f, 1.0f));
	}
	updateViewMatrix();
}

void CCamera::updateViewMatrix()
{
	_mxView = glm::lookAt(_view, _center, _up);
	_bviewUpdate = true;
}

// Convert spherical coordinates to Cartesian coordinates
void CCamera::updateViewMatrix(float theta, float phi)
{
	_view.x = _center.x + _radius * sin(phi) * cos(theta);
	_view.y = _center.y + _radius * cos(phi);
	_view.z = _center.z + _radius * sin(phi) * sin(theta);
	_mxView = glm::lookAt(_view, _center, _up);
	_bviewUpdate = true;
}

void CCamera::updateRadius(float delta) {
	_radius += delta;
	if (_radius < 1.0f) _radius = 1.0f;  // ����̤p�Z��
	// ��s�۾���m
	_view.x = _center.x + _radius * sin(_phi) * cos(_theta);
	_view.y = _center.y + _radius * cos(_phi);
	_view.z = _center.z + _radius * sin(_phi) * sin(_theta);
	updateViewMatrix();
}

const glm::mat4& CCamera::getProjectionMatrix() 
{
	return _mxProj;
}

const glm::mat4& CCamera::getViewMatrix()
{
	return _mxView;
}

const glm::mat4& CCamera::getViewProjectionMatrix() const
{
	if ( _bviewUpdate || _bprojUpdate ) {
		_mxViewProj = _mxProj * _mxView;
		_bviewUpdate = false;
		_bprojUpdate = false;
	}
	return _mxViewProj;
}

CCamera::Type CCamera::getProjectionType() const
{
	return _type;
}