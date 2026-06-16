#include "WoTProperty.h"
#include "WoT.h"
#include "WoTTd.h"

using namespace std;

void ThingProperty::UpdateDefaults(ThingInteractionType proptype) {
    // switch (proptype)
    // {
    //     case eALARM:
    //         ValueType = VAL_BOOLEAN;
    //         PropSchema = "AlarmProperty";
    //         unit = "none";
    //         keyname = "alarm";
    //     break;
    //     case eBOOLEAN:
    //         ValueType = VAL_BOOLEAN;
    //         PropSchema = "BooleanProperty";
    //         unit = "none";
    //         keyname = "bool";
    //     break;
    //     case eBRIGHTNESS:
    //         ValueType = VAL_NUMBER;
    //         PropSchema = "BrightnessProperty";
    //         unit = "percent";
    //         minimum = 0;
    //         maximum = 100;
    //         keyname = "brightness";
    //     break;
    //     case eCOLOR:
    //         ValueType = VAL_STRING;
    //         PropSchema = "ColorProperty";
    //         unit = "none";
    //         keyname = "color";
    //     break;
    //     case eCOLOR_TEMPERATURE:
    //         ValueType = VAL_NUMBER;
    //         PropSchema = "ColorTemperatureProperty";
    //         unit = "kelvin";
    //         keyname = "colortemp";
    //     break;
    //     case eCURRENT:
    //         ValueType = VAL_NUMBER;
    //         PropSchema = "CurrentProperty";
    //         unit = "ampere";
    //         keyname = "current";
    //     break;
    //     case eFREQUENCY:
    //         ValueType = VAL_NUMBER;
    //         PropSchema = "FrequencyProperty";
    //         unit = "none";
    //         keyname = "freq";
    //     break;
    //     case eHEATING_COOLING:
    //         ValueType = VAL_STRING;
    //         PropSchema = "HeatingCoolingProperty";
    //         unit = "none";
    //         keyname = "heating";
    //     break;
    //     case eIMAGE:
    //         ValueType = VAL_NO_STATE;
    //         PropSchema = "ImageProperty";
    //         keyname = "image";
    //     break;
    //     case eINSTANTANEOUS_POWER:
    //         ValueType = VAL_NUMBER;
    //         PropSchema = "InstantaneousPowerProperty";
    //         keyname = "instpower";
    //     break;
    //     case eLEAK:
    //         ValueType = VAL_BOOLEAN;
    //         PropSchema = "LeakProperty";
    //         keyname = "leak";
    //     break;
    //     case eLEVEL:
    //         ValueType = VAL_NUMBER;
    //         PropSchema = "LevelProperty";
    //         unit = "percent";
    //         minimum = 0;
    //         maximum = 100;
    //         keyname = "level";
    //     break;
    //     case eLOCKED:
    //         ValueType = VAL_STRING;
    //         PropSchema = "LockedProperty";
    //         keyname = "locked";
    //     break;
    //     case eMOTION:
    //         ValueType = VAL_BOOLEAN;
    //         PropSchema = "MotionProperty";
    //         keyname = "motion";
    //     break;
    //     case eON_OFF:
    //         ValueType = VAL_BOOLEAN;
    //         PropSchema = "OnOffProperty";
    //         keyname = "on";
    //     break;
    //     case eOPEN:
    //         ValueType = VAL_BOOLEAN;
    //         PropSchema = "OpenProperty";
    //         keyname = "open";
    //     break;
    //     case ePUSHED:
    //         ValueType = VAL_BOOLEAN;
    //         PropSchema = "PushedProperty";
    //         keyname = "pushed";
    //     break;
    //     case eTARGET_TEMPERATURE:
    //         ValueType = VAL_NUMBER;
    //         PropSchema = "TargetTemperatureProperty";
    //         unit = "celsius";
    //         keyname = "targettemp";
    //     break;
    //     case eTEMPERATURE:
    //         ValueType = VAL_NUMBER;
    //         PropSchema = "TemperatureProperty";
    //         unit = "celsius";
    //         keyname = "temp";
    //     break;
    //     case eTHERMOSTAT:
    //         ValueType = VAL_STRING;
    //         PropSchema = "ThermostatModeProperty";
    //         keyname = "thermostatMode";
    //     break;
    //     case eVIDEO:
    //         ValueType = VAL_NO_STATE;
    //         PropSchema = "VideoProperty";
    //         keyname = "video";
    //     break;
    //     case eVOLTAGE:
    //         ValueType = VAL_NUMBER;
    //         PropSchema = "VoltageProperty";
    //         unit = "volt";
    //         keyname = "voltage";
    //     break;
    //     case eKEEP_LAST:
    //         ValueType = VAL_NO_STATE;
    //         PropSchema = "LAST_PROPERTY";
    //         keyname = "NULL";
    //     break;
    // }
};

void ThingProperty::Initialize(std::string propTitle, std::string propValType, bool readonly)
{
    PropParameterSize = 1;
    title = propTitle;
    if (propTitle == "temperature")
    {
        atType = "saref:Temperature";
    }
    else if (propTitle == "humidity")
    {
        atType = "saref:Humidity";
    }
    else if (propTitle == "door")
    {
        atType = "saref:Door";
    }
    else if (propTitle == "motion")
    {
        atType = "saref:Motion";
    }
    else if (propTitle == "ColorTemperature")
    {
        atType = "saref:ColorTemperature";
    }
    else if (propTitle == "Illuminance")
    {
        atType = "saref:Light";
    }
    pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_PROPERTY,"Initialize called for non object type:",title.c_str());
    if (propValType == "boolean")
    {
        values[0].ValueType = VAL_BOOLEAN;
        values[0].size = 1;
        values[0].name = propTitle;
    }
    else if (propValType == "number")
    {
        values[0].ValueType = VAL_NUMBER;
        values[0].size = 8;
        values[0].name = propTitle;
    }
    else if (propValType == "integer")
    {
        values[0].ValueType = VAL_INTEGER;
        values[0].size = 8;
        values[0].name = propTitle;
    }
    else if (propValType == "string")
    {
        values[0].ValueType = VAL_STRING;
        values[0].size = 32;
        values[0].name = propTitle;
    }
    else if (propValType == "object")
    {
        values[0].ValueType = VAL_OBJECT;
        values[0].size = 1;
        values[0].name = propTitle;
    }
    else
    {
        values[0].ValueType = VAL_NO_STATE;
        values[0].size = 1;
        values[0].name = propTitle;
    }
}

uint8_t
ThingProperty::FindParamIndex(std::string ParameterName)
{
    uint8_t retVal = 0xFF;
    for (uint8_t paramcnt = 0; paramcnt < PropParameterSize; paramcnt++)
    {
        if (values[PropParameterSize].name == ParameterName)
        {
            retVal = paramcnt;
            break;
        }
    }
    return (retVal);
}

uint8_t
ThingProperty::Initialize(std::string ParameterName, std::string propTitle,
                          std::string propValType, bool readonly)
{
    title = propTitle;
    ValueType = VAL_OBJECT;
    pLogger->PrintLog(
        LOG_LEVEL_DEBUG, LOGGER_COMP_PROPERTY,
        "Initialize called for object type:", ParameterName.c_str());
    pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_PROPERTY,
                      "Initialize called for propTitle:", title.c_str());
    if (propValType == "boolean")
    {
        values[PropParameterSize].ValueType = VAL_BOOLEAN;
        values[PropParameterSize].size = 1;
        values[PropParameterSize].name = ParameterName;
    }
    else if (propValType == "number")
    {
        values[PropParameterSize].ValueType = VAL_NUMBER;
        values[PropParameterSize].size = 8;
        values[PropParameterSize].name = ParameterName;
    }
    else if (propValType == "integer")
    {
        values[PropParameterSize].ValueType = VAL_INTEGER;
        values[PropParameterSize].size = 8;
        values[PropParameterSize].name = ParameterName;
    }
    else if (propValType == "string")
    {
        values[PropParameterSize].ValueType = VAL_STRING;
        values[PropParameterSize].size = 32;
        values[PropParameterSize].name = ParameterName;
    }
    else if (propValType == "object")
    {
        values[PropParameterSize].ValueType = VAL_OBJECT;
        values[PropParameterSize].size = 1;
        values[PropParameterSize].name = ParameterName;
    }
    else
    {
        values[PropParameterSize].ValueType = VAL_NO_STATE;
        values[PropParameterSize].size = 1;
        values[PropParameterSize].name = ParameterName;
    }
    PropParameterSize++;
    return(PropParameterSize);
}
// void ThingProperty::Set(bool NewValue)
// {
//     if (!readOnly)
//     {
//         value.boolean = NewValue;
//         if (prop_Callback !=nullptr)
//         {
//             cout << "ThingProperty::Set=" << NewValue << endl;
//             prop_Callback(NewValue);
//         }
//     }
// }

// void ThingProperty::Set(double NewValue)
// {
//     if (!readOnly)
//     {
//         value.number = NewValue;
//     }
// }

// void ThingProperty::Set(char *NewValue)
// {
//     if (!readOnly)
//     {
//         value.string = NewValue;
//     }
// }

// bool ThingProperty::Get(void)
// {
//     if (ValueType == VAL_BOOLEAN)
//     {
//         return value.boolean;
//     }
// }

/* Called when the parameter size is 1 or when the value type is not object*/
std::string
ThingProperty::GetValueTypeString(void)
{
    if (values[0].ValueType == VAL_BOOLEAN)
    {
        return ("boolean");
    }
    else if (values[0].ValueType == VAL_NUMBER)
    {
        return ("number");
    }
    else if (values[0].ValueType == VAL_STRING)
    {
        return ("string");
    }
    else if (values[0].ValueType == VAL_INTEGER)
    {
        return ("integer");
    }
    else if (values[0].ValueType == VAL_OBJECT)
    {
        return ("object");
    }
    else
    {
        return ("number");
    }
}

std::string
ThingProperty::GetValueTypeString(ThingInteractionValueType ValueType)
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
    else if (ValueType == VAL_INTEGER)
    {
        return ("integer");
    }
    else if (ValueType == VAL_OBJECT)
    {
        return ("object");
    }
    else
    {
        return ("number");
    }
}

std::string
ThingProperty::GetKeyName(void)
{
    return (keyname);
}

std::string ThingProperty::Serialize(cJSON *InPropertyPtr)
{
    cJSON_AddStringToObject(InPropertyPtr,"title", title.c_str());
    cJSON_AddStringToObject(InPropertyPtr,"@type", atType.c_str());
    pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_PROPERTY,"Serialize called for propTitle:",title.c_str());
    if (PropParameterSize > 1)
    {
        cJSON *InputPropSer = cJSON_CreateObject();
        cJSON_AddStringToObject(InPropertyPtr,"type","object");
        for (uint8_t indexSize=0; indexSize < PropParameterSize; indexSize++)
        {
            cJSON *InputPropItem = cJSON_CreateObject();
            cJSON_AddStringToObject(InputPropItem,"type",GetValueTypeString(values[indexSize].ValueType).c_str());
            cJSON_AddItemToObject(InputPropSer, values[indexSize].name.c_str(),InputPropItem);
        }
        cJSON_AddItemToObject(InPropertyPtr,"properties", InputPropSer);
    }
    else
    {
        cJSON_AddStringToObject(InPropertyPtr,"type", GetValueTypeString().c_str());
        // cJSON_AddStringToObject(InPropertyPtr,"readOnly", values[0].readOnly.c_str());
        cJSON_AddItemToObject(InPropertyPtr, "writeOnly", cJSON_CreateFalse());
    }
    return (title);
}

ThingProperty::ThingProperty(void)
{
    // title = propTitle;
    // UpdateDefaults(propType);
    // PropSchema = GetPropSchema(propType);
    // readOnly = readonly;
    minimum = 0;
    maximum = 0;
    PropParameterSize = 0;
    unit = "none";
    pLogger = Logger::getInstance();
    pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_PROPERTY, ":Initializing");
}

// void OnOffProperty::OnoffProperty(std::string propTitle,bool readonly,
// prop_callback_ptr OnOffCallback)
// {
//     title = propTitle;
//     readOnly = readonly;
//     ValueType = VAL_BOOLEAN;
//     PropSchema = "OnOffProperty";
//     keyname = "on";
//     prop_Callback = OnOffCallback;
// }

// void OnOffProperty::Set(bool NewValue)
// {
//     if (!readOnly)
//     {
//         value.boolean = NewValue;
//         if (prop_Callback !=nullptr)
//         {
//             cout << "ThingProperty::Set=" << NewValue << endl;
//             prop_Callback(NewValue);
//         }
//     }
// }