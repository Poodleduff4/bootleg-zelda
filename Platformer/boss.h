#pragma once
#include <string.h>
#include "Enemy.h"
#include "Animation.h"
#include "SFML/Graphics.hpp"
extern std::vector<Projectile> enemyProjectiles;
extern sf::Vector2f playerCenter;
extern sf::Vector2f mousePos;
extern std::vector<AnimationEvent> animations;
extern int frameCount;

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
	float maxSpeed = 1;
	sf::Clock bossTimer;
	int shootTime;
	int facingRight = true;
	bool shotCharge = false;
	bool first = true;
	int waitFrames;
	sf::Clock reloadTime;
	sf::Vector2f scale = sf::Vector2f(1.f, 1.f);

	Boss(sf::Texture& t1, int h) {
		sprite.setTexture(t1);
		sprite.setOrigin(t1.getSize().x / 2, t1.getSize().y / 2);
		maxHealth = h;
		health = h;
	}

	void update(sf::Texture& t1, sf::Texture& t2) {
		if (first) 
		{
			bossTimer.restart();
			reloadTime.restart();
			first = false;
		}
		//change sprite direction
		/*if ((velocity.x < 0 && lastVel > 0) || (velocity.x > 0 && lastVel < 0) && !shotCharge)
		{
			sprite.scale(-1.f, 1.f);
			facingRight = !facingRight;
			scale = sprite.getScale();
		}*/
		if (playerCenter.x < sprite.getPosition().x && sprite.getScale().x == 1.f)
		{
			sprite.scale(-1, 1);
			facingRight = !facingRight;
		}
		else if (playerCenter.x > sprite.getPosition().x && sprite.getScale().x == -1.f)
		{
			sprite.scale(-1, 1);
			facingRight = !facingRight;
		}
		
		if (bossTimer.getElapsedTime().asMilliseconds() > 0)
		{
			if(reloadTime.getElapsedTime().asMilliseconds() > 2000)
			{
				shotCharge = true;
				waitFrames = (9 * (1 / 0.2)) + frameCount;
			}
		}
		std::cout << frameCount << " | " << waitFrames << '\n';
		if (shotCharge)
		{
			/*sprite.setTexture(t2);
			sprite.setScale(scale);*/
		}
		else if(frameCount > waitFrames)
		{
			sprite.move(velocity * maxSpeed);
			lastVel = velocity.x;
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