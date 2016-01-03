//
// Created by awhite on 1/2/16.
//

#include "Building.h"
#include <iostream>

Building::Building(const std::string &name, const std::string &keyseq, sf::Vector2i size, sf::Vector2i center,
                   int x1, int y1, int x2, int y2)
        : name(name), key_sequence(keyseq), size(size), center(center),
          tx1(x1), tx2(x2), ty1(y1), ty2(y2) {
    std::cout<<"Texture coordinates"<<tx1<<" "<<ty1<<" "<<tx2<<" "<<ty2<<std::endl;

}

Building::~Building() {

}

bool Building::canPlace(int i, int i1, char open) const {
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
    thing[0].position.x = square_size * x1;
    thing[0].position.y = square_size * y1;
    thing[1].position.x = square_size * x2;
    thing[1].position.y = square_size * y1;
    thing[2].position.x = square_size * x2;
    thing[2].position.y = square_size * y2;
    thing[3].position.x = square_size * x1;
    thing[3].position.y = square_size * y2;
    std::cout << x1 << " " << y1 << " " << x2 << " " << y2 << std::endl;

}
