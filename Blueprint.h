//
// Created by awhite on 1/1/16.
//

#ifndef SYMMETRICALITY_BLUEPRINT_H
#define SYMMETRICALITY_BLUEPRINT_H
#include "utilities.hpp"
#include "Symmetry.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include "Building.h"
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
    const std::unordered_set<sf::Vector2i> & getImpliedDesignation(int level);
    void setDesignationToggle(int x,int y,int z,application_pattern e=RECTANGLE,bool build=false);
    void setDesignationToggle(sf::Vector3i,application_pattern e=RECTANGLE,bool build=false);
    void serialize(const std::string& file)const;
    void export_csv(const std::string &file)const;
    void deserialize(const std::string& file);
    void setDesignation(char);
    bool isDesignating()const;
    sf::Vector3i getDesignationStart()const;
    const std::vector<Symmetry>& getSymmetries()const;
    const sf::Vector2i getStartPoint()const;
    void addSymmetry(int,int,Symmetry::Symmetry_Type);
    void setStart(int,int);
	bool canPlace(int x, int y, int z, const Building &building);
    void placeBuilding(int x,int y,int z,const Building& building);
    const std::unordered_map<sf::Vector2i,std::string> getLevelBuildings(int level);
	void setBuilding(const Building* the_thing);
private:
    void getBounds(int& minx,int& miny,int&maxx,int&maxy)const;
    void setDesignation(int x,int y,int z,char d);
    void designateRectangle(bool building);
	void designateCircle(bool building);
	void designateLine(bool building);
	void designate(application_pattern e = RECTANGLE,bool building=false);
	void remove_occupation(int x,int y,int z,sf::Vector2i size);
    std::vector<sf::Vector2i> applySymmetry(sf::Vector2i)const;
    application_pattern designation_type=NONE;
    sf::Vector3i designation_start,designation_end;
    sf::Vector2i blueprint_start_point;
    char current_designation;
	const Building* current_building=NULL;
    std::map<int,std::unordered_map<sf::Vector2i,char> > _Designations;
    std::map<int,std::unordered_map<sf::Vector2i,std::string> > _Buildings;
	std::map<int, std::unordered_map<sf::Vector2i, sf::Vector2i> > _Buildings_size;
	std::vector<sf::Vector2i> getInRadius(int x, int y, sf::Vector2i rad);
	std::map<int, std::unordered_set<sf::Vector2i> > _occupation;
	std::map<int,std::unordered_set<sf::Vector2i> > _implied;
    std::vector<Symmetry> _symmetries;
    bool start_set=false;
};


#endif //SYMMETRICALITY_BLUEPRINT_H
