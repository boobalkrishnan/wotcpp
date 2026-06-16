#ifndef WOTTHING_H
#define WOTTHING_H
#include <ctime>
#include "WoTAction.h"
#include "WoTEvent.h"
#include "WoTProperty.h"

#define WEBTHING_PROPERTY_MAX 10
#define WEBTHING_ACTION_MAX 10
#define WEBTHING_EVENT_MAX 10
using namespace std;
using namespace Logging;

typedef void (*thing_callback_ptr)(bool);
typedef void (*Property_readcallback_ptr)(uint8_t Protocol, std::string ThingID, std::string PropID, std::string propertyHref, 
                                                    ThingInteractionValueType PropValType, ThingInteractionValue *PropertyValue);
typedef void (*Event_Observcallback_ptr)(std::string EventID, std::string EventTitle, ThingInteractionValueType PropValType, ThingInteractionValue *PropertyValue);
typedef void (*Event_SubscribeCallback_ptr)(uint8_t Protocol, std::string ThingID, std::string EventID, std::string eventHref);

enum thingType
{
    Exposed_Thing = 0,
    Consumed_Thing
};

#define LOGGER_COMP_THING "Thing"

class SecurityScheme
{
public:
    std::string scheme;
    std::string schemeIn;
};

class Thing
{
private:
    std::string allFields[22] = {"context",
                                 "type",
                                 "id",
                                 "title",
                                 "titles",
                                 "description",
                                 "descriptions",
                                 "version",
                                 "created",
                                 "modified",
                                 "support",
                                 "base",
                                 "properties",
                                 "actions",
                                 "events",
                                 "links",
                                 "forms",
                                 "security",
                                 "securityDefinitions",
                                 "profile",
                                 "schemaDefinitions",
                                 "uriVariables"};
    std::string requiredFields[4] = {"context", "title", "security", "securityDefinitions"};
    std::string defaultFields[1] = {};
    unsigned int ThingNo;

public:
    Logger *pLogger = NULL; // Create the object pointer for Logger Class
    thingType myType;
    std::string context;
    std::string type;
    std::string id;
    std::string title;
    std::string titles;
    std::string description;
    std::string descriptions;
    // WoTVersionInfo version;
    std::time_t created;
    std::time_t modified;
    std::string support;
    std::string base;
    // WoTLink links;
    // WoTForms forms;
    std::string security;
    SecurityScheme securityDefinitions;
    std::string profile;
    std::string schemaDefinitions;
    std::string uriVariables;
    ThingProperty *Thing_Property[WEBTHING_PROPERTY_MAX];
    ThingAction *Thing_Action[WEBTHING_ACTION_MAX];
    ThingEvent *Thing_Event[WEBTHING_EVENT_MAX];
    // unsigned char AddAffordance(WoTInteractionAffordance *AffordanceInput);
    unsigned int propertyIndex;
    unsigned int actionIndex;
    unsigned int eventIndex;
    // Thing(unsigned int thingNo, std::string id, std::string title, const char
    // **type);
    Thing(void);
    void Initialize(unsigned int thingNo, std::string id, std::string title,
                    std::string type);
    // Thing(std::string _context, std::string _id, std::string _title,
    // std::string _type);
    void SetThingNo(unsigned int thingNo);

    int GetProperIndex(std::string property);
    int GetActionIndex(std::string actionin);
    int GetEventIndex(std::string eventin);
    unsigned char addProperty(ThingProperty *property);
    unsigned char addAction(ThingAction *action);
    unsigned char addEvent(ThingEvent *event);
};

class ExposedThing : public Thing
{
public:
    // ExposedThing(std::string context, std::string id, std::string title,
    // std::string type);
    ExposedThing(void);
    void Initialize(unsigned int thingNo, std::string id, std::string title,
                    std::string type);
    unsigned char
    setPropertyReadHandler(int prop_index,
                           prop_readcallback_ptr readhandler_callback);
    unsigned char
    setPropertyWriteHandler(int prop_index,
                            prop_writecallback_ptr writehandler_callback);
    unsigned char
    setActionInvokeHandler(int action_index,
                           action_invokecallback_ptr actionhandler_callback);
    void Expose(void);
    void EmitEvent(std::string EventID, std::string out);
};

class ConsumedThing : public Thing
{
public:
    uint8_t ClientProtocol;
    Property_readcallback_ptr property_readcallback;
    Event_Observcallback_ptr event_observcallback;
    Event_SubscribeCallback_ptr event_Subscribecallback;
    void SetPropertyReadCallback(Property_readcallback_ptr readhandler_callback, Event_SubscribeCallback_ptr EventSubscribe_callback);
    // ExposedThing(std::string context, std::string id, std::string title,
    // std::string type);
    void SetClientProtocol(uint8_t ProtocolID);
    ConsumedThing(void);
    void Initialize(unsigned int thingNo, std::string id, std::string title,
                    std::string type);
    void Consume(void);
    void EmitEvent(std::string EventID, std::string out);
    void SubscribeEvent(std::string EventID);
    ThingInteractionValueType ReadProperty(std::string property, ThingInteractionValue *PropertyValue);
    void WriteProperty(std::string property, ThingInteractionValue *PropertyValue);
    void SetEventObserveCallback(Event_Observcallback_ptr EventObserve_callback);
};

#endif