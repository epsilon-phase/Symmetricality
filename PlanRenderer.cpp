//
// Created by awhite on 12/27/15.
//

#include "PlanRenderer.h"
#include <sstream>
#include "Hud.h"
#include <iostream>
#include "tinyfiledialogs.h"
PlanRenderer::PlanRenderer() {
	configurables.push_back(&designator);
	configurables.push_back(&buildings);
	floornables.push_back(&designator);
	floornables.push_back(&buildings);
	facets.push_back(&designator);
	facets.push_back(&buildings);
	facets.push_back(&symmetry);
	for (auto f : facets)
		f->set_square_size(m_square_size);
	//This was done to allow visual studio to compile. The vile thing doesn't allow non static initializers.
    designation_colors.insert('d');
    designation_colors.insert('j');
    designation_colors.insert('i');
    designation_colors.insert('u');
    designation_colors.insert('x');
	setFloor(0);
    current_designation = designation_colors.begin();
    blueprint.setDesignation(*current_designation);
}

PlanRenderer::~PlanRenderer() {

}

void PlanRenderer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();

    
	target.draw(designator, states);
    target.draw(buildings, states);
	target.draw(symmetry, states);
    if (blueprint.isDesignating())
        target.draw(Designation_preview, states);
    states.texture = NULL;
    target.draw(Cursor, states);
	target.draw(building_mode?build_menu:menu);
    viewable_area=target.getView();
}
void PlanRenderer::LoadConfiguration(Json::Value& v) {
	for (auto i : configurables)
		i->LoadConfiguration(v);
	auto b = buildings.GetMenuEntries();
	for (auto& f : b.second) {
		build_menu.addItem(*b.first, f.second.getTextureRect(), [=]() {this->setBuilding(f.first); });
		_building_types.insert(std::make_pair(f.second.getName(),f.second));
	}
	current_building = _building_types.begin();
	blueprint.setBuilding(&current_building->second);
	auto a = designator.GetMenuEntries();
	for (auto& f : a.second) {
		build_menu.addItem(*a.first, f.second, [=]() {this->setDesignation(f.first); });
	}
	
}
void PlanRenderer::build_vertex_array() {
	for (auto i : facets)
		i->UpdateFromBlueprint(blueprint);
    buildCursorArray();
    build_designation();
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
                    blueprint.insert(cursorpos.x, cursorpos.y, Floornum, *current_designation);
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
					for (auto f : facets)
						f->set_square_size(m_square_size);
                } else
                    change_designation(true);
                break;
            case sf::Keyboard::Dash:
                if (event.key.control) {
                    m_square_size--;
					for (auto f : facets)
						f->set_square_size(m_square_size);
                } else
                    change_designation(false);
                break;
            case sf::Keyboard::B:
                building_mode = !building_mode;
                break;
            case sf::Keyboard::Return:
                blueprint.setDesignation(*current_designation);
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
                setFloor(0);
				
            }
            clear_primed = true;
        } else
            clear_primed = false;
    }

}

void PlanRenderer::move_cursor(int x, int y) {
    cursorpos.x += x;
    cursorpos.y += y;
    sf::Vector2f raw(cursorpos.x*m_square_size,cursorpos.y*m_square_size);
    sf::Vector2f transformed=getTransform().transformPoint(raw);
    if(transformed.x>viewable_area.getSize().x/2){
        move(-m_square_size*std::abs(x),0);
    }else{
        if(transformed.x<viewable_area.getCenter().x-viewable_area.getSize().x/2)
            move(m_square_size*std::abs(x),0);
    }
    if(transformed.y>viewable_area.getSize().y/2){
        move(0,-m_square_size*std::abs(y));
    }else{
        if(transformed.y<viewable_area.getCenter().y-viewable_area.getSize().y/2)
            move(0,m_square_size*std::abs(y));
    }
    build_vertex_array();
}

void PlanRenderer::move_z(int offset) {
    setFloor(offset+Floornum);
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
            --current_building;
        } else {
            if (current_designation == designation_colors.begin())
                current_designation = designation_colors.end();
            --current_designation;
        }
    }

    blueprint.setDesignation(*current_designation);
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
	if(!building_mode&&menu.handle_event(event,b))return ;
	if (building_mode&&build_menu.handle_event(event, b))return;
    auto transformed = getInverseTransform().transformPoint(b);
    sf::Vector2i mouse_position = sf::Vector2i((int) floor(transformed.x / m_square_size),
                                               (int) floor(transformed.y / m_square_size));

    if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == 0)
			blueprint.setDesignationToggle(
				sf::Vector3i(mouse_position.x, mouse_position.y, Floornum),
				sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)? 
					Blueprint::LINE
					:(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)?
						Blueprint::CIRCLE : Blueprint::RECTANGLE)
					, building_mode);
		else if (event.mouseButton.button == 1){
			right_button_down = true;
			since_last_click = sf::Clock();
			this->old_mouse_pos = b;
		}
    }
    if(event.type==sf::Event::MouseWheelMoved){
        m_square_size+=event.mouseWheel.delta;
		for (auto f : facets)
			f->set_square_size(m_square_size);
    }
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == 1) {
        right_button_down = false;
        auto f=since_last_click.restart().asMilliseconds();
		if (f < 150){
			if (building_mode){
				build_menu.open(b);
			}
			else{
				menu.open(b);
			}
		}else if (f <= 500){
            click_count++;
            if(click_count>2)
                setPosition(0,0);
        }else
            click_count=0;

    }

    designation_changed = true;
    build_vertex_array();
}

void PlanRenderer::handleMouseOver(const sf::Vector2f &relative_to_view) {
    sf::Vector2f transformed_point = getInverseTransform().transformPoint(relative_to_view);
    if (right_button_down) {
        this->move(relative_to_view.x - old_mouse_pos.x, relative_to_view.y - old_mouse_pos.y);
        old_mouse_pos = relative_to_view;
    }
    sf::Vector2i mouse_position = sf::Vector2i((int) floor(transformed_point.x / m_square_size),
                                               (int) floor(transformed_point.y / m_square_size));
    if (blueprint.isDesignating())
        cursorpos = mouse_position;
    build_designation();

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



bool PlanRenderer::canPlace() const {
    return Cursor[0].color != sf::Color::Red;
}



void PlanRenderer::initializeMenu(){
	
	int menu_tex_width = menu_utility_texture.getSize().x / 3;
	int menu_tex_height = menu_utility_texture.getSize().y;
    menu.addItem(menu_utility_texture, sf::IntRect(0, 0, menu_tex_width, menu_tex_height),
        [=](){
        const char *filters = "*.ser";
        std::string file_path = the_hud->default_file_path + "/blueprint.ser";
        const char *filename = tinyfd_saveFileDialog("Save File", file_path.c_str(), 1,
            &filters,"Serialized File");
        if (filename != NULL) {
            serialize(std::string(filename));
        }
    });
    menu.addItem(menu_utility_texture, sf::IntRect(menu_tex_width, 0, menu_tex_width, menu_tex_height),
        [=](){
        const char *filters = "*.ser";
        std::string file_path = the_hud->default_file_path + "/blueprint.ser";
        const char *filename = tinyfd_openFileDialog("open File", file_path.c_str(), 1,
            &filters, "Serialized File",false);
        if (filename != NULL) {
            deserialize(std::string(filename));
        }
    });
    menu.addItem(menu_utility_texture, sf::IntRect(menu_tex_width*2, 0, menu_tex_width, menu_tex_height),
        [=](){
        const char *filters = "*.csv";
        std::string file_path = the_hud->default_file_path + "/blueprint.csv";
        const char *filename = tinyfd_saveFileDialog("export File", file_path.c_str(), 1,
            &filters, "Comma Separated value");
        if (filename != NULL) {
            export_csv(std::string(filename));
        }
    });
}
void PlanRenderer::setDesignation(char e){
	this->current_designation = designation_colors.find(e);
	blueprint.setDesignation(e);
}
void PlanRenderer::setBuilding(const std::string& r){
	this->current_building = _building_types.find(r);
	this->blueprint.setBuilding(&_building_types.find(r)->second);
}