#ifndef WOT_H
#define WOT_H

#include <iostream>
#include "Logger.h"
#include "WoTTd.h"
#include "WoTServient.h"

// int SearchProperty(int ThingIndex, std::string PropertyId);
// void StartWoT(void);



#define LOGGER_COMP_WOT "WOT"

class WoT{
    private:

    public:
        WoTServient *servient;
        ThingDescription *ConsumeThingDesc;
        ThingDescription *ExposeThingDesc;
        Logger* pLogger = NULL; // Create the object pointer for Logger Class
        WoT(void);
        int AddThing(ExposedThing *NewThing);
        ExposedThing *GetThing(std::string ThingId);
        ExposedThing *produce(std::string Td_String);
        void AddServient(WoTServient *ServientIn);
        void subscribe(void);
        void discover(void);
        ConsumedThing *consume(std::string ThingTd);
};

#endif