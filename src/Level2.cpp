#include "Level2.hpp" // level header file
#include "GameUI.hpp"
#include "Success.hpp"
using namespace std;
using namespace sf;

void Level2::Show(RenderWindow& window, Event& event)
{
	while (LevelHelper::shouldKeepPlaying())
	{
		////// mandatory //////
		cout << "in level 3 " << endl;
		Success success;
		LevelFailed levelFailedObj;
		LevelHelper levelhelp;
		GameUI::init();
		Helper::resetEnemyDiedCounter();
		Player::resetMissileCounter();
		Background background;
		Player yuri;

		CustomText custext1;
		CustomText custext2;
		////// mandatory //////

		bool someone_is_alive;
		vector<Enemy2> first_enemies(3);
		vector<Enemy3> second_enemies(4);
		for (auto& enemy : first_enemies)
		{
			enemy.setStyle(Enemy1::Style(2));
		}
		for (auto& enemy : second_enemies)
		{
			enemy.setStyle(Enemy1::Style(3));
		}
		while (window.isOpen())
		{
			Helper::resetClock();
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
				{
					window.close();
					exit(0);
				}

				else if (event.type == Event::MouseButtonReleased)
				{
					if (event.mouseButton.button == Mouse::Left)
					{
						yuri.startFiringBullet();
						GameUI::handleClose(window, window.mapPixelToCoords(Mouse::getPosition(window)));
						success.handleClose(window.mapPixelToCoords(Mouse::getPosition(window)));
						levelFailedObj.handleClose(window.mapPixelToCoords(Mouse::getPosition(window)));
					}
				}
			}
			window.clear(Color::Blue);
			background.Show(window);
			if (Mouse::isButtonPressed(Mouse::Right))
			{
				yuri.fireMissile();
			}
			yuri.fireBullet();
			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				yuri.moveLeft();
			}
			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				yuri.moveRight();
			}
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				yuri.moveUp();
			}
			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				yuri.moveDown();
			}

			someone_is_alive = false;

			for (auto& this_enemy : first_enemies)
			{
				if (this_enemy.isDead == false)
				{
					custext1.Show(window, "Wave 1", 80, 200, 200, 2, true, 0.1);
					if (someone_is_alive == false)
						someone_is_alive = true;
					this_enemy.move(307);
					this_enemy.fireBullet(yuri, 6300, 5800, 560);
					this_enemy.fireMissile(yuri, 5000, 2000, 378);
					levelhelp.isHitBody(yuri, this_enemy);
					levelhelp.isHitBullet(yuri, this_enemy);
					levelhelp.isHitMissile(yuri, this_enemy);
					this_enemy.Show(window);
				}
			}

			if (someone_is_alive == false)
			{
				for (auto& this_enemy : second_enemies)
				{
					if (this_enemy.isDead == false)
					{
						if (custext1.hidingFinished == true)
						{
							custext2.Show(window, "Wave 2", 80, 200, 200, 2, true, 0.1);
						}
						if (someone_is_alive == false)
							someone_is_alive = true;
						this_enemy.move(307);
						this_enemy.fireBullet(yuri, 6300, 5800, 560);
						this_enemy.fireMissile(yuri, 5000, 3000, 380);
						levelhelp.isHitBody(yuri, this_enemy);
						levelhelp.isHitBullet(yuri, this_enemy);
						levelhelp.isHitMissile(yuri, this_enemy);
						this_enemy.Show(window);
					}
				}
			}

			yuri.Show(window);

			if (yuri.isDead)
			{
				if (levelFailedObj.isFinishedShowing(window))
				{
					break;
				}
			}

			if (Helper::enemiesDied() == 7)
			{
				if (success.isFinishedShowing(window))
				{
					return;
				}
			}

			GameUI::showPlayerUI(window);

			window.display();
		}
	}
}
