//
// Created by awhite on 12/28/15.
//

#ifndef SYMMETRICALITY_UTLITIES_HPP
#define SYMMETRICALITY_UTLITIES_HPP

#include <SFML/Graphics.hpp>
#include "city.h"
#include "json/json.h"
class Vector2iComp {
public:
	bool operator()(const sf::Vector2i& a, const sf::Vector2i &b) {
		return a.x == b.x ? a.y < b.y : a.x < b.x;
	}
};
namespace std {
	template<> struct hash<sf::Vector2i> {
	public:
		std::size_t operator()(const sf::Vector2i &a)const {
			return static_cast<size_t>(CityHash64((char *)&a, sizeof(a)));
		}
	};
}
inline void textureRectangleToVertex(sf::IntRect a, sf::Vertex* vert) {
	vert[0].texCoords = sf::Vector2f(float(a.left), float(a.top));
	vert[1].texCoords = sf::Vector2f(float(a.left + a.width), float(a.top));
	vert[2].texCoords = sf::Vector2f(float(a.left + a.width), float(a.top + a.height));
	vert[3].texCoords = sf::Vector2f(float(a.left), float(a.top + a.height));
}
inline sf::Color getFromJson(Json::Value v) {
	return sf::Color(v.get("R", 0).asInt(), v.get("G", 0).asInt(), v.get("B", 0).asInt());
}
#endif //SYMMETRICALITY_UTLITIES_HPP