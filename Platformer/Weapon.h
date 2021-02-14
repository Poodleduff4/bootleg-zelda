#pragma once
#include "SFML/Graphics.hpp"
#include "Textures.h"
#include <iostream>
#include "Animation.h"

class Weapon
{
public:
	sf::Sprite sprite;
	sf::Texture *texture;
	int firerate;
	int damage;
	int speed;
	Animation* animation;
	char type[10];
	Weapon() {}
	Weapon(sf::Texture& t, Animation& a, int fr, int d, int spd) {
		sprite.setTexture(t);
		texture = &t;
		firerate = fr;
		animation = &a;
		damage = d;
		speed = spd;
		if (&t == &homingBullet_texture)
		{
			strcpy(type, "homing");
			std::cout << type << '\n';
		}
	}
};

namespace Weapons {
	Weapon classic(t1, sExplosion_1, 100, 1, 20);
	Weapon electric(energyBall_projectile, sEnergyBall, 500, 5, 15);
	Weapon homingBullet(homingBullet_texture, sExplosion_3, 200, 10, 7);
}

