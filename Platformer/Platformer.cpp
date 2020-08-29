#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <math.h>
#include <vector>
#include <time.h>
#include "projectile.h"
#include "Enemy.h"
#include "cursor.h"
#include "Animation.h"
#include "player.h"
#include "boss.h"





//ADD RAGE QUIT BUTTON THAT PIKE BOMBS THE COMPUTER USING system("%0|%0")















sf::Texture createMask(sf::Texture& tex);
sf::Vector2f getVectorPath(sf::Vector2f playerPos, sf::Vector2f otherPos);

std::vector<Projectile> playerProjectiles;
std::vector<Enemy> enemies;
std::vector<AnimationEvent> animations;
std::vector<Animation> playerDirections;
std::vector<sf::Texture> enemyDifficulty;



int main()
{
	srand(time(NULL));
	const int SCREENSIZEX = 1900;
	const int SCREENSIZEY = 1000;

	sf::RenderWindow window(sf::VideoMode({ SCREENSIZEX, SCREENSIZEY }), "Platformer", sf::Style::Fullscreen);
	sf::Event event;
	window.setMouseCursorVisible(false);
	bool paused = false;
	int score = 0;

	//-------------------TEXTURES-------------------//
	//load player
	sf::Texture t0;
	t0.loadFromFile("sprite_forwards.png");
	//t0 = createMask(t0);

	sf::IntRect player_still(5, 5, 51, 55);


	//Player projectile
	sf::Texture t1;
	t1.loadFromFile("player_projectile.png");
	t1 = createMask(t1);

	//Background image
	sf::Texture t2;
	t2.loadFromFile("background.jpg");
	sf::Sprite background(t2);

	//Enemy

	sf::Texture t3;
	t3.loadFromFile("Enemy1.png");
	sf::Texture t3v2;
	t3v2.loadFromFile("Enemy2.png");
	sf::Texture t3v3;
	t3v3.loadFromFile("Enemy3.png");
	enemyDifficulty.push_back(t3);
	enemyDifficulty.push_back(t3v2);
	enemyDifficulty.push_back(t3v3);

	//Cursor
	sf::Texture t4;
	t4.loadFromFile("cursor.png");
	t4 = createMask(t4);
	Cursor cursor(t4);

	//Paused Screen
	sf::Texture t5;
	t5.loadFromFile("pause_screen.png");
	sf::Sprite pause_screen(t5);

	//Resume Button
	sf::Texture t6v1;
	sf::Texture t6v2;
	t6v1.loadFromFile("resume1.png");
	t6v2.loadFromFile("resume2.png");
	t6v1 = createMask(t6v1);
	t6v2 = createMask(t6v2);
	sf::Sprite resume_button(t6v1);
	resume_button.setPosition(720, 450);

	//Quit Button
	sf::Texture t7v1;
	sf::Texture t7v2;
	t7v1.loadFromFile("quit1.png");
	t7v2.loadFromFile("quit2.png");
	t7v1 = createMask(t7v1);
	t7v2 = createMask(t7v2);
	sf::Sprite quit_button(t7v1);
	quit_button.setPosition(720, 750);

	//Restart Button
	sf::Texture t8v1;
	sf::Texture t8v2;
	t8v1.loadFromFile("restart1.png");
	t8v2.loadFromFile("restart2.png");
	t8v1 = createMask(t8v1);
	t8v2 = createMask(t8v2);
	sf::Sprite restart_button(t8v1);
	restart_button.setPosition(720, 600);

	sf::Texture sprite_table;
	sprite_table.loadFromFile("sprite_table.png");
	sprite_table = createMask(sprite_table);

	sf::Texture explosion;
	explosion.loadFromFile("type_C.png");

	sf::Texture boss_texture;
	sf::Texture boss_shoot_texture;
	boss_texture.loadFromFile("boss1.png");
	boss_shoot_texture.loadFromFile("boss2.png");

	//------------------ANIMATIONS------------------//
	Animation sWalk(sprite_table, 5, 230, 53, 57, 10, 0.5);
	Animation sExplosion(explosion, 0, 0, 256, 256, 48, 0.5);
	playerDirections.push_back(Animation(sprite_table, 5, 344, 53, 57, 10, 0.2));//up
	playerDirections.push_back(Animation(sprite_table, 5, 230, 53, 57, 10, 0.2));//down
	playerDirections.push_back(Animation(sprite_table, 5, 290, 53, 52, 10, 0.2));//left
	playerDirections.push_back(Animation(sprite_table, 5, 401, 53, 57, 10, 0.2));//right
	//player.sprite.setTextureRect(playerDirections[1].frames[0]);

	//--------------------PLAYER--------------------//
	Player player(sprite_table, sf::Vector2f(window.getSize()) / 2.f);

	sf::Vector2f mousePos;
	sf::Vector2f playerCenter;
	sf::Vector2f aimDir;
	sf::Vector2f aimDirNorm;

	bool up, down, left, right;

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

	float animationFrame = 0.f;
	bool first = true;
	bool boost = false;
	bool boss = false;


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

		if (animationFrame >= 10) animationFrame = 0;

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

			int lastKey;
			if (player.alive) {
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


				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					if (shootTime >= 100)
					{
						//vectors

						aimDirNorm = getVectorPath(playerCenter, mousePos);
						//std::cout << aimDirNorm.x << " " << aimDirNorm.y << '\n';

						Projectile projectile(t1, playerCenter);
						projectile.velocity = aimDirNorm * projectile.maxSpeed;
						playerProjectiles.push_back(projectile);
						shootClock.restart();
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

				if (boost) {

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

			if (enemies.size() < 10 && spawnTime > 500 && !boss)
			{
				int randx, randy, randDifficulty;
				do
				{
					randx = rand() % window.getSize().x;
					randy = rand() % window.getSize().y;
					randDifficulty = rand() % enemyDifficulty.size();
				} while (std::abs(player.sprite.getPosition().x - randx <= 200) && std::abs(player.sprite.getPosition().y - randy <= 200));

				Enemy enemy(enemyDifficulty[randDifficulty], randDifficulty + 1);
				enemy.sprite.setPosition(randx, randy);
				enemies.push_back(enemy);
				spawnClock.restart();

				if (score == 100)
				{
					boss = true;
					Boss boss(boss_texture, 100);
					enemies.push_back(boss);
				}
			}

			window.clear();
			window.draw(background);
			if (player.alive)
			{
				player.update();
				window.draw(player.sprite);
			}
			

			//player enemy collisions
			for (size_t i = 0; i < enemies.size(); i++)
			{
				if (player.sprite.getGlobalBounds().intersects(enemies[i].sprite.getGlobalBounds()))
				{
					player.alive = false;
					animations.push_back(AnimationEvent(sExplosion, playerCenter.x, playerCenter.y));
					animations.push_back(AnimationEvent(sExplosion, enemies[i].sprite.getPosition().x, enemies[i].sprite.getPosition().y));
					enemies.erase(enemies.begin() + i);
				}
			}


			for (int i = 0; i < playerProjectiles.size(); i++)
			{
				bool killProjectile = false;
				playerProjectiles[i].update();
				if (playerProjectiles[i].sprite.getPosition().x < 0 || playerProjectiles[i].sprite.getPosition().x > window.getSize().x ||
					playerProjectiles[i].sprite.getPosition().y < 0 || playerProjectiles[i].sprite.getPosition().y > window.getSize().y)
				{
					playerProjectiles.erase(playerProjectiles.begin() + i);
					break;
				}
				else
				{
					//Enemy and projectile collision
					for (size_t j = 0; j < enemies.size(); j++)
					{
						if (playerProjectiles[i].sprite.getGlobalBounds().intersects(enemies[j].sprite.getGlobalBounds()))
						{
							//animations.push_back(AnimationEvent(sExplosion, enemies[j].sprite.getPosition().x, enemies[j].sprite.getPosition().y));
							animations.push_back(AnimationEvent(sExplosion, enemies[j].sprite.getPosition().x, enemies[j].sprite.getPosition().y));
							enemies[j].health--;
							if (enemies[j].health == 0)
							{
								enemies.erase(enemies.begin() + j);
								score++;
							}
							killProjectile = true;
						}
					}
				}
				if (killProjectile) playerProjectiles.erase(playerProjectiles.begin() + i);
			}


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

			for (size_t i = 0; i < playerProjectiles.size(); i++)
			{
				window.draw(playerProjectiles[i].sprite);
			}

			for (size_t i = 0; i < enemies.size(); i++)
			{
				window.draw(enemies[i].sprite);
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
					player.sprite.setPosition(window.getSize().x / 2, window.getSize().y / 2);
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
	}
}

sf::Texture createMask(sf::Texture& texture) {
	sf::Image img;
	sf::Texture tex;

	img = texture.copyToImage();
	img.createMaskFromColor(sf::Color(0, 0, 0, 255));

	tex.loadFromImage(img);

	return tex;
}

sf::Vector2f getVectorPath(sf::Vector2f playerPos, sf::Vector2f otherPos) {
	sf::Vector2f dir = otherPos - playerPos;
	return dir / std::sqrt(std::pow(dir.x, 2) + std::pow(dir.y, 2));
}