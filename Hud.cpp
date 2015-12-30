//
// Created by awhite on 12/29/15.
//

#include <sstream>
#include "Hud.h"

Hud::Hud(PlanRenderer &r) : renderer(r) {
    this->Hud_font.loadFromFile("LinLibertine_DRah.ttf");
    this->Location.setCharacterSize(15);
    this->Location.setFont(Hud_font);
    this->Designation_type.setCharacterSize(15);
    this->Designation_type.setFont(Hud_font);
    this->Save.setFont(Hud_font);
    Save.setCharacterSize(15);
    Save.setPosition(0, 50);
    Designation_type.setPosition(0, 75);
    Location.setPosition(0, 15);
    update_text();
}

Hud::~Hud() {

}

bool Hud::handle_event(const sf::Event &event) {
    bool handle_it = true;
    if (event.type == sf::Event::TextEntered && save_type != NO_ENTRY) {
        if (event.text.unicode < 128) {
            if (event.text.unicode == 8) {
                save_file.pop_back();
            } else
                save_file.push_back(static_cast<char>(event.text.unicode));
        }
    } else if (event.type == sf::Event::KeyPressed) {
        handle_it = save_type == NO_ENTRY;
        switch (event.key.code) {
            case sf::Keyboard::F5:
                if (save_type == NO_ENTRY) {
                    save_type = event.key.shift ? SAVE_EXPORT : SAVE_SERIALIZE;
                } else
                    finish_save();
                break;
            case sf::Keyboard::Return:
                if(save_type!=NO_ENTRY)
                finish_save();
                break;
        }
    }
    update_text();
    return handle_it;
}

void Hud::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(Designation_type);
    target.draw(Location);
    if (save_type != NO_ENTRY)
        target.draw(Save);
}

void Hud::update_text() {
    std::stringstream f;
    if (old_desig != renderer.current_designation->first) {

        f << "Designating: " << renderer.current_designation->first;
        Designation_type.setString(f.str());
        old_desig = renderer.current_designation->first;
    }
    f.str("");//empty the stringstream
    if (old_cursor != renderer.cursorpos || old_floor != renderer.Floornum) {
        old_cursor = renderer.cursorpos;
        old_floor = renderer.Floornum;
        f << "(" << old_cursor.x << "," << old_cursor.y << "," << old_floor << ")";
        Location.setString(f.str());
    }
    f.str("");
    switch (save_type) {
        case SAVE_EXPORT:
            f << "Export to:";
            break;
        case SAVE_SERIALIZE:
            f << "Save to:";
            break;
        case LOAD_DESERIALIZE:
            f << "Load from:";
            break;
        case NO_ENTRY:
            f << "Working file:";
            break;
    }
    f << save_file;
    Save.setString(f.str());
}

void Hud::finish_save() {
    switch (save_type) {
        case SAVE_SERIALIZE:
            renderer.serialize(save_file);
            break;
        case SAVE_EXPORT:
            renderer.export_csv(save_file);
            break;
        case LOAD_DESERIALIZE:
            renderer.deseserialize(save_file);
            break;
    }
    save_type = NO_ENTRY;
}
