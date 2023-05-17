#include "Camera.h"

Camera::Camera(glm::vec3 pos) {
	m_cameraPosition = pos;
	m_cameraRight	 = glm::vec3(1.0f, 0.0f, 0.0f);
	m_cameraUp		 = glm::vec3(0.0f, 1.0f, 0.0f);
	m_cameraFront	 = glm::vec3(0.0f, 0.0f, 1.0f);
	m_cameraWatching = glm::vec3(0.0f, 0.0f, -1.0f);
}

glm::mat4 Camera::move(glm::vec3 offset) {

	m_cameraPosition += offset;

	//												the Up is the y axys
	return glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraWatching, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::getPerspective(int width, int height) {
	return glm::perspective(glm::radians(m_FOV), float(width / height), 0.1f, 100.0f);
}

glm::mat4 Camera::point(int posx, int posy) {

	float sensitivity = 0.1f;

	m_pitch += posy * sensitivity;
	m_yaw += posx * sensitivity;

	m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);

	m_cameraWatching.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_cameraWatching.y = sin(glm::radians(m_pitch));
	m_cameraWatching.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_cameraWatching, m_cameraFront = glm::normalize(m_cameraWatching);

	m_cameraFront.y = 0.0f;
	m_cameraRight	= glm::cross(m_cameraFront, m_cameraUp);

	return glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraWatching, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 Camera::getFrontSpeed() {
	return m_cameraFront * m_cameraSpeedConst;
}

glm::vec3 Camera::getRightSpeed() {
	return m_cameraRight * m_cameraSpeedConst;
}

void Camera::zoom(float diff_FOV) {
	m_FOV = glm::clamp(m_FOV + diff_FOV, 10.0f, 110.0f);
}