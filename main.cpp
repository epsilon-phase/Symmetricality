#include <iostream>
#include <SFML/Graphics.hpp>
#include "PlanRenderer.h"
#include "Hud.h"
#include "GetPot"
#include <string>

using namespace std;

int main() {
    sf::RenderWindow target(sf::VideoMode(512, 256), "Symmetricality");
    sf::View view = target.getView();
    view.setCenter(0, 0);
    target.setView(view);
    GetPot config("Symmetricality.pot");
    PlanRenderer plan;
    Hud display(plan);
    display.set_default_file_path(config("default_path", "."));
    designation_colors['d'] = sf::Color(config("colors/dig/R", 200), config("colors/dig/G", 200),
                                        config("colors/dig/B", 0));
    designation_colors['i'] = sf::Color(config("colors/up_down_stairs/R", 0), config("colors/up_down_stairs/G", 255),
                                        config("colors/up_down_stairs/B", 0));
    designation_colors['j'] = sf::Color(config("colors/downward_stairs/R", 255),
                                        config("colors/downward_stairs/G", 255), config("colors/downward_stairs/B", 0));
    designation_colors['u'] = sf::Color(config("colors/upward_stairs/R", 255), config("colors/upward_stairs/G", 0),
                                        config("colors/upward_stairs/B", 0));

    while (target.isOpen()) {
        sf::Event e;
        while (target.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                target.close();
            }
            if (e.type == sf::Event::MouseButtonPressed) {
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
    cout << "Hello, World!" << endl;
    return 0;
}