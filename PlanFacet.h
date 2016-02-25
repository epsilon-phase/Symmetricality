#ifndef PLAN_FACET_H
#define PLAN_FACET_H
#include <SFML/Graphics.hpp>
#include "Blueprint.h"
class PlanFacet:public sf::Drawable, sf::Transformable
{
	
protected:
	int m_square_size;
	sf::Texture* texture = nullptr;
	sf::VertexArray FacetArray;
	void draw(sf::RenderTarget &target, sf::RenderStates states)const;
public:
	PlanFacet();
	~PlanFacet();
	virtual void UpdateFromBlueprint(Blueprint& b)=0;
	void set_square_size(int s) {
		m_square_size = s;
	}
};
#endif
