#include "SymmetryFacet.h"



SymmetryFacet::SymmetryFacet()
{
}


SymmetryFacet::~SymmetryFacet()
{
}
void SymmetryFacet::UpdateFromBlueprint(Blueprint& blueprint) {
	sf::Vertex *current;
	auto symmetries = blueprint.getSymmetries();
	size_t s = 0;
	for (auto z : symmetries)
		s += z.getVertexCount();
	FacetArray.resize(s + 4);
	FacetArray.setPrimitiveType(sf::PrimitiveType::Lines);
	int index = 0;
	for (int i = 0; i < symmetries.size(); i++) {
		symmetries[i].buildSymmetryArray(&FacetArray[index], m_square_size);
		index += symmetries[i].getVertexCount();
	}
	auto blueprint_start_point = blueprint.getStartPoint();
	current = &FacetArray[FacetArray.getVertexCount() - 4];
	current[0].position = sf::Vector2f((blueprint_start_point.x) * m_square_size,
		(blueprint_start_point.y) * m_square_size);
	current[1].position = sf::Vector2f((1 + blueprint_start_point.x) * m_square_size,
		(1 + blueprint_start_point.y) * m_square_size);
	current[2].position = sf::Vector2f((blueprint_start_point.x) * m_square_size,
		(blueprint_start_point.y + 1) * m_square_size);
	current[3].position = sf::Vector2f((blueprint_start_point.x + 1) * m_square_size,
		(blueprint_start_point.y) * m_square_size);
	current[0].color = sf::Color::Magenta;
	current[1].color = sf::Color::Cyan;
	current[2].color = sf::Color::White;
	current[3].color = current[0].color;
}