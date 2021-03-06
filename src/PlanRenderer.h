//
// Created by Violet White on 12/27/15.
//

#ifndef SYMMETRICALITY_PLANRENDERER_H
#define SYMMETRICALITY_PLANRENDERER_H

#include "Blueprint.h"
#include "Building.h"
#include "BuildingFacet.h"
#include "DesignationFacet.h"
#include "RadialMenu.h"
#include "Symmetry.h"
#include "SymmetryFacet.h"
#include "utilities.hpp"
#include <SFML/Graphics.hpp>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>
/**
 * Prototype class for terrible circular requirements. Perfect for cold
 * weekends.
 */
class Hud;
class PlanRenderer : public sf::Drawable, sf::Transformable {
  std::set<char> designation_colors;
  friend class Hud;
  DesignationFacet designator;
  BuildingFacet buildings;
  SymmetryFacet symmetry;
  std::vector<Configurable *> configurables;
  // TODO decide on a better name than floornables
  std::vector<Floornable *> floornables;
  std::vector<PlanFacet *> facets;

  //    sf::VertexArray Rendering_plan;
  sf::VertexArray Symmetries;
  sf::VertexArray Designation_preview;
  sf::VertexArray Cursor;
  sf::VertexArray Buildings;
  std::map<std::string, Building> _building_types;
  Hud *the_hud;
  int Floornum = 0;
  void setFloor(int n) {
    for (auto i : floornables)
      i->setFloor(n);
    Floornum = n;
  }
  sf::Vector2i cursorpos = sf::Vector2i(0, 0);
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
   * A pointer to the types of designation currently supported(with associated
   * colors)
   */
  std::set<char>::const_iterator current_designation =
      designation_colors.begin();

  bool building_changed = false;
  bool designation_changed = false;

  std::map<std::string, Building>::const_iterator current_building;

  /**
   * If true, then there has been no keyboard input since pressing C, once this
   * is true, then the blueprint may be cleared.
   */
  bool clear_primed = false;
  bool building_mode = false;
  /**
   * Mouse stuff, for panning and zooming.
   */
  bool right_button_down = false;
  sf::Vector2f old_mouse_pos = sf::Vector2f(0, 0);
  sf::Clock since_last_click;
  int click_count = 0;
  /**
   * Used to determine if the blueprint needs to be shifted for the cursor to be
   * visible. Updated every call to draw.
   */
  mutable sf::View viewable_area;

public:
  PlanRenderer();

  virtual ~PlanRenderer();
  void set_pos(sf::Vector2i, char);
  /**
   * Handle the input.
   */
  void handle_event(sf::Event event);
  void handle_mouse(sf::Event event, const sf::Vector2f &b);
  void handleMouseOver(const sf::Vector2f &b);

  void setDesignation(char e);
  void setBuilding(const std::string &r);
  void LoadConfiguration(Json::Value &v);
  /**
   * Write a serialized representation of the current blueprint to a file
   */
  void serialize(const std::string &) const;
  /**
   * Export a Csv(Comma separated value) file with z-levels separated by #>
   */
  void export_csv(const std::string &) const;
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

  bool canPlace() const;

protected:
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
  void buildCursorArray();
  void initializeMenu();
  RadialMenu menu;
  RadialMenu build_menu;
};

#endif // SYMMETRICALITY_PLANRENDERER_H
