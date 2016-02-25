#include "DesignationFacet.h"



DesignationFacet::DesignationFacet():PlanFacet(),Configurable()
{
	designation_colors['d'] = sf::Color(200, 200, 0);
	designation_colors['j'] = sf::Color(255, 255, 0);
	designation_colors['i'] = sf::Color(0, 255, 0);
	designation_colors['u'] = sf::Color(255, 0, 0);
	designation_colors['x'] = sf::Color(0, 0, 0);
}
DesignationFacet::~DesignationFacet()
{
}
void DesignationFacet::LoadConfiguration(Json::Value& v) {
	Json::Value designation = v.get("designation", "");
	designationsUseTextures = designation.get("use_textures", "").asBool();
	for (auto i : designation.get("colors", "")) {
		designation_colors[i.get("char", "").asString()[0]] = getFromJson(i.get("color", ""));
	}
	if (designationsUseTextures) {
		std::string designation_tex_file = designation.get("texture_file", "").asString();
		if (designation_tex_file ==
			"") {//If there is no designation sheet, then it must not be possible to modify it to fit the required parameters
			designationsUseTextures = false;
			return;
		}
		this->designation_src.loadFromFile(designation_tex_file);
		int designation_width = designation.get("width", 10).asInt();
		int designation_height = designation.get("height", 10).asInt();
		//The order of the designations in the image must be dig,downwards stairs, upwards stairs, up/down stairs, ramp, channel
		designation_texcoords['d'] = sf::IntRect(0, 0, designation_width, designation_height);
		designation_texcoords['j'] = sf::IntRect(designation_width, 0, designation_width, designation_height);
		designation_texcoords['u'] = sf::IntRect(designation_width * 2, 0, designation_width, designation_height);
		designation_texcoords['i'] = sf::IntRect(designation_width * 3, 0, designation_width, designation_height);
		designation_texcoords['r'] = sf::IntRect(designation_width * 4, 0, designation_width, designation_height);
		designation_texcoords['h'] = sf::IntRect(designation_width * 5, 0, designation_width, designation_height);
		designation_texcoords['I'] = sf::IntRect(designation_width * 6, 0, designation_width, designation_height);
		designation_texcoords['x'] = sf::IntRect(designation_width * 7, 0, designation_width, designation_height);
		designation_colors['I']= sf::Color(255, 160, 0);
		for (auto f : designation_texcoords) {
			sf::Color nc = designation_colors[f.first];
			for (int ix = 0; ix < f.second.width; ix++) {
				for (int iy = 0; iy < f.second.height; iy++) {
					auto pp = designation_src.getPixel(f.second.left + ix, f.second.top + iy);
					if (sf::Color::White.toInteger() == pp.toInteger()) {
						designation_src.setPixel(f.second.left + ix, f.second.top + iy, nc);
					}
				}
			}
		}
		designation_colors.erase('I');
		texture = new sf::Texture();
		texture->loadFromImage(designation_src);
	}
	
}
void DesignationFacet::UpdateFromBlueprint(Blueprint& blueprint) {

	auto current_floor = &blueprint.getLevelDesignation(floor);
	FacetArray.resize(current_floor->size() * 4);
	FacetArray.setPrimitiveType(sf::Quads);
	//TODO figure out a more efficient way to do this..
	//TODO figure out if it makes sense to make it more efficient than this.
	int iter = 0;
	sf::Vertex *current;
	for (auto i : *current_floor) {
		current = &FacetArray[iter];
		generate_designation_tile(i.first.x, i.first.y, i.second, current);
		iter += 4;
	}
	auto current_implications = blueprint.getImpliedDesignation(floor);
	if (!current_implications.empty()) {
		for (auto i : current_implications) {
			if (current_floor->find(i) != current_floor->end())
				continue;//This has already been drawn
			FacetArray.resize(4 + FacetArray.getVertexCount());
			current = &FacetArray[FacetArray.getVertexCount() - 4];
			generate_designation_tile(i.x, i.y, 'I', current);
		}
	}
}
void DesignationFacet::generate_designation_tile(int x, int y, char designation, sf::Vertex *c) {
	c[0].position = sf::Vector2f(x * m_square_size, y * m_square_size);
	c[1].position = sf::Vector2f((x + 1) * m_square_size, y * m_square_size);
	c[2].position = sf::Vector2f((x + 1) * m_square_size, (1 + y) * m_square_size);
	c[3].position = sf::Vector2f((x)* m_square_size, (1 + y) * m_square_size);

	if (designationsUseTextures) {
		textureRectangleToVertex(designation_texcoords[designation], c);
	}
	else
		for (int z = 0; z < 4; z++)
			c[z].color = designation_colors.find(designation)->second;
}