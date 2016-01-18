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
#include "Building.h"
#include "GetPot"
#include "RadialMenu.h"

/**
* Prototype class for terrible circular requirements. Perfect for cold weekends.
*/
class Hud;
class PlanRenderer : public sf::Drawable, sf::Transformable {
	std::map<char, sf::Color> designation_colors;
friend class Hud;
    sf::VertexArray Rendering_plan;
    sf::VertexArray Symmetries;
    sf::VertexArray Designation_preview;
    sf::VertexArray Cursor;
    sf::VertexArray Buildings;
    std::map<std::string,Building> _building_types;
    Hud* the_hud;
    int Floornum = 0;
    sf::Vector2i cursorpos=sf::Vector2i(0,0);
    int m_square_size = 10;
    Blueprint blueprint;
    sf::Texture buildingTexture;
    sf::Texture menu_utility_texture;
	/**
	* Designation stuff
	*/
	std::map<char, sf::IntRect> designation_texcoords;
	sf::Image designation_src;
	sf::Texture designationTexture;
	bool designationsUseTextures = false;
	/**
	* A pointer to the types of designation currently supported(with associated colors)
	*/
	std::map<char, sf::Color>::const_iterator current_designation = designation_colors.begin();
    
	bool building_changed=false;
    bool designation_changed=false;

    std::map<std::string,Building>::const_iterator current_building;
	
    /**
     * If true, then there has been no keyboard input since pressing C, once this is true, then the blueprint may be cleared.
     */
    bool clear_primed=false;
    bool building_mode=false;
    /**
     * Mouse stuff, for panning and zooming.
     */
    bool right_button_down=false;
    sf::Vector2f old_mouse_pos=sf::Vector2f(0,0);
    sf::Clock since_last_click;
    int click_count=0;
    /**
     * Used to determine if the blueprint needs to be shifted for the cursor to be visible.
     * Updated every call to draw.
     */
    mutable sf::View viewable_area;
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
    void loadBuildingTexture(const std::string& filename);
    
    void getLoadBuildings(Json::Value);
    void loadDesignationConfiguration(Json::Value &);
	void setDesignation(char e);
	void setBuilding(const std::string& r);
    /**
    * Write a serialized representation of the current blueprint to a file
    */
    void serialize(const std::string&)const;
    /**
    * Export a Csv(Comma separated value) file with z-levels separated by #>
    */
    void export_csv(const std::string &)const;
    /**
    * Deserialize a file from the serialize function
    */
    void deserialize(const std::string &);
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


	bool canPlace()const;
    /**
     * Creates the designation square
     */
    void generate_designation_tile(int x, int y, char designation, sf::Vertex *c);
protected:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
private:
    void buildDesignationArray();
    void buildBuildingArray();
    void buildCursorArray();
    void buildSymmetryArray();
	void initializeMenu();
	RadialMenu menu;
	RadialMenu build_menu;
};


#endif //SYMMETRICALITY_PLANRENDERER_H
