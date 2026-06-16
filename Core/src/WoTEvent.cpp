#include "WoT.h"
#include "WoTEvent.h"

using namespace std;

void ThingEvent::UpdateDefaults(ThingInteractionType Eventtype)
{
    switch (Eventtype)
    {
        case eALARM:
            ValueType = VAL_BOOLEAN;
            EventSchema = "AlarmAction";
            unit = "none";
            keyname = "alarm";
        break;
        case eBOOLEAN:
            ValueType = VAL_BOOLEAN;
            EventSchema = "BooleanAction";
            unit = "none";
            keyname = "bool";
        break;
        case eBRIGHTNESS:
            ValueType = VAL_NUMBER;
            EventSchema = "BrightnessAction";
            unit = "percent";
            minimum = 0;
            maximum = 100;
            keyname = "brightness";
        break;
        case eCOLOR:
            ValueType = VAL_STRING;
            EventSchema = "ColorAction";
            unit = "none";        
            keyname = "color";
        break;
        case eCOLOR_TEMPERATURE:
            ValueType = VAL_NUMBER;
            EventSchema = "ColorTemperatureAction";
            unit = "kelvin";
            keyname = "colortemp";
        break;
        case eCURRENT:
            ValueType = VAL_NUMBER;
            EventSchema = "CurrentAction";
            unit = "ampere";
            keyname = "current";
        break;
        case eFREQUENCY:
            ValueType = VAL_NUMBER;
            EventSchema = "FrequencyAction";
            unit = "none";
            keyname = "freq";
        break;
        case eHEATING_COOLING:
            ValueType = VAL_STRING;
            EventSchema = "HeatingCoolingAction";
            unit = "none";
            keyname = "heating";
        break;
        case eIMAGE:
            ValueType = VAL_NO_STATE;
            EventSchema = "ImageAction";
            keyname = "image";
        break;
        case eINSTANTANEOUS_POWER:
            ValueType = VAL_NUMBER;
            EventSchema = "InstantaneousPowerAction";
            keyname = "instpower";
        break;
        case eLEAK:
            ValueType = VAL_BOOLEAN;
            EventSchema = "LeakAction";
            keyname = "leak";
        break;
        case eLEVEL:
            ValueType = VAL_NUMBER;
            EventSchema = "LevelAction";
            unit = "percent";
            minimum = 0;
            maximum = 100;        
            keyname = "level";
        break;
        case eLOCKED:
            ValueType = VAL_STRING;
            EventSchema = "LockedAction";
            keyname = "locked";
        break;
        case eMOTION:
            ValueType = VAL_BOOLEAN;
            EventSchema = "MotionAction";
            keyname = "motion";
        break;
        case eON_OFF:
            ValueType = VAL_BOOLEAN;
            EventSchema = "OnOffAction";
            keyname = "on";
        break;
        case eOPEN:
            ValueType = VAL_BOOLEAN;
            EventSchema = "OpenAction";
            keyname = "open";
        break;
        case ePUSHED:
            ValueType = VAL_BOOLEAN;
            EventSchema = "PushedAction";
            keyname = "pushed";
        break;
        case eTARGET_TEMPERATURE:
            ValueType = VAL_NUMBER;
            EventSchema = "TargetTemperatureAction";
            unit = "celsius";
            keyname = "targettemp";
        break;
        case eTEMPERATURE:
            ValueType = VAL_NUMBER;
            EventSchema = "TemperatureAction";
            unit = "celsius";        
            keyname = "temp";
        break;
        case eTHERMOSTAT:
            ValueType = VAL_STRING;
            EventSchema = "ThermostatModeAction";
            keyname = "thermostatMode";
        break;
        case eVIDEO:
            ValueType = VAL_NO_STATE;
            EventSchema = "VideoAction";
            keyname = "video";
        break;
        case eVOLTAGE:
            ValueType = VAL_NUMBER;
            EventSchema = "VoltageAction";
            unit = "volt";        
            keyname = "voltage";
        break;
        case eKEEP_LAST:
            ValueType = VAL_NO_STATE;
            EventSchema = "LAST_Action";
            keyname = "NULL";
        break;
    }
};
ThingInteractionValueType ThingEvent::GetValueType(std::string typeString)
{
    if (typeString == "boolean")
    {
        return (VAL_BOOLEAN);
    }
    else if (typeString == "number")
    {
        return(VAL_NUMBER);
    }
    else if (typeString == "integer")
    {
        return(VAL_INTEGER);
    }
    else if (typeString == "string")
    {
        return(VAL_STRING);
    }
    else if (typeString == "object")
    {
        return(VAL_OBJECT);
    }
    else
    {
        return(VAL_NO_STATE);
    }
}
void ThingEvent::InitializeInputs(uint8_t inputindex, std::string EventinputName, std::string inputType,uint8_t inputParamSize)
{
    cout<< "InitializeInputs EventinputName:" << EventinputName << endl;
    Inputs[inputindex].title = EventinputName;
    Inputs[inputindex].ValueType = GetValueType(inputType);
    Inputs[inputindex].size = inputParamSize;
}

void ThingEvent::Initialize(std::string EventTitle, uint8_t inputSize, uint8_t outputSize)
{
    title = EventTitle;
    // UpdateDefaults(EventType);
    //PropSchema = GetPropSchema(propType);
    // readOnly = readonly;
    subscribed = false;
    ValueType = VAL_NUMBER;
    InputSize = inputSize;
    // OutputSize = outputSize;
}

bool ThingEvent::Subscribed(void)
{
    return(subscribed);
}

bool ThingEvent::Subscribe(void)
{
    subscribed = true;
    return(subscribed);
}

bool ThingEvent::UnSubscribe(void)
{
    subscribed = false;
    return(subscribed);
}

std::string ThingEvent::GetValueTypeString(void)
{
    if (ValueType == VAL_BOOLEAN)
    {
        return ("boolean");
    }
    else if (ValueType == VAL_NUMBER)
    {
        return ("number");
    }
    else if (ValueType == VAL_STRING)
    {
        return ("string");
    }
    else
    {
        return ("number");
    }
}

std::string ThingEvent::GetKeyName(void)
{
    return (keyname);
}

std::string ThingEvent::GetValueTypeString(ThingInteractionValueType valueTypeIn)
{
    if (valueTypeIn == VAL_BOOLEAN)
    {
        return ("boolean");
    }
    else if (valueTypeIn == VAL_STRING)
    {
        return ("string");
    }
    else if (valueTypeIn == VAL_OBJECT)
    {
        return ("object");
    }
    else if (valueTypeIn == VAL_INTEGER)
    {
        return ("integer");
    }    
    else
    {
        return ("number");
    }
}


std::string ThingEvent::Serialize(cJSON *InEventPtr)
{
    // std::string RetString = NULL;
    // InActionPtr->set("@type", ActionSchema);
    // InActionPtr->set("title", title);
    // InActionPtr->set("readOnly", readOnly);
    pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_EVENT,"InputSize:", InputSize);
    if (InputSize > 0)
    {
        cJSON *InputSer = cJSON_CreateObject();
        cJSON *InputEvntSer = cJSON_CreateObject();
        cJSON_AddStringToObject(InputSer,"type","object");

        for (uint8_t indexSize=0; indexSize < InputSize; indexSize++)
        {
            cJSON *InputPropItem = cJSON_CreateObject();
            cJSON_AddStringToObject(InputPropItem,"type",GetValueTypeString(Inputs[indexSize].ValueType).c_str());
            cJSON_AddItemToObject(InputEvntSer, Inputs[indexSize].title.c_str(),InputPropItem);
        }
        cJSON_AddItemToObject(InputSer,"properties", InputEvntSer);
        cJSON_AddItemToObject(InEventPtr,"input", InputSer);
    }
    else
    {
        // InActionPtr->set("type", GetValueType());
    }
    return (title);
}

std::string ThingEvent::SerializeData(cJSON *InEventPtr)
{
    // std::string RetString = NULL;
    // InActionPtr->set("@type", ActionSchema);
    // InActionPtr->set("title", title);
    // InActionPtr->set("readOnly", readOnly);
    pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_EVENT,"InputSize:", InputSize);
    if (InputSize > 0)
    {
        cJSON *InputSer = cJSON_CreateObject();
        // cJSON *InputEvntSer = cJSON_CreateObject();
        // InputSer.set("type","object");
        for (uint8_t indexSize=0; indexSize < InputSize; indexSize++)
        {
            // Poco::JSON::Object InputPropItem;
            // InputPropItem.set("type",GetValueTypeString(Inputs[indexSize].ValueType));
            if (Inputs[indexSize].ValueType == VAL_NUMBER)
            {
                cJSON_AddNumberToObject(InputSer,Inputs[indexSize].title.c_str(),Inputs[indexSize].Value.number);
            }
            else if (Inputs[indexSize].ValueType == VAL_INTEGER)
            {
                cJSON_AddNumberToObject(InputSer,Inputs[indexSize].title.c_str(),Inputs[indexSize].Value.integer);
            }
            else if (Inputs[indexSize].ValueType == VAL_STRING)
            {
                cJSON_AddStringToObject(InputSer,Inputs[indexSize].title.c_str(),Inputs[indexSize].Value.stringin);
            }
            else if (Inputs[indexSize].ValueType == VAL_BOOLEAN)
            {
                
                cJSON_AddNumberToObject(InputSer,Inputs[indexSize].title.c_str(),Inputs[indexSize].Value.boolean);
            }
        }
        // cJSON_AddItemToObject(InputSer,"properties", InputEvntSer);
        cJSON_AddItemToObject(InEventPtr,"data", InputSer);
    }
    else
    {
        if (Inputs[0].ValueType == VAL_NUMBER)
        {
            cJSON_AddNumberToObject(InEventPtr,Inputs[0].title.c_str(),Inputs[0].Value.number);
        }
        else if (Inputs[0].ValueType == VAL_INTEGER)
        {
            cJSON_AddNumberToObject(InEventPtr,Inputs[0].title.c_str(),Inputs[0].Value.integer);
        }
        else if (Inputs[0].ValueType == VAL_STRING)
        {
            cJSON_AddStringToObject(InEventPtr,Inputs[0].title.c_str(),Inputs[0].Value.stringin);
        }
        else if (Inputs[0].ValueType == VAL_BOOLEAN)
        {
            cJSON_AddNumberToObject(InEventPtr,Inputs[0].title.c_str(),Inputs[0].Value.boolean);
        }
    }
    return (title);
}

ThingEvent::ThingEvent(void)
{
    // title = propTitle;
    // UpdateDefaults(propType);
    // PropSchema = GetPropSchema(propType);
    // readOnly = readonly;
    minimum = 0;
    maximum = 0;
    unit = "none";
    pLogger = Logger::getInstance();
	pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_EVENT,":Initializing");
}