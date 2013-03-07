#ifndef MAPH
#define MAPH

#define SFML_STATIC
#include <SFML/Graphics.hpp>

namespace rts {
	const int TILE_W = 160;
	const int TILE_H = 80;

	const int HEIGHT_MULTIPLIER = 30;

	class Tile {
		int height;
		sf::Sprite *img;
	public:
		Tile(){};
		Tile(sf::Sprite*, int=0);

		int getHeight();
		void setHeight(int);

		sf::Sprite* getImage();
		void setImage(sf::Sprite*);
	};

	class Map {
		long w, h; // width and height needed to display the entire map
		long viewx, viewy; // topleftmost pixel of the view
		int tileCount; // array is tileCount x tileCount

		sf::Sprite grass;

		Tile **tiles;
	public:
		Map(){};
		Map(int);

		void initMap(int);

		Tile getTile(int,int);
		void setTile(int,int,Tile);
		void setTile(int,int,sf::Sprite*,int=0);

		void setView(long,long);
		void changeView(long,long);

		void Render(sf::RenderWindow*,int=0);
	};
};

#endif