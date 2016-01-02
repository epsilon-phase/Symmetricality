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
#include "Blueprint.h"
class PlanRenderer : public sf::Drawable, sf::Transformable {
    std::map<char, sf::Color> designation_colors = {{'d', sf::Color(200, 200, 0)},
                                                           {'j', sf::Color(255, 255, 0)},
                                                           {'i', sf::Color(0, 255, 0)},
                                                           {'u', sf::Color(255, 0, 0)},
                                                           {'x', sf::Color(0, 0, 0)}};
friend class Hud;
    sf::VertexArray Rendering_plan;
    sf::VertexArray Symmetries;
    sf::VertexArray Designation_preview;
    sf::VertexArray Cursor;
    int Floornum = 0;
    sf::Vector2i cursorpos=sf::Vector2i(0,0);
    int m_square_size = 10;
    Blueprint blueprint;

    /**
     * A pointer to the types of designation currently supported(with associated colors)
     */
    std::map<char,sf::Color>::const_iterator current_designation=designation_colors.begin();
    /**
     * If true, then there has been no keyboard input since pressing C, once this is true, then the blueprint may be cleared.
     */
    bool clear_primed=false;
public:
    PlanRenderer();

    virtual ~PlanRenderer();
    void set_pos(sf::Vector2i,char);
    /**
     * Handle the input.
     */
    void handle_event(sf::Event event);
    void handle_mouse(sf::Event event,const sf::Vector2f& b);
    void handleMouseOver(const sf::Vector2f& b);
    void setColor(char f,sf::Color c);
private:
    /**
     * Move up and down z-levels
     */
    void move_z(int offset);
    /**
     * Move around laterally.
     */
    void move_cursor(int x, int y);
    /**
     * Rebuild the vertex array that renders the plan.
     */
    void build_vertex_array();
    void add_symmetry(Symmetry::Symmetry_Type type);
    void build_designation();
    void change_designation(bool up);
    /**
     * Write a serialized representation of the current blueprint to a file
     */
    void serialize(const std::string& )const;
    /**
     * Export a Csv(Comma separated value) file with z-levels separated by #>
     */
    void export_csv(const std::string &)const;
    /**
     * Deserialize a file from the serialize function
     */
    void deserialize(const std::string &);
protected:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};


#endif //SYMMETRICALITY_PLANRENDERER_H
