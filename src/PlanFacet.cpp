#include "PlanFacet.h"

PlanFacet::PlanFacet()
{
}

PlanFacet::~PlanFacet()
{
	if (texture != nullptr)
		delete texture;
}
void PlanFacet::draw(sf::RenderTarget& target, sf::RenderStates states)const {
	states.transform *= getTransform();
	states.texture = texture;
	target.draw(FacetArray, states);
}