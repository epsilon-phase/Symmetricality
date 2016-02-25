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
};

#endif