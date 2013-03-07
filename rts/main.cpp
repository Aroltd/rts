#define SFML_STATIC

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "sockwrap.h"
#include "Map.h"

int main() {
	//sf::RenderWindow rw(sf::VideoMode::getDesktopMode(), "test", sf::Style::Fullscreen);
	sf::RenderWindow rw(sf::VideoMode(800, 800), "test");
	rw.setFramerateLimit(60);

	rts::Map gameMap(32);

	long mclk = 0;
	sf::Clock uclk;

	while(rw.isOpen()) {
        sf::Event event;
        while (rw.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                rw.close();
			if (event.type == sf::Event::Resized)
				rw.setSize(sf::Vector2u(event.size.width,event.size.height));
        }
		
		rw.clear();
		
		if(true) {
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
				gameMap.changeView(0,-4);

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
				gameMap.changeView(0,4);

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
				gameMap.changeView(-4,0);

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
				gameMap.changeView(4,0);
		}

		sf::RectangleShape test(sf::Vector2f(400,400));
		test.setFillColor(sf::Color(0,0,0,0));
		test.setOutlineColor(sf::Color(255,255,255));
		test.setOutlineThickness(4);
		test.setPosition(200,200);

		gameMap.Render(&rw,200);
		rw.draw(test);

		rw.display();
	}
}