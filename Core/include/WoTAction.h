#ifndef WOTACTION
#define WOTACTION

#include <iostream>
#include <string>
#include <Poco/JSON/Object.h>
#include "Logger.h"
#include "WoTInteractions.h"

using namespace Logging;

#define LOGGER_COMP_ACTION "WoTAction"

typedef void (*action_callback_ptr)(void);
typedef uint8_t (*action_invokecallback_ptr)(std::string ThingId, std::string ActionId, Poco::JSON::Object::Ptr pObject);
typedef void (*action_writecallback_ptr)(std::string ThingId, std::string ActionId, ThingInteractionValue *ActionValue);


struct ActionParameters_t
{
    uint8_t size;
    std::string title;
    ThingInteractionValueType ValueType;
    //ThingInteractionType Type;
    ThingInteractionValue Value;
};

#define WOT_ACTION_MAX_INPUTS    5
#define WOT_ACTION_MAX_OUTPUTS   5

class ThingAction
{
    private:
        action_callback_ptr prop_OnChange;

        int Action_id;
     
        std::string ActionSchema;
        ThingInteractionType type;             /* @type */

        ThingInteractionValue value;
        int minimum;                        /* minimum */
        int maximum;                        /* maximum */
        bool readOnly;                      /* readOnly */
        std::string unit;                   /* unit */
        void UpdateDefaults(ThingInteractionType Actiontype);
    public:
        WoTForms form[5];
        std::string title;
        Logger* pLogger = NULL; // Create the object pointer for Logger Class
        std::string GetValueTypeString(void);
        std::string GetValueTypeString(ThingInteractionValueType valueTypeIn);
        std::string GetKeyName(void);     
        ThingInteractionValueType ValueType;   /* type */    
        std::string keyname;    
        ActionParameters_t Inputs[WOT_ACTION_MAX_INPUTS];
        uint8_t InputSize;
        ActionParameters_t Outputs[WOT_ACTION_MAX_OUTPUTS];
        uint8_t OutputSize;
        ThingAction(void);

        void Initialize(std::string ActionTitle, uint8_t inputSize, uint8_t outputSize);
        ThingInteractionValueType GetValueType(std::string typeString);
        void InitializeOutputs(uint8_t outputindex, std::string ActionoutputName, std::string outputType,uint8_t outputParamSize);
        void InitializeInputs(uint8_t inputindex, std::string ActioninputName, std::string inputType,uint8_t inputParamSize);  
        std::string Serialize(Poco::JSON::Object *InActionPtr);
        action_invokecallback_ptr action_InvokeHandler;
        action_writecallback_ptr action_WriteHandler;     
};

#endif /* WOTACTION */
