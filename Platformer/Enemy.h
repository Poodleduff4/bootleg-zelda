#pragma once

#include <iostream>
#include <Windows.h>
#include <SFML/Graphics.hpp>
#include <math.h>


struct Enemy
{
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f velocity;
	int health;
	float maxSpeed = 5;
	Enemy(Enemy &e) {
		texture = e.texture;
		sprite = e.sprite;
		velocity = e.velocity;
		health = e.health;
	}
	Enemy(sf::Texture& t, sf::Vector2f pos, int h) {
		sprite.setPosition(pos.x, pos.y);
		sprite.setTexture(t);
		sprite.setOrigin(31, 21);
	}

	void update() {
		sprite.move(velocity);
	}
};