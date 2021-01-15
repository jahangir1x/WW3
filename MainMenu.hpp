#ifndef MainMenu_air321
#define MainMenu_air321

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <bits/stdc++.h>
using namespace std;
using namespace sf;

class MainMenu
{
public:
	enum MenuResult
	{
		Nothing,
		Play,
		Help,
		Credits,
		Exit
	};

	MenuResult Show(RenderWindow& window, Event& menuEvent);

private:
	Vector2i mousePosition;
	bool clicked;
	MenuResult result;

	Texture gameLogoTex;
	Sprite gameLogoSprite;

	Texture playTex;
	IntRect playRect;
	Sprite playSprite;
	Clock playClock;

	Texture helpTex;
	IntRect helpRect;
	Sprite helpSprite;
	Clock helpClock;

	Texture creditsTex;
	IntRect creditsRect;
	Sprite creditsSprite;
	Clock creditsClock;

	Texture exitTex;
	IntRect exitRect;
	Sprite exitSprite;
	Clock exitClock;
};
#endif
