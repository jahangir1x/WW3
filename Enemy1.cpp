#include "Enemy1.hpp" // fix level with intervals
#include "Collision.h"
#include "GameHandler.hpp"
#include "LevelFailed.hpp"
#include "helperFunc.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <bits/stdc++.h>
using namespace std;
using namespace sf;

Enemy1::Enemy1(RenderWindow& window)
{
	srand(time(NULL));
	windowSizeX = window.getSize().x;
	windowSizeY = window.getSize().y;

	enemy1Texture.loadFromFile("res/enemy1_spritesheet.png");
	enemy1Sprite.setTexture(enemy1Texture);
	rect.left = 0;
	rect.top = 0;
	rect.height = 118;
	rect.width = 88;
	enemy1Sprite.setTextureRect(rect);
	enemy1Sprite.setPosition(windowSizeX / 2 - rect.width / 2, windowSizeY - rect.height - 30);

	explosion.texture.loadFromFile("res/explosion.png");
	explosion.sprite.setTexture(explosion.texture);
	explosion.rect.left = 0;
	explosion.rect.top = 0;
	explosion.rect.height = 65;
	explosion.rect.width = 51;
	explosion.sprite.setTextureRect(explosion.rect);

	bigExplosion.sprite.setTexture(explosion.texture);
	bigExplosion.rect.left = 0;
	bigExplosion.rect.top = 0;
	bigExplosion.rect.height = 65;
	bigExplosion.rect.width = 51;
	bigExplosion.sprite.setTextureRect(explosion.rect);

	healthValue = 100;
	bulletInterval = 0.8;
	missileInterval = 1.5;
	isDead = false;
	moveInit = false;
	moveFin = false;
	moveLeftFin = false;
	moveRightFin = false;
	shouldDisappear = false;
	shouldExplode = false;
	bulletDamage = 5;
	missileDamage = 8;

	bulletLeft.speed = 500;
	bulletLeft.texture.loadFromFile("res/enemy1_bullet.png");
	bulletLeft.sprite.setTexture(bulletLeft.texture);

	bulletRight.speed = 500;
	bulletRight.texture.loadFromFile("res/enemy1_bullet.png");
	bulletRight.sprite.setTexture(bulletRight.texture);

	missile.speed = 400;
	missile.texture.loadFromFile("res/enemy1_missile.png");
	missile.sprite.setTexture(missile.texture);
	missile.missileCount = 9;
}

void Enemy1::Show(RenderWindow& window)
{
	if (enemy1Clock.getElapsedTime().asSeconds() > 0.2)
	{
		if (rect.left == 264)
		{
			rect.left = 0;
		}
		else
		{
			rect.left += 88;
		}
		// cout << "left: " << playerRect.left << endl;
		enemy1Sprite.setTexture(enemy1Texture);
		enemy1Sprite.setTextureRect(rect);
		enemy1Clock.restart();
	}

	for (i = 0; i < bulletsLeft.size(); i++)
	{
		bulletsLeft[i].sprite.move(0, bulletsLeft[i].speed * gamehandler.getElapsedTime());
		if (bulletsLeft[i].sprite.getPosition().x < 0 || bulletsLeft[i].sprite.getPosition().x > windowSizeX || bulletsLeft[i].sprite.getPosition().y < 0 || bulletsLeft[i].sprite.getPosition().y > windowSizeY)
		{
			bulletsLeft.erase(bulletsLeft.begin() + i);
		}
		bulletsRight[i].sprite.move(0, bulletsRight[i].speed * gamehandler.getElapsedTime());
		if (bulletsRight[i].sprite.getPosition().x < 0 || bulletsRight[i].sprite.getPosition().x > windowSizeX || bulletsRight[i].sprite.getPosition().y < 0 || bulletsRight[i].sprite.getPosition().y > windowSizeY)
		{
			bulletsRight.erase(bulletsRight.begin() + i);
		}
	}

	for (i = 0; i < bulletsLeft.size(); i++)
	{
		window.draw(bulletsLeft[i].sprite);
		window.draw(bulletsRight[i].sprite);
	}

	for (i = 0; i < missiles.size(); i++)
	{
		missiles[i].sprite.move(0, missiles[i].speed * gamehandler.getElapsedTime());
		if (missiles[i].sprite.getPosition().x < 0 || missiles[i].sprite.getPosition().x > windowSizeX || missiles[i].sprite.getPosition().y < 0 || missiles[i].sprite.getPosition().y > windowSizeY)
		{
			missiles.erase(missiles.begin() + i);
		}
	}

	for (i = 0; i < missiles.size(); i++)
	{
		window.draw(missiles[i].sprite);
	}

	// cout << "explosions: " << explosions.size() << endl;

	// cout << "missile: " << missiles.size() << endl;
	// cout << "bullets: " << bulletsLeft.size() << endl;
	if (!shouldDisappear)
		window.draw(enemy1Sprite);

	if (shouldExplode)
	{
		if (bigExplosion.rect.left >= 255)
		{
			shouldDisappear = true;
		}
		if (bigExplosionClock.getElapsedTime().asSeconds() > 0.2)
		{
			if(bigExplosion.rect.left <= 306) bigExplosion.rect.left += 51;
			bigExplosion.sprite.setTextureRect(bigExplosion.rect);
			window.draw(bigExplosion.sprite);
			bigExplosionClock.restart();
		}
		window.draw(bigExplosion.sprite);
	}
	// cout << "rect left: " << bigExplosion.rect.left << endl;

	for (i = 0; i < explosions.size(); i++)
	{
		if (explosions[i].rect.left >= 306)
		{
			explosions.erase(explosions.begin() + i);
		}
		if (explosions[i].clock.getElapsedTime().asSeconds() > 0.2)
		{
			explosions[i].rect.left += 51;
			// cout << "expl rect: " << i << " " << explosions[i].rect.left << endl;
			// explosions[i].sprite.setTexture(explosion.texture);
			explosions[i].sprite.setTextureRect(explosions[i].rect);
			// window.draw(explosions[i].sprite);
			explosions[i].clock.restart();
		}
		// window.draw(explosions[i].sprite);
	}
	for (i = 0; i < explosions.size(); i++)
	{
		window.draw(explosions[i].sprite);
	}
}

void Enemy1::isHit(Sprite& targetSprite, unsigned int id, float damage, bool should_disappear_after_hit)
{
	if (healthValue > 0)
	{
		if (Collision::BoundingBoxTest(enemy1Sprite, targetSprite))
		{
			if (should_disappear_after_hit == false)
			{
				if (hitClock.getElapsedTime().asMilliseconds() > 1700)
				{
					if (Collision::PixelPerfectTest(enemy1Sprite, targetSprite))
					{
						cout << "hit non disappered:" << id << endl;
						healthValue -= damage;

						cout << "enemy1 health: " << healthValue << endl;
					}
					hitClock.restart();
				}
			}
			else if (find(prevCollidedObj.begin(), prevCollidedObj.end(), id) == prevCollidedObj.end())
			{
				if (Collision::PixelPerfectTest(enemy1Sprite, targetSprite))
				{
					prevCollidedObj.push_back(id);
					cout << "hit: " << id << endl;
					show_explosion(targetSprite.getPosition());
					healthValue -= damage;
					cout << "enemy1 health: " << healthValue << endl;
					if (should_disappear_after_hit)
					{
						targetSprite.setColor(Color(0, 0, 0, 0));
					}
				}
			}
		}
	}
	else
	{
		isDead = true;
		Die();
	}
}

void Enemy1::Die()
{
	cout << "enemy dead" << endl;
	bigExplosion.sprite.setOrigin(25.5, 32.5);
	bigExplosion.sprite.setPosition(enemy1Sprite.getGlobalBounds().left + enemy1Sprite.getGlobalBounds().width / 2, enemy1Sprite.getGlobalBounds().top + enemy1Sprite.getGlobalBounds().height / 2);
	bigExplosion.sprite.setScale(3, 3);
	shouldExplode = true;
}

void Enemy1::fireBullet(int interval_milliseconds, int interval_offset, float speed)
{
	if (!isDead)
	{
		if (bulletClock.getElapsedTime().asMilliseconds() > helperFunc::randRange(interval_milliseconds - interval_offset, interval_milliseconds + interval_offset))
		{
			bulletLeft.sprite.setPosition(enemy1Sprite.getPosition().x + 16, enemy1Sprite.getPosition().y + 80);
			bulletLeft.speed = speed;
			bulletLeft.id = rand() + rand() + rand();
			cout << "enemyleftbullet: " << bulletLeft.id << endl;
			bulletsLeft.push_back(bulletLeft);
			bulletRight.sprite.setPosition(enemy1Sprite.getPosition().x + 66, enemy1Sprite.getPosition().y + 80);
			bulletRight.speed = speed;
			bulletRight.id = rand() + rand() + rand();
			cout << "enemyrightbullet: " << bulletRight.id << endl;
			bulletsRight.push_back(bulletRight);
			bulletClock.restart();
			// cout << "enemy1 fired bullets" << endl;
		}
	}
}

void Enemy1::fireMissile(int interval_milliseconds, int interval_offset, float speed)
{
	if (!isDead)
	{
		if (missile.missileCount > 0)
		{
			if (missileClock.getElapsedTime().asMilliseconds() > helperFunc::randRange(interval_milliseconds - interval_offset, interval_milliseconds + interval_offset))
			{
				missile.sprite.setPosition(enemy1Sprite.getPosition().x + rect.width / 2 - 5, enemy1Sprite.getPosition().y + 40);
				missile.speed = speed;
				missile.id = rand() + rand() + rand();
				cout << "enemymissile: " << missile.id << endl;
				missiles.push_back(missile);
				missileClock.restart();
				missile.missileCount--;
				// cout << "enemy1 fired missile" << endl;
			}
		}
	}
}

void Enemy1::fireBullet_to(Vector2f targetPos, int interval_milliseconds, int interval_offset, float speed, int offset)
{
	if (bulletClock.getElapsedTime().asMilliseconds() > helperFunc::randRange(interval_milliseconds - interval_offset, interval_milliseconds + interval_offset))
	{
		bulletOriginPosLeft = Vector2f(enemy1Sprite.getPosition().x + 16, enemy1Sprite.getPosition().y + 68);
		bulletLeft.sprite.setPosition(bulletOriginPosLeft);
		// bulletsLeft.push_back(bulletLeft);

		bulletOriginPosRight = Vector2f(enemy1Sprite.getPosition().x + 66, enemy1Sprite.getPosition().y + 68);

		bulletRight.sprite.setPosition(bulletOriginPosRight);
		// bulletsRight.push_back(bulletRight);
		cout << "enemy fired bullet" << endl;
		bulletClock.restart();
	}
	for (i = 0; i < bulletsLeft.size(); i++)
	{
		shootPos.x = helperFunc::randRange(targetPos.x - offset, targetPos.x + offset);
		shootPos.y = helperFunc::randRange(targetPos.y - offset, targetPos.y + offset);

		shootBulletTemp = helperFunc::getNormalizedVector(shootPos, bulletOriginPosLeft);
		shootBulletTemp.x *= gamehandler.getElapsedTime() * speed;
		shootBulletTemp.y *= gamehandler.getElapsedTime() * speed;

		bulletsLeft[i].sprite.move(shootBulletTemp);

		if (bulletsLeft[i].sprite.getPosition().x < 0 || bulletsLeft[i].sprite.getPosition().x > windowSizeX || bulletsLeft[i].sprite.getPosition().y < 0 || bulletsLeft[i].sprite.getPosition().y > windowSizeY)
		{
			bulletsLeft.erase(bulletsLeft.begin() + i);
		}

		shootBulletTemp = helperFunc::getNormalizedVector(shootPos, bulletOriginPosRight);
		shootBulletTemp.x *= gamehandler.getElapsedTime() * speed;
		shootBulletTemp.y *= gamehandler.getElapsedTime() * speed;

		bulletsRight[i].sprite.move(shootBulletTemp); // need to calculate vector shoot pos according to left

		if (bulletsRight[i].sprite.getPosition().x < 0 || bulletsRight[i].sprite.getPosition().x > windowSizeX || bulletsRight[i].sprite.getPosition().y < 0 || bulletsRight[i].sprite.getPosition().y > windowSizeY)
		{
			bulletsRight.erase(bulletsRight.begin() + i);
		}
	}
}

void Enemy1::fireMissile_to(Vector2f targetPos, int interval_milliseconds, int interval_offset, float speed, int offset)
{
	if (missile.missileCount > 0)
	{
		if (missileClock.getElapsedTime().asMilliseconds() > helperFunc::randRange(interval_milliseconds - interval_offset, interval_milliseconds + interval_offset))
		{
			missileOriginPos = Vector2f(enemy1Sprite.getPosition().x + rect.width / 2 - 5, enemy1Sprite.getPosition().y + 20);
			missile.sprite.setPosition(missileOriginPos);
			// missiles.push_back(missile);
			missile.missileCount--;
			missileClock.restart();
			cout << "enemy fired missile" << endl;
		}

		for (i = 0; i < missiles.size(); i++)
		{
			shootPos.x = helperFunc::randRange(targetPos.x - offset, targetPos.x + offset);
			shootPos.y = helperFunc::randRange(targetPos.y - offset, targetPos.y + offset);

			shootMissileTemp = helperFunc::getNormalizedVector(shootPos, missileOriginPos);
			shootMissileTemp.x *= gamehandler.getElapsedTime() * speed;
			shootMissileTemp.y *= gamehandler.getElapsedTime() * speed;

			missiles[i].sprite.move(shootMissileTemp);
			if (missiles[i].sprite.getPosition().x < 0 || missiles[i].sprite.getPosition().x > windowSizeX || missiles[i].sprite.getPosition().y < 0 || missiles[i].sprite.getPosition().y > windowSizeY)
			{
				missiles.erase(missiles.begin() + i);
			}
		}
	}
}

void Enemy1::moveDown(float speed)
{
	tempVect = helperFunc::getNormalizedVector(Vector2f(enemy1Sprite.getPosition().x, windowSizeY + 100), Vector2f(enemy1Sprite.getPosition().x, enemy1Sprite.getPosition().y));
	tempVect.x *= gamehandler.getElapsedTime() * speed;
	tempVect.y *= gamehandler.getElapsedTime() * speed;

	// cout << "enemy1 moved down" << endl;
	enemy1Sprite.move(tempVect);
}

void Enemy1::moveUp(float speed)
{
	tempVect = helperFunc::getNormalizedVector(Vector2f(enemy1Sprite.getPosition().x, -windowSizeY), Vector2f(enemy1Sprite.getPosition().x, enemy1Sprite.getPosition().y));
	tempVect.x *= gamehandler.getElapsedTime() * speed;
	tempVect.y *= gamehandler.getElapsedTime() * speed;

	// cout << "enemy1 moved up" << endl;
	enemy1Sprite.move(tempVect);
}

void Enemy1::moveLeft(float speed)
{
	tempVect = helperFunc::getNormalizedVector(Vector2f(-windowSizeX, enemy1Sprite.getPosition().y), Vector2f(enemy1Sprite.getPosition().x, enemy1Sprite.getPosition().y));
	tempVect.x *= gamehandler.getElapsedTime() * speed;
	tempVect.y *= gamehandler.getElapsedTime() * speed;
	// cout << "temp: " << tempVect.x << " " << tempVect.y << endl;

	enemy1Sprite.move(tempVect);
	for (i = 0; i < explosions.size(); i++)
	{
		explosions[i].sprite.move(tempVect);
	}
	// cout << "enemy1 moved left" << endl;
}

void Enemy1::moveRight(float speed)
{
	tempVect = helperFunc::getNormalizedVector(Vector2f(windowSizeX + 100, enemy1Sprite.getPosition().y), Vector2f(enemy1Sprite.getPosition().x, enemy1Sprite.getPosition().y));
	tempVect.x *= gamehandler.getElapsedTime() * speed;
	tempVect.y *= gamehandler.getElapsedTime() * speed;

	enemy1Sprite.move(tempVect);
	for (i = 0; i < explosions.size(); i++)
	{
		explosions[i].sprite.move(tempVect);
	}
	// cout << "enemy1 moved right" << endl;
}

void Enemy1::move_to(Vector2f targetPos, float speed, int offset)
{
	if (!isDead)
	{
		// if (moveClock.getElapsedTime().asMilliseconds() > helperFunc::randRange(interval_milliseconds - interval_offset, interval_milliseconds + interval_offset))
		if (moveInit == false)
		{
			movePos.x = helperFunc::randRange(targetPos.x - offset, targetPos.x + offset);
			movePos.y = helperFunc::randRange(targetPos.y - offset, targetPos.y + offset);
			// cout << "init: " << movePos.x << " " << movePos.y << endl;
			// cout << interval_milliseconds << " " << interval_offset << endl;
			moveNorm = helperFunc::getNormalizedVector(movePos, enemy1Sprite.getPosition());
			moveNorm.x *= gamehandler.getElapsedTime() * speed;
			moveNorm.y *= gamehandler.getElapsedTime() * speed;
			// cout << "initm: " << moveNorm.x << " " << moveNorm.y << endl;
			enemy1Sprite.move(moveNorm);
			for (i = 0; i < explosions.size(); i++)
			{
				explosions[i].sprite.move(moveNorm);
			}
			moveClock.restart();
			moveInit = true;
		}
		else
		{
			if (enemy1Sprite.getGlobalBounds().contains(movePos) == false && moveFin == false)
			{
				moveNorm = helperFunc::getNormalizedVector(movePos, enemy1Sprite.getPosition());
				moveNorm.x *= gamehandler.getElapsedTime() * speed;
				moveNorm.y *= gamehandler.getElapsedTime() * speed;
				enemy1Sprite.move(moveNorm);
				for (i = 0; i < explosions.size(); i++)
				{
					explosions[i].sprite.move(moveNorm);
				}
			}
			if (enemy1Sprite.getGlobalBounds().contains(movePos) == true && moveFin == false)
			{
				moveFin = true;
				moveRightFin = true;
			}
			if (moveFin == true)
			{
				if (moveLeftFin == false)
				{
					moveLeft(speed);
				}
				if (enemy1Sprite.getGlobalBounds().contains(0, enemy1Sprite.getGlobalBounds().top) == true)
				{
					moveLeftFin = true;
					moveRightFin = false;
				}
				if (moveRightFin == false)
				{
					moveRight(speed);
				}
				if (enemy1Sprite.getGlobalBounds().contains(windowSizeX, enemy1Sprite.getGlobalBounds().top) == true)
				{
					moveLeftFin = false;
					moveRightFin = true;
				}
			}
		}
		// cout << "init: " << movePos.x << " " << movePos.y << endl;
		// cout << "enem: " << enemy1Sprite.getPosition().x << " " << enemy1Sprite.getPosition().y << endl;
	}
}

void Enemy1::show_explosion(Vector2f pos)
{
	explosion.sprite.setOrigin(25.5, 32.5);
	explosion.sprite.setPosition(pos);
	explosions.push_back(explosion);
}