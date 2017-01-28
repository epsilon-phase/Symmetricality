#ifndef DESIGNATION_FACET_H
#define DESIGNATION_FACET_H
#include "PlanFacet.h"
#include "Configurable.h"
#include "Floornable.h"

class DesignationFacet :
	public PlanFacet,public Configurable,public Floornable
{
	std::map<char, sf::IntRect> designation_texcoords;
	sf::Image designation_src;
	sf::Texture designationTexture;
	bool designationsUseTextures = false;
	
	std::map<char, sf::Color> designation_colors;
public:
	DesignationFacet();
	~DesignationFacet();
	void LoadConfiguration(Json::Value& v);
	void UpdateFromBlueprint(Blueprint& b);
	std::pair<sf::Texture*,std::vector<std::pair<char, sf::IntRect> > > GetMenuEntries()const {
		std::vector<std::pair<char, sf::IntRect> > result;
		for (auto& r : designation_texcoords)
            if(r.first!='I')
			result.push_back(r);
        if(texture!=nullptr)
    		return std::make_pair(texture, result);
        sf::Texture* r=const_cast<sf::Texture*>(&designationTexture);
    		return std::make_pair(r, result);
	}
private:
	void generate_designation_tile(int x, int y, char designation, sf::Vertex* c);
};

#endif
