//
// Created by awhite on 12/27/15.
//

#ifndef SYMMETRICALITY_PLANRENDERER_H
#define SYMMETRICALITY_PLANRENDERER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <unordered_map>
#include "utilities.hpp"
#include "Symmetry.h"

const static std::map<char, sf::Color> designation_colors = {{'d', sf::Color(200, 200, 0)},
                                                             {'j', sf::Color(255, 255, 0)},
                                                             {'i', sf::Color(0, 255, 0)},
                                                             {'u', sf::Color(255, 0, 0)},
                                                             {'x',sf::Color(0,0,0)}};
class PlanRenderer : public sf::Drawable, sf::Transformable {
friend class Hud;
    sf::VertexArray Rendering_plan;
    sf::VertexArray Symmetries;
    sf::VertexArray Designation_preview;
    int Floornum = 0;
    sf::Vector2i cursorpos=sf::Vector2i(0,0);
    int m_square_size = 10;
    std::unordered_map<sf::Vector2i, char> *current_floor = NULL;
    std::map<int,std::unordered_map<sf::Vector2i, char> > Designations;
    sf::Vector3i m_start_desig, m_end_desig;
    sf::Vector2i blueprint_start_point=sf::Vector2i(0,0);
    enum designation_type{
        RECTANGLE,
        CIRCLE,
        LINE,
        NONE
    };
    designation_type current_designation_type=NONE;
    bool is_designating = false;
    bool drawing_circle=false;
    std::map<char,sf::Color>::const_iterator current_designation=designation_colors.begin();
    std::vector<Symmetry> symmetries;
    bool designations_updated=false;
    bool is_removing=false;

public:
    PlanRenderer();

    virtual ~PlanRenderer();
    void set_pos(sf::Vector2i,char);
    void handle_event(sf::Event event);
private:
    void move_z(int offset);

    void move_cursor(int x, int y);

    void build_vertex_array();

    void set_Designation(int x,int y,int z);
    void insert(int x,int y,int z);
    void insert();
    void add_symmetry(Symmetry::Symmetry_Type type);
    void do_designation(designation_type e=RECTANGLE);
    void build_designation();
    void change_designation(bool up);

    void serialize(const std::string& )const;
    void export_csv(const std::string &)const;
    void deserialize(const std::string &);
    void getBounds(int &minx,int &miny, int &maxx,int &maxy)const;
    void designate_rectangle();
    void designate_circle();
    void designate_line();
protected:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};


#endif //SYMMETRICALITY_PLANRENDERER_H
