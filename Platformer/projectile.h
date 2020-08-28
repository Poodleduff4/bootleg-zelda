#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
//#include "Platformer.cpp"


sf::Texture createMask(sf::Texture &t);

struct Projectile
{
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f velocity;
	float maxSpeed = 20;

	Projectile(sf::Texture &t, sf::Vector2f pos) {
		sprite.setPosition(pos.x, pos.y);
		sprite.setTexture(t);
	}

	void update() {
		sprite.move(velocity);
	}
};