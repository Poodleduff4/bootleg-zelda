#pragma once

#include <iostream>
#include <Windows.h>
#include <SFML/Graphics.hpp>
#include <math.h>


	/*Enemy(Enemy &e) {
		texture = e.texture;
		sprite = e.sprite;
		velocity = e.velocity;
		health = e.health;
	}*/

class Enemy
{
public:
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f velocity;
	int health;
	float maxSpeed = 5;

	Enemy() {}

	Enemy(sf::Texture& t, int h) {
		sprite.setTexture(t);
		sprite.setOrigin(t.getSize().x / 2, t.getSize().y / 2);
		health = h;
	}

	void update() {
		sprite.move(velocity);
	}
};