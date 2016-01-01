//
// Created by awhite on 12/29/15.
//

#include <sstream>
#include "Hud.h"
#include "tinyfiledialogs.h"

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
    if (event.type == sf::Event::KeyPressed) {
        handle_it = save_type == NO_ENTRY;
        switch (event.key.code) {
            case sf::Keyboard::F1:
                draw_this=!draw_this;
                break;
            case sf::Keyboard::F5:
                /*if (save_type == NO_ENTRY) {
                    save_type = event.key.shift ? SAVE_EXPORT : SAVE_SERIALIZE;
                } else*/
            {
                const char *filters = event.key.shift ? "*.csv" : "*.ser";
                const char *filename = tinyfd_saveFileDialog("Save File",
                                                             event.key.shift ? "blueprint.csv" : "blueprint.ser", 1,
                                                             &filters,
                                                             event.key.shift ? "Csv file" : "Serialized File");
				if (filename != NULL){
					this->save_file = std::string(filename);
					save_type = event.key.shift ? SAVE_EXPORT : SAVE_SERIALIZE;
					finish_save();
				}
            }   
                break;
            case sf::Keyboard::F6: {
                const char *filters = "*.ser";
                const char *filename = tinyfd_openFileDialog("Load blueprint", "blueprint.ser", 1, &filters, "None", 0);
				if (filename){
					this->save_file = std::string(filename);
					save_type = LOAD_DESERIALIZE;
					finish_save();
				}
            }
                break;
        }
    }
    update_text();
    return handle_it;
}

void Hud::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    if (draw_this) {
        target.draw(Designation_type);
        target.draw(Location);
        target.draw(Save);
    }
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
        if (renderer.current_designation_type != PlanRenderer::NONE) {
            f << " " << std::abs(renderer.m_start_desig.x - renderer.m_end_desig.x) << "x" <<
            std::abs(renderer.m_start_desig.y - renderer.m_end_desig.y) << "x" <<
            std::abs(renderer.m_start_desig.z - renderer.m_end_desig.z);

        }
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
            renderer.deserialize(save_file);
            break;
    }
    save_type = NO_ENTRY;
}

void Hud::save_screenshot(const sf::Image &that) {
    std::stringstream a;
    a << save_file.substr(0, save_file.size() - 4) << "-" << screencap_no << ".png";
    screencap_no++;
    that.saveToFile(a.str());
}
