//
// Created by awhite on 12/28/15.
//

#include "Symmetry.h"

Symmetry::Symmetry(Symmetry_Type type):m_type(type) {

}

sf::Vector2i Symmetry::fromPos(const sf::Vector2i &pos) const {
    sf::Vector2i result;

    switch(m_type){
        case X:
        {
            int diff=pos.x-cursor.x;
            result.x=cursor.x-diff;
            result.y=pos.y;
        };
            break;
        case Y:
        {
            int diff=pos.y-cursor.y;
            result.y=cursor.y-diff;
            result.x=pos.x;
        };
            break;
        case Radial:
        {
            int diffx=pos.x-cursor.x;
            int diffy=pos.y-cursor.y;
            result.x=cursor.x-diffx;
            result.y=cursor.y-diffy;
        };
            break;
        case Rotational:
        {
            sf::Vector2i a;
            a.x=pos.x-cursor.x;
            a.y=pos.y-cursor.y;
            result.x=-a.y+cursor.x;
            result.y=a.x+cursor.y;
        };
            break;
        default:
            break;
    }
    return result;
}

bool Symmetry::operator==(const Symmetry &b) const {
    if(m_type==b.m_type){
        switch(m_type){
            case X:
                return cursor.x==b.cursor.x;
            case Y:
                return cursor.y==b.cursor.y;
            case Radial:
                return cursor==b.cursor;
            case Rotational:
                return cursor==b.cursor;
        }
    }
    return false;
}

int Symmetry::getRepetitionRequired() {
    if(m_type==Rotational)
        return 3;
    return 1;
}

sf::Color Symmetry::getColor() const {
    switch(m_type){
        case X:
            return sf::Color::Green;
        case Y:
            return sf::Color::Red;
        case Radial:
            return sf::Color::Blue;
        case Rotational:
            return sf::Color::Magenta;
    }
}

Symmetry::Symmetry(const Symmetry &symmetry):m_type(symmetry.m_type),cursor(symmetry.cursor) {


}

void Symmetry::setCursor(sf::Vector2i i) {
    cursor=i;

}
