//
// Created by awhite on 1/2/16.
//

#ifndef SYMMETRICALITY_BUILDING_H
#define SYMMETRICALITY_BUILDING_H
#include <string>
#include <SFML/Graphics.hpp>
#include "GetPot"
class Building {
	std::string key_sequence;
	std::string name;
	sf::Vector2i size;
	sf::Vector2i render_end;
	sf::Vector2i render_start;
	//the center on the size
	sf::Vector2i center;
	int tx1, tx2, ty1, ty2;
public:
	static Building fromPot(int number, GetPot& conf);
	Building(const std::string &name, const std::string &keyseq, sf::Vector2i size, sf::Vector2i center,
		int x1, int y1, int x2, int y2);
	Building(){}
	~Building();
	enum overhang_flag{
		SOLID_REQUIRED = 0,
		SOLID_OPTIONAL = 1,
		OVERHANG_REQUIRED = 2
	};
	bool canPlace(int, int, char open)const;
	std::string getSequence()const;
	std::string getName()const;
	sf::Vector2i getSize()const;
	sf::Vector2i getCenter()const;
	/**
	 * Stores the texture coordinates in the pointer to the vertex
	 */
	void getTexCoords(sf::Vertex* thing);
	void getAdjustedCoords(int x, int y, int square_size, sf::Vertex* thing);
private:
	std::vector<overhang_flag> flags;
	std::vector <signed char> passable;
};


#endif //SYMMETRICALITY_BUILDING_H
