//
// Created by awhite on 1/1/16.
//

#include "Blueprint.h"
#include <cmath>
#include <fstream>
#include <iostream>
Blueprint::Blueprint() {
  _Designations[0] = coordinate_map<char>();
  _Designations[0].reserve(0);
  _occupation[0].reserve(0);
}

Blueprint::~Blueprint() {}

void Blueprint::insert(int x, int y, int z, char designation) {
  auto things_accrued = applySymmetry(sf::Vector2i(x, y));
  for (auto i : things_accrued) {
    setDesignation(i.x, i.y, z, designation);
  }
}

const coordinate_map<char> &Blueprint::getLevelDesignation(int level) {
  if (_Designations.find(level) == _Designations.end())
    _Designations[level] = coordinate_map<char>();
  return _Designations.find(level)->second;
}

void Blueprint::designate(Blueprint::application_pattern e, bool building) {
  switch (e) {
  case RECTANGLE:
    designateRectangle(building);
    break;
  case CIRCLE:
    designateCircle(building);
    break;
  case LINE:
    designateLine(building);
    break;
  }
}

void Blueprint::setDesignationToggle(int x, int y, int z, application_pattern e,
                                     bool building_mode) {
  if (start_set) {
    designation_end = sf::Vector3i(x, y, z);
    designate(designation_type, building_mode);
    start_set = false;
  } else {
    designation_type = e;
    start_set = true;
    designation_start = sf::Vector3i(x, y, z);
  }
}
void Blueprint::stop_designation() { start_set = false; }
void Blueprint::serialize(const std::string &file) const {
  std::ofstream output(file);
  output << _symmetries.size() << std::endl;
  for (auto i : _symmetries)
    output << i.getCursor().x << " " << i.getCursor().y << " "
           << (int)i.getType() << std::endl;

  for (auto i : _Designations) {
    for (auto z : i.second) {
      auto position = z.first;
      output << position.x << " " << position.y << " " << i.first << " "
             << z.second << "\n";
    }
  }
  for (auto i : _Buildings)
    for (auto z : i.second) {
      auto position = z.first;
      output << position.x << " " << position.y << " " << i.first << " b "
             << z.second << "\n";
    }
  output.close();
}

void Blueprint::export_csv(const std::string &file) const {
  auto r = _Designations.rbegin();
  while (r->second.empty())
    r++;
  int minx, miny, maxx, maxy;
  getBounds(minx, miny, maxx, maxy);
  std::ofstream zzip(file);
  auto build_name = file;
  build_name.insert(file.size() - 4, "-build");
  std::ofstream build_output(build_name);
  zzip << "#dig start(" << std::abs(minx) + 1 << ";" << std::abs(miny) + 1
       << ")" << std::endl;
  build_output << "#build start(" << std::abs(minx) + 1 << ";"
               << std::abs(miny) + 1 << ")" << std::endl;
  while (r != _Designations.rend()) {
    bool building_for_this_level =
        _Buildings.find(r->first) != _Buildings.end();
    for (int y = miny; y <= maxy; y++) {
      for (int x = minx; x <= maxx; x++) {
        auto f = r->second.find(sf::Vector2i(x, y));
        if (building_for_this_level) {
          auto b = _Buildings.find(r->first)->second;
          ;
          if (b.find(sf::Vector2i(x, y)) != b.end()) {
            build_output << b.find(sf::Vector2i(x, y))->second;
          } else {
            build_output << " ";
          }
        } else
          build_output << " ";
        if (f != r->second.end()) {
          zzip << f->second;
        } else {
          zzip << " ";
        }
        zzip << ",";
        build_output << ",";
      }
      zzip << "#\r\n";
      build_output << "#\r\n";
    }

    r++;
    if (r != _Designations.rend()) {
      zzip << "#>\r\n";
      build_output << "#>\r\n";
    }
  }
  build_output.close();
  zzip.close();
}

void Blueprint::deserialize(const std::string &file) {
  char d;
  int x, y, z;
  std::ifstream input(file);
  input >> z;
  _symmetries.clear();
  for (int i = 0; i < z; i++) {
    int c;
    Symmetry::Symmetry_Type f = Symmetry::Symmetry_Type::X;
    input >> x >> y >> c;
    f = (Symmetry::Symmetry_Type)c;
    Symmetry z(f);
    z.setCursor(sf::Vector2i(x, y));
    _symmetries.push_back(z);
  }
  // Clear out the old stuff
  _Buildings.clear();
  _Designations.clear();
  // Load the new stuff
  while (input >> x >> y >> z >> d) {
    if (d == 'b') { // Then it is a building.
      std::string keys;
      input >> keys;
      _Buildings[z][sf::Vector2i(x, y)] = keys;
      continue;
    }
    _Designations[z][sf::Vector2i(x, y)] = d;
  }
  _implied.clear();
  build_implied();
}

void Blueprint::setDesignation(int x, int y, int z, char d) {
  _occupation[z].reserve(0);
  _implied[z + 1].reserve(0);
  _implied[z - 1].reserve(0);
  if (_occupation.find(z)->second.find(sf::Vector2i(x, y)) ==
      _occupation.find(z)->second.end()) {
    if (d != 'x') {
      auto c = this->_Designations[z].find(sf::Vector2i(x, y));
      if (c != this->_Designations[z].end()) {
        if (c->second == 'r' || c->second == 'i' || c->second == 'u')
          this->_implied[z + 1].erase(sf::Vector2i(x, y));
        if (c->second == 'h' || c->second == 'i' || c->second == 'j')
          this->_implied[z - 1].erase(sf::Vector2i(x, y));
      }
      this->_Designations[z][sf::Vector2i(x, y)] = d;
      if (d == 'r' || d == 'i' || d == 'j')
        this->_implied[z + 1].insert(sf::Vector2i(x, y));
      if (d == 'h' || d == 'i' || d == 'j')
        this->_implied[z - 1].insert(sf::Vector2i(x, y));
    } else {
      auto c = this->_Designations[z].find(sf::Vector2i(x, y));
      if (c != this->_Designations[z].end()) {
        if (c->second == 'r' || c->second == 'i' || c->second == 'j')
          this->_implied[z + 1].erase(sf::Vector2i(x, y));
        if (c->second == 'h' || c->second == 'i' || c->second == 'j')
          this->_implied[z - 1].erase(sf::Vector2i(x, y));
      }
      this->_Designations[z].erase(sf::Vector2i(x, y));
    }
  }
}

void Blueprint::getBounds(int &minx, int &miny, int &maxx, int &maxy) const {
  minx = 1 << 30;
  miny = 1 << 30;
  maxx = -minx;
  maxy = -miny;
  for (auto i : _Designations) {
    for (auto a : i.second) {
      if (a.first.x < minx)
        minx = a.first.x;
      if (a.first.x > maxx)
        maxx = a.first.x;
      if (a.first.y < miny)
        miny = a.first.y;
      if (a.first.y > maxy)
        maxy = a.first.y;
    }
  }
}

void Blueprint::designateRectangle(bool building) {
  if (designation_start.x > designation_end.x)
    std::swap(designation_start.x, designation_end.x);
  if (designation_start.y > designation_end.y)
    std::swap(designation_start.y, designation_end.y);
  if (designation_start.z > designation_end.z)
    std::swap(designation_start.z, designation_end.z);
  for (int x = designation_start.x; x <= designation_end.x; x++) {
    for (int y = designation_start.y; y <= designation_end.y; y++) {
      for (int z = designation_start.z; z <= designation_end.z; z++) {
        if (building) {
          placeBuilding(x, y, z, *current_building);
        } else {
          insert(x, y, z, current_designation);
        }
      }
    }
  }
}

void Blueprint::designateCircle(bool building) {
  int dx = designation_start.x - designation_end.x,
      dy = designation_start.y - designation_end.y;
  int rx = abs(dx), ry = abs(dy);
  float rx2 = rx * rx, ry2 = ry * ry;
  ;

  for (int x = designation_start.x - rx; x <= designation_start.x + rx; x++) {
    for (int y = designation_start.y - ry; y <= designation_start.y + ry; y++) {
      dx = designation_start.x - x;
      dy = designation_start.y - y;
      if (std::pow(x - designation_start.x, 2) / rx2 +
              std::pow(y - designation_start.y, 2) / ry2 <=
          1) {
        for (int z = designation_start.z; z <= designation_end.z; z++)
          if (building) {
            placeBuilding(x, y, z, *current_building);
          } else {
            insert(x, y, z, current_designation);
          }
      }
    }
  }
}

void Blueprint::designateLine(bool building) {
  int step_x = 1, step_y = 1;
  const int startx = designation_start.x, starty = designation_start.y;
  const int endx = designation_end.x, endy = designation_end.y;
  if (startx > endx)
    step_x = -1;
  if (starty > endy)
    step_y = -1;
  const float dx = abs(endx - startx);
  const float dy = abs(endy - starty);
  float err;
  if (dx > dy) {
    err = dx / 2.0f;
    int x = startx;
    int y = starty;
    while (x != endx) {
      for (int i = std::min(designation_start.z, designation_end.z);
           i <= std::max(designation_start.z, designation_end.z); i++) {
        if (building) {
          placeBuilding(x, y, i, *current_building);
        } else {
          insert(x, y, i, current_designation);
        }
      }
      err = err - dy;
      if (err < 0) {
        x = x + step_x;
        err = err + dy;
      }
      y += step_y;
    }
  } else {
    int x = startx;
    int y = starty;
    err = dy / 2.0f;
    while (y != endy) {
      for (int i = std::min(designation_start.z, designation_end.z);
           i <= std::max(designation_start.z, designation_end.z); i++) {
        if (building) {
          placeBuilding(x, y, i, *current_building);
        } else {
          insert(x, y, i, current_designation);
        }
      }
      err = err - dx;
      if (err < 0) {
        x += step_x;
        err += dy;
      }
      y += step_y;
    }
  }
}

void Blueprint::setDesignation(char i) { this->current_designation = i; }

void Blueprint::setBuilding(const Building *the_thing) {
  current_building = the_thing;
}

bool Blueprint::isDesignating() const { return start_set; }

void Blueprint::addSymmetry(int x, int y, Symmetry::Symmetry_Type type) {
  auto s = Symmetry(type);
  s.setCursor(sf::Vector2i(x, y));
  auto v = std::find(_symmetries.begin(), _symmetries.end(), s);
  if (v != _symmetries.end()) {
    _symmetries.erase(v);
  } else {
    _symmetries.push_back(s);
  }
}

void Blueprint::setDesignationToggle(sf::Vector3i i,
                                     Blueprint::application_pattern e,
                                     bool building) {
  setDesignationToggle(i.x, i.y, i.z, e, building);
}

sf::Vector3i Blueprint::getDesignationStart() const {
  return designation_start;
}

const std::vector<Symmetry> &Blueprint::getSymmetries() const {
  return _symmetries;
}

const sf::Vector2i Blueprint::getStartPoint() const {
  return blueprint_start_point;
}

void Blueprint::setStart(int i, int i1) {
  blueprint_start_point.x = i;
  blueprint_start_point.y = i1;
}

bool Blueprint::canPlace(int x, int y, int z, const Building &building) {
  auto f = building.getSize();
  auto c = building.getCenter();

  int x1 = x + c.x - f.x;
  int y1 = y + c.y - f.y;
  int x2 = x + (f.x - c.x);
  int y2 = y + (f.y - c.y);
  if (f.x == 1 && f.y == 1) {
    x1 = 0;
    x2 = 0;
    y1 = 0;
    y2 = 0;
  }
  if (f.x == 3 && f.y == 3) {
    x1 = -1;
    y1 = -1;
    x2 = 1;
    y2 = 1;
  }
  for (auto i : getInRadius(x, y, building.getSize()))
    if (_occupation[z].find(i) != _occupation[z].end())
      return false;
  for (int i = x1; i <= x2; i++)
    for (int j = y1; j <= y2; j++) {
      if (_Designations[z].end() ==
          _Designations[z].find(sf::Vector2i(i + x, j + y))) {
        return false;
      } else {
        if (_Designations[z].find(sf::Vector2i(i + x, y + j))->second != 'd')
          return false;
      }
    }
  return true;
}

std::vector<sf::Vector2i> Blueprint::applySymmetry(sf::Vector2i start) const {
  std::vector<sf::Vector2i> things_accrued;
  things_accrued.push_back(start);
  std::vector<sf::Vector2i> temp;
  if (_symmetries.size() > 0) {
    Symmetry::Symmetry_Type old = _symmetries.front().getType();
    for (auto s : _symmetries) {
      if (old != s.getType()) {
        for (auto f : temp)
          things_accrued.push_back(f);
        temp.clear();
        old = s.getType();
      }
      for (auto i : things_accrued) {
        sf::Vector2i a = i;
        for (int r = 0; r < s.getRepetitionRequired(); r++) {
          a = s.fromPos(a);
          temp.push_back(a);
        }
      }
    }
    if (temp.size() > 0)
      for (auto f : temp)
        things_accrued.push_back(f);
  }
  return things_accrued;
}

void Blueprint::placeBuilding(int x, int y, int z, const Building &building) {
  auto symmetry_results = applySymmetry(sf::Vector2i(x, y));
  for (auto i : symmetry_results) {
    if (building.getSequence() == "") {
      if (_Buildings[z].find(i) != _Buildings[z].end()) {
        remove_occupation(i.x, i.y, z, _Buildings_size[z].find(i)->second);
      }
      _Buildings[z].erase(i);
      _Buildings_size[z].erase(i);
    } else if (canPlace(i.x, i.y, z, building)) {
      _Buildings[z][i] = building.getSequence();
      remove_occupation(i.x, i.y, z,
                        _Buildings_size[z][i]); // remove old occupation
      _Buildings_size[z][i] = building.getSize();
      for (auto curpos : getInRadius(i.x, i.y, building.getSize()))
        _occupation[z].insert(curpos);
    }
  }
}

const coordinate_map<std::string> Blueprint::getLevelBuildings(int level) {
  return _Buildings[level];
}

void Blueprint::remove_occupation(int x, int y, int z, sf::Vector2i size) {
  for (auto i : getInRadius(x, y, size)) {
    _occupation[z].erase(i);
  }
}

std::vector<sf::Vector2i> Blueprint::getInRadius(int x, int y,
                                                 sf::Vector2i rad) {
  int sx = 0, sy = 0, ex = 0, ey = 0;
  std::vector<sf::Vector2i> result;
  if (rad.x == 3 && rad.y == 3) {
    sx = -1;
    sy = -1;
    ex = 1;
    ey = 1;
  }
  for (int ix = sx; ix <= ex; ix++) {
    for (int iy = sy; iy <= ey; iy++) {
      result.push_back(sf::Vector2i(x + ix, y + iy));
    }
  }
  return result;
}

const coordinate_set &Blueprint::getImpliedDesignation(int level) {
  return _implied[level];
}

void Blueprint::build_implied() {
  for (auto cfloor : _Designations) {
    for (auto desig : cfloor.second) {
      switch (desig.second) {
      case 'h':
      case 'j':
        _implied[cfloor.first - 1].reserve(0);
        _implied[cfloor.first - 1].insert(desig.first);
        break;
      case 'r':
      case 'u':
        _implied[cfloor.first + 1].reserve(0);
        _implied[cfloor.first + 1].insert(desig.first);
        break;
      case 'i':
        _implied[cfloor.first - 1].reserve(0);
        _implied[cfloor.first + 1].reserve(0);
        _implied[cfloor.first - 1].insert(desig.first);
        _implied[cfloor.first + 1].insert(desig.first);
        break;
      }
    }
  }
}
sf::Vector2i Blueprint::getLastStart() const {
  return sf::Vector2i(designation_start.x, designation_start.y);
}