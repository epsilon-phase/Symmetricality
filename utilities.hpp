//
// Created by awhite on 12/28/15.
//

#ifndef SYMMETRICALITY_UTLITIES_HPP
#define SYMMETRICALITY_UTLITIES_HPP

#include <SFML/Graphics.hpp>
#include "city.h"
class Vector2iComp{
public:
    bool operator()(const sf::Vector2i& a,const sf::Vector2i &b){
        return a.x==b.x?a.y<b.y:a.x<b.x;
    }
};
namespace std {
 template<> struct hash<sf::Vector2i> {
    public:
        std::size_t operator()(const sf::Vector2i &a)const {
            return CityHash64((char *) &a, sizeof(a));
        }
    };
}
#endif //SYMMETRICALITY_UTLITIES_HPP
