/**
* @file WoT.cpp
* @brief root web of thing source file exports WoT class and its
* definitions and basic functions.
*
* @author Boobalakrishnan
*
* @date dd/mmm/yyyy
*/
#include <iostream>
#include "Logger.h"
#include "WoTThing.h"
#include "WoTTd.h"
#include "WoTServient.h"
#include "WoT.h"

using namespace Logging;

#define WOT_THINGS_SUPPORT_MAX 20

ExposedThing *MyThings[WOT_THINGS_SUPPORT_MAX];
int MyThingCount=0;

int WoT::AddThing(ExposedThing *NewThing)
{
	MyThings[MyThingCount] = NewThing;
	// MyThings[MyThingCount]->SetThingNo(MyThingCount);
	cout << "ExposedThing Add=" << NewThing->id.c_str() << endl;
	// try
	// {
	if (servient != NULL)
	{
		cout << "servient is not NULL:" <<servient<<endl;
		servient->AddExposedThing(MyThings[MyThingCount]);
	}
	else
	{
		cout << "servient is NULL" <<endl;
	}
	// }
	// catch (std::exception& e)
	// {
	// 	std::cerr << "Exception caught : " << e.what() << std::endl;
	// }
	MyThingCount++;
	return (0);
}

ExposedThing* WoT::GetThing(std::string ThingId)
{
	int ThingIndex=0;
	cout << "MyThings=" << ThingId << endl;
	while (ThingIndex<MyThingCount)
	{
		cout << "MyThings=" << MyThings[ThingIndex]->title << endl;

		if (MyThings[ThingIndex]->title == ThingId)
		{
			break;
		}
		else
		{
			ThingIndex++;
		}
	}


	return (MyThings[ThingIndex]);
}

// int SearchProperty(int ThingIndex, std::string PropertyId)
// {
// 	int PropertyIndex=0;
// 	// while (PropertyIndex<MyThings[ThingIndex]->propertyIndex)
// 	// {
// 	// 	if (MyThings[ThingIndex]->Thing_Property[PropertyIndex]->title == PropertyId)
// 	// 	{
// 	// 		break;
// 	// 	}
// 	// 	else
// 	// 	{
// 	// 		PropertyIndex++;
// 	// 	}
// 	// }
// 	return PropertyIndex;
// }


WoT::WoT(void)
{
	// Constructor for WoT
    pLogger = Logger::getInstance();
	pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOT,":Initializing");
	MyThingCount=0;
}

void WoT::AddServient(WoTServient *ServientIn)
{
	pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOT,":AddServient Call");
    servient = ServientIn;
}
ConsumedThing *Consumed_Thing_Rtr;
ConsumedThing *WoT::consume(std::string TD_String)
{
	pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOT,":consume Call");
    ConsumeThingDesc = new ThingDescription(TD_String);

	Consumed_Thing_Rtr = ConsumeThingDesc->build_consumedThing();
	if (Consumed_Thing_Rtr != NULL)
	{
		cout << "ConsumeThingDesc built successfully" << std::endl;
	}
	else
	{
		cout << "ConsumeThingDesc build failed" << std::endl;
	}
	servient->AddConsumedThing(Consumed_Thing_Rtr);
	pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOT,":consume Call, Consumed_Thing_Rtr->id:",Consumed_Thing_Rtr->id.c_str());
	return(Consumed_Thing_Rtr);
}

ExposedThing *WoT::produce(std::string Td_String)
{
	pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOT,":produce Call");
    ExposeThingDesc = new ThingDescription(Td_String);
	
	//ThingDesc->bu
	return (ExposeThingDesc->build_exposedThing());
}

