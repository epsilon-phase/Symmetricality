#ifndef SYMMETRY_FACET_H
#define SYMMETRY_FACET_H
#include "PlanFacet.h"
class SymmetryFacet :
	public PlanFacet
{
public:
	SymmetryFacet();
	~SymmetryFacet();
	void UpdateFromBlueprint(Blueprint& blueprint);
};

#endif