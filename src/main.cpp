#include <iostream>
#include <SFML/Graphics.hpp>
#include "PlanRenderer.h"
#include "Hud.h"
#include <string>
#include <fstream>
#include "utilities.hpp"
using namespace std;

int main() {
    sf::RenderWindow target(sf::VideoMode(512, 256), "Symmetricality");
    sf::View view = target.getView();
    view.setCenter(0, 0);
    target.setView(view);
    std::ifstream j("Symmetricality.json");
    Json::Value root;
    j >> root;
    j.close();
    PlanRenderer plan;
    Hud display(plan);
    display.set_default_file_path(root.get("default_path", "").asString());
	plan.LoadConfiguration(root);
    while (target.isOpen()) {
        sf::Event e;
        while (target.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                target.close();
            }
            if (e.type == sf::Event::MouseButtonPressed||e.type==sf::Event::MouseButtonReleased
				|| e.type == sf::Event::MouseWheelMoved || e.type == sf::Event::MouseMoved) {
                auto coord = sf::Mouse::getPosition(target);
                auto mcoord = target.mapPixelToCoords(coord, view);
                plan.handle_mouse(e, mcoord);
            }
            if (e.type == sf::Event::MouseMoved) {
                auto coord = sf::Mouse::getPosition(target);
                auto mcoord = target.mapPixelToCoords(coord, view);
                plan.handleMouseOver(mcoord);
            }
            if (e.type == sf::Event::Resized) {
                view.setSize(e.size.width, e.size.height);
            }
            if (display.handle_event(e))
                plan.handle_event(e);
            if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::F2) {
                    display.save_screenshot(target.capture());
                }
            }
        }
        target.clear();
        target.setView(view);
        target.draw(plan);
        target.setView(target.getDefaultView());
        target.draw(display);
        target.display();
    }
    return 0;
}