#include "Map.h" 

rts::Tile::Tile(sf::Sprite *spr, int h) {
	this->img = spr;
	this->height = h;
}

int rts::Tile::getHeight() {
	return height;
}

void rts::Tile::setHeight(int h) {
	this->height = h;
}

sf::Sprite* rts::Tile::getImage() {
	return img;
}

void rts::Tile::setImage(sf::Sprite *nimg) {
	this->img = nimg;
}

rts::Map::Map(int nTiles) {
	this->initMap(nTiles);
}

void rts::Map::initMap(int nTiles) {
	this->tileCount = nTiles;

	this->tiles = new Tile*[nTiles];
	for(int i = 0; i < nTiles; i++)
		this->tiles[i] = new Tile;

	static sf::Texture tmp;
	tmp.loadFromFile("grid.png");
	grass.setTexture(tmp);

	this->viewx = viewy = 0;
	this->w = nTiles * TILE_W;
	this->h = nTiles * TILE_H;
}

rts::Tile rts::Map::getTile(int i, int j) {
	return tiles[i][j];
}

void rts::Map::setTile(int i, int j, Tile t) {
	tiles[i][j] = t;
}

void rts::Map::setTile(int i, int j, sf::Sprite *s, int h) {
	tiles[i][j] = Tile(s, h);
}

void rts::Map::setView(long x, long y) {
	viewx = x;
	viewy = y;
}

void rts::Map::changeView(long x, long y) {
	if(viewx + x >= 0 && viewx + x <= w)
		viewx += x;
	else
		viewx = (viewx+x<0)?0:w;

	if(viewy + y >= 0 && viewy + y <= h)
		viewy += y;
	else
		viewy = (viewy+y<0)?0:h;
}

void rts::Map::Render(sf::RenderWindow *rw) {
	int cw = rw->getSize().x;
	int ch = rw->getSize().y;

	for(int y = 0; y < tileCount; y++) {
		for(int x = 0; x < tileCount; x++) {

			long tx0 = (((w/2)-(TILE_W/2)) + ((TILE_W/2)*x) - ((TILE_W/2)*y)),
			     ty0 = ((TILE_H/2)*y+(TILE_H/2)*x),
				 tx1 = tx0 + TILE_W,
				 ty1 = tx1 + TILE_H;

			//if((ty0 >= viewy && ty1 <= viewy+ch) && (tx1 >= viewx && tx0 <= viewx+cw)) {
				grass.setPosition(tx0-viewx, ty0-viewy);
				rw->draw(grass);
			//}
		}
	}
}