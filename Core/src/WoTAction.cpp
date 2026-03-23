#include "WoTAction.h"
#include "WoT.h"

using namespace std;

void
ThingAction::UpdateDefaults (ThingInteractionType Actiontype)
{
  switch (Actiontype)
    {
    case eALARM:
      ValueType = VAL_BOOLEAN;
      ActionSchema = "AlarmAction";
      unit = "none";
      keyname = "alarm";
      break;
    case eBOOLEAN:
      ValueType = VAL_BOOLEAN;
      ActionSchema = "BooleanAction";
      unit = "none";
      keyname = "bool";
      break;
    case eBRIGHTNESS:
      ValueType = VAL_NUMBER;
      ActionSchema = "BrightnessAction";
      unit = "percent";
      minimum = 0;
      maximum = 100;
      keyname = "brightness";
      break;
    case eCOLOR:
      ValueType = VAL_STRING;
      ActionSchema = "ColorAction";
      unit = "none";
      keyname = "color";
      break;
    case eCOLOR_TEMPERATURE:
      ValueType = VAL_NUMBER;
      ActionSchema = "ColorTemperatureAction";
      unit = "kelvin";
      keyname = "colortemp";
      break;
    case eCURRENT:
      ValueType = VAL_NUMBER;
      ActionSchema = "CurrentAction";
      unit = "ampere";
      keyname = "current";
      break;
    case eFREQUENCY:
      ValueType = VAL_NUMBER;
      ActionSchema = "FrequencyAction";
      unit = "none";
      keyname = "freq";
      break;
    case eHEATING_COOLING:
      ValueType = VAL_STRING;
      ActionSchema = "HeatingCoolingAction";
      unit = "none";
      keyname = "heating";
      break;
    case eIMAGE:
      ValueType = VAL_NO_STATE;
      ActionSchema = "ImageAction";
      keyname = "image";
      break;
    case eINSTANTANEOUS_POWER:
      ValueType = VAL_NUMBER;
      ActionSchema = "InstantaneousPowerAction";
      keyname = "instpower";
      break;
    case eLEAK:
      ValueType = VAL_BOOLEAN;
      ActionSchema = "LeakAction";
      keyname = "leak";
      break;
    case eLEVEL:
      ValueType = VAL_NUMBER;
      ActionSchema = "LevelAction";
      unit = "percent";
      minimum = 0;
      maximum = 100;
      keyname = "level";
      break;
    case eLOCKED:
      ValueType = VAL_STRING;
      ActionSchema = "LockedAction";
      keyname = "locked";
      break;
    case eMOTION:
      ValueType = VAL_BOOLEAN;
      ActionSchema = "MotionAction";
      keyname = "motion";
      break;
    case eON_OFF:
      ValueType = VAL_BOOLEAN;
      ActionSchema = "OnOffAction";
      keyname = "on";
      break;
    case eOPEN:
      ValueType = VAL_BOOLEAN;
      ActionSchema = "OpenAction";
      keyname = "open";
      break;
    case ePUSHED:
      ValueType = VAL_BOOLEAN;
      ActionSchema = "PushedAction";
      keyname = "pushed";
      break;
    case eTARGET_TEMPERATURE:
      ValueType = VAL_NUMBER;
      ActionSchema = "TargetTemperatureAction";
      unit = "celsius";
      keyname = "targettemp";
      break;
    case eTEMPERATURE:
      ValueType = VAL_NUMBER;
      ActionSchema = "TemperatureAction";
      unit = "celsius";
      keyname = "temp";
      break;
    case eTHERMOSTAT:
      ValueType = VAL_STRING;
      ActionSchema = "ThermostatModeAction";
      keyname = "thermostatMode";
      break;
    case eVIDEO:
      ValueType = VAL_NO_STATE;
      ActionSchema = "VideoAction";
      keyname = "video";
      break;
    case eVOLTAGE:
      ValueType = VAL_NUMBER;
      ActionSchema = "VoltageAction";
      unit = "volt";
      keyname = "voltage";
      break;
    case eKEEP_LAST:
      ValueType = VAL_NO_STATE;
      ActionSchema = "LAST_Action";
      keyname = "NULL";
      break;
    }
};

void
ThingAction::Initialize (std::string ActionTitle, uint8_t inputSize,
                         uint8_t outputSize)
{
  title = ActionTitle;
  // UpdateDefaults(ActionType);
  // PropSchema = GetPropSchema(propType);
  // readOnly = readonly;
  ValueType = VAL_NUMBER;
  InputSize = inputSize;
  OutputSize = outputSize;
}

ThingInteractionValueType
ThingAction::GetValueType (std::string typeString)
{
  if (typeString == "boolean")
    {
      return (VAL_BOOLEAN);
    }
  else if (typeString == "number")
    {
      return (VAL_NUMBER);
    }
  else if (typeString == "integer")
    {
      return (VAL_INTEGER);
    }
  else if (typeString == "string")
    {
      return (VAL_STRING);
    }
  else if (typeString == "object")
    {
      return (VAL_OBJECT);
    }
  else
    {
      return (VAL_NO_STATE);
    }
}
void
ThingAction::InitializeInputs (uint8_t inputindex, std::string ActioninputName,
                               std::string inputType, uint8_t inputParamSize)
{
  if (InputSize > 0)
    {
      Inputs[inputindex].title = ActioninputName;
      Inputs[inputindex].ValueType = GetValueType (inputType);
      Inputs[inputindex].size = inputParamSize;
    }
  else
    {
      pLogger->PrintLog (LOG_LEVEL_DEBUG, LOGGER_COMP_ACTION,
                         "Input size was set to : 0");
    }
}

void
ThingAction::InitializeOutputs (uint8_t outputindex,
                                std::string ActionoutputName,
                                std::string outputType,
                                uint8_t outputParamSize)
{
  if (OutputSize > 0)
    {
      Outputs[outputindex].title = ActionoutputName;
      Outputs[outputindex].ValueType = GetValueType (outputType);
      Outputs[outputindex].size = outputParamSize;
    }
  else
    {
      pLogger->PrintLog (LOG_LEVEL_DEBUG, LOGGER_COMP_ACTION,
                         "Output size was set to : 0");
    }
}

std::string
ThingAction::GetValueTypeString (void)
{
  if (ValueType == VAL_BOOLEAN)
    {
      return ("boolean");
    }
  else if (ValueType == VAL_STRING)
    {
      return ("string");
    }
  else if (ValueType == VAL_OBJECT)
    {
      return ("object");
    }
  else if (ValueType == VAL_INTEGER)
    {
      return ("integer");
    }
  else
    {
      return ("number");
    }
}

std::string
ThingAction::GetValueTypeString (ThingInteractionValueType valueTypeIn)
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

std::string
ThingAction::GetKeyName (void)
{
  return (keyname);
}

std::string
ThingAction::Serialize (Poco::JSON::Object *InActionPtr)
{
  // std::string RetString = NULL;
  // InActionPtr->set("@type", ActionSchema);
  // InActionPtr->set("title", title);
  // InActionPtr->set("readOnly", readOnly);
  if (InputSize > 0)
    {
      Poco::JSON::Object InputSer;
      Poco::JSON::Object InputPropSer;
      InputSer.set ("type", "object");
      for (uint8_t indexSize = 0; indexSize < InputSize; indexSize++)
        {
          Poco::JSON::Object InputPropItem;
          InputPropItem.set ("type",
                             GetValueTypeString (Inputs[indexSize].ValueType));
          InputPropSer.set (Inputs[indexSize].title, InputPropItem);
        }
      InputSer.set ("properties", InputPropSer);
      InActionPtr->set ("input", InputSer);
    }
  else
    {
      // InActionPtr->set("type", GetValueType());
    }
  return (title);
}

ThingAction::ThingAction (void)
{
  // title = propTitle;
  // UpdateDefaults(propType);
  // PropSchema = GetPropSchema(propType);
  // readOnly = readonly;
  minimum = 0;
  maximum = 0;
  unit = "none";
  pLogger = Logger::getInstance ();
  pLogger->PrintLog (LOG_LEVEL_DEBUG, LOGGER_COMP_ACTION, ":Initializing");
}