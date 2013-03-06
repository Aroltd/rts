#define SFML_STATIC

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "sockwrap.h"
#include "Map.h"

int main() {
	sf::RenderWindow rw(sf::VideoMode(1024, 768), "test");
	rw.setFramerateLimit(60);

	rts::Map gameMap(24);

	long clk = 0;

	while(rw.isOpen()) {
        sf::Event event;
        while (rw.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                rw.close();
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

		gameMap.Render(&rw);

		rw.display();
	}
}