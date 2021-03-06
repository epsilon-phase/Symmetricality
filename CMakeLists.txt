cmake_minimum_required(VERSION 3.2)
project(Symmetricality)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
set(SOURCE_FILES src/main.cpp src/Blueprint.cpp src/Blueprint.h src/Building.cpp src/Building.h)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(CMAKE_BUILD_TYPE DEBUG)

find_package(SFML 2 REQUIRED graphics system window )
add_executable(Symmetricality ${SOURCE_FILES} src/PlanRenderer.cpp src/PlanRenderer.h src/utilities.hpp src/Symmetry.cpp src/Symmetry.h src/city.cc src/city.h src/Hud.cpp src/Hud.h src/tinyfiledialogs.h src/tinyfiledialogs.c src/RadialMenu.h src/RadialMenu.cpp src/jsoncpp.cpp src/json/json.h src/json/json-forwards.h src/PlanFacet.cpp src/PlanFacet.h src/BuildingFacet.cpp src/BuildingFacet.h src/DesignationFacet.cpp src/DesignationFacet.h src/SymmetryFacet.cpp src/SymmetryFacet.h src/Configurable.cpp src/Configurable.h src/Floornable.cpp src/Floornable.h)
target_link_libraries(Symmetricality ${SFML_LIBRARIES})
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
endif()
configure_file(LinLibertine_DRah.ttf LinLibertine_DRah.ttf COPYONLY)
configure_file(Symmetricality.json Symmetricality.json COPYONLY)
configure_file(assets/sprite_sheet.png buildings.png COPYONLY)
configure_file(assets/designations.png designations.png COPYONLY)
configure_file(assets/menu_continuance.png menu_continuance.png COPYONLY)
configure_file(assets/menu_utility_icons.png menu_utility_icons.png COPYONLY)
