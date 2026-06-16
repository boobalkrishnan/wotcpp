#ifndef WOTADAPTER_H
#define WOTADAPTER_H

#include "Logger.h"
#include "cJSON.h"
// #include "WoT.h"
// #include "WoTProperty.h"
// #include "WoTAction.h"
// #include "WoTEvent.h"
#include "BindClient.h"
#include "BindServer.h"
#include "WoTThing.h"

using namespace std;
using namespace Logging;
#define WOT_SERVIENT_MAX_SERVERS 5
#define WOT_SERVIENT_MAX_CLIENTS 5
#define LOGGER_COMP_WOTADAPTER "WoTAdapter"

enum InteractionType
{
    ProperyInteraction = 0,
    ActionInteraction,
    EventInteraction
};

class WoTAdapter
{
public:
    uint8_t ServerCount;
    uint8_t ClientCount;
    WoTAdapter(void);
    void Process(void);
    void Start (void);
    void RegisterServer(string ThingName);
    void AddServer (BindServer *ServerIn);
    void AddClient (BindClient *ClientIn);
    static void ReadProperty(uint8_t Protocol, std::string ThingID, std::string PropID, std::string propertyIn,
                             ThingInteractionValueType PropValType,
                             ThingInteractionValue *PropertyValue);
    static void SubscribeEvent(uint8_t Protocol, std::string ThingID, std::string EventID, std::string eventHref);
    static void EventHandler(std::string ThingID, std::string EventID, cJSON * eventData);
    void EmitEvent(std::string EventID, cJSON *out);
    void WoTAdapterInit(ExposedThing **Exposed_Thing_In, ConsumedThing **Consumed_Thing_In);
    Logger *pLogger = NULL; // Create the object pointer for Logger Class
    static InteractionType GetInteractionType(std::string Interaction_Type);
    int ParseThingsURI(std::string RxURI, std::string *ArrayOfURI);
    static ExposedThing *GetExposedThing(std::string ThingID);
    static ConsumedThing* GetConsumedThing (std::string ThingID);
    static void EncodePayload(ThingInteractionValueType inputType,
                  ThingInteractionValue *InValue,
                  cJSON *JsonData); // ostream& DataOut);
    static void DecodePayload(std::string inputType, std::string inputstring,
                              ThingInteractionValue *InValue);
    static void DecodePayload(ThingInteractionValueType inputType,
                              std::string inputstring,
                              ThingInteractionValue *InValue);
        static void CreateForms(std::string HostIn,std::string InteractionURI,cJSON *form);
    static void WoTAdapterThingHndl(std::string HostIn, std::string ThingId,
                                    std::string DeviceId,
                                    cJSON *ThingDesc);
    static void WoTAdapterAffordanceHndl(std::string HostIn,
                                         std::string ThingId,
                                         std::string Root_Name,
                                         std::string Interaction_Type,
                                         cJSON *ThingDesc);
    bool WoTAdapterGet(std::string RxURI, cJSON *ThingDesc);
    bool WoTAdapterPut(std::string RxURI, cJSON *JsonMeta);
    static void
    WoTAdapterGetProperty(std::string ThingId, std::string DeviceId,
                          std::string PropId,
                          cJSON *JsonData); // ostream& DataOut);
    static void WoTAdapterSetProperty(std::string ThingId, std::string DeviceId,
                                      std::string PropId, istream &DataIn,
                                      uint32_t Datalen);
    static void WoTAdapterInvokeAction(std::string ThingId,
                                       std::string Root_Name, std::string CmdId,
                                       istream &DataIn, uint32_t Datalen);
    static void WoTAdapterGetInteraction(
        std::string ThingId, std::string DeviceId, std::string Interaction_Type,
        std::string Interaction_Name,
        cJSON *JsonData); // ostream& DataOut);
    static void WoTAdapterSetInteraction(std::string ThingId,
                                         std::string DeviceId,
                                         std::string Interaction_Type,
                                         std::string Interaction_Name,
                                         istream &DataIn, uint32_t Datalen);
    static void
    WoTAdapterGetSubscribed(std::string ThingId, std::string Root_Name,
                            std::string Interaction_Type,
                            std::string Interaction_Name,
                            cJSON *JsonMeta); // ostream& DataOut);
};

#endif