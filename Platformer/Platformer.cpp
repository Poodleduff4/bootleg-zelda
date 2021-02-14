#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <math.h>
#include <vector>
#include <time.h>
#include <algorithm>
#include "projectile.h"
#include "Enemy.h"
#include "cursor.h"
#include "Animation.h"
#include "player.h"
#include "boss.h"
#include "Collision.h"
#include "health_bar.h"
#include "Textures.h"
#include "Homing.h"

//ADD RAGE QUIT BUTTON THAT PIKE BOMBS THE COMPUTER USING system("%0|%0")

sf::Texture createMask(sf::Texture &tex);
sf::Vector2f getVectorPath(sf::Vector2f playerPos, sf::Vector2f otherPos);
sf::Vector2f getQuadrant(float angle);
sf::Vector2f multiplyVector(sf::Vector2f left, sf::Vector2f right);
Enemy* find_nearest_enemy(sf::Vector2f pos);
void removeHomingMarkers(HomingTarget*);

std::vector<Projectile> playerProjectiles;
std::vector<Homing> homingProjectiles;
std::vector<Projectile> enemyProjectiles;
std::vector<Enemy> enemies;
std::vector<AnimationEvent> animations;
std::vector<Animation> playerDirections;
std::vector<sf::Texture> enemyDifficulty;
std::vector<HomingTarget> homingTargets;

sf::Vector2f mousePos;
sf::Vector2f playerCenter;
int frameCount = 0;
int gameDifficulty = 1;
Boss boss(boss_texture, 100);
bool boss_alive = false;

sf::RenderWindow window(sf::VideoMode({1900, 1000}), "Platformer", sf::Style::Fullscreen);

int main()
{
	srand(time(NULL));
	sf::Event event;
	initializeTextures(window);
	window.setMouseCursorVisible(false);
	bool paused = false;
	int score = 0;
	int kills = 1;

	//--------------------SPRITES--------------------// initialized in Textures.h
	sf::Sprite background(t2);

	sf::Sprite pause_screen(t5);

	sf::Sprite resume_button(t6v1);
	resume_button.setPosition(720, 450);

	sf::Sprite quit_button(t7v1);
	quit_button.setPosition(720, 750);

	sf::Sprite restart_button(t8v1);
	restart_button.setPosition(720, 600);

	sf::Sprite bossHealthBarFull(bossHealthBarMax_texture);
	bossHealthBarFull.setOrigin(bossHealthBarMax_texture.getSize().x / 2, 0);
	bossHealthBarFull.setPosition(window.getSize().x / 2, 10);

	//player.sprite.setTextureRect(playerDirections[1].frames[0]);

	//--------------------PLAYER--------------------//
	Player player(sprite_table, sf::Vector2f(window.getSize()) / 2.f);

	//--------------------Weapon--------------------//
	player.weapon = &Weapons::classic;

	//---------------------Boss---------------------//

	HealthBar bossHealthBar(bossHealthBar_texture);

	Cursor cursor(t4);

	sf::Vector2f aimDir;
	sf::Vector2f aimDirNorm;

	bool up, down, left, right;
	up = down = left = right = false;

	sf::Clock timer;

	sf::Clock shootClock;
	int shootTime;
	sf::Clock spawnClock;
	int spawnTime;
	sf::Clock deathClock;
	int deathTime;
	sf::Clock boostClock;
	int boostTime;
	sf::Clock boostCooldownClock;
	int boostCooldown;
	sf::Clock triggerClock;
	int triggerTime;

	sf::Clock bossShootTimer;

	sf::Clock weaponSwitchClock;

	float animationFrame = 0.f;
	bool first = true;
	bool boost = false;

	int waitFrames;

	for (size_t i = 0; i < 5; i++)
	{
		Enemy enemy(t3, 1);
		enemy.sprite.setPosition(sf::Vector2f(rand() % window.getSize().x, rand() % window.getSize().y));
		enemies.push_back(enemy);
	}

	while (window.isOpen())
	{
		shootTime = shootClock.getElapsedTime().asMilliseconds();
		spawnTime = spawnClock.getElapsedTime().asMilliseconds();
		deathTime = deathClock.getElapsedTime().asMilliseconds();
		boostTime = boostClock.getElapsedTime().asMilliseconds();
		boostCooldown = boostCooldownClock.getElapsedTime().asMilliseconds(); //cooldown for boosting
		triggerTime = triggerClock.getElapsedTime().asMilliseconds();

		if (boostCooldown < 5000 and first)
		{
			boostCooldown = 5000;
		}
		window.setFramerateLimit(60);
		playerCenter = sf::Vector2f(player.sprite.getPosition().x + 23, player.sprite.getPosition().y + 39);
		mousePos = sf::Vector2f(sf::Mouse::getPosition(window));

		if (animationFrame >= 10)
			animationFrame = 0;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				break;
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					paused = !paused;
				}
			}
		}
		if (!paused)
		{

			char lastKey;
			if (player.alive)
			{
				if (GetAsyncKeyState(0x57))
				{
					lastKey = 0x57;
					up = true;
					down = false;
					left = false;
					right = false;
					player.velocity.y--;
					//player.sprite.move(0, -player.maxSpeed);
					//std::cout << "W Pressed" << '\n';
				}
				if (GetAsyncKeyState(0x53))
				{
					lastKey = 0x53;
					down = true;
					up = false;
					left = false;
					right = false;
					player.velocity.y++;
					//player.sprite.move(0, player.maxSpeed);
					//std::cout << "S Pressed" << '\n';
				}
				if (GetAsyncKeyState(0x41))
				{
					lastKey = 0x41;
					left = true;
					up = false;
					down = false;
					right = false;
					player.velocity.x--;
					//player.sprite.move(-player.maxSpeed, 0);
					//std::cout << "A Pressed" << '\n';
				}
				if (GetAsyncKeyState(0x44))
				{
					lastKey = 0x44;
					right = true;
					up = false;
					down = false;
					left = false;
					player.velocity.x++;
					//player.sprite.move(player.maxSpeed, 0);
					//std::cout << "D Pressed" << '\n';
				}

				if (up && lastKey != 0x57)
				{
					player.sprite.setTextureRect(playerDirections[0].frames[0]);
					animationFrame = 0;
				}
				else if (up && lastKey == 0x57)
				{
					player.sprite.setTextureRect(playerDirections[0].frames[(int)animationFrame]);
				}

				if (down && lastKey != 0x53)
				{
					player.sprite.setTextureRect(playerDirections[1].frames[0]);
					animationFrame = 0;
				}
				else if (down && lastKey == 0x53)
				{
					player.sprite.setTextureRect(playerDirections[1].frames[(int)animationFrame]);
				}

				if (left && lastKey != 0x41)
				{
					player.sprite.setTextureRect(playerDirections[2].frames[0]);
					animationFrame = 0;
				}
				else if (left && lastKey == 0x41)
				{
					player.sprite.setTextureRect(playerDirections[2].frames[(int)animationFrame]);
				}

				if (right && lastKey != 0x44)
				{
					player.sprite.setTextureRect(playerDirections[3].frames[0]);
					animationFrame = 0;
				}
				else if (right && lastKey == 0x44)
				{
					player.sprite.setTextureRect(playerDirections[3].frames[(int)animationFrame]);
				}
				if (!up && !down && !right && !left)
				{
					player.sprite.setTextureRect(player_still);
				}
				up = false;
				down = false;
				left = false;
				right = false;
				animationFrame += playerDirections[0].speed;

				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && shootTime >= player.weapon->firerate)
				{
					if (strcmp(player.weapon->type, "homing") != 0)
					{
						aimDirNorm = getVectorPath(playerCenter, mousePos);
						Projectile projectile(player.weapon->texture, playerCenter, player.weapon->speed, *player.weapon->animation);
						projectile.velocity = aimDirNorm;
						projectile.damage = player.weapon->damage;
						playerProjectiles.push_back(projectile);
						shootClock.restart();
					}
					else //homing projectile
					{
						Enemy* targetEnemy = find_nearest_enemy(mousePos);
						if (targetEnemy != nullptr && !boss_alive)
						{
							aimDirNorm = getVectorPath(playerCenter, mousePos);

							Homing homingProjectile(homingBullet_texture, playerCenter, targetEnemy, 15, *player.weapon->animation);
							homingProjectile.velocity = aimDirNorm;
							homingProjectile.damage = player.weapon->damage;

							HomingTarget homingMarker(homingMarker_texture, targetEnemy);
							homingProjectile.marker = homingMarker;

							std::cout << "id " << homingMarker.id << '\n';
							homingProjectiles.push_back(homingProjectile);
							homingTargets.push_back(homingMarker);
							shootClock.restart();
						}
						else if(boss_alive)
						{
							/*if (std::abs(pos.x - enemyPos.x) < min.x && std::abs(pos.y - enemyPos.y) < min.y)
							{

							}*/


							aimDirNorm = getVectorPath(playerCenter, boss.sprite.getPosition());
							Homing homingProjectile(homingBullet_texture, playerCenter, &boss, 15, *player.weapon->animation);
							homingProjectile.velocity = aimDirNorm;
							homingProjectile.damage = player.weapon->damage;
							homingProjectiles.push_back(homingProjectile);
							HomingTarget homingMarker(homingMarker_texture, &boss);
							shootClock.restart();
						}
						std::cout << "shot\n";
					}
				}

				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && triggerTime > 200)
				{
					if (boost == true) //turn boost off
					{
						boostCooldownClock.restart().asMilliseconds();
						std::cout << "boost off" << '\n';
						boost = false;
					}
					else if (boostCooldownClock.getElapsedTime().asMilliseconds() > 5000) //turn boost on
					{
						std::cout << "boost on" << '\n';
						boostClock.restart();
						boost = true;
					}
					triggerClock.restart();
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1) && weaponSwitchClock.getElapsedTime().asMilliseconds() > 1000)
				{
					player.weapon = &Weapons::classic;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2) && weaponSwitchClock.getElapsedTime().asMilliseconds() > 1000)
				{
					player.weapon = &Weapons::electric;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3) && weaponSwitchClock.getElapsedTime().asMilliseconds() > 1000)
				{
					player.weapon = &Weapons::homingBullet;
				}

				if (boost)
				{

					if (boostClock.getElapsedTime().asMilliseconds() < 3000)
					{
						player.maxSpeed = playerSpeed * 2;
					}
					else
					{
						std::cout << "boost off time up" << '\n';
						player.maxSpeed = playerSpeed;
						boost = false;
						boostCooldownClock.restart().asMilliseconds();
					}
				}
				else
				{
					player.maxSpeed = playerSpeed;
				}
			}

			if (enemies.size() < 10 && spawnTime > 1000 && !boss_alive)
			{
				int randx, randy, randDifficulty;
				do
				{
					randx = rand() % window.getSize().x;
					randy = rand() % window.getSize().y;
					randDifficulty = rand() % enemyDifficulty.size();
				} while (std::abs(player.sprite.getPosition().x - randx <= 200) && std::abs(player.sprite.getPosition().y - randy <= 200));

				Enemy enemy(enemyDifficulty[randDifficulty], (randDifficulty + 1) * gameDifficulty);
				enemy.sprite.setPosition(randx, randy);
				enemies.push_back(enemy);
				spawnClock.restart();
			}

			if (kills % 20 == 0 && !boss_alive)
			{
				int randx, randy;
				boss_alive = true;
				do
				{
					randx = rand() % window.getSize().x;
					randy = rand() % window.getSize().y;
				} while (std::abs(player.sprite.getPosition().x - randx <= 200) && std::abs(player.sprite.getPosition().y - randy <= 200));

				boss = Boss(boss_texture, 100 * gameDifficulty);
				boss.sprite.setTexture(boss_shoot_texture);
				boss.sprite.setPosition(randx, randy);
				bossHealthBar = HealthBar(bossHealthBar_texture);

				for (size_t i = 0; i < enemies.size(); i++)
				{
					animations.push_back(AnimationEvent(sExplosion_2, enemies[i].sprite.getPosition().x, enemies[i].sprite.getPosition().y));
				}
				enemies.clear();
			}

			//player enemy collisions
			for (size_t i = 0; i < enemies.size(); i++)
			{

				if (player.sprite.getGlobalBounds().intersects(enemies[i].sprite.getGlobalBounds()))
				{
					player.alive = false;
					animations.push_back(AnimationEvent(sExplosion_3, playerCenter.x, playerCenter.y));
					animations.push_back(AnimationEvent(sExplosion_3, enemies[i].sprite.getPosition().x, enemies[i].sprite.getPosition().y));
					//removeHomingMarkers(&enemies[i]);
					enemies.erase(enemies.begin() + i);
				}
			}

			//projectiles
			for (int i = 0; i < playerProjectiles.size(); i++)
			{
				bool killProjectile = false;
				playerProjectiles[i].update();
				if (playerProjectiles[i].sprite.getPosition().x < 0 || playerProjectiles[i].sprite.getPosition().x > window.getSize().x ||
					playerProjectiles[i].sprite.getPosition().y < 0 || playerProjectiles[i].sprite.getPosition().y > window.getSize().y)
				{
					killProjectile = true;
				}

				//Enemy and projectile collision
				for (size_t j = 0; j < enemies.size(); j++)
				{
					if (playerProjectiles[i].sprite.getGlobalBounds().intersects(enemies[j].sprite.getGlobalBounds()))
					{
						enemies[j].health-=playerProjectiles[i].damage;
						if (enemies[j].health <= 0)
						{
							enemies[j].alive = false;
							animations.push_back(AnimationEvent(sExplosion_2, playerProjectiles[i].sprite.getPosition().x, playerProjectiles[i].sprite.getPosition().y));
							//removeHomingMarkers(&enemies[j]);
							enemies.erase(enemies.begin() + j);
							kills++;
							score++;
						}
						else
						{
							animations.push_back(AnimationEvent(playerProjectiles[i].anim, playerProjectiles[i].sprite.getPosition().x, playerProjectiles[i].sprite.getPosition().y));
						}
						killProjectile = true;
					}
				}

				//projectile and boss collision
				if (boss_alive)
				{
					if (Collision::PixelPerfectTest(playerProjectiles[i].sprite, boss.sprite))
					{
						boss.health-=playerProjectiles[i].damage;
						bossHealthBar.update(boss.health, boss.maxHealth);
						animations.push_back(AnimationEvent(playerProjectiles[i].anim, playerProjectiles[i].sprite.getPosition().x, playerProjectiles[i].sprite.getPosition().y));
						if (boss.health < 1)
						{
							boss_alive = false;
							gameDifficulty++;
							boss.bossDifficulty++;
							int numBulletsOnDeath = 10;
							for (int j = 1; j <= numBulletsOnDeath; j++)
							{
								/*Projectile projectile(energyBall_projectile, boss.sprite.getPosition(), 20, *player.weapon->animation);
								float angle = 360 / numBulletsOnDeath * j;
								projectile.velocity = multiplyVector(getQuadrant(angle), getVectorPath(sf::Vector2f(boss.sprite.getPosition().x + std::sin(angle), boss.sprite.getPosition().y + std::cos(angle)), boss.sprite.getPosition())) / 5.f;
								enemyProjectiles.push_back(projectile);*/
							}
							boss.reset(boss_texture);
							kills++;
							score++;
						}
						playerProjectiles.erase(playerProjectiles.begin() + i);
					}
				}

				if (killProjectile)
					playerProjectiles.erase(playerProjectiles.begin() + i);
			}

			for (size_t i = 0; i < enemyProjectiles.size(); i++)
			{
				if (Collision::PixelPerfectTest(player.sprite, enemyProjectiles[i].sprite) && player.alive)
				{
					player.alive = false;
					animations.push_back(AnimationEvent(sExplosion_2, player.sprite.getPosition().x, player.sprite.getPosition().y));
					enemyProjectiles.erase(enemyProjectiles.begin() + i);
				}
			}

			//homing projectiles
			for (int i = 0; i < homingProjectiles.size(); i++)
			{
				bool killProjectile = false;
				homingProjectiles[i].update();
				if (homingProjectiles[i].sprite.getPosition().x < 0 || homingProjectiles[i].sprite.getPosition().x > window.getSize().x ||
					homingProjectiles[i].sprite.getPosition().y < 0 || homingProjectiles[i].sprite.getPosition().y > window.getSize().y)
				{
					killProjectile = true;
				}
				for (size_t j = 0; j < enemies.size(); j++)
				{
					if (homingProjectiles[i].sprite.getGlobalBounds().intersects(enemies[j].sprite.getGlobalBounds()))
					{
						enemies[j].health -= homingProjectiles[i].damage;
						if (enemies[j].health <= 0)
						{
							animations.push_back(AnimationEvent(homingProjectiles[i].anim, homingProjectiles[i].sprite.getPosition().x, homingProjectiles[i].sprite.getPosition().y));
							enemies.erase(enemies.begin() + j);
							kills++;
							score++;
						}
						else
						{
							animations.push_back(AnimationEvent(sExplosion_2, homingProjectiles[i].sprite.getPosition().x, homingProjectiles[i].sprite.getPosition().y));
						}
						killProjectile = true;
					}
				}
				if(boss_alive)
				{
					if (homingProjectiles[i].sprite.getGlobalBounds().intersects(boss.sprite.getGlobalBounds()))
					{
						boss.health -= homingProjectiles[i].damage;
						bossHealthBar.update(boss.health, boss.maxHealth);
						animations.push_back(AnimationEvent(homingProjectiles[i].anim, homingProjectiles[i].sprite.getPosition().x, homingProjectiles[i].sprite.getPosition().y));
						if (boss.health <= 0)
						{
							boss_alive = false;
							gameDifficulty++;
							boss.bossDifficulty++;
							int numBulletsOnDeath = 10;
							boss.reset(boss_texture);
							kills++;
							score++;
						}
						killProjectile = true;
					}
				}
				if (killProjectile)
				{
					removeHomingMarkers(&homingProjectiles[i].marker);
					homingProjectiles.erase(homingProjectiles.begin() + i);
					std::cout << homingProjectiles[i].marker.numUpdates << '\n';

				}
			}
			//player and boss collision
			if (boss_alive)
			{
				if (player.alive)
				{
					if (Collision::PixelPerfectTest(player.sprite, boss.sprite))
					{
						player.alive = false;
						boss.health--;
						bossHealthBar.update(boss.health, boss.maxHealth);
						animations.push_back(AnimationEvent(sExplosion_3, playerCenter.x, playerCenter.y));
						if (boss.health < 1)
						{
							std::cout << "dead\n";
							boss_alive = false;
							boss.reset(boss_texture);
							animations.push_back(AnimationEvent(sExplosion_3, boss.sprite.getPosition().x, boss.sprite.getPosition().y));
						}
					}
				}
			}

			//move enemies towards the player
			for (int i = 0; i < enemies.size(); i++)
			{
				enemies[i].velocity = getVectorPath(enemies[i].sprite.getPosition(), playerCenter);
				enemies[i].update();
				if (enemies[i].sprite.getPosition().x < 0 || enemies[i].sprite.getPosition().x > window.getSize().x ||
					enemies[i].sprite.getPosition().y < 0 || enemies[i].sprite.getPosition().y > window.getSize().y)
				{
					enemies.erase(enemies.begin() + i);
				}
			}

			//player.alive = true;

			//boss shooting
			if (boss_alive)
			{
				
				sf::Vector2f bossShootPos = sf::Vector2f(boss.facingRight ? boss.sprite.getPosition().x + 142 : boss.sprite.getPosition().x - 142,
														 boss.facingRight ? boss.sprite.getPosition().y + -64 : boss.sprite.getPosition().y - 64);
				boss.velocity = getVectorPath(boss.sprite.getPosition(), playerCenter);

				if (boss.shotCharge)
				{

					animations.push_back(AnimationEvent(sEnergyBall, bossShootPos.x, bossShootPos.y));
					boss.shotCharge = false;
					boss.shoot = true;
					boss.reloadTime.restart();
					waitFrames = (9 * (1 / 0.2) + frameCount) / (2000 / boss.fireRate);
				}
				if (boss.shoot && frameCount >= waitFrames)
				{
					Projectile bossProjectile(energyBall_projectile, bossShootPos, 15, sEnergyBall);
					bossProjectile.velocity = getVectorPath(bossShootPos, playerCenter);
					enemyProjectiles.push_back(bossProjectile);
					boss.shoot = false;
					boss.reloadTime.restart();
				}
			}

			window.clear();
			window.draw(background);
			if (player.alive)
			{
				player.update();
				window.draw(player.sprite);
			}

			for (size_t i = 0; i < playerProjectiles.size(); i++)
			{
				window.draw(playerProjectiles[i].sprite);
			}

			for (size_t i = 0; i < enemyProjectiles.size(); i++)
			{
				enemyProjectiles[i].update();
				window.draw(enemyProjectiles[i].sprite);
			}

			for (size_t i = 0; i < homingProjectiles.size(); i++)
			{
				homingProjectiles[i].update();
				window.draw(homingProjectiles[i].sprite);
			}

			for (size_t i = 0; i < enemies.size(); i++)
			{
				window.draw(enemies[i].sprite);
			}
			if (boss_alive)
			{
				boss.update(boss_texture, boss_shoot_texture);
				window.draw(bossHealthBarFull);
				window.draw(bossHealthBar.sprite);
				window.draw(boss.sprite);
			}
			for (size_t i = 0; i < homingTargets.size(); i++)
			{
				homingTargets[i].update();
				window.draw(homingTargets[i].sprite);
			}

			for (size_t i = 0; i < animations.size(); i++)
			{
				if (!animations[i].anim.isEnd())
				{
					animations[i].anim.update();
					animations[i].draw(window);
				}
				else
				{
					animations.erase(animations.begin() + i);
				}
			}
			cursor.sprite.setPosition(mousePos);
			window.draw(cursor.sprite);
		}
		else
		{
			window.draw(pause_screen);
			cursor.sprite.setPosition(mousePos);
			if (resume_button.getGlobalBounds().contains(mousePos))
			{
				resume_button.setTexture(t6v2);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					paused = !paused;
					shootClock.restart();
				}
			}
			else
			{
				resume_button.setTexture(t6v1);
			}

			if (restart_button.getGlobalBounds().contains(mousePos))
			{
				restart_button.setTexture(t8v2);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					paused = !paused;
					shootClock.restart();
					player.alive = true;
					enemies.clear();
					int playerSpawnX;
					int playerSpawnY;
					do
					{
						playerSpawnX = rand() % window.getSize().x;
						playerSpawnY = rand() % window.getSize().y;
					} while (std::abs(playerSpawnX - boss.sprite.getPosition().x) > 500 && std::abs(playerSpawnY - boss.sprite.getPosition().y) > 500);


					player.sprite.setPosition(playerSpawnX, playerSpawnY);
				}
			}
			else
			{
				restart_button.setTexture(t8v1);
			}

			if (quit_button.getGlobalBounds().contains(mousePos))
			{
				quit_button.setTexture(t7v2);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					window.close();
					break;
				}
			}
			else
			{
				quit_button.setTexture(t7v1);
			}

			window.draw(resume_button);
			window.draw(restart_button);
			window.draw(quit_button);

			cursor.sprite.setPosition(mousePos);
			window.draw(cursor.sprite);
		}
		window.display();
		frameCount++;
	}
}

sf::Texture createMask(sf::Texture &texture)
{
	sf::Image img;
	sf::Texture tex;

	img = texture.copyToImage();
	img.createMaskFromColor(sf::Color(0, 0, 0, 255));

	tex.loadFromImage(img);

	return tex;
}

sf::Vector2f getVectorPath(sf::Vector2f to, sf::Vector2f from)
{
	sf::Vector2f dir = from - to;
	return dir / float(std::sqrt(std::pow(dir.x, 2) + std::pow(dir.y, 2)));
}

sf::Vector2f getQuadrant(float angle)
{
	if (angle <= 90)
	{
		return sf::Vector2f(-1, 1);
	}
	else if (angle <= 180)
	{
		return sf::Vector2f(-1, -1);
	}
	else if (angle <= 270)
	{
		return sf::Vector2f(-1, 1);
	}
	else if (angle <= 360)
	{
		return sf::Vector2f(1, 1);
	}
	else
	{
		return sf::Vector2f(0, 0);
	}
}

sf::Vector2f multiplyVector(sf::Vector2f left, sf::Vector2f right)
{
	sf::Vector2f final;
	final.x = left.x * right.x;
	final.y = left.y * right.y;
	return final;
}

Enemy* find_nearest_enemy(sf::Vector2f pos) {
	if (enemies.size() > 0)
	{


		Enemy* nearest = &enemies[0];
		sf::Vector2f enemyPos = enemies[0].sprite.getPosition();
		sf::Vector2f min = sf::Vector2f(std::abs(pos.x - enemyPos.x), std::abs(pos.y - enemyPos.y));
		for (size_t i = 0; i < enemies.size(); i++)
		{
			enemyPos = enemies[i].sprite.getPosition();
			if (std::abs(pos.x - enemyPos.x) < min.x && std::abs(pos.y - enemyPos.y) < min.y)
			{
				min = sf::Vector2f(std::abs(pos.x - enemyPos.x), std::abs(pos.y - enemyPos.y));
				nearest = &enemies[i];
			}
		}
		return nearest;
	}
	else
	{
		return nullptr;
	}
}

void removeHomingMarkers(HomingTarget* tar) {
	for (int i = 0; i < homingTargets.size(); i++)
	{
		std::cout << tar->id << " | " << homingTargets[i].id << '\n';
		if (tar->id == homingTargets[i].id)
		{
			homingTargets.erase(homingTargets.begin() + i);
			std::cout << "erase";
			return;
		}
	}
}