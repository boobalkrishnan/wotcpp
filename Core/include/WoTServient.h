#ifndef WOTSERVIENT_H
#define WOTSERVIENT_H

// #include "WoT.h"
// #include "WoTProperty.h"
// #include "WoTAction.h"
// #include "WoTEvent.h"
#include "BindClient.h"
#include "BindServer.h"
#include "Logger.h"
#include "WoTThing.h"
#include "WoTAdapter.h"


#define WOT_SERVIENT_MAX_EXPOSED_THING 10

using namespace Logging;

#define LOGGER_COMP_SERVIENT "WoTServient"

class WoTServient
{
private:
  bool ServientRunning;
  void build_default_clients (void);
  void start_dnssd (void);

public:
  WoTAdapter WoTAdapterRef;
  Logger *pLogger = NULL; // Create the object pointer for Logger Class


  bool DnssdEnabled = false;
  std::string Hostname;
  int CataloguePort;
  ExposedThing *Exposed_Thing[10];
  int ExposedThingIndex;
    ConsumedThing *Consumed_Thing[10];
    int ConsumedThingIndex;
  std::string DnsInstanceName;
  static bool WoTServientGet (std::string RxURI,
                              Poco::JSON::Object *ThingDesc);
  static bool WoTServientPut (std::string RxURI,
                              Poco::JSON::Object::Ptr JsonMeta);
  void Start (void);
  void Shutdown (void);
  WoTServient (void);
  void Process (void);
  void Initialize (int CataloguePort, std::string hostname, bool dns_enabled,
                   std::string dns_instance_name);
  ExposedThing *GetExposedThing (std::string ThingID);
  void AddExposedThing (ExposedThing *ThingInput);
    ConsumedThing *GetConsumedThing (std::string ThingID);
    void AddConsumedThing (ConsumedThing *ThingInput);
  int RemoveExposedThing (int ThingID);
  int EnableExposedThing (int ThingID);
  int DisableExposedThing (int ThingID);
  void AddServer (BindServer *ServerIn);
  int RemoveServer (int ServerIndex);

  void AddClient (BindClient *ClientIn);
  int RemoveClient (int ClientIndex);

  // ThingProperty *property[WEBTHING_PROPERTY_MAX];
  // int propertyIndex;
  // Thing(std::string _context, std::string _id, std::string _title,
  // std::string _type);
  // void SetThingNo(int ThingNo);
  // unsigned char addProperty(ThingProperty* _property,thing_callback_ptr
  // pFunc);
};

#endif