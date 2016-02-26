#ifndef CONFIGURABLE_H
#define CONFIGURABLE_H
#include "json/json.h"
class Configurable
{
public:
	Configurable();
	virtual ~Configurable();
	virtual void LoadConfiguration(Json::Value&)=0;
};
#endif