// #include "BindHttpServer.h"
#include "WoTThing.h"
#include "WoTAdapter.h"
#include "BindServer.h"
#include "WoTServient.h"




void WoTServient::build_default_clients(void)
{
    // ToDo
}
bool WoTServient::WoTServientGet(std::string RxURI, cJSON *ThingDesc)
{
    //return(WoTAdapterGet(RxURI, ThingDesc));
    return (true);
}
bool WoTServient::WoTServientPut(std::string RxURI, cJSON *JsonMeta)
{
    //return(WoTAdapterPut(RxURI, JsonMeta));
    return (true);
}

WoTServient::WoTServient(void)
{

}

void WoTServient::EmitEvent(std::string EventID, cJSON *out)
{
    WoTAdapterRef.EmitEvent(EventID,out);
}

void WoTServient::Process(void)
{
    WoTAdapterRef.Process();
}

void WoTServient::Initialize(int CataloguePort, std::string hostname,bool dns_enabled, std::string dns_instance_name)
{
    CataloguePort = CataloguePort;
    Hostname = hostname;
    ServientRunning = false;
    
    ExposedThingIndex=0;
    pLogger = Logger::getInstance();
	pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_SERVIENT,":Initializing");
    build_default_clients();
    WoTAdapterRef.WoTAdapterInit(Exposed_Thing, Consumed_Thing);
}

void WoTServient::AddServer(BindServer *ServerIn)
{
    pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_SERVIENT,":AddServer call");
    WoTAdapterRef.AddServer(ServerIn);
    
}

void WoTServient::AddClient(BindClient *ClientIn)
{
    cout << ClientIn << endl;
    WoTAdapterRef.AddClient(ClientIn);
}

void WoTServient::Start(void)
{
    unsigned char ServerCnt=0;
    pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_SERVIENT,":Start call");    
    WoTAdapterRef.Start();
    ServientRunning =  true;
}

ExposedThing* WoTServient::GetExposedThing (std::string ThingID)
{
    ExposedThing *Exposed_Thing_Rtr=NULL;
    for (int thing_index=0;thing_index < ExposedThingIndex; thing_index++)
    {
        if (Exposed_Thing[thing_index]->id == ThingID)
        {
            Exposed_Thing_Rtr = Exposed_Thing[thing_index];
        }
    }
    return (Exposed_Thing_Rtr);
}

void WoTServient::AddExposedThing (ExposedThing *ThingInput)
{
    // cout <<"AddExposedThing call:"<< std::endl;
    pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_SERVIENT,":AddExposedThing call,",ThingInput->id.c_str());    
    Exposed_Thing[ExposedThingIndex++] = ThingInput;
    // ThingInput->AdapterRef = &WoTAdapterRef;
    WoTAdapterRef.RegisterServer(ThingInput->title);
}

void WoTServient::AddConsumedThing (ConsumedThing *ThingInput)
{
    // cout <<"AddExposedThing call:"<< std::endl;
    pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_SERVIENT,":AddExposedThing call,",ThingInput->id.c_str());    
    Consumed_Thing[ConsumedThingIndex++] = ThingInput;
    ThingInput->SetPropertyReadCallback(WoTAdapterRef.ReadProperty,
                                        WoTAdapterRef.SubscribeEvent);
}