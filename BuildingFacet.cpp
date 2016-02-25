#include "BuildingFacet.h"



BuildingFacet::BuildingFacet()
{
	texture = new sf::Texture();
}


BuildingFacet::~BuildingFacet()
{
}
void BuildingFacet::LoadConfiguration(Json::Value& v) {
	Json::Value buildings = v.get("buildings", "[]");
	texture->loadFromFile(v.get("building_sheet", "").asString());
	for (auto i : buildings) {
		Building b = Building::fromJson(i);
		_building_types[b.getSequence()] = b;
	}
}
void BuildingFacet::UpdateFromBlueprint(Blueprint& blueprint) {
	sf::Vertex *current;
	const std::unordered_map<sf::Vector2i, std::string> &f = blueprint.getLevelBuildings(Floornum);
	FacetArray.resize(f.size() * 4);
	FacetArray.setPrimitiveType(sf::Quads);
	if (f.size() == 0)
		return;
	current = &FacetArray[0];
	for (auto i : f) {
		std::string e = i.second;
		_building_types[e].getTexCoords(current);
		_building_types[e].getAdjustedCoords(i.first.x, i.first.y, m_square_size, current);
		current += 4;
	}
}