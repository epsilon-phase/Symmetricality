//
// Created by awhite on 12/29/15.
//

#ifndef SYMMETRICALITY_HUD_H
#define SYMMETRICALITY_HUD_H
#include "PlanRenderer.h"

class Hud:public sf::Drawable {
    friend class PlanRenderer;
    sf::Font Hud_font;
    sf::Vector2i old_cursor;
    int old_floor;
    char old_desig;
	bool wasBuilding = false;
    sf::Text Designation_type;
    sf::Text Location;
    sf::Text Save;
    int screencap_no=0;
    std::string save_file;
    std::string default_file_path;
    bool draw_this=true;
    enum Entry_status{
    NO_ENTRY,
        SAVE_SERIALIZE,
        SAVE_EXPORT,
        LOAD_DESERIALIZE
    };
    Entry_status save_type=NO_ENTRY;
    PlanRenderer& renderer;
public:
    Hud(PlanRenderer&);
    ~Hud();
    bool handle_event(const sf::Event &event);
    void save_screenshot(const sf::Image& that);
    void set_default_file_path(const std::string& s);
private:
    void update_text();
    void finish_save();
protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;
};


#endif //SYMMETRICALITY_HUD_H
