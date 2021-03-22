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
	int type; 
	//char type[10];
	Weapon() {}
	Weapon(sf::Texture& t, Animation& a, int tp, int fr, int dam, int spd) {
		sprite.setTexture(t);
		texture = &t;
		firerate = fr;
		animation = &a;
		damage = dam;
		speed = spd;
		type = tp;
		/*if (&t == &homingBullet_texture)
		{
			strcpy(type, "homing");
			std::cout << type << '\n';
		}*/
	}
};

namespace Weapons {
	Weapon classic(t1, sExplosion_1, 0, 100, 1, 20);
	Weapon electric(energyBall_projectile, sEnergyBall, 1, 500, 2, 15);
	Weapon homingBullet(homingBullet_texture, sExplosion_3, 2, 200, 10, 7);
}

