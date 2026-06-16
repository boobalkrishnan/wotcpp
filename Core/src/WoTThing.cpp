#include <iostream>
#include <string>

#include "WoTAction.h"
#include "WoTAdapter.h"
#include "WoTEvent.h"
#include "WoTProperty.h"
#include "WoTServient.h"
#include "WoTTd.h"
#include "WoTThing.h"

using namespace std;
// using Poco::UUID;
// using Poco::UUIDGenerator;

/**
 * This constructor initializes the Thing class object
 * @author Boobalakrishnan
 * @param _context WoT context of the thing
 * @param _id WoT ID of the thing
 * @param _title WoT title of the thing
 * @param _type WoT type of the thing
 * @return None
 */
// Thing::Thing(std::string _context, std::string _id, std::string _title,
// std::string _type)
// {
//     ThingNo = 0;
//     context = _context;
// 	id = _id;
// 	title = _title;
// 	type = _type;
// 	// property = {};
//     propertyIndex = 0;
//     cout << "propertyIndex:" << propertyIndex<< endl;
// };

Thing::Thing(void) {
    // context = tdinput["@context"];

    // titles = tdinput["titles"];
    ////description = tdinput["description"];
    // descriptions = tdinput["descriptions"];
    // WoTVersionInfo version;
    // support = tdinput["support"];
    // base = tdinput["base"];
};

void Thing::Initialize(unsigned int thingNoIn, std::string idIn,
                       std::string titleIn, std::string typeIn)
{
    ThingNo = thingNoIn;
    type = typeIn;
    id = "urn:uuid:" + titleIn; // Poco::UUIDGenerator().createRandom().toString();
    title = titleIn;
    security = "basic_sc";
    context = "https://www.w3.org/2022/wot/td/v1.1";
    securityDefinitions.scheme = "basic";
    securityDefinitions.schemeIn = "header";
    cout << "Thing Initialize ID:" << id.c_str() << endl;
};
void Thing::SetThingNo(unsigned int thingNo)
{
    ThingNo = ThingNo;
};

int Thing::GetProperIndex(std::string propertyin)
{
    int ret_index = 0xFFFF;
    cout << "Thing::PropertyIndex:" << propertyIndex << endl;
    for (int PropIndex = 0; PropIndex < propertyIndex; PropIndex++) //
    {
        if (propertyin == Thing_Property[PropIndex]->title)
        {
            cout << "Thing_Property[PropIndex]->title:" << Thing_Property[PropIndex]->title << endl;
            ret_index = PropIndex;
            break;
        }
    }
    return (ret_index);
};

int Thing::GetActionIndex(std::string actionin)
{
    int ret_index = 0xFFFF;
    for (int ActnIndex = 0; ActnIndex < actionIndex; ActnIndex++) //
    {
        if (actionin == Thing_Action[ActnIndex]->title)
        {
            ret_index = ActnIndex;
            break;
        }
    }
    return (ret_index);
};

int Thing::GetEventIndex(std::string eventin)
{
    int ret_index = 0xFFFF;
    cout << "eventin:" << propertyIndex << endl;
    for (int EvntIndex = 0; EvntIndex < eventIndex; EvntIndex++) //
    {
        cout << "Thing_Event[EvntIndex]->title:" << Thing_Event[EvntIndex]->title << endl;
        if (eventin == Thing_Event[EvntIndex]->title)
        {
            ret_index = EvntIndex;
            break;
        }
    }
    return (ret_index);
};

unsigned char Thing::addProperty(ThingProperty *property)
{
    unsigned char ret_index = 0xFF;

    if (propertyIndex < WEBTHING_PROPERTY_MAX)
    {
        Thing_Property[propertyIndex] = property;
        ret_index = propertyIndex++;
    }
    cout << "propertyIndex:" << propertyIndex << endl;
    return ret_index;
};

unsigned char
Thing::addEvent(ThingEvent *event)
{
    unsigned char ret_index = 0xFF;

    if (eventIndex < WEBTHING_EVENT_MAX)
    {
        Thing_Event[eventIndex] = event;
        ret_index = eventIndex++;
    }
    cout << "eventIndex:" << eventIndex << endl;
    return ret_index;
};

unsigned char
Thing::addAction(ThingAction *action)
{
    unsigned char ret_index = 0xFF;

    if (actionIndex < WEBTHING_ACTION_MAX)
    {
        Thing_Action[actionIndex] = action;
        ret_index = actionIndex++;
    }
    cout << "actionIndex:" << actionIndex << endl;
    return ret_index;
};

ExposedThing::ExposedThing(void)
{
    propertyIndex = 0;
    actionIndex = 0;
    eventIndex = 0;
    myType = Consumed_Thing;
};

void ExposedThing::Initialize(unsigned int thingNo, std::string id,
                              std::string title, std::string type)
{
    Thing::Initialize(thingNo, id, title, type);
    cout << "ExposedThing Initialize ID:" << id.c_str() << endl;
    myType = Exposed_Thing;
};

// unsigned char ExposedThing::addAction(ThingAction* _action)
// {
//     unsigned char ret_index=0xFF;

//     if (actionIndex < WEBTHING_ACTION_MAX)
//     {
//         action[actionIndex] = _action;
//         ret_index = actionIndex++;
//     }
//     cout << "actionIndex:" << actionIndex<< endl;
//     return ret_index;
// };

unsigned char
ExposedThing::setActionInvokeHandler(
    int action_index, action_invokecallback_ptr actionhandler_callback)
{
    unsigned char ret_index = 0xFF;

    if (action_index < WEBTHING_PROPERTY_MAX)
    {
        (*Thing_Action[action_index]).action_InvokeHandler = actionhandler_callback;
        ret_index = action_index; // SUCCESS
    }
    if ((*Thing_Action[action_index]).action_InvokeHandler != NULL)
    {
        cout << "setPropertyReadHandler action_index:" << action_index << endl;
    }
    return ret_index;
};

unsigned char
ExposedThing::setPropertyReadHandler(
    int prop_index, prop_readcallback_ptr readhandler_callback)
{
    unsigned char ret_index = 0xFF;

    if (prop_index < WEBTHING_PROPERTY_MAX)
    {
        (*Thing_Property[prop_index]).prop_ReadHandler = readhandler_callback;
        ret_index = prop_index; // SUCCESS
    }
    if ((*Thing_Property[prop_index]).prop_ReadHandler != NULL)
    {
        cout << "setPropertyReadHandler prop_index:" << prop_index << endl;
    }
    return ret_index;
};

unsigned char
ExposedThing::setPropertyWriteHandler(
    int prop_index, prop_writecallback_ptr writehandler_callback)
{
    unsigned char ret_index = 0xFF;

    if (prop_index < WEBTHING_PROPERTY_MAX)
    {
        (*Thing_Property[prop_index]).prop_WriteHandler = writehandler_callback;
        ret_index = prop_index; // SUCCESS
    }
    if ((*Thing_Property[prop_index]).prop_WriteHandler != NULL)
    {
        cout << "setPropertyWriteHandler prop_index:" << prop_index << endl;
    }
    return ret_index;
};

ConsumedThing::ConsumedThing(void)
{
    propertyIndex = 0;
    actionIndex = 0;
    eventIndex = 0;
    myType = Consumed_Thing;
};

void ConsumedThing::SetClientProtocol(uint8_t ProtocolID)
{
    ClientProtocol = ProtocolID;
}
void ConsumedThing::Initialize(unsigned int thingNo, std::string id,
                               std::string title, std::string type)
{
    Thing::Initialize(thingNo, id, title, type);
    cout << "ConsumedThing Initialize ID:" << id.c_str() << endl;
};

ThingInteractionValueType ConsumedThing::ReadProperty(std::string property,
                            ThingInteractionValue *PropertyValue)
{
    int prop_index = 0xFFFF;
    prop_index = GetProperIndex(property);
    // ThingInteractionValueType PropValType = Thing_Property[prop_index]->ValueType;
    cout << "ReadProperty Index:" << prop_index << endl;
    if (prop_index < WEBTHING_PROPERTY_MAX)
    {
        if (property_readcallback != NULL)
        {
            cout << Thing_Property[prop_index]->form[0].href << endl;
            cout << Thing_Property[prop_index]->values[0].ValueType << endl;
            property_readcallback(ClientProtocol, title, property, Thing_Property[prop_index]->form[0].href,
                                  Thing_Property[prop_index]->values[0].ValueType,
                                  PropertyValue);
        }
        else
        {
            cout << "property_readcallback:NULL" << endl;
        }
    } 
    return (Thing_Property[prop_index]->values[0].ValueType);
};

void ConsumedThing::SubscribeEvent(std::string EventID)
{
    int event_index = 0xFFFF;
    event_index = GetEventIndex(EventID);
    if (event_index < WEBTHING_EVENT_MAX)
    {
        if (event_observcallback != NULL)
        {
            cout << "Subscribing to Event:" << EventID << endl;
            event_Subscribecallback(ClientProtocol, title, EventID, Thing_Event[event_index]->form[0].href);
        }
        else
        {
            cout << "event_observcallback:NULL" << endl;
        }
    }
};

void ConsumedThing::SetPropertyReadCallback(
    Property_readcallback_ptr readhandler_callback,Event_SubscribeCallback_ptr EventSubscribe_callback)
{
    property_readcallback = readhandler_callback;
    event_Subscribecallback = EventSubscribe_callback;
    cout << "ConsumedThing SetPropertyReadCallback" << endl;
};

void ConsumedThing::WriteProperty(std::string property,
                                  ThingInteractionValue *PropertyValue)
{
    unsigned char prop_index = 0xFF;
    prop_index = GetProperIndex(property);
    if (prop_index < WEBTHING_PROPERTY_MAX)
    {
        cout << "ReadProperty Index:" << prop_index << endl;
    }
};

void ConsumedThing::SetEventObserveCallback(Event_Observcallback_ptr EventObserve_callback)
{
    event_observcallback = EventObserve_callback;
    cout << "ConsumedThing SetPropertyReadCallback" << endl;    
}
