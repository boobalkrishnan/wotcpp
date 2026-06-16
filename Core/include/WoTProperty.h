#ifndef WOTPROPERTY_H
#define WOTPROPERTY_H

#include "Logger.h"
#include "WoTInteractions.h"
#include "cJSON.h"
#include <iostream>
#include <string>
using namespace Logging;

// enum ThingPropertyValueType {
//   VAL_NO_STATE,
//   VAL_BOOLEAN,
//   VAL_NUMBER,
//   VAL_STRING
// };

// union ThingPropertyValue {
//   bool boolean;
//   double number;
//   char* string;
// };

// enum PropertyUnits{
// 	eNONE,
// 	eCELCUIS,
// 	eFARHENITE,
// 	ePERCENT,
// 	eMILLISECONDS
// };

// enum ThingPropertyType{
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

#define LOGGER_COMP_PROPERTY "WoTProperty"

typedef void (*prop_callback_ptr) (void);
typedef ThingInteractionValueType (*prop_readcallback_ptr) (
    std::string ThingId, std::string PropId,
    ThingInteractionValue *PropertyValue);
typedef void (*prop_writecallback_ptr) (std::string ThingId,
                                        std::string PropId,
                                        ThingInteractionValueType ValueType,
                                        ThingInteractionValue *PropertyValue);

struct PropParameters_t
{
  uint8_t size;
  std::string name;
  ThingInteractionValueType ValueType;
  ThingInteractionValue Value;
  int minimum;      /* minimum */
  int maximum;      /* maximum */
  bool readOnly;    /* readOnly */
  bool writeOnly;   /* WriteOnly */
  std::string unit; /* unit */
};

#define WOT_THINGS_MAX_PROPERTY 5

class ThingProperty
{
private:
  prop_callback_ptr prop_OnChange;

  int prop_id;

  int minimum; /* minimum */
  int maximum; /* maximum */
  // bool readOnly;                      /* readOnly */
  std::string unit; /* unit */
  void UpdateDefaults (ThingInteractionType proptype);

public:
  std::string PropSchema;
  std::string atType; /* @type */
  uint8_t PropParameterSize;
  // PropParameters_t value;
  PropParameters_t values[WOT_THINGS_MAX_PROPERTY];
  std::string title;
  Logger *pLogger = NULL; // Create the object pointer for Logger Class
  std::string GetValueTypeString (void);
  uint8_t FindParamIndex (std::string ParameterName);
  std::string GetValueTypeString (ThingInteractionValueType ValueType);
  std::string GetKeyName (void);
  ThingInteractionValueType ValueType; /* type */
  std::string keyname;
  bool subscribed;
  WoTForms form[5];
  // ThingProperty(std::string propTitle, ThingPropertyType propType, bool
  // readonly);
  ThingProperty (void);
  void Initialize (std::string propTitle, std::string propType, bool readonly);
  uint8_t Initialize (std::string ParameterName, std::string propTitle,
                      std::string propValType, bool readonly);
  std::string Serialize (cJSON *InPropertyPtr);
  prop_readcallback_ptr prop_ReadHandler;
  prop_writecallback_ptr prop_WriteHandler;
  // void Set(bool NewValue);
  // void Set(double NewValue);
  // void Set(char* NewValue);
  // bool Get(void);
  // double Get(void);
  // char* Get(void);
  // ThingProperty(ThingPropertyType thingtype, bool defaultval,
  //                 bool min, bool max, bool readonly, PropertyUnits
  //                 thing_unit);
  // ThingProperty(ThingPropertyType thingtype, double defaultval,
  //                 double min, double max, bool readonly, PropertyUnits
  //                 thing_unit);
  // ThingProperty(ThingPropertyType thingtype, char* defaultval,
  //                 char* min, char* max, bool readonly, PropertyUnits
  //                 thing_unit);
};

// class OnOffProperty
// {
//     private:

//     public:
//       OnOffProperty(std::string propTitle, bool readonly, prop_callback_ptr
//       OnOffCallback); void Set(bool NewValue); bool Get(void);
// };

#endif