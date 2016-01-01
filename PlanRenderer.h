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
    /**
     * An ordered map containing each of the levels in a hashmap(associated with an integer)
     */
    std::map<char,sf::Color>::const_iterator current_designation=designation_colors.begin();
    /**
     * The list of symmetries currently active on the blueprint
     */
    std::vector<Symmetry> symmetries;
    /**
     * True if the vertex array with the designations needs to be updated.
     */
    bool designations_updated=false;
    /**
     * A flag that ought to be removed/simplified
     */
    bool is_removing=false;
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
    /**
     * Set the designation without additional processing(like symmetries)
     */
    void set_Designation(int x,int y,int z);
    /**
     * Insert the
     */
    void insert(int x,int y,int z);
    void insert();
    void add_symmetry(Symmetry::Symmetry_Type type);
    void do_designation(designation_type e=RECTANGLE);
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
    /**
     * Return the lateral bounds of the whole blueprint
     */
    void getBounds(int &minx,int &miny, int &maxx,int &maxy)const;
    /**
     * Designate a rectangle from m_start_desig to m_end_desig
     */
    void designate_rectangle();
    /**
     * Designate a circle with a radius of the distance between m_start_desig and m_end_desig
     */
    void designate_circle();
    /**
     *  Designate a line between m_start_desig and m_end_desig
     */
    void designate_line();
protected:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};


#endif //SYMMETRICALITY_PLANRENDERER_H
