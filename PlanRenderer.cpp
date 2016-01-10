//
// Created by awhite on 12/27/15.
//

#include "PlanRenderer.h"
#include <sstream>

PlanRenderer::PlanRenderer() {
    //This was done to allow visual studio to compile. The vile thing doesn't allow non static initializers.
    designation_colors['d'] = sf::Color(200, 200, 0);
    designation_colors['j'] = sf::Color(255, 255, 0);
    designation_colors['i'] = sf::Color(0, 255, 0);
    designation_colors['u'] = sf::Color(255, 0, 0);
    designation_colors['x'] = sf::Color(0, 0, 0);
    current_designation = designation_colors.begin();
    blueprint.setDesignation(current_designation->first);
}

PlanRenderer::~PlanRenderer() {

}

void PlanRenderer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    
	if (designationsUseTextures){
		states.texture = &designationTexture;
	}
    target.draw(Rendering_plan, states);
	states.texture = NULL;
	target.draw(Symmetries, states);
    
    states.texture = &buildingTexture;
    target.draw(Buildings, states);
    if (blueprint.isDesignating())
        target.draw(Designation_preview, states);
	states.texture = NULL;
	target.draw(Cursor, states);
}

void PlanRenderer::build_vertex_array() {
    buildDesignationArray();
    buildCursorArray();
    buildBuildingArray();
    buildSymmetryArray();
    build_designation();
}

void PlanRenderer::buildDesignationArray() {
    sf::Vertex *current;
    auto current_floor = &blueprint.getLevelDesignation(Floornum);
    Rendering_plan.resize(current_floor->size() * 4);
    Rendering_plan.setPrimitiveType(sf::Quads);
    //TODO figure out a more efficient way to do this..
    //TODO figure out if it makes sense to make it more efficient than this.
    int iter = 0;
    for (auto i:*current_floor) {
        current = &Rendering_plan[iter];
        generate_designation_tile(i.first.x,i.first.y,i.second,current);
        iter += 4;
    }
}

void PlanRenderer::set_pos(sf::Vector2i i, char i1) {
    build_vertex_array();
}

void PlanRenderer::handle_event(sf::Event event) {
    if (event.type == sf::Event::KeyPressed) {
        int offset_size = event.key.shift ? 10 : 1;
        switch (event.key.code) {
            case sf::Keyboard::Left:
            case sf::Keyboard::Numpad4:
                move_cursor(-offset_size, 0);
                break;
            case sf::Keyboard::Numpad6:
            case sf::Keyboard::Right:
                move_cursor(offset_size, 0);
                break;
            case sf::Keyboard::Up:
            case sf::Keyboard::Numpad8:
                move_cursor(0, -offset_size);
                break;
            case sf::Keyboard::Down:
            case sf::Keyboard::Numpad2:
                move_cursor(0, offset_size);
                break;
            case sf::Keyboard::Space:
                if (building_mode) {
                    blueprint.placeBuilding(cursorpos.x, cursorpos.y, Floornum, current_building->second);
                    building_changed = true;
                }
                else {
                    designation_changed = true;
                    blueprint.insert(cursorpos.x, cursorpos.y, Floornum, current_designation->first);
                }
                break;
            case sf::Keyboard::Comma:
                move_z(1);
                break;
            case sf::Keyboard::Period:
                move_z(-1);
                break;
            case sf::Keyboard::R:
                add_symmetry(Symmetry::Radial);
                break;
            case sf::Keyboard::X:
                add_symmetry(Symmetry::X);
                break;
            case sf::Keyboard::Y:
                add_symmetry(Symmetry::Y);
                break;
            case sf::Keyboard::Num9:
                add_symmetry(Symmetry::Rotational);
                break;
            case sf::Keyboard::Equal:
                if (event.key.control) {
                    m_square_size++;
                } else
                    change_designation(true);
                break;
            case sf::Keyboard::Dash:
                if (event.key.control) {
                    m_square_size--;
                } else
                    change_designation(false);
                break;
            case sf::Keyboard::B:
                building_mode = !building_mode;
                break;
            case sf::Keyboard::Return:
                blueprint.setDesignation(current_designation->first);
                blueprint.setDesignationToggle(cursorpos.x, cursorpos.y, Floornum,
                                               event.key.shift ? Blueprint::CIRCLE : (event.key.control
                                                                                      ? Blueprint::LINE
                                                                                      : Blueprint::RECTANGLE),
																					  building_mode);
                designation_changed = true;
                break;
            case sf::Keyboard::Numpad9:
                move_cursor(offset_size, -offset_size);
                break;
            case sf::Keyboard::Numpad1:
                move_cursor(-offset_size, offset_size);
                break;
            case sf::Keyboard::Numpad7:
                move_cursor(-offset_size, -offset_size);
                break;
            case sf::Keyboard::Numpad3:
                move_cursor(offset_size, offset_size);
                break;
            case sf::Keyboard::S:
                blueprint.setStart(cursorpos.x, cursorpos.y);
                break;
        }
        if (event.key.code == sf::Keyboard::C) {
            if (clear_primed) {
                blueprint = Blueprint();
                Floornum = 0;
            }
            clear_primed = true;
        } else
            clear_primed = false;
    }

}

void PlanRenderer::move_cursor(int x, int y) {
    cursorpos.x += x;
    cursorpos.y += y;
    build_vertex_array();
}

void PlanRenderer::move_z(int offset) {
    this->Floornum += offset;
    build_vertex_array();
}


void PlanRenderer::add_symmetry(Symmetry::Symmetry_Type type) {
    blueprint.addSymmetry(cursorpos.x, cursorpos.y, type);

}

void PlanRenderer::build_designation() {
    if (blueprint.isDesignating()) {
        const sf::Vector3i &start = blueprint.getDesignationStart();
        const sf::Vector3i &end = sf::Vector3i(cursorpos.x, cursorpos.y, Floornum);
        Designation_preview.resize(8);
        Designation_preview.setPrimitiveType(sf::PrimitiveType::Lines);
        sf::Vertex *the_square = &Designation_preview[0];
        the_square[0].position = sf::Vector2f((0.5f + start.x) * m_square_size, (0.5f + start.y) * m_square_size);
        the_square[1].position = sf::Vector2f((0.5f + end.x) * m_square_size, (0.5f + start.y) * m_square_size);
        the_square[2].position = sf::Vector2f((0.5f + start.x) * m_square_size, (0.5f + start.y) * m_square_size);
        the_square[3].position = sf::Vector2f((0.5f + start.x) * m_square_size, (0.5f + end.y) * m_square_size);
        the_square[4].position = sf::Vector2f((0.5f + end.x) * m_square_size, (0.5f + start.y) * m_square_size);
        the_square[5].position = sf::Vector2f((0.5f + end.x) * m_square_size, (0.5f + end.y) * m_square_size);
        the_square[6].position = sf::Vector2f((0.5f + start.x) * m_square_size, (0.5f + end.y) * m_square_size);
        the_square[7].position = sf::Vector2f((0.5f + end.x) * m_square_size, (0.5f + end.y) * m_square_size);
        for (int i = 0; i < 8; i++)
            the_square[i].color = sf::Color(109, 93, 98);
    } else {
        Designation_preview.resize(0);
    }

}

void PlanRenderer::change_designation(bool up) {

    if (up) {
        if (building_mode) {
            current_building++;
            if (current_building == _building_types.end())
                current_building = _building_types.begin();
        } else {
            current_designation++;
            if (current_designation == designation_colors.end())
                current_designation = designation_colors.begin();
        }
    } else {
        if (building_mode) {
            if (current_building == _building_types.begin())
                current_building = _building_types.end();
            current_building--;
        } else {
            if (current_designation == designation_colors.begin())
                current_designation = designation_colors.end();
            current_designation--;
        }
    }

    blueprint.setDesignation(current_designation->first);
	blueprint.setBuilding(&current_building->second);
}

void PlanRenderer::export_csv(const std::string &string) const {
    blueprint.export_csv(string);
}

void PlanRenderer::serialize(const std::string &string) const {
    blueprint.serialize(string);
}

void PlanRenderer::deserialize(const std::string &string) {
    blueprint.deserialize(string);
}

void PlanRenderer::handle_mouse(sf::Event event, const sf::Vector2f &b) {
    sf::Vector2i mouse_position = sf::Vector2i((int) floor(b.x / m_square_size), (int) floor(b.y / m_square_size));
    blueprint.setDesignationToggle(sf::Vector3i(mouse_position.x, mouse_position.y, Floornum),
                                   event.mouseButton.button == 0 ? (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)
                                                                    ? Blueprint::CIRCLE : Blueprint::RECTANGLE)
                                                                 : Blueprint::LINE,building_mode);
    designation_changed = true;
    build_vertex_array();
}

void PlanRenderer::handleMouseOver(const sf::Vector2f &b) {
    sf::Vector2i mouse_position = sf::Vector2i((int) floor(b.x / m_square_size), (int) floor(b.y / m_square_size));
    if (blueprint.isDesignating())
        cursorpos = mouse_position;
    build_designation();

}

void PlanRenderer::setColor(char f, sf::Color c) {
    designation_colors[f] = c;
}

void PlanRenderer::loadBuildingTexture(const std::string &filename) {
    this->buildingTexture.loadFromFile(filename);

}

void PlanRenderer::getLoadBuildings(GetPot &pot) {
    int number = pot("buildings/BuildingCount", 0);
    std::cout << "Found " << number << " Buildings.\n";
    for (int i = 0; i < number; i++) {
        Building a = Building::fromPot(i, pot);
        _building_types[a.getSequence()] = a;
    }
    current_building = _building_types.begin();
}

void PlanRenderer::buildBuildingArray() {
    sf::Vertex *current;
    const std::unordered_map<sf::Vector2i, std::string> &f = blueprint.getLevelBuildings(Floornum);
    Buildings.resize(f.size() * 4);
    Buildings.setPrimitiveType(sf::Quads);
    if (f.size() == 0)
        return;
    current = &Buildings[0];
    for (auto i:f) {
        std::string e = i.second;
        _building_types[e].getTexCoords(current);
        _building_types[e].getAdjustedCoords(i.first.x, i.first.y, m_square_size, current);
        current++;
        current++;
        current++;
        current++;
    }
}

void PlanRenderer::buildCursorArray() {
    sf::Vertex *current;
    Cursor.resize(4);
    Cursor.setPrimitiveType(sf::PrimitiveType::Quads);
    current = &Cursor[0];
    current[0].position.x = (0.25f + cursorpos.x) * m_square_size;
    current[0].position.y = (0.25f + cursorpos.y) * m_square_size;
    current[1].position = sf::Vector2f(current[0].position.x + .5f * m_square_size, current[0].position.y);
    current[2].position = sf::Vector2f(current[1].position.x, current[1].position.y + 0.5f * m_square_size);
    current[3].position = sf::Vector2f(current[2].position.x - 0.5f * m_square_size, current[2].position.y);
    for (int i = 0; i < 4; i++)
        current[i].color =
                building_mode && !blueprint.canPlace(cursorpos.x, cursorpos.y, Floornum, current_building->second)
                ? sf::Color::Red//the building can't be built
                : sf::Color::White;//the building can be built.
}

void PlanRenderer::buildSymmetryArray() {
    sf::Vertex *current;
    auto symmetries = blueprint.getSymmetries();
    size_t s=0;
    for(auto z : symmetries)
        s+=z.getVertexCount();
    Symmetries.resize(s+4);
    Symmetries.setPrimitiveType(sf::PrimitiveType::Lines);
    int index=0;
    for (int i = 0; i < symmetries.size(); i++) {
        symmetries[i].buildSymmetryArray(&Symmetries[index],m_square_size);
        index+=symmetries[i].getVertexCount();
    }
    auto blueprint_start_point = blueprint.getStartPoint();
    current = &Symmetries[Symmetries.getVertexCount() - 4];
    current[0].position = sf::Vector2f((blueprint_start_point.x) * m_square_size,
                                       ( blueprint_start_point.y) * m_square_size);
    current[1].position = sf::Vector2f((1 + blueprint_start_point.x) * m_square_size,
                                       (1 + blueprint_start_point.y) * m_square_size);
    current[2].position = sf::Vector2f((blueprint_start_point.x) * m_square_size,
                                       (blueprint_start_point.y+1) * m_square_size);
    current[3].position=sf::Vector2f((blueprint_start_point.x + 1) * m_square_size,
                                    (blueprint_start_point.y) * m_square_size);
    current[0].color = sf::Color::Magenta;
    current[1].color = sf::Color::Cyan;
    current[2].color = sf::Color::White;
    current[3].color=current[0].color;
}

bool PlanRenderer::canPlace()const{
	return Cursor[0].color != sf::Color::Red;
}

void PlanRenderer::generate_designation_tile(int x, int y, char designation, sf::Vertex *c) {
    c[0].position = sf::Vector2f(x * m_square_size, y * m_square_size);
    c[1].position = sf::Vector2f((x + 1) * m_square_size, y * m_square_size);
    c[2].position = sf::Vector2f((x + 1) * m_square_size, (1 + y) * m_square_size);
    c[3].position = sf::Vector2f((x) * m_square_size, (1 + y) * m_square_size);

	if (designationsUseTextures){
		textureRectangleToVertex(designation_texcoords[designation], c);
	}else
		for (int z = 0; z < 4; z++)
			c[z].color = designation_colors.find(designation)->second;
}

void PlanRenderer::loadDesignationConfiguration(GetPot &pot) {
	designationsUseTextures = (bool)pot("designation/use_textures",0);
    setColor('d', sf::Color(pot("colors/dig/R", 200), pot("colors/dig/G", 200),
                                 pot("colors/dig/B", 0)));
    setColor('i', sf::Color(pot("colors/up_down_stairs/R", 0), pot("colors/up_down_stairs/G", 255),
                                 pot("colors/up_down_stairs/B", 0)));
    setColor('j', sf::Color(pot("colors/downward_stairs/R", 255),
                                 pot("colors/downward_stairs/G", 255), pot("colors/downward_stairs/B", 0)));
	pot.set_prefix("colors/upward_stairs/");
    setColor('u', sf::Color(pot("R", 255), pot("G", 0),pot("B", 0)));
	pot.set_prefix("colors/channel/");
	setColor('h', sf::Color(pot("R", 132),pot("G",128),pot("B",132)));
	pot.set_prefix("colors/ramp/");
	setColor('r', sf::Color(pot("R", 132), pot("G", 32), pot("B", 132)));
	pot.set_prefix("designation/");
	if (designationsUseTextures){
		std::string designation_tex_file = pot("designation_tex", "");
		if (designation_tex_file == ""){//If there is no designation sheet, then it must not be possible to modify it to fit the required parameters
			designationsUseTextures = false;
			return;
		}
		this->designation_src.loadFromFile(designation_tex_file); 
		int designation_width = pot("width", 10);
		int designation_height = pot("height", 10);
		//The order of the designations in the image must be dig,downwards stairs, upwards stairs, up/down stairs, ramp, channel
		designation_texcoords['d'] = sf::IntRect(0, 0, designation_width, designation_height);
		designation_texcoords['j'] = sf::IntRect(designation_width, 0, designation_width, designation_height);
		designation_texcoords['u'] = sf::IntRect(designation_width*2,0, designation_width, designation_height);
		designation_texcoords['i'] = sf::IntRect(designation_width * 3, 0, designation_width, designation_height);
		designation_texcoords['r'] = sf::IntRect(designation_width * 4, 0, designation_width, designation_height);
		designation_texcoords['h'] = sf::IntRect(designation_width * 5, 0, designation_width, designation_height);
		for (auto f : designation_texcoords){
			sf::Color nc = designation_colors[f.first];
			for (int ix = 0; ix < f.second.width;ix++){
				for (int iy = 0; iy < f.second.height; iy++){
					auto pp=designation_src.getPixel(f.second.left + ix, f.second.top + iy);
					if (sf::Color::White.toInteger() == pp.toInteger()){
						designation_src.setPixel(f.second.left + ix, f.second.top + iy, nc);
					}
				}
			}
		}
		designationTexture.loadFromImage(designation_src);
		std::cout << "Image of size:" << designation_src.getSize().x << "x" << designation_src.getSize().y << std::endl;
	}
	//Reset prefix so that it does not interfere with other things
	pot.set_prefix("");
}
