#ifndef BUILDING_FACET_H
#define BUILDING_FACET_H
#include "PlanFacet.h"
#include "Configurable.h"
#include "Floornable.h"
#include "Building.h"

class BuildingFacet :
	public PlanFacet,public Configurable,public Floornable
{
	std::map<std::string, Building> _building_types;
public:
	BuildingFacet();
	~BuildingFacet();
	void LoadConfiguration(Json::Value&);
	void UpdateFromBlueprint(Blueprint& b);
	std::pair < sf::Texture*, std::vector<std::pair<std::string, Building> > > GetMenuEntries()const {
		std::vector<std::pair<std::string, Building> > result;
		for (auto& i : _building_types) {
			result.push_back(make_pair(i.first,i.second));
		}
		return std::make_pair(texture, result);
	}
};

#endif