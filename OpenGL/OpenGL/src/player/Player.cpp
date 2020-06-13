#include "Player.h"

Player::Player() : pos(0.f, 0.f, 0.f), camera(&pos) {

}

Player::Player(float px, float py, float pz) : pos(px, py, pz), camera(&pos) {
	this->moving_speed = 6.0f;
	this->camera.setPosition(px, py, pz);
	this->camera.setDirectionUp(0.f, 1.f, 0.f);
	this->camera.setCameraFront(0.f, 0.f, -1.f);
}

Player::~Player() {
}

void Player::moveFront(float dt) {
	float dist = moving_speed * dt;
	this->camera.moveFront(dist);
}

void Player::moveBack(float dt) {
	float dist = moving_speed * dt;
	this->camera.moveBack(dist);
}

void Player::moveLeft(float dt) {
	float dist = moving_speed * dt;
	this->camera.moveLeft(dist);
}

void Player::moveRight(float dt) {
	float dist = moving_speed * dt;
	this->camera.moveRight(dist);
}

void Player::moveUp(float dt) {
	float dist = moving_speed * dt;
	this->camera.moveUp(dist);
}

void Player::moveDown(float dt) {
	float dist = moving_speed * dt;
	this->camera.moveDown(dist);
}

glm::vec3 Player::getFront() {
	return this->camera.camera_front;
}

glm::mat4 Player::getViewMatrix() {
	//mc::Printer::printVec3(camera.camera_front);
	return camera.getViewMatrix();
}