#pragma once
#include <string.h>
#include "Enemy.h"
extern std::vector<Projectile> playerProjectiles;
class Boss
{
public:
	int updates = 0;
	bool shoot = false;
	sf::Sprite sprite;
	sf::Vector2f velocity;
	float lastVel;
	int maxHealth;
	int health;
	float maxSpeed = 5;
	sf::Clock bossTimer;
	int shootTime;
	int facingRight = false;

	Boss(sf::Texture& t1, int h) {
		sprite.setTexture(t1);
		sprite.setOrigin(t1.getSize().x / 2, t1.getSize().y / 2);
		maxHealth = h;
		health = h;
	}

	void update(sf::Texture& t1, sf::Texture& t2) {
		if (updates % 3000 == 0)
		{
			shoot = true;
			shootTime = bossTimer.getElapsedTime().asMilliseconds();
		}
		if ((velocity.x < 0 && lastVel > 0) || (velocity.x > 0 && lastVel < 0) && !shoot)
		{
			sprite.scale(-1.f, 1);
			facingRight = !facingRight;
		}
		if (!shoot)
		{
		sprite.move(velocity);
		lastVel = velocity.x;

		}
		if (bossTimer.getElapsedTime().asMilliseconds() - shootTime > 5)
		{
			shoot = false;
		}
		updates++;
	}

	void reset(sf::Texture& t1) {
		health = maxHealth;
		shoot = false;
		updates = 0;
		facingRight = false;
		sprite.setTexture(t1);
	}
};