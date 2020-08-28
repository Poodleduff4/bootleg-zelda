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
	float maxSpeed = 5;

	Enemy(sf::Texture& t, sf::Vector2f pos) {
		sprite.setPosition(pos.x, pos.y);
		sprite.setTexture(t);
		sprite.setOrigin(31, 21);
	}

	void update() {
		sprite.move(velocity);
	}
};