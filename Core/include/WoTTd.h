#ifndef WOTTD
#define WOTTD

#include "WoTThing.h"
#include "cJSON.h"
#include <variant>
// #include "Schema.h"
#include "Logger.h"
#include "WoTAction.h"
#include "WoTEvent.h"
#include "WoTProperty.h"

// using nlohmann::json;
// using nlohmann::json_uri;
// using nlohmann::json_schema::json_validator;

using namespace std;
using namespace Logging;

#define LOGGER_COMP_TD "ThingDescription"

class WoTSchemaData
{

private:
  const uint8_t fieldSize = 7;
  std::string allFields[7]
      = { "type",         "title",     "titles",  "description",
          "descriptions", "writeOnly", "readOnly" };
  std::string requiredFields[1] = {};
  std::string defaultFields[1] = {};
  std::string exposedFields[7] = {};

public:
  std::string atType; // JSON-LD keyword to label the object with semantic tags
                      // (or types)
  std::string title;  // Provides a human-readable title (e.g., display a text
                      // for UI representation) based on a default language.
  std::string titles; // Provides multi-language human-readable titles (e.g.,
                      // display a text for UI representation in different
                      // languages). Also see MultiLanguage.
  std::string description;  // Provides additional (human-readable) information
                            // based on a default language.
  std::string descriptions; // Can be used to support (human-readable)
                            // information in different languages.
  // std::any constVal;
  // std::any defaultVal;
  // std::string unit;
  // std::any enumVal;
  // bool readOnly;    //
  // bool writeOnly;   //
  // std::string type; //
  // std::any value;
};

class WoTSchemaArray : public WoTSchemaData
{
private:
  const uint8_t fieldSize = 3;
  std::string allFields[3] = { "items", "minItems", "maxItems" };
  std::string requiredFields[1] = {};
  std::string defaultFields[1] = {};
  std::string exposedFields[3] = {};

public:
  // std::vector<std::any> items;
  uint8_t maxItems;
  uint8_t minItems;
};

class WoTSchemaNumber : public WoTSchemaData
{
private:
  const uint8_t fieldSize = 5;
  std::string allFields[5] = { "minimum", "exclusiveMinimum", "maximum",
                               "exclusiveMaximum", "multipleOf" };
  std::string requiredFields[1] = {};
  std::string defaultFields[1] = {};
  std::string exposedFields[5] = {};

public:
  double minimum;
  double exclusiveMin;
  double maximum;
  double exclusiveMax;
  double multipleOf;
};

class WoTSchemaInteger : public WoTSchemaData
{
private:
  const uint8_t fieldSize = 5;
  std::string allFields[5] = { "minimum", "exclusiveMinimum", "maximum",
                               "exclusiveMaximum", "multipleOf" };
  std::string requiredFields[1] = {};
  std::string defaultFields[1] = {};
  std::string exposedFields[5] = {};

public:
  // std::any minimum;
  // std::any exclusiveMin;
  // std::any maximum;
  // std::any exclusiveMax;
  // std::any multipleOf;
};

class WoTSchemaObject : public WoTSchemaData
{
private:
  const uint8_t fieldSize = 2;
  std::string allFields[2] = { "properties", "required" };
  std::string requiredFields[1] = {};
  std::string defaultFields[1] = {};
  std::string exposedFields[2] = {};

public:
  // std::vector<std::any>
  //     properties; // Data schema nested definitions; Map of DataSchema
  // std::vector<std::any> required; // Defines which members of the object type
  //                                 // are mandatory; Array of string
};

class WoTInteractionAffordance
{
private:
  std::string allFields[6] = { "type",         "description", "descriptions",
                               "descriptions", "proxy",       "scheme" };
  std::string requiredFields[1] = { "scheme" };
  std::string defaultFields[1] = {};

protected:
  std::string InteractionType;

public:
  std::string type;
  std::string title;
  std::string titles;
  std::string description;
  std::string descriptions;
  WoTForms forms;
  std::string uriVariables;
};

class WoTPropertyAffordance : public WoTInteractionAffordance
{
private:
  std::string allFields[1] = { "observable" };
  std::string requiredFields[1] = {};
  std::string defaultFields[1] = { "observable" };

public:
  bool observable;
  WoTPropertyAffordance () { InteractionType = "Property"; }
};

class WoTActionAffordance : public WoTInteractionAffordance
{
private:
  std::string allFields[5]
      = { "input", "output", "safe", "idempotent", "synchronous" };
  std::string requiredFields[1] = {};
  std::string defaultFields[3] = { "safe", "idempotent" };

public:
  WoTSchemaData input;
  WoTSchemaData output;
  bool safe;
  bool idempotent;
  bool synchronous;
  WoTActionAffordance () { InteractionType = "Action"; }
};

class WoTEventAffordance : public WoTInteractionAffordance
{
private:
  std::string allFields[4]
      = { "subscription", "data", "dataResponse", "cancellation" };
  std::string requiredFields[1] = {};
  std::string defaultFields[1] = {};

public:
  WoTSchemaData subscription;
  WoTSchemaData data;
  WoTSchemaData dataResponse;
  WoTSchemaData cancellation;
  WoTEventAffordance () { InteractionType = "Event"; }
};

class WoTSecurityScheme
{
private:
  std::string allFields[6] = { "type",         "description", "descriptions",
                               "descriptions", "proxy",       "scheme" };
  std::string requiredFields[1] = { "scheme" };
  std::string defaultFields[1] = {};
  std::string SecurityScheme[9]
      = { "nosec", "combo",  "basic",  "digest", "bearer",
          "psk",   "oauth2", "apikey", "auto" };

public:
  std::string type;
  std::string description;
  std::string descriptions;
  std::string proxy;
  std::string scheme;
  std::string schemeIn;
};

class WoTExpectedResponse
{
private:
  std::string allFields[1] = { "contentType" };
  std::string requiredFields[1] = { "contentType" };
  std::string defaultFields[1] = {};

public:
  std::string contentType;
};

class WoTAddiExpectedResponse
{
private:
  std::string allFields[3] = { "success", "contentType", "schema" };
  std::string requiredFields[1] = {};
  std::string defaultFields[2] = { "success", "contentType" };

public:
  std::string success;
  std::string contentType;
  std::string schema;
};

class WoTLink
{
private:
  std::string allFields[6]
      = { "href", "type", "rel", "anchor", "sizes", "hreflang" };
  std::string requiredFields[1] = { "href" };
  std::string defaultFields[1] = {};

public:
  std::string href;
  std::string type;
  std::string rel;
  std::string anchor;
  std::string sizes;
  std::string hreflang;
};

class WoTVersionInfo
{
private:
  std::string allFields[2] = { "instance", "model" };
  std::string requiredFields[1] = { "instance" };
  std::string defaultFields[1] = {};

public:
  std::string instance;
  std::string model;
};

class ThingDescription
{
private:
public:
  std::string context;
  std::string atType;
  cJSON *wot_td_schema;
  bool td_valid = false;
  cJSON *wot_td;
  Logger *pLogger = NULL; // Create the object pointer for Logger Class
  ThingDescription (std::string Td);
  static void TdLoader (const std::string uri, cJSON *schema);
  // bool Validate(std::string Td, std::string TdSchema);
  bool Validate (cJSON *Td);
  void ExtractForm (cJSON *FormItemFragment, WoTForms *NewForm);
  ThingProperty *build_property (std::string propname, cJSON *propItemFragment);
  ThingAction *build_action (std::string actnname, cJSON *actnItemFragment);
  ThingEvent *build_event (std::string evntname, cJSON *evntItemFragment);
  ExposedThing *build_exposedThing (void);
  ConsumedThing *build_consumedThing (void);
  // Thing BuildThing(void);
};

#endif /* WOTTD */
