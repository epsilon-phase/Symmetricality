//
// Created by awhite on 12/27/15.
//

#include "PlanRenderer.h"
#include <sstream>
#include "Hud.h"
#include "tinyfiledialogs.h"
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

    if (designationsUseTextures) {
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
	target.draw(building_mode?build_menu:menu);
    viewable_area=target.getView();
}

void PlanRenderer::build_vertex_array() {
    buildDesignationArray();
    buildCursorArray();
    buildBuildingArray();
    buildSymmetryArray();
    build_designation();
}

void PlanRenderer::buildDesignationArray() {

    auto current_floor = &blueprint.getLevelDesignation(Floornum);
    Rendering_plan.resize(current_floor->size() * 4);
    Rendering_plan.setPrimitiveType(sf::Quads);
    //TODO figure out a more efficient way to do this..
    //TODO figure out if it makes sense to make it more efficient than this.
    int iter = 0;
    sf::Vertex *current;
    for (auto i:*current_floor) {
        current = &Rendering_plan[iter];
        generate_designation_tile(i.first.x, i.first.y, i.second, current);
        iter += 4;
    }
    auto current_implications=blueprint.getImpliedDesignation(Floornum);
    if(!current_implications.empty()){
        for(auto i : current_implications){
            if(current_floor->find(i)!=current_floor->end())
                continue;//This has already been drawn
            Rendering_plan.resize(4+Rendering_plan.getVertexCount());
            current=&Rendering_plan[Rendering_plan.getVertexCount()-4];
            generate_designation_tile(i.x,i.y,'I',current);

        }
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
            --current_building;
        } else {
            if (current_designation == designation_colors.begin())
                current_designation = designation_colors.end();
            --current_designation;
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

void PlanRenderer::setColor(char f, sf::Color c) {
    designation_colors[f] = c;
}

void PlanRenderer::loadBuildingTexture(const std::string &filename) {
    this->buildingTexture.loadFromFile(filename);

}
void PlanRenderer::getLoadBuildings(Json::Value v){
    Json::Value buildings = v.get("buildings", "[]");
    std::cout << buildings.size() << " buildings found" << std::endl;
    for (auto i : buildings){
        Building b = Building::fromJson(i);
        _building_types[b.getSequence()] = b;
    }
    current_building = _building_types.begin();
    for (auto i : _building_types){
        build_menu.addItem(buildingTexture, i.second.getTextureRect(), [=](){setBuilding(i.first); });
    }
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
    size_t s = 0;
    for (auto z : symmetries)
        s += z.getVertexCount();
    Symmetries.resize(s + 4);
    Symmetries.setPrimitiveType(sf::PrimitiveType::Lines);
    int index = 0;
    for (int i = 0; i < symmetries.size(); i++) {
        symmetries[i].buildSymmetryArray(&Symmetries[index], m_square_size);
        index += symmetries[i].getVertexCount();
    }
    auto blueprint_start_point = blueprint.getStartPoint();
    current = &Symmetries[Symmetries.getVertexCount() - 4];
    current[0].position = sf::Vector2f((blueprint_start_point.x) * m_square_size,
                                       (blueprint_start_point.y) * m_square_size);
    current[1].position = sf::Vector2f((1 + blueprint_start_point.x) * m_square_size,
                                       (1 + blueprint_start_point.y) * m_square_size);
    current[2].position = sf::Vector2f((blueprint_start_point.x) * m_square_size,
                                       (blueprint_start_point.y + 1) * m_square_size);
    current[3].position = sf::Vector2f((blueprint_start_point.x + 1) * m_square_size,
                                       (blueprint_start_point.y) * m_square_size);
    current[0].color = sf::Color::Magenta;
    current[1].color = sf::Color::Cyan;
    current[2].color = sf::Color::White;
    current[3].color = current[0].color;
}

bool PlanRenderer::canPlace() const {
    return Cursor[0].color != sf::Color::Red;
}

void PlanRenderer::generate_designation_tile(int x, int y, char designation, sf::Vertex *c) {
    c[0].position = sf::Vector2f(x * m_square_size, y * m_square_size);
    c[1].position = sf::Vector2f((x + 1) * m_square_size, y * m_square_size);
    c[2].position = sf::Vector2f((x + 1) * m_square_size, (1 + y) * m_square_size);
    c[3].position = sf::Vector2f((x) * m_square_size, (1 + y) * m_square_size);

    if (designationsUseTextures) {
        textureRectangleToVertex(designation_texcoords[designation], c);
    } else
        for (int z = 0; z < 4; z++)
            c[z].color = designation_colors.find(designation)->second;
}

void PlanRenderer::loadDesignationConfiguration(Json::Value& v){
    Json::Value designation = v.get("designation", "");
    designationsUseTextures=designation.get("use_textures", "").asBool();
    for (auto i : designation.get("colors","")){
        setColor(i.get("char", "").asString()[0], getFromJson(i.get("color", "")));
    }
    if (designationsUseTextures){
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
        setColor('I', sf::Color(255, 160, 0));
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
        current_designation = designation_colors.begin();
        designationTexture.loadFromImage(designation_src);
    }
    menu_utility_texture.loadFromFile(v.get("menu_texture", "menu_utility_icons.png").asString());
    initializeMenu();
}

void PlanRenderer::initializeMenu(){
	for (auto i : designation_texcoords){
		if (i.first != 'I')//no implied tile
			menu.addItem(designationTexture, i.second, [=](){this->setDesignation(i.first); });
	}
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