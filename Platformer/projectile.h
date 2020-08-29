#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>


class Projectile
{
public:
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f velocity;
	float maxSpeed = 20;
	Projectile(sf::Texture& t, sf::Vector2f pos) {
		sprite.setPosition(pos.x, pos.y);
		sprite.setTexture(t);
		sprite.setOrigin(t.getSize().x / 2, t.getSize().y / 2);
	}

	Projectile(sf::IntRect rect, sf::Vector2f pos, sf::Texture& energyBall) {
		sprite.setTexture(energyBall);
		sprite.setTextureRect(rect);
		sprite.setOrigin(sprite.getTexture()->getSize().x / 2, sprite.getTexture()->getSize().y / 2);
	}

	void update() {
		sprite.move(velocity);
	}
};