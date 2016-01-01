//
// Created by awhite on 12/27/15.
//

#include "PlanRenderer.h"
#include <iostream>
#include <fstream>

PlanRenderer::PlanRenderer() {
    blueprint.setDesignation(current_designation->first);
}

PlanRenderer::~PlanRenderer() {

}

void PlanRenderer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = NULL;
    target.draw(Rendering_plan, states);
    target.draw(Symmetries, states);
    target.draw(Cursor,states);
    if (blueprint.isDesignating())
        target.draw(Designation_preview, states);

}
//terrible practice to make a square for a remarkably inflexible piece of code
#define MAK_SQUAR(X, Y, C) current[0].position=sf::Vector2f(X*m_square_size,Y*m_square_size);\
                         current[1].position=sf::Vector2f((X+1)*m_square_size,Y*m_square_size);\
                         current[2].position=sf::Vector2f((X+1)*m_square_size,(1+ Y)*m_square_size);\
                         current[3].position=sf::Vector2f((X)*m_square_size,(1+Y)*m_square_size);\
                         for(int z=0;z<4;z++)\
                            current[z].color=C;

void PlanRenderer::build_vertex_array() {
    sf::Vertex *current;
    auto current_floor = &blueprint.getLevelDesignation(Floornum);
    auto is_designating = blueprint.isDesignating();

    Rendering_plan.resize(current_floor->size() * 4);
    Rendering_plan.setPrimitiveType(sf::PrimitiveType::Quads);
    //TODO figure out a more efficient way to do this..
    //TODO figure out if it makes sense to make it more efficient than this.
    int iter = 0;
    for (auto i:*current_floor) {
        current = &Rendering_plan[iter];
        current[0].position = sf::Vector2f(i.first.x * m_square_size, i.first.y * m_square_size);
        current[1].position = sf::Vector2f((i.first.x + 1) * m_square_size, i.first.y * m_square_size);
        current[2].position = sf::Vector2f((i.first.x + 1) * m_square_size, (1 + i.first.y) * m_square_size);
        current[3].position = sf::Vector2f((i.first.x) * m_square_size, (1 + i.first.y) * m_square_size);
        for (int z = 0; z < 4; z++)
            current[z].color = designation_colors.find(i.second)->second;
        iter += 4;
    }
    Cursor.resize(4);
    Cursor.setPrimitiveType(sf::PrimitiveType::Quads);
    current = &Cursor[0];
    current[0].position.x = (0.25f+cursorpos.x) * m_square_size;
    current[0].position.y = (0.25f+cursorpos.y) * m_square_size;
    current[1].position = sf::Vector2f(current[0].position.x + .5f*m_square_size, current[0].position.y);
    current[2].position = sf::Vector2f(current[1].position.x, current[1].position.y + 0.5f*m_square_size);
    current[3].position = sf::Vector2f(current[2].position.x - 0.5f*m_square_size, current[2].position.y);

    auto symmetries = blueprint.getSymmetries();
    for (int z = 0; z < 4; z++)
        current[z].color = sf::Color(255, 255, 255);
    Symmetries.resize(3 * symmetries.size() + 3);
    Symmetries.setPrimitiveType(sf::PrimitiveType::Triangles);

    for (int i = 0; i < symmetries.size(); i++) {
        current = &Symmetries[i * 3];
        auto v = symmetries[i].getCursor();
        current[0].position = sf::Vector2f(v.x * m_square_size, v.y * m_square_size);
        current[1].position = sf::Vector2f((1 + v.x) * m_square_size, v.y * m_square_size);
        current[2].position = sf::Vector2f((0.5f + v.x) * m_square_size, (1 + v.y) * m_square_size);
        for (int z = 0; z < 3; z++)
            current[z].color = symmetries[i].getColor();
    }
    auto blueprint_start_point = blueprint.getStartPoint();
    current = &Symmetries[Symmetries.getVertexCount() - 3];
    current[0].position = sf::Vector2f((blueprint_start_point.x) * m_square_size,
                                       (1 + blueprint_start_point.y) * m_square_size);
    current[1].position = sf::Vector2f((1 + blueprint_start_point.x) * m_square_size,
                                       (1 + blueprint_start_point.y) * m_square_size);
    current[2].position = sf::Vector2f((blueprint_start_point.x + 0.5f) * m_square_size,
                                       (blueprint_start_point.y) * m_square_size);
    current[0].color = sf::Color::Magenta;
    current[1].color = sf::Color::Cyan;
    current[2].color = sf::Color::White;
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
                blueprint.insert(cursorpos.x, cursorpos.y, Floornum, current_designation->first);
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
                change_designation(true);
                break;
            case sf::Keyboard::Dash:
                change_designation(false);
                break;
            case sf::Keyboard::Return:
                blueprint.setDesignation(current_designation->first);
                blueprint.setDesignationToggle(cursorpos.x, cursorpos.y, Floornum,
                                               event.key.shift ? Blueprint::CIRCLE : (event.key.control
                                                                                      ? Blueprint::LINE
                                                                                      : Blueprint::RECTANGLE));
                break;
            case sf::Keyboard::Numpad9:
                move_cursor(-offset_size, -offset_size);
                break;
            case sf::Keyboard::Numpad1:
                move_cursor(offset_size, offset_size);
                break;
            case sf::Keyboard::Numpad7:
                move_cursor(offset_size, -offset_size);
                break;
            case sf::Keyboard::Numpad3:
                move_cursor(-offset_size, offset_size);
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
        current_designation++;
        if (current_designation == designation_colors.end())
            current_designation = designation_colors.begin();
    } else {
        if (current_designation == designation_colors.begin())
            current_designation = designation_colors.end();
        current_designation--;
    }
    blueprint.setDesignation(current_designation->first);
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
                                   event.mouseButton.button == 0 ? Blueprint::RECTANGLE : Blueprint::LINE);
    build_vertex_array();
}
void PlanRenderer::handleMouseOver(const sf::Vector2f& b){
    sf::Vector2i mouse_position = sf::Vector2i((int) floor(b.x / m_square_size), (int) floor(b.y / m_square_size));
    cursorpos=mouse_position;
    build_designation();

}
