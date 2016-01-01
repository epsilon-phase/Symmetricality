//
// Created by awhite on 1/1/16.
//

#ifndef SYMMETRICALITY_BLUEPRINT_H
#define SYMMETRICALITY_BLUEPRINT_H
#include "utilities.hpp"
#include "Symmetry.h"
#include <vector>
#include <unordered_map>
#include <map>
class Blueprint {
public:
    enum application_pattern{
        RECTANGLE,
        LINE,
        CIRCLE,
        NONE
    };
    Blueprint();
    ~Blueprint();
    void insert(int x,int y,int z,char designation);
    const std::unordered_map<sf::Vector2i,char>& getLevelDesignation(int level);
    void designate(application_pattern e=RECTANGLE);
    void setDesignationToggle(int x,int y,int z,application_pattern e=RECTANGLE);
    void setDesignationToggle(sf::Vector3i,application_pattern e=RECTANGLE);
    void serialize(const std::string& file)const;
    void export_csv(const std::string &file)const;
    void deserialize(const std::string& file);
    void setDesignation(char);
    bool isDesignating()const;
    sf::Vector3i getDesignationStart()const;
    const std::vector<Symmetry>& getSymmetries()const;
    const sf::Vector2i getStartPoint()const;
    void addSymmetry(int,int,Symmetry::Symmetry_Type);
private:
    void getBounds(int& minx,int& miny,int&maxx,int&maxy)const;
    void setDesignation(int x,int y,int z,char d);
    void designateRectangle();
    void designateCircle();
    void designateLine();
    application_pattern designation_type=NONE;
    sf::Vector3i designation_start,designation_end;
    sf::Vector2i blueprint_start_point;
    char current_designation;
    std::map<int,std::unordered_map<sf::Vector2i,char> > _Designations;
    std::vector<Symmetry> _symmetries;
    bool start_set=false;
};


#endif //SYMMETRICALITY_BLUEPRINT_H
