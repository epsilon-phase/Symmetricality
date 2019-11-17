//
// Created by awhite on 12/28/15.
//

#include "Symmetry.h"

Symmetry::Symmetry(Symmetry_Type type) :m_type(type) {
}

sf::Vector2i Symmetry::fromPos(const sf::Vector2i &pos) const {
	sf::Vector2i result;

	switch (m_type) {
	case X:
	{
		int diff = pos.x - cursor.x;
		result.x = cursor.x - diff;
		result.y = pos.y;
	};
	break;
	case Y:
	{
		int diff = pos.y - cursor.y;
		result.y = cursor.y - diff;
		result.x = pos.x;
	};
	break;
	case Radial:
	{
		int diffx = pos.x - cursor.x;
		int diffy = pos.y - cursor.y;
		result.x = cursor.x - diffx;
		result.y = cursor.y - diffy;
	};
	break;
	case Rotational:
	{
		sf::Vector2i a;
		a.x = pos.x - cursor.x;
		a.y = pos.y - cursor.y;
		result.x = -a.y + cursor.x;
		result.y = a.x + cursor.y;
	};
	break;
	default:
		break;
	}
	return result;
}

bool Symmetry::operator==(const Symmetry &b) const {
	if (m_type == b.m_type) {
		switch (m_type) {
		case X:
			return cursor.x == b.cursor.x;
		case Y:
			return cursor.y == b.cursor.y;
		case Radial:
			return cursor == b.cursor;
		case Rotational:
			return cursor == b.cursor;
		}
	}
	return false;
}

int Symmetry::getRepetitionRequired() {
	if (m_type == Rotational)
		return 3;
	return 1;
}

sf::Color Symmetry::getColor() const {
	switch (m_type) {
	case X:
		return sf::Color::Green;
	case Y:
		return sf::Color::Red;
	case Radial:
		return sf::Color::Blue;
	case Rotational:
	default:
		return sf::Color::Magenta;
	}
}

Symmetry::Symmetry(const Symmetry &symmetry) :m_type(symmetry.m_type), cursor(symmetry.cursor) {
}

void Symmetry::setCursor(sf::Vector2i i) {
	cursor = i;
}
#define add_vec(V,X,Y) sf::Vector2f(V.x+X,V.y+Y)
void Symmetry::buildSymmetryArray(sf::Vertex* current, int square_size) const {
	for (int i = 0; i < getVertexCount(); i++)
		current[i].color = getColor();
	int x = cursor.x, y = cursor.y;
	switch (m_type) {
	case X:
		current[0].position = sf::Vector2f(x + 0.5f, y);
		current[1].position = sf::Vector2f(x + 0.5f, y + 1);
		current[2].position = sf::Vector2f(x, y + 0.4f);
		current[3].position = sf::Vector2f(x + 1, y + 0.4f);
		current[4].position = sf::Vector2f(x, y + 0.6f);
		current[5].position = sf::Vector2f(x + 1, y + 0.6f);
		break;
	case Y:
		current[0].position = sf::Vector2f(x + 0.4f, y);
		current[1].position = sf::Vector2f(x + 0.4f, y + 1);
		current[2].position = sf::Vector2f(x + 0.6f, y);
		current[3].position = sf::Vector2f(x + 0.6f, y + 1);
		current[4].position = sf::Vector2f(x, y + 0.5f);
		current[5].position = sf::Vector2f(x + 1, y + 0.5f);

		break;
	case Radial:
		current[0].position = sf::Vector2f(x, y + 1);
		current[1].position = sf::Vector2f(x + 1, y);
		current[2].position = current[0].position;
		current[3].position = add_vec(current[2].position, 0, -0.45f);
		current[4].position = current[0].position;
		current[5].position = add_vec(current[4].position, 0.45f, 0);
		current[6].position = current[1].position;
		current[7].position = add_vec(current[6].position, -0.45f, 0);
		current[8].position = current[1].position;
		current[9].position = add_vec(current[6].position, 0, 0.45f);

		break;
	case Rotational:
		current[0].position = sf::Vector2f(x + 0.5f, y + 0.5f);
		current[1].position = sf::Vector2f(x + 1, y + 0.5f);

		current[2].position = sf::Vector2f(x + 1, y + 0.5f);
		current[3].position = sf::Vector2f(x + 0.5f, y);

		current[4].position = sf::Vector2f(x + 0.5f, y);
		current[5].position = sf::Vector2f(x, y + 0.5f);

		current[6].position = sf::Vector2f(x, y + 0.5f);
		current[7].position = sf::Vector2f(x + 0.5f, y + 1);

		current[8].position = sf::Vector2f(x + 0.5f, y + 1);
		current[9].position = sf::Vector2f(x + 0.5f, y + 0.75f);
		current[10].position = current[7].position;
		current[11].position = sf::Vector2f(x + 0.25f, y + 1);
		break;
	}
	for (int i = 0; i <= getVertexCount(); i++) {
		current[i].position.x *= square_size;
		current[i].position.y *= square_size;
	}
}

int Symmetry::getVertexCount() const {
	switch (m_type) {
	case X:
	case Y:
		return 6;
	case Radial:
		return 10;
	case Rotational:
	default:
		return 12;
	}
}