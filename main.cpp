#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>

float randFloat(float min, float max)
{
	return min + static_cast<float>(rand()) / RAND_MAX * (max - min);
}

class Star
{
public:
	sf::CircleShape circle;
	float angle;
	float distance;
	float speed;
	float brightOffset;

	Star(sf::CircleShape circle, float angle, float distance, float speed, float brightOffset) : circle(circle), angle(angle), distance(distance), speed(speed), brightOffset(brightOffset) {}
	sf::Color getColor(void) const { return circle.getFillColor(); }
	void setPos(float x, float y) { circle.setPosition(sf::Vector2f(x, y)); }
	void setColor(sf::Color color) { circle.setFillColor(color); }
};

class Planet
{
	sf::Sprite sprite;
	float scale;
	float orbitalRadius;
	float orbitalAngle;
	float orbitalSpeed;
public:
	Planet(sf::Texture& tex, float scale, float orbitalRadius, float orbitalSpeed) : scale(scale), orbitalRadius(orbitalRadius), orbitalAngle(0), orbitalSpeed(orbitalSpeed / 2)
	{
		sprite.setTexture(tex);
		sprite.setOrigin(tex.getSize().x / 2, tex.getSize().y / 2);
		sprite.setScale(scale, scale);
		sprite.setColor(sf::Color(255, 255, 255, 255));
	}

	void update()
	{
		orbitalAngle += orbitalSpeed;

		float x = cos(orbitalAngle) * orbitalRadius;
		float y = sin(orbitalAngle) * orbitalRadius;

		sprite.setPosition(sf::Vector2f(x, y));
	}

	void draw(sf::RenderWindow& window) { window.draw(sprite, sf::BlendAlpha); }

	float getOrbitRadius(void) const { return orbitalRadius; }
};

int main()
{
	srand(static_cast<unsigned>(time(0)));

	const int WIDTH = 1920;
	const int HEIGHT = 1080;

	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "NEKO GALAXY", sf::Style::Fullscreen);
	window.setFramerateLimit(60);

	sf::View view = window.getDefaultView();
	view.setCenter(0.f, 0.f);
	view.zoom(1.1f);
	window.setView(view);

	const int ARM_COUNT = 4;
	const float SPIRAL_TIGHTNESS = 0.5f;
	const int STAR_COUNT = 3000;
	const float M_PI = 3.141592653589;

	std::vector<Star> stars;

	for (int i = 0; i < STAR_COUNT; ++i)
	{
		float arm = static_cast<float>(rand() % ARM_COUNT);
		float angle = randFloat(0, 2 * M_PI) + arm * (2 * M_PI / ARM_COUNT);
		float distance = randFloat(50.f, 400.f);

		float spread = randFloat(-15.f, 15.f);
		float x = cos(angle) * (SPIRAL_TIGHTNESS * angle * distance) + spread;
		float y = sin(angle) * (SPIRAL_TIGHTNESS * angle * distance) + spread;

		sf::CircleShape c(randFloat(0.5f, 2.f));

		c.setFillColor(sf::Color(200 + rand() % 55, 200 + rand() % 55, 200 + rand() % 55, 255));

		c.setPosition(sf::Vector2f(x, y));

		stars.push_back(Star(c, angle, distance, randFloat(0.001f, 0.005f), randFloat(0, 2 * M_PI)));
	}

	sf::Texture nebulaText;

	if (!nebulaText.loadFromFile("./sprites/nebula.png"))
	{
		return -1;
	}

	sf::Sprite nebula;
	nebula.setTexture(nebulaText);
	nebula.setOrigin(nebulaText.getSize().x / 2, nebulaText.getSize().y / 2);
	nebula.setPosition(0, 0);
	nebula.setScale(2.f, 2.f);
	nebula.setColor(sf::Color(255, 255, 255, 100));

	sf::Texture sunText;
	
	if (!sunText.loadFromFile("./sprites/sun.png"))
	{
		return -1;
	}

	sf::Sprite sun;
	sun.setTexture(sunText);
	sun.setOrigin(sunText.getSize().x / 2, sunText.getSize().y / 2);
	sun.setPosition(0, 0);
	sun.setScale(0.12f, 0.12f);

	sf::Texture mercuryTex;
	if (!mercuryTex.loadFromFile("./sprites/mercury.png")) return -1;

	sf::Texture venusTex;
	if (!venusTex.loadFromFile("./sprites/venus.png")) return -1;

	sf::Texture earthTex;
	if (!earthTex.loadFromFile("./sprites/earth.png")) return -1;

	sf::Texture marsTex;
	if (!marsTex.loadFromFile("./sprites/mars.png")) return -1;

	sf::Texture jupiterTex;
	if (!jupiterTex.loadFromFile("./sprites/jupiter.png")) return -1;

	sf::Texture saturnTex;
	if (!saturnTex.loadFromFile("./sprites/saturn.png")) return -1;

	sf::Texture uranusTex;
	if (!uranusTex.loadFromFile("./sprites/uranus.png")) return -1;

	sf::Texture neptuneTex;
	if (!neptuneTex.loadFromFile("./sprites/neptune.png")) return -1;

	std::vector<Planet> planets;

	planets.push_back(Planet(mercuryTex, 0.05f, 90.f, 0.03f));    // small, close
	planets.push_back(Planet(venusTex, 0.07f, 140.f, 0.022f));   // brighter
	planets.push_back(Planet(earthTex, 0.08f, 190.f, 0.017f));   // main
	planets.push_back(Planet(marsTex, 0.07f, 240.f, 0.013f));   // similar to earth
	planets.push_back(Planet(jupiterTex, 0.17f, 320.f, 0.009f));   // large
	planets.push_back(Planet(saturnTex, 0.15f, 400.f, 0.006f));   // also large
	planets.push_back(Planet(uranusTex, 0.11f, 480.f, 0.0045f));  // decent size
	planets.push_back(Planet(neptuneTex, 0.11f, 560.f, 0.0035f));  // furthest

	std::vector<sf::CircleShape> orbitRings;
	for (const auto& planet : planets)
	{
		sf::CircleShape ring(planet.getOrbitRadius());
		ring.setOrigin(planet.getOrbitRadius(), planet.getOrbitRadius());
		ring.setPosition(0.f, 0.f);
		ring.setFillColor(sf::Color::Transparent);
		ring.setOutlineColor(sf::Color(255, 255, 255, 40));  // Light white, semi-transparent
		ring.setOutlineThickness(1.f);
		orbitRings.push_back(ring);
	}


	sf::Clock clk;

	while (window.isOpen())
	{
		sf::Event ev;

		while (window.pollEvent(ev)) 
		{
			if (ev.type == sf::Event::Closed) window.close();
			if (ev.type == sf::Event::KeyPressed) if (ev.key.code == sf::Keyboard::Escape) window.close();
		}

		float t = clk.getElapsedTime().asSeconds();

		nebula.setRotation(sin(t * 0.5f) * 60.f);
		float scalePulse = sin(t * 0.8f) * 0.3f;
		nebula.setScale(3.f + scalePulse, 3.f + scalePulse);

		sf::Uint8 alpha = 150 + static_cast<sf::Uint8>(sin(t * 2.f) * 50.f);
		nebula.setColor(sf::Color(255, 100, 200, alpha));


		for (auto& star : stars) {
			star.angle += star.speed;
			float x = cos(star.angle) * (SPIRAL_TIGHTNESS * star.angle * star.distance);
			float y = sin(star.angle) * (SPIRAL_TIGHTNESS * star.angle * star.distance);
			star.circle.setPosition(x, y);

			sf::Color color = star.getColor();

			float brightness = 128 + sin(clk.getElapsedTime().asSeconds() * 5 + star.brightOffset) * 127;

			color.a = static_cast<uint8_t>(brightness);

			star.setColor(color);
		}

		for (auto& planet : planets)
			planet.update();

		

		window.clear();

		window.draw(nebula, sf::BlendAdd);
		window.draw(nebula, sf::BlendAdd);

		for (auto& star : stars)
			window.draw(star.circle);

		window.draw(sun, sf::BlendAdd);

		for (auto& ring : orbitRings)
			window.draw(ring);
		
		for (auto& planet : planets)
			planet.draw(window);

		window.display();
	}

	return 0;
}