#ifndef DESIGNATION_FACET_H
#define DESIGNATION_FACET_H
#include "PlanFacet.h"
#include "Configurable.h"
#include "Floornable.h"
class DesignationFacet :
	public PlanFacet,public Configurable,public Floornable
{
	sf::Image designation_src;
	sf::Texture designationTexture;
	bool designationsUseTextures = false;
	std::map<char, sf::IntRect> designation_texcoords;
	std::map<char, sf::Color> designation_colors;
public:
	DesignationFacet();
	~DesignationFacet();
	void LoadConfiguration(Json::Value& v);
	void UpdateFromBlueprint(Blueprint& b);
private:
	void generate_designation_tile(int x, int y, char designation, sf::Vertex* c);
};

#endif