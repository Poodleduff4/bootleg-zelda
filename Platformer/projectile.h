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

		sprite.setPosition(pos);
		sprite.setTexture(t);
		sprite.setOrigin(t.getSize().x / 2, t.getSize().y / 2);
	}

	Projectile(sf::IntRect &rect, sf::Vector2f pos, sf::Texture& energyBall) {
		sprite.setTexture(energyBall);
		sprite.setTextureRect(rect);
		sprite.setOrigin(sprite.getTextureRect().width/2, sprite.getTextureRect().height / 2);
		sprite.setPosition(pos);
		//maxSpeed = -maxSpeed;
	}

	void update() {
		sprite.move(velocity * maxSpeed);
	}
};