//
// Created by awhite on 1/2/16.
//

#include "Building.h"
#include <sstream>

Building::Building(const std::string &name, const std::string &keyseq, sf::Vector2i size, sf::Vector2i center,
                   int x1, int y1, int x2, int y2)
        : name(name), key_sequence(keyseq), size(size), center(center),
          tx1(x1), tx2(x2), ty1(y1), ty2(y2) {

}

Building::~Building() {

}

bool Building::canPlace(int x, int y, char open) const {
    //TODO use this to verify that the building can be placed on the tile
    return false;
}

std::string Building::getSequence() const {
    return key_sequence;
}

std::string Building::getName() const {
    return name;
}

sf::Vector2i Building::getSize() const {
    return size;
}

sf::Vector2i Building::getCenter() const {
    return center;
}

void Building::getTexCoords(sf::Vertex *thing) {
    thing[0].texCoords.y = ty1;
    thing[0].texCoords.x = tx1;
    thing[1].texCoords.y = ty1;
    thing[1].texCoords.x = tx2;
    thing[2].texCoords.y = ty2;
    thing[2].texCoords.x = tx2;
    thing[3].texCoords.y = ty2;
    thing[3].texCoords.x = tx1;


}

void Building::getAdjustedCoords(int x, int y, int square_size, sf::Vertex *thing) {
    auto f = getSize();
    auto c = getCenter();
    int x1 = x + c.x - f.x;
    int y1 = y + c.y - f.y;
    int x2 = x + (f.x - c.x);
    int y2 = y + (f.y - c.y);
    if(size.x==1&&size.y==1){
        x1=x;
        x2=x+1;
        y1=y;
        y2=y1+1;
    }
	if (size.x == 3 && size.y == 3){
		x1 = x - 1;
		x2 = x + 2;//No idea why this produces a reasonably sized sprite.
		y1 = y - 1;// Probably because it's already x+1 for the normal tile, so it needs another addition to bump it into the right range.
		y2 = y + 3;
	}
    thing[0].position.x = square_size * x1;
    thing[0].position.y = square_size * y1;
    thing[1].position.x = square_size * x2;
    thing[1].position.y = square_size * y1;
    thing[2].position.x = square_size * x2;
    thing[2].position.y = square_size * y2;
    thing[3].position.x = square_size * x1;
    thing[3].position.y = square_size * y2;

}

Building Building::fromPot(int number, GetPot &conf) {
    std::stringstream z;
    z << "buildings/" << number << "/" << "Buildingname";
    std::string curbuild = std::to_string(number);
    std::string bname = conf(z.str().c_str(), "");
	std::string blocking;
    std::cout << "Building_name:" << bname << std::endl;
    z.str("");
    z << "buildings/" << number << "/" << "key_sequence";

    std::string keySeq = conf(z.str().c_str(), "");
    std::cout << "Key sequence:" << keySeq << std::endl;
    z.str("");
    z << "buildings/" << number << "/" << "size_x";
    int sx = conf(z.str().c_str(), 0);
    z.str("");
    z << "buildings/" << number << "/" << "size_y";
    int sy = conf(z.str().c_str(), 0);
    z.str("");
    z << "buildings/" << number << "/" << "center_x";
    int cx = conf(z.str().c_str(), 0);
    z.str("");
    z << "buildings/" << number << "/" << "center_y";
    int cy = conf(z.str().c_str(), 0);
    z.str("");
    z << "buildings/" << number << "/" << "texturecoords/X1";

    int tx1 = conf(z.str().c_str(), 0);
    z.str("");
    z << "buildings/" << number << "/" << "texturecoords/Y1";
    int ty1 = conf(z.str().c_str(), 0);
    z.str("");
    z << "buildings/" << number << "/" << "texturecoords/X2";
    int tx2 = conf(z.str().c_str(), 0);
    z.str("");
    z << "buildings/" << number << "/" << "texturecoords/Y2";
    int ty2 = conf(z.str().c_str(), 0);
	z.str();
	z << "buildings/" << number << "/blocking";
	blocking = conf(z.str().c_str(),"0");

    Building f(bname,keySeq,sf::Vector2i(sx,sy),sf::Vector2i(cx,cy),tx1,ty1,tx2,ty2);
	f.passable.resize(sx*sy);
	for (int i = 0; i < blocking.size(); i++)
		f.passable.push_back(blocking[i] == '1' ? true : false);
	return f;
}
