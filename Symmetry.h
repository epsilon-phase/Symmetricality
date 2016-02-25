//
// Created by awhite on 12/28/15.
//

#ifndef SYMMETRICALITY_SYMMETRY_H
#define SYMMETRICALITY_SYMMETRY_H

#include <SFML/Graphics.hpp>
//Tyger Tyger burning bright,
//in the forests of the night;
//What immortal hand or eye
//could frame thy fearful symmetry?
class Symmetry {
	sf::Vector2i cursor;

public:
	enum Symmetry_Type {
		X, Y, Radial, Rotational
	};
	Symmetry(Symmetry_Type type);
	Symmetry(const Symmetry&);
	Symmetry() {}
	~Symmetry() {}
	int getRepetitionRequired();
	sf::Vector2i fromPos(const sf::Vector2i& pos)const;
	sf::Color getColor()const;
	bool operator==(const Symmetry& b)const;
	static Symmetry getX(sf::Vector2i z) {
		auto a = Symmetry(X);
		a.cursor = z;
		return a;
	}
	static Symmetry getY(sf::Vector2i z) {
		auto a = Symmetry(Y);
		a.cursor = z;
		return a;
	}
	static Symmetry getRadial(sf::Vector2i z) {
		auto a = Symmetry(Radial);
		a.cursor = z;
		return a;
	}
	static Symmetry getRotational(sf::Vector2i z) {
		auto a = Symmetry(Rotational);
		a.cursor = z;
		return a;
	}
	Symmetry_Type getType()const { return m_type; }
	void setCursor(sf::Vector2i);
	sf::Vector2i getCursor()const { return cursor; }
	void buildSymmetryArray(sf::Vertex*, int)const;
	int getVertexCount()const;
private:
	Symmetry_Type m_type;
};

#endif //SYMMETRICALITY_SYMMETRY_H
