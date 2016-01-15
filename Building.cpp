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
    int x1, x2, y1, y2;
    x1 = x + render_start.x;
    y1 = y + render_start.y;
    x2 = x + render_end.x;
    y2 = y + render_end.y;
    thing[0].position.x = square_size * x1;
    thing[0].position.y = square_size * y1;
    thing[1].position.x = square_size * x2;
    thing[1].position.y = square_size * y1;
    thing[2].position.x = square_size * x2;
    thing[2].position.y = square_size * y2;
    thing[3].position.x = square_size * x1;
    thing[3].position.y = square_size * y2;

}
sf::IntRect Building::getTextureRect()const{
	sf::IntRect f;
	f.top = ty1;
	f.left = tx1;
	f.width = tx2 - tx1;
	f.height = ty2 - ty1;
	return f;
}
Building Building::fromPot(int number, GetPot &conf) {
    std::stringstream z;

    z << "buildings/" << number << "/";
    conf.set_prefix(z.str().c_str());
    z.str("");
    z << "Buildingname";

    std::string bname = conf(z.str().c_str(), "");

    std::string blocking;
    std::cout << "Building_name:" << bname << std::endl;
    z.str("");
    z << "key_sequence";

    std::string keySeq = conf(z.str().c_str(), "");
    std::cout << "Key sequence:" << keySeq << std::endl;
    z.str("");
    z << "size_x";
    int sx = conf(z.str().c_str(), 0);
    z.str("");
    z << "size_y";
    int sy = conf(z.str().c_str(), 0);
    z.str("");
    z << "center_x";
    int cx = conf(z.str().c_str(), 0);
    z.str("");
    z << "center_y";
    int cy = conf(z.str().c_str(), 0);
    z.str("");
    z << "render_start_x";
    int rx1 = conf(z.str().c_str(), sx);
    z.str("");
    z << "render_start_y";
    int ry1 = conf(z.str().c_str(), sy);
    z.str("");
    z << "render_end_x";
    int rx2 = conf(z.str().c_str(), sx);
    z.str("");
    z << "render_end_y";
    int ry2 = conf(z.str().c_str(), sy);
    z.str("");
    z << "texturecoords/X1";

    int tx1 = conf(z.str().c_str(), 0);
    z.str("");
    z << "texturecoords/Y1";
    int ty1 = conf(z.str().c_str(), 0);
    z.str("");
    z << "texturecoords/X2";
    int tx2 = conf(z.str().c_str(), 0);
    z.str("");
    z << "texturecoords/Y2";
    int ty2 = conf(z.str().c_str(), 0);
    z.str();
    z << "/blocking";
    blocking = conf(z.str().c_str(), "0");

    Building f(bname, keySeq, sf::Vector2i(sx, sy), sf::Vector2i(cx, cy), tx1, ty1, tx2, ty2);
    f.passable.resize(sx * sy);
    for (int i = 0; i < blocking.size(); i++)
        f.passable.push_back(blocking[i] == '1' ? true : false);
    z << "buildings/" << number << "/";
    f.render_start = sf::Vector2i(rx1, ry1);
    f.render_end = sf::Vector2i(rx2, ry2);
    conf.set_prefix("");
    return f;
}
