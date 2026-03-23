#ifndef WOTEVENT
#define WOTEVENT

#include <iostream>
#include <string>
#include <Poco/JSON/Object.h>

#include "Logger.h"
#include "WoTInteractions.h"


using namespace Logging;

// enum ThingEventValueType {
//   VAL_NO_STATE,
//   VAL_BOOLEAN,
//   VAL_NUMBER,
//   VAL_STRING
// };

// union ThingEventValue {
//   bool boolean;
//   double number;
//   char* string;
// };

// enum EventUnits{
// 	eNONE,
// 	eCELCUIS,
// 	eFARHENITE,
// 	ePERCENT,
// 	eMILLISECONDS
// };

// enum ThingEventType{
// 	eALARM,
// 	eBOOLEAN,
// 	eBRIGHTNESS,
// 	eCOLOR,
// 	eCOLOR_TEMPERATURE,
// 	eCURRENT,
// 	eFREQUENCY,
// 	eHEATING_COOLING,
// 	eIMAGE,
// 	eINSTANTANEOUS_POWER,
// 	eLEAK,
// 	eLEVEL,
// 	eLOCKED,
// 	eMOTION,
// 	eON_OFF,
// 	eOPEN,
// 	ePUSHED,
// 	eTARGET_TEMPERATURE,
// 	eTEMPERATURE,
// 	eTHERMOSTAT,
// 	eVIDEO,
// 	eVOLTAGE,
// 	eKEEP_LAST
// };

#define LOGGER_COMP_EVENT "WoTEvent"

typedef void (*event_callback_ptr)(void);
typedef ThingInteractionValueType (*event_readcallback_ptr)(std::string ThingId, std::string EventId, ThingInteractionValue *EventValue);
typedef void (*event_writecallback_ptr)(std::string ThingId, std::string EventId, ThingInteractionValue *EventValue);

struct EventInputs_t
{
    std::string title;
    uint8_t size;
    ThingInteractionValueType ValueType;
    ThingInteractionType Type;
    ThingInteractionValue Value;
};

#define WOT_EVENT_MAX_INPUTS    10
#define WOT_EVENT_MAX_PAYLOAD   10

class ThingEvent
{
    private:

        event_callback_ptr Event_OnChange;

        int Event_id;

        // ThingInteractionType type;             /* @type */

        // ThingInteractionValue value;
        int minimum;                        /* minimum */
        int maximum;                        /* maximum */
        bool readOnly;                      /* readOnly */
        std::string unit;                   /* unit */
        void UpdateDefaults(ThingInteractionType Eventtype);

    public:
        std::string EventSchema; // @type
        std::string title;  // title 
        uint8_t InputSize;
        EventInputs_t Inputs[WOT_EVENT_MAX_INPUTS];
        EventInputs_t Payload[WOT_EVENT_MAX_PAYLOAD];
        Logger* pLogger = NULL; // Create the object pointer for Logger Class
        std::string GetValueTypeString(void);
        std::string GetValueTypeString(ThingInteractionValueType valueTypeIn);
        std::string GetKeyName(void);  
        ThingInteractionValueType ValueType;   /* type */       
        // ThingInteractionValueType ValueType;   /* type */
        WoTForms form[5];
        std::string keyname;
        bool subscribed;
        //ThingProperty(std::string propTitle, ThingPropertyType propType, bool readonly);
        ThingEvent(void); 
        void Initialize(std::string EventTitle, uint8_t inputSize, uint8_t outputSize);
        ThingInteractionValueType GetValueType(std::string typeString);
        void InitializeOutputs(uint8_t outputindex, std::string EventoutputName, std::string outputType,uint8_t outputParamSize);
        void InitializeInputs(uint8_t inputindex, std::string EventinputName, std::string inputType,uint8_t inputParamSize);  
        bool Subscribed(void);
        bool Subscribe(void);
        std::string Serialize(Poco::JSON::Object *InEventPtr);
        std::string SerializeData(Poco::JSON::Object *InEventPtr);
        event_readcallback_ptr Event_ReadHandler;
        event_writecallback_ptr Event_WriteHandler;
};

#endif /* WOTEVENT */
