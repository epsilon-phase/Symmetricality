#include "RadialMenu.h"
#include <cmath>
#include <iostream>
RadialMenu::RadialMenu()
{
	continuation.loadFromFile("menu_continuance.png");
}


RadialMenu::~RadialMenu()
{
}
void RadialMenu::addItem(const sf::Texture&t, sf::IntRect texrect, std::function<void()> act){
	if (displayables.size() >= max*menu_added_artificially){
		sf::RectangleShape z;
		z.setTexture(&continuation);
		z.setSize(sf::Vector2f(rect_size, rect_size));
		z.setFillColor(sf::Color(255, 255, 255)); 
		displayables.push_back(z);
		actions.push_back([=](){
			this->increaseStart();
		});
		menu_added_artificially++;
	}
	sf::RectangleShape e;
	e.setSize(sf::Vector2f(rect_size, rect_size));
	e.setTexture(&t);
	e.setTextureRect(texrect);
	actions.push_back(act);
	displayables.push_back(e);
}
void RadialMenu::draw(sf::RenderTarget& target, sf::RenderStates)const{
	if (closed)
		return;
	int c_offset = opening ? rect_size - (rect_size - since_opened/open_rate) : rect_size;
	since_opened++;
	sf::Vector2f start_pos(pos.x+c_offset, pos.y+c_offset);
	int divisor = std::min((int)displayables.size()-start_index,max);
    int count_displayed = 0;
	for (unsigned int i = 0; i < displayables.size(); i++){
		sf::Transform t;
		t = t.rotate(i*(360.0f / divisor),pos.x,pos.y);
		if (i < start_index)
			continue;
        if (count_displayed == max)
            break;
        count_displayed ++;
		if (i == selected){
			displayables[i].setOutlineColor(sf::Color(255, 127, 0));
			displayables[i].setOutlineThickness(1.5);
		}
		else{
			displayables[i].setOutlineThickness(0);
		}
		displayables[i].setPosition(t.transformPoint(start_pos));
		target.draw(displayables[i]);
	}
	if (since_opened/open_rate >= rect_size)
		opening = false;
}
void RadialMenu::open(sf::Vector2f pos){
	this->pos = pos;
	closed = false;
	since_opened = 0;
	opening = true;
	start_index = 0;
}
bool RadialMenu::handle_event(sf::Event evt,sf::Vector2f coord){
	if (closed)//If the menu is closed, no event shall be handled.
		return false;//this means that the event shall continue into the rest of the Planrenderer input method
	if (evt.type == sf::Event::MouseMoved){//for updating the selection for drawing.
		selected=intersects(coord);
		/*The menu does not block other events from happening when the mouse is moved 
		* and it is open.
		*/
		return false;
	}
    if (evt.type == sf::Event::MouseWheelMoved&&intersects(sf::Vector2f(evt.mouseWheel.x, evt.mouseWheel.y))){
        start_index += evt.mouseWheel.delta;
        if (start_index < 0){
            start_index = displayables.size() - max;
        }
        return true;
    }
	if (evt.type == sf::Event::MouseButtonPressed){//for menu interaction and closing.
		if (evt.mouseButton.button == 0){
			int i = intersects(coord);
			if (i == -1){
				close();
			}
			else{
				actions[i]();//call the associated action
			}
			return true;//This means that the PlanRenderer input method shall not proceed in handling this one event.
		}
	}
	return false;
}
int RadialMenu::intersects(sf::Vector2f f)const{
	for (unsigned int i = 0; i < displayables.size(); i++){
		if (i >= start_index &&
			displayables[i].getGlobalBounds().contains(f)){
			return i;
			break;
		}
	}
	return -1;
}
void RadialMenu::close(){
	closed = true;
}
void RadialMenu::increaseStart(){
	start_index += max;
}