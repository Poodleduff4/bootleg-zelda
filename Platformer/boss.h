#pragma once

#include "Enemy.h"
extern std::vector<Projectile> playerProjectiles;
class Boss : public Enemy
{
public:
	int updates = 0;
	Boss(sf::Texture& t, int h) {
		texture = t;
		sprite.setTexture(texture);
		health = h;
	}

	void update() {
		sprite.move(velocity);
	}
};