#include "Hud.h"
#include "PlanRenderer.h"
#include "utilities.hpp"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main() {
  sf::RenderWindow target(sf::VideoMode(512, 256), "Symmetricality");
  sf::View view = target.getView(), hud_view = target.getView();
  view.setCenter(0, 0);
  target.setView(view);
  std::ifstream j("Symmetricality.json");
  Json::Value root;
  j >> root;
  j.close();
  PlanRenderer plan;
  Hud display(plan);
  display.set_default_file_path(root.get("default_path", "").asString());
  target.setFramerateLimit(60);
  plan.LoadConfiguration(root);
  while (target.isOpen()) {
    sf::Event e;
    while (target.pollEvent(e)) {
      if (e.type == sf::Event::Closed) {
        target.close();
      }
      if (e.type == sf::Event::MouseButtonPressed ||
          e.type == sf::Event::MouseButtonReleased ||
          e.type == sf::Event::MouseWheelMoved ||
          e.type == sf::Event::MouseMoved) {
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
        hud_view.setSize(e.size.width, e.size.height);
        hud_view.setCenter(e.size.width / 2, e.size.height / 2);
      }
      if (display.handle_event(e))
        plan.handle_event(e);
      if (e.type == sf::Event::KeyPressed) {
        if (e.key.code == sf::Keyboard::F2) {
          sf::Texture c;
          c.create(target.getSize().x, target.getSize().y);
          c.update(target);
          display.save_screenshot(c.copyToImage());
        }
      }
    }
    target.clear();
    target.setView(view);
    target.draw(plan);
    target.setView(hud_view);
    target.draw(display);
    target.display();
  }
  return 0;
}
