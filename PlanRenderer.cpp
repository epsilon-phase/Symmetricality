//
// Created by awhite on 12/27/15.
//

#include "PlanRenderer.h"
#include <iostream>
#include <fstream>

PlanRenderer::PlanRenderer() {
    this->Designations[0] = std::unordered_map<sf::Vector2i, char>();
    this->current_floor = &Designations.begin()->second;

}

PlanRenderer::~PlanRenderer() {

}

void PlanRenderer::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = NULL;
    target.draw(Rendering_plan, states);
    target.draw(Symmetries, states);
    if (is_designating)
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
    Rendering_plan.resize(current_floor->size() * 4 + 4 + 4 * is_designating);
    Rendering_plan.setPrimitiveType(sf::PrimitiveType::Quads);
    if (designations_updated) {
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
    }
    if (current_designation_type != NONE) {
        current = &Rendering_plan[Rendering_plan.getVertexCount() - 8];
        MAK_SQUAR(m_start_desig.x, m_start_desig.y, sf::Color(255, 220, 220));
    }
    current = &Rendering_plan[Rendering_plan.getVertexCount() - 4];
    current[0].position.x = cursorpos.x * m_square_size;
    current[0].position.y = cursorpos.y * m_square_size;
    current[1].position = sf::Vector2f(current[0].position.x + m_square_size, current[0].position.y);
    current[2].position = sf::Vector2f(current[1].position.x, current[1].position.y + m_square_size);
    current[3].position = sf::Vector2f(current[2].position.x - m_square_size, current[2].position.y);
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
    (*current_floor)[i] = i1;
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
                insert();
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
                if (event.key.shift)
                    do_designation(CIRCLE);
                else {
                    if (event.key.control)
                        do_designation(LINE);
                    else
                        do_designation(RECTANGLE);
                }
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
    }

}

void PlanRenderer::move_cursor(int x, int y) {
    cursorpos.x += x;
    cursorpos.y += y;
    if (is_designating)
        m_end_desig = sf::Vector3i(cursorpos.x, cursorpos.y, Floornum);
    build_vertex_array();
}

void PlanRenderer::move_z(int offset) {
    this->Floornum += offset;
    current_floor = &Designations[Floornum];
    build_vertex_array();
}

void PlanRenderer::set_Designation(int x, int y, int z) {
    if (!is_removing)
        Designations[z][sf::Vector2i(x, y)] = current_designation->first;
    else
        Designations[z].erase(sf::Vector2i(x, y));
}

void PlanRenderer::insert(int x, int y, int z) {
    std::vector<sf::Vector2i> things_accrued;
    things_accrued.push_back(sf::Vector2i(x, y));
    std::vector<sf::Vector2i> temp;
    for (auto s : symmetries) {
        for (auto i : things_accrued) {
            sf::Vector2i a = i;
            for (int z = 0; z < s.getRepetitionRequired(); z++) {
                a = s.fromPos(a);
                temp.push_back(a);
            }
        }
        for (auto f:temp)
            things_accrued.push_back(f);
        temp.clear();
    }
    for (auto i:things_accrued) {
        set_Designation(i.x, i.y, z);
    }
    designations_updated = true;
}

void PlanRenderer::insert() {
    insert(cursorpos.x, cursorpos.y, Floornum);
}

void PlanRenderer::add_symmetry(Symmetry::Symmetry_Type type) {
    auto s = Symmetry(type);
    s.setCursor(cursorpos);
    auto v = std::find(symmetries.begin(), symmetries.end(), s);
    if (v != symmetries.end()) {
        symmetries.erase(v);
    } else {
        symmetries.push_back(s);
    }

}

void PlanRenderer::do_designation(designation_type e) {
    m_end_desig = sf::Vector3i(cursorpos.x, cursorpos.y, Floornum);

    switch (current_designation_type) {
        case RECTANGLE:
            designate_rectangle();
            current_designation_type = NONE;
            break;
        case CIRCLE:
            designate_circle();
            current_designation_type = NONE;
            break;
        case LINE:
            designate_line();
            current_designation_type = NONE;
            break;
        case NONE:
            is_designating = true;
            current_designation_type = e;
            m_start_desig = sf::Vector3i(cursorpos.x, cursorpos.y, Floornum);
            break;
    }
    build_vertex_array();
}

void PlanRenderer::build_designation() {
    if (current_designation_type != NONE) {
        const sf::Vector3i &start = m_start_desig;
        const sf::Vector3i &end = m_end_desig;
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
    is_removing = current_designation->first == 'x';
}

void PlanRenderer::export_csv(const std::string &string) const {
    auto r = Designations.rbegin();
    int minx, miny, maxx, maxy;
    getBounds(minx, miny, maxx, maxy);
    std::ofstream zzip(string);
    zzip << "#dig start(" << std::abs(minx) + 1 << ";" << std::abs(miny) + 1 << ")" << std::endl;
    while (r != Designations.rend()) {
        for (int y = miny; y <= maxy; y++) {
            for (int x = minx; x <= maxx; x++) {
                auto f = r->second.find(sf::Vector2i(x, y));
                if (f != r->second.end()) {
                    zzip << f->second;
                } else {
                    zzip << " ";
                }
                zzip << ",";
            }
            zzip << "#" << std::endl;
        }
        r++;
        if (r != Designations.rend()) {
            zzip << "#>\r\n";
        }
    }
    zzip.close();
}

void PlanRenderer::serialize(const std::string &string) const {
    std::ofstream output(string);
    for (auto i:Designations) {
        for (auto z:i.second) {
            auto position = z.first;
            output << i.first << " " << position.x << " " << position.y << " " << z.second << " " << "\n";
        }
    }
    output.close();
}

void PlanRenderer::deserialize(const std::string &string) {
    char d;
    int x, y, z;
    std::ifstream input(string);
    Designations.clear();
    while (input >> x >> y >> z >> d) {
        Designations[z][sf::Vector2i(x, y)] = d;
    }
    build_vertex_array();
}

void PlanRenderer::getBounds(int &minx, int &miny, int &maxx, int &maxy) const {
    minx = 1 << 30;
    miny = 1 << 30;
    maxx = -minx;
    maxy = -miny;
    for (auto i: Designations) {
        for (auto a:i.second) {
            if (a.first.x < minx)
                minx = a.first.x;
            if (a.first.x > maxx)
                maxx = a.first.x;
            if (a.first.y < miny)
                miny = a.first.y;
            if (a.first.y > maxy)
                maxy = a.first.y;
        }
    }
}

void PlanRenderer::designate_rectangle() {
    if (m_start_desig.x > m_end_desig.x)
        std::swap(m_start_desig.x, m_end_desig.x);
    if (m_start_desig.y > m_end_desig.y)
        std::swap(m_start_desig.y, m_end_desig.y);
    if (m_start_desig.z > m_end_desig.z)
        std::swap(m_start_desig.z, m_end_desig.z);
    for (int x = m_start_desig.x; x <= m_end_desig.x; x++) {
        for (int y = m_start_desig.y; y <= m_end_desig.y; y++) {
            for (int z = m_start_desig.z; z <= m_end_desig.z; z++) {
                insert(x, y, z);
            }
        }
    }
}

void PlanRenderer::designate_circle() {
    int dx = m_start_desig.x - m_end_desig.x,
            dy = m_start_desig.y - m_end_desig.y;
    int radius = (int) std::sqrt(dx * dx + dy * dy);
    for (int x = m_start_desig.x - radius; x <= m_start_desig.x + radius; x++) {
        for (int y = m_start_desig.y - radius; y <= m_start_desig.y + radius; y++) {
            dx = m_start_desig.x - x;
            dy = m_start_desig.y - y;
            if (std::sqrt(dx * dx + dy * dy) <= radius) {
                for (int z = m_start_desig.z; z <= m_end_desig.z; z++)
                    insert(x, y, z);
            }
        }
    }
}

void PlanRenderer::designate_line() {
    int step_x = 1, step_y = 1;
    const int startx = m_start_desig.x,
            starty = m_start_desig.y;
    const int endx = m_end_desig.x,
            endy = m_end_desig.y;
    if (startx > endx)
        step_x = -1;
    if (starty > endy)
        step_y = -1;
    const float dx = abs(endx - startx);
    const float dy = abs(endy - starty);
    float err;
    if (dx > dy) {
        err = dx / 2.0f;
        int x = startx;
        int y = starty;
        while (x != endx) {
            for (int i = std::min(m_start_desig.z, m_end_desig.z); i <= std::max(m_start_desig.z, m_end_desig.z); i++) {
                insert(x, y, i);
            }
            err = err - dy;
            if (err < 0) {
                x = x + step_x;
                err = err + dy;
            }
            y += step_y;
        }
    } else {
        int x = startx;
        int y = starty;
        err = dy / 2.0f;
        while (y != endy) {
            for (int i = std::min(m_start_desig.z, m_end_desig.z); i <= std::max(m_start_desig.z, m_end_desig.z); i++) {
                insert(x, y, i);
            }
            err = err - dx;
            if (err < 0) {
                x += step_x;
                err += dy;
            }
            y += step_y;
        }
    }
}

