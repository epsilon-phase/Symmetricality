#ifndef RADIALMENU_H
#define RADIALMENU_H
#include "utilities.hpp"
#include <functional>
class RadialMenu :
	public sf::Drawable
{
	/**
	* How many items are displayed at once.
	*/
	const int max=8;
	/**
	* The index of the item that is the first to be displayed.
	*/
	int start_index = 0;
	mutable std::vector<sf::RectangleShape> displayables;
	std::vector<std::function<void()> > actions;
	mutable bool opening = false;
	mutable int since_opened = 0;
	int menu_added_artificially = 0;
	/**
	* Is the menu closed?
	*/
	bool closed = true;
	sf::Vector2f pos;
	int selected = -1;
	const int rect_size=20;
	const int open_rate = 30;
public:
	RadialMenu();
	~RadialMenu();
	void addItem(const sf::Texture&, sf::IntRect,std::function<void()> act);
	void open(sf::Vector2f);
	bool handle_event(sf::Event evt, sf::Vector2f coord);
	void close();
	void increaseStart();
	int getSize()const{ return displayables.size(); }
protected:
	virtual void draw(sf::RenderTarget&, sf::RenderStates)const;
private:
	int intersects(sf::Vector2f)const;

};

#endif