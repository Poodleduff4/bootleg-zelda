#include "SFML/Graphics.hpp"
#include "Animation.h"
#include "cursor.h"
extern std::vector<sf::Texture> enemyDifficulty;
extern std::vector<Animation> playerDirections;
extern sf::Texture createMask(sf::Texture& texture);

	sf::Texture t0;
	sf::IntRect player_still(5, 5, 51, 55);
	//Player projectile
	sf::Texture t1;
	//Background image
	sf::Texture t2;
	//Enemy
	sf::Texture t3;
	sf::Texture t3v2;
	sf::Texture t3v3;

	//Cursor
	sf::Texture t4;

	//Paused Screen
	sf::Texture t5;

	//Resume Button
	sf::Texture t6v1;
	sf::Texture t6v2;

	//Quit Button
	sf::Texture t7v1;
	sf::Texture t7v2;

	//Restart Button
	sf::Texture t8v1;
	sf::Texture t8v2;
	

	sf::Texture sprite_table;

	sf::Texture explosion;

	sf::Texture explosion2;

	sf::Texture explosion3;
	
	sf::Texture boss_texture;

	sf::Texture boss_shoot_texture;
	
	sf::Texture energyBall_texture;
	
	sf::Texture bossHealthBar_texture;
	
	sf::Texture bossHealthBarMax_texture;
	
	
	

	//------------------ANIMATIONS------------------//
	Animation sWalk(sprite_table, 5, 230, 53, 57, 10, 0.5);
	Animation sEnergyBall(energyBall_texture, 20, 40, 110, 90, 9, 0.2);
	Animation sExplosion_3(explosion, 0, 0, 256, 256, 48, 0.5);
	Animation sExplosion_2(explosion2, 0, 0, 192, 192, 64, 0.5);
	


	void initializeTextures(sf::RenderWindow& window) {
		t0.loadFromFile("sprite_forwards.png");
		//t0 = createMask(t0);

		t1.loadFromFile("player_projectile.png");
		t2.loadFromFile("background.jpg");
		t3.loadFromFile("Enemy1.png");
		t3v2.loadFromFile("Enemy2.png");
		t3v3.loadFromFile("Enemy3.png");
		enemyDifficulty.push_back(t3);
		enemyDifficulty.push_back(t3v2);
		enemyDifficulty.push_back(t3v3);
		t4.loadFromFile("cursor.png");
		t4 = createMask(t4);

		t5.loadFromFile("pause_screen.png");
		t6v1.loadFromFile("resume1.png");
		t6v2.loadFromFile("resume2.png");
		t6v1 = createMask(t6v1);
		t6v2 = createMask(t6v2);
		
		t7v1.loadFromFile("quit1.png");
		t7v2.loadFromFile("quit2.png");
		t7v1 = createMask(t7v1);
		t7v2 = createMask(t7v2);
		
		t8v1.loadFromFile("restart1.png");
		t8v2.loadFromFile("restart2.png");
		t8v1 = createMask(t8v1);
		t8v2 = createMask(t8v2);
		
		sprite_table.loadFromFile("sprite_table.png");
		sprite_table = createMask(sprite_table);
		explosion.loadFromFile("type_C.png");
		explosion2.loadFromFile("type_B.png");
		explosion3.loadFromFile("type_A.png");
		boss_texture.loadFromFile("boss1.png");
		boss_shoot_texture.loadFromFile("boss2.png");
		energyBall_texture.loadFromFile("energyBall_straight.png");
		bossHealthBar_texture.loadFromFile("health_bar.png");
		bossHealthBarMax_texture.loadFromFile("health_bar_max.png");
		
		
		playerDirections.push_back(Animation(sprite_table, 5, 344, 53, 57, 10, 0.2));//up
		playerDirections.push_back(Animation(sprite_table, 5, 230, 53, 57, 10, 0.2));//down
		playerDirections.push_back(Animation(sprite_table, 5, 290, 53, 52, 10, 0.2));//left
		playerDirections.push_back(Animation(sprite_table, 5, 401, 53, 57, 10, 0.2));//right
	}