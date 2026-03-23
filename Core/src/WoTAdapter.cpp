#include "WoTAdapter.h"
#include "BindClient.h"
#include "BindServer.h"
#include "Logger.h"
#include "WoTThing.h"
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/StreamCopier.h>
#include <Poco/Util/ServerApplication.h>

using namespace std;

// static WoT WoTRef;
ExposedThing **Exposed_Thing_Ref;
ConsumedThing **Consumed_Thing_Ref;
BindServer *Servers[WOT_SERVIENT_MAX_SERVERS];
BindClient *Clients[WOT_SERVIENT_MAX_CLIENTS];

void WoTAdapter::Process(void)
{
}
WoTAdapter::WoTAdapter(void)
{
    ClientCount = 0;
    ServerCount = 0;
}

void WoTAdapter::RegisterServer(string ThingName)
{
    Servers[0]->RegisterServer(ThingName);
}

void WoTAdapter::AddServer(BindServer *ServerIn)
{
    pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOTADAPTER, ":AddServer call");
    Servers[ServerCount++] = ServerIn;
    ServerIn->SetHandlers(WoTAdapterGetInteraction,
                          WoTAdapterSetInteraction,
                          WoTAdapterThingHndl,
                          WoTAdapterAffordanceHndl,
                          WoTAdapterGetSubscribed);
}

void WoTAdapter::AddClient(BindClient *ClientIn)
{
    cout << ClientIn << endl;
    if (ClientIn == NULL)
    {
        pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOTADAPTER, "ClientIn is NULL");
        return;
    }
    pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOTADAPTER, ":AddClient call");
    Clients[ClientCount] = ClientIn;
    Clients[ClientCount]->SetEventHandler(WoTAdapter::EventHandler);
    if (Clients[ClientCount] != NULL)
    {
        pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOTADAPTER, "Clients[0] is not NULL");
    }
    ClientCount++;
}

void WoTAdapter::Start(void)
{
    unsigned char ServerCnt = 0, ClientCnt = 0;
    pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOTADAPTER, ":Start call");
    // Refresh the forms of interactions
    for (ServerCnt = 0; ServerCnt < ServerCount; ServerCnt++)
    {
        if (Servers[ServerCnt] != NULL)
        {
            // Start the BindServer. Example, Webserver for http
            Servers[ServerCnt]->Start();
        }
    }
    pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOTADAPTER, "ClientCount:", ClientCount);
    for (ClientCnt = 0; ClientCnt < ClientCount; ClientCnt++)
    {
        if (Clients[ClientCnt] != NULL)
        {
            // Start the BindServer. Example, Webserver for http
            Clients[ClientCnt]->Start();
        }
    }
}

void WoTAdapter::EventHandler(std::string ThingID, std::string EventID, Poco::JSON::Object::Ptr eventInput)
{
    ThingInteractionValueType PropValType;
    ThingInteractionValue PropertyValue;

    cout << "EventHandler called for ThingID:" << ThingID.c_str() << endl;
    ConsumedThing *Consumed_Thing = GetConsumedThing(ThingID);
    uint8_t EventIdx = Consumed_Thing->GetEventIndex(EventID);
    PropValType = Consumed_Thing->Thing_Event[EventIdx]->Inputs[0].ValueType;
    std::string eventTitle = Consumed_Thing->Thing_Event[EventIdx]->Inputs[0].title;
    uint8_t Size = Consumed_Thing->Thing_Event[EventIdx]->Inputs[0].size;
    cout << "EventHandler eventTitle:" << eventTitle.c_str() << (int)Size << endl;
    std::string eventData = eventInput->getValue<std::string>(eventTitle);
    DecodePayload(PropValType, eventData, &PropertyValue);
    if (Consumed_Thing != NULL)
    {
        if (Consumed_Thing->event_observcallback != NULL)
        {
            Consumed_Thing->event_observcallback(EventID, eventTitle, PropValType, &PropertyValue);
        }
        else
        {
            cout << "Event callback is NULL for ThingID:" << ThingID.c_str() << endl;
        }
    }
}

void WoTAdapter::EmitEvent(std::string EventID, std::string out)
{
    Servers[0]->EmitEvent(EventID, out);
}

void WoTAdapter::ReadProperty(uint8_t Protocol, std::string ThingID, std::string PropID, 
                                std::string propertyHref, ThingInteractionValueType PropValType, ThingInteractionValue *PropertyValue)
{
    cout << Clients[0] << endl;
    if ((Clients[0]) == NULL)
    {
        cout << "WoTAdapter::ReadProperty called with NULL ClientIn" << endl;
        // return;
    }
    cout << "WoTAdapter::ReadProperty called for: " << (int) PropValType << endl;
    std::string value_string = Clients[0]->ReadProperty(Protocol, ThingID, PropID, propertyHref);
    cout << "WoTAdapter::value_string Received:" << value_string << endl;
    Poco::JSON::Parser parser;
    auto result = parser.parse(value_string);
    Poco::JSON::Object ValueJson = *result.extract<Poco::JSON::Object::Ptr>();
    DecodePayload(PropValType, ValueJson.getValue<std::string>("value"), PropertyValue);
}

void WoTAdapter::SubscribeEvent(uint8_t Protocol, std::string ThingID, std::string EventID, std::string eventHref)
{
    cout << Clients[0] << endl;
    Clients[0]->SubscribeEvent(Protocol, ThingID, EventID, eventHref);
}

void WoTAdapter::WoTAdapterInit(ExposedThing **Exposed_Thing_In, ConsumedThing **Consumed_Thing_In) // BindServer *ServerIn, BindClient **ClientIn,
{
    Exposed_Thing_Ref = Exposed_Thing_In;
    Consumed_Thing_Ref = Consumed_Thing_In;
    // ServerIn = ServerIn;
    // ClientIn = ClientIn;
    pLogger = Logger::getInstance();

    pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOTADAPTER, ":Initializing");
}

ExposedThing *WoTAdapter::GetExposedThing(std::string ThingID)
{
    ExposedThing *Exposed_Thing_Rtr = NULL;
    Logger *pLogger = NULL;
    pLogger = Logger::getInstance();
    for (int thing_index = 0; thing_index < 10; thing_index++)
    {
        if (Exposed_Thing_Ref[thing_index] != NULL)
        {
            pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOTADAPTER, "ThingID:", ThingID.c_str());
            pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOTADAPTER, "title:", (*Exposed_Thing_Ref[thing_index]).title.c_str());
            if ((*Exposed_Thing_Ref[thing_index]).title.c_str() == ThingID)
            {
                pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOTADAPTER, "Exposed_Thing_Ref matching", thing_index);
                // pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOTADAPTER,Exposed_Thing_Ref[thing_index]->id.c_str());
                Exposed_Thing_Rtr = Exposed_Thing_Ref[thing_index];
                break;
            }
            else
            {
                pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOTADAPTER, "Exposed_Thing_Ref not matching", thing_index);
            }
        }
        else
        {
            pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOTADAPTER, "Exposed_Thing_Ref has null");
        }
    }
    return (Exposed_Thing_Rtr);
}

ConsumedThing *WoTAdapter::GetConsumedThing(std::string ThingID)
{
    ConsumedThing *Consumed_Thing_Rtr = NULL;
    Logger *pLogger = NULL;
    pLogger = Logger::getInstance();
    for (int thing_index = 0; thing_index < 10; thing_index++)
    {
        if (Consumed_Thing_Ref[thing_index] != NULL)
        {
            pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOTADAPTER, "ThingID:", ThingID.c_str());
            pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOTADAPTER, "title:", (*Consumed_Thing_Ref[thing_index]).title.c_str());
            if ((*Consumed_Thing_Ref[thing_index]).title.c_str() == ThingID)
            {
                pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOTADAPTER, "Consumed_Thing_Ref matching", thing_index);
                // pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOTADAPTER,Exposed_Thing_Ref[thing_index]->id.c_str());
                Consumed_Thing_Rtr = Consumed_Thing_Ref[thing_index];
                break;
            }
            else
            {
                pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOTADAPTER, "Consumed_Thing_Ref not matching", thing_index);
            }
        }
        else
        {
            pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOTADAPTER, "Consumed_Thing_Ref has null");
        }
    }
    return (Consumed_Thing_Rtr);
}

int WoTAdapter::ParseThingsURI(std::string RxURI, std::string *ArrayOfURI)
{
    std::string delimiter = "/";
    int LevelOfUTI = 0;
    size_t pos = 0;
    std::string token;
    cout << "Parsing:" << RxURI << std::endl;
    while ((pos = RxURI.find(delimiter)) != std::string::npos)
    {
        cout << "Parsing:" << RxURI << std::endl;
        token = RxURI.substr(0, pos);

        ArrayOfURI[LevelOfUTI] = token;
        cout << "Parsed:" << ArrayOfURI[LevelOfUTI] << std::endl;
        LevelOfUTI++;
        RxURI.erase(0, pos + delimiter.length());
    }
    cout << "Parsing:" << RxURI << std::endl;
    ArrayOfURI[LevelOfUTI] = RxURI;
    cout << "Parsed:" << ArrayOfURI[LevelOfUTI] << std::endl;
    return LevelOfUTI;
}

Poco::JSON::Array::Ptr WoTAdapter::CreateForms(std::string HostIn, std::string InteractionURI)
{
    Poco::JSON::Object ReturnJson;
    Poco::JSON::Array::Ptr Myform_array = new Poco::JSON::Array();
    std::string uri_created;
    Poco::JSON::Object Myform_href;
    /* Todo: Create the URI based on the server (if http or ws or mqtt)
        Forced to http now */
    // uri_created = "http://" + HostIn + "/" + InteractionURI;
    uri_created = HostIn + "/" + InteractionURI;
    Myform_href.set("href", uri_created);
    Myform_array->set(0, Myform_href);
    ReturnJson.set("forms", Myform_array);
    // securityDef[0].set("scheme",ThingReq->securityDefinitions.scheme);
    // securityDef[0].set("In",ThingReq->securityDefinitions.schemeIn);
    // securityDefs.set(ThingReq->security, securityDef[0]);
    return (Myform_array);
}

void WoTAdapter::WoTAdapterThingHndl(std::string HostIn, std::string ThingId, std::string Root_Name, Poco::JSON::Object *ThingDesc)
{
    Thing *ThingReq;
    ThingReq = GetExposedThing(ThingId);
    if (ThingReq != NULL)
    {
        cout << "ThingReqID:" << ThingReq->id << std::endl;
        ThingDesc->set("id", ThingReq->id);
        ThingDesc->set("@context", ThingReq->context);
        ThingDesc->set("title", ThingReq->title);
        ThingDesc->set("@type", ThingReq->type);
        cout << "ThingReq context:" << ThingReq->context << std::endl;
        cout << "propertyIndex:" << ThingReq->propertyIndex << std::endl;
        Poco::JSON::Object PropDesc[ThingReq->propertyIndex];
        Poco::JSON::Object Properties;
        for (int PropIndex = 0; PropIndex < ThingReq->propertyIndex; PropIndex++) //
        {
            std::string KeyName;
            std::string Interact_link;
            KeyName = ThingReq->Thing_Property[PropIndex]->Serialize(&PropDesc[PropIndex]);
            // KeyName = KeyName + to_string(PropIndex);
            Interact_link = ThingReq->title + "/property/" + KeyName;
            PropDesc[PropIndex].set("forms", CreateForms(HostIn, Interact_link));
            PropDesc[PropIndex].set("type", ThingReq->Thing_Property[PropIndex]->GetValueTypeString());

            Properties.set(KeyName, PropDesc[PropIndex]);
        }
        ThingDesc->set("properties", Properties);
        cout << "actionIndex:" << ThingReq->actionIndex << std::endl;
        if (ThingReq->actionIndex)
        {
            Poco::JSON::Object ActionDesc[ThingReq->actionIndex];
            Poco::JSON::Object Action;
            for (int ActIndex = 0; ActIndex < ThingReq->actionIndex; ActIndex++) //
            {
                std::string KeyName;
                std::string Interact_link;
                KeyName = ThingReq->Thing_Action[ActIndex]->Serialize(&ActionDesc[ActIndex]);
                // KeyName = KeyName + to_string(PropIndex);
                Interact_link = ThingReq->title + "/action/" + KeyName;
                ActionDesc[ActIndex].set("forms", CreateForms(HostIn, Interact_link));
                ActionDesc[ActIndex].set("type", ThingReq->Thing_Action[ActIndex]->GetValueTypeString());

                Action.set(KeyName, ActionDesc[ActIndex]);
            }
            ThingDesc->set("actions", Action);
        }
        cout << "eventIndex:" << ThingReq->eventIndex << std::endl;
        if (ThingReq->eventIndex)
        {
            Poco::JSON::Object EventDesc[ThingReq->eventIndex];
            Poco::JSON::Object Event;
            for (int EvntIndex = 0; EvntIndex < ThingReq->eventIndex; EvntIndex++) //
            {
                std::string KeyName;
                std::string Interact_link;
                KeyName = ThingReq->Thing_Event[EvntIndex]->Serialize(&EventDesc[EvntIndex]);
                // KeyName = KeyName + to_string(PropIndex);
                Interact_link = ThingReq->title + "/event/" + KeyName + "/subscription";
                EventDesc[EvntIndex].set("forms", CreateForms(HostIn, Interact_link));
                EventDesc[EvntIndex].set("type", ThingReq->Thing_Action[EvntIndex]->GetValueTypeString());

                Event.set(KeyName, EventDesc[EvntIndex]);
            }
            ThingDesc->set("events", Event);
        }
        Poco::JSON::Array::Ptr Security_array = new Poco::JSON::Array();
        Security_array->set(0, ThingReq->security);
        ThingDesc->set("security", Security_array);
        Poco::JSON::Object securityDef[1];
        Poco::JSON::Object securityDefs;
        securityDef[0].set("scheme", ThingReq->securityDefinitions.scheme);
        securityDef[0].set("In", ThingReq->securityDefinitions.schemeIn);
        securityDefs.set(ThingReq->security, securityDef[0]);

        ThingDesc->set("securityDefinitions", securityDefs);
        cout << "ThingReq context:type" << ThingReq->type << std::endl;
        cout << "ThingReq:" << ThingReq << std::endl;
    }
    else
    {
        ThingDesc->set("error", "No Thing found");
    }
}

void WoTAdapter::WoTAdapterAffordanceHndl(std::string HostIn,
                                          std::string ThingId,
                                          std::string Root_Name,
                                          std::string Interaction_Type,
                                          Poco::JSON::Object *ThingDesc)
{
    Thing *ThingReq;
    ThingReq = GetExposedThing(ThingId);
    if (ThingReq != NULL)
    {
        Poco::JSON::Object PropDesc[ThingReq->propertyIndex];
        Poco::JSON::Object Properties;
        for (int PropIndex = 0; PropIndex < ThingReq->propertyIndex; PropIndex++) //
        {
            std::string KeyName;
            std::string Interact_link;
            KeyName = ThingReq->Thing_Property[PropIndex]->Serialize(&PropDesc[PropIndex]);
            // KeyName = KeyName + to_string(PropIndex);
            Interact_link = ThingReq->title + "/property/" + KeyName;
            PropDesc[PropIndex].set("forms", CreateForms(HostIn, Interact_link));
            PropDesc[PropIndex].set("type", ThingReq->Thing_Property[PropIndex]->GetValueTypeString());

            Properties.set(KeyName, PropDesc[PropIndex]);
        }

        ThingDesc->set("properties", Properties);
        ThingDesc->set("security", ThingReq->security);
    }
    else
    {
        ThingDesc->set("error", "No Thing found");
    }
}

bool WoTAdapter::WoTAdapterGet(std::string RxURI, Poco::JSON::Object *ThingDesc)
{
    std::string ThingURI[10];
    int RequestLevel = 0;
    Poco::JSON::Object Temp;
    RequestLevel = ParseThingsURI(RxURI, ThingURI);
    cout << "RequestLevel:" << RequestLevel << std::endl;
    if (RequestLevel == 2)
    {
        // WoTAdapterThingHndl(ThingURI[1], ThingDesc);
        return (true);
    }
    else
    {
        return (false);
        // return (WoTAdapterThingHndl(ThingURI[1]));
    }
}

#if 1

void WoTAdapter::EncodePayload(ThingInteractionValueType inputType, ThingInteractionValue *InValue, Poco::JSON::Object *JsonData) // ostream& DataOut)
{
    cout << "Received Payload Type:" << int(inputType) << std::endl;
    if (inputType == VAL_BOOLEAN)
    {
        if (InValue->boolean == 1)
        {
            JsonData->set("value", true);
            // DataOut.write("true",4);
        }
        else
        {
            JsonData->set("value", false);
            // DataOut.write("false",5);
        }
    }
    else if (inputType == VAL_INTEGER)
    {
        JsonData->set("value", InValue->integer);
        // DataOut << InValue->integer;
    }
    else if (inputType == VAL_NUMBER)
    {
        JsonData->set("value", InValue->number);
        // DataOut << InValue->number;
    }
    else if (inputType == VAL_STRING)
    {
        JsonData->set("value", InValue->stringin);
        // DataOut << InValue->string;
    }
    else
    {
        JsonData->set("value", "Object Type Not supported");
        // DataOut << "Object Type Not supported";
    }
}

void WoTAdapter::WoTAdapterGetProperty(std::string ThingId, std::string Root_Name, std::string PropId, Poco::JSON::Object *JsonData) // ostream& DataOut)
{
    Thing *ThingReq;
    std::string value_string;
    ThingInteractionValueType valueType;
    ThingInteractionValue GetValue;
    cout << "ThingReqID:" << ThingId << ":" << PropId << std::endl;
    ThingReq = GetExposedThing(ThingId);
    if (ThingReq != NULL)
    {
        // cout << ThingReq->id<< std::endl;
        cout << ThingReq->propertyIndex << std::endl;
        for (int PropIndex = 0; PropIndex < ThingReq->propertyIndex; PropIndex++) //
        {
            std::string KeyName;
            KeyName = ThingReq->Thing_Property[PropIndex]->title;
            cout << "KeyName:" << KeyName << std::endl;
            cout << "PropId:" << PropId << std::endl;
            if (KeyName == PropId)
            {
                // JsonMeta->set("value", "Off");
                if (ThingReq->Thing_Property[PropIndex]->prop_ReadHandler != NULL)
                {
                    valueType = ThingReq->Thing_Property[PropIndex]->prop_ReadHandler(ThingId, PropId, &GetValue);
                    // EncodePayload(valueType, &GetValue, DataOut);
                    EncodePayload(valueType, &GetValue, JsonData);
                }
                else
                {
                    cout << "prop_ReadHandler:NULL" << std::endl;
                }
                break;
            }
        }
    }
}

void WoTAdapter::DecodePayload(std::string inputType, std::string inputstring, ThingInteractionValue *InValue)
{
    cout << "Received Payload Type:" << inputType << std::endl;
    if (inputType == "boolean")
    {
        InValue->boolean = 0;
        if (strcmp(inputstring.c_str(), "true") == 0)
        {
            InValue->boolean = 1;
        }
    }
    else if (inputType == "number")
    {
        InValue->number = stoi(inputstring);
    }
    else if (inputType == "string")
    {
        strcpy(InValue->stringin, inputstring.c_str());
        // InValue->string = &inputstring;
    }
    else if (inputType == "integer")
    {
        uint64_t value;
        std::istringstream iss(inputstring);
        iss >> value;
        // std::cout << value;
        InValue->integer = value;
    }
}

void WoTAdapter::DecodePayload(ThingInteractionValueType inputType, std::string inputstring, ThingInteractionValue *InValue)
{
    cout << "Received Payload :" << inputstring << std::endl;
    if (inputType == VAL_BOOLEAN)
    {
        InValue->boolean = false;
        if ((strcmp(inputstring.c_str(), "1")==0) || strcmp(inputstring.c_str(), "true") == 0 ||
            strcmp(inputstring.c_str(), "True") == 0 ||
            strcmp(inputstring.c_str(), "TRUE") == 0)
        {
            InValue->boolean = true;
        }
    }
    else if (inputType == VAL_NUMBER)
    {
        InValue->number = stof(inputstring);
    }
    else if (inputType == VAL_STRING)
    {
        strcpy(InValue->stringin, inputstring.c_str());
        // InValue->string = &inputstring;
    }
    else if (inputType == VAL_INTEGER)
    {
        uint64_t value;
        std::istringstream iss(inputstring);
        iss >> value;
        // std::cout << value;
        InValue->integer = value;
    }
}

void WoTAdapter::WoTAdapterSetProperty(std::string ThingId,
                                       std::string Root_Name,
                                       std::string PropId,
                                       istream &DataIn,
                                       uint32_t Datalen)
{
    Thing *ThingReq;
    char *inputstream[255];
    std::string value_string;
    ThingInteractionValue SetValue;
    ThingInteractionValueType ValueType;

    cout << "WoTAdapterSetProperty:" << ThingId << ":" << std::to_string(Datalen) << std::endl;
    ThingReq = GetExposedThing(ThingId);
    if (Datalen > 255)
    {
        // Datalen = DataIn.getContentLength();
    }
    std::stringstream body;
    Poco::StreamCopier::copyStream(DataIn, body);

    value_string = body.str();
    // value_string = JsonMeta->getValue<std::string>("value");
    cout << "value_string:" << value_string << std::endl;
    // value_string.resize(Datalen);
    // DataIn.read(&value_string[0], Datalen);
    // bool value = DataIn.get();

    // cout << "key:" << value_string<<":"<<std::to_string(Datalen)<< std::endl;
    // SetValue.boolean = 0;
    // std::string True="true";
    // if (strcmp(value_string.c_str(),64))
    // if (stoi(value_string)==64)
    // {
    //     SetValue.boolean = 1;
    // }
    // SetValue.boolean = value_string=="true"?true:false;
    for (int PropIndex = 0; PropIndex < ThingReq->propertyIndex; PropIndex++) //
    {
        std::string KeyName;
        KeyName = ThingReq->Thing_Property[PropIndex]->title;
        cout << "KeyName:" << KeyName << std::endl;
        cout << "PropId:" << PropId << std::endl;
        if (KeyName == PropId)
        {
            /* Can directly ThingReq->Thing_Property[PropIndex]->ValueType can be used. */
            std::string ValueTypeStr = ThingReq->Thing_Property[PropIndex]->GetValueTypeString();
            // ValueType = ThingReq->Thing_Property[PropIndex]->ValueType;
            DecodePayload(ValueTypeStr, value_string, &SetValue);

            ThingReq->Thing_Property[PropIndex]->prop_WriteHandler(ThingId, PropId, ValueType, &SetValue);
        }
    }
}
#endif

void WoTAdapter::WoTAdapterInvokeAction(std::string ThingId,
                                        std::string Root_Name,
                                        std::string CmdId,
                                        istream &DataIn,
                                        uint32_t Datalen)
{
    Poco::JSON::Object::Ptr pObject;
    Thing *ThingReq;
    char *inputstream[255];
    std::string value_string;
    ThingInteractionValue SetValue;
    ThingInteractionValueType ValueType;

    cout << "WoTAdapterInvokeAction:" << ThingId << ":" << CmdId << std::endl;
    ThingReq = GetExposedThing(ThingId);
    // value_string = JsonMeta->getValue<std::string>("value");
    // cout << "key:" << value_string<< std::endl;
    // value_string.resize(Datalen);
    // DataIn.read(&value_string[0], Datalen);

    // cout << "key:" << value_string<<":"<<std::to_string(Datalen)<< std::endl;

    for (int ActnIndex = 0; ActnIndex < ThingReq->actionIndex; ActnIndex++) //
    {
        std::string KeyName;
        KeyName = ThingReq->Thing_Action[ActnIndex]->title;
        cout << "KeyName:" << KeyName << std::endl;
        cout << "CmdId:" << CmdId << std::endl;
        if (KeyName == CmdId)
        {
            // cout << "Datalen:"<<std::to_string(Datalen)<<endl;
            if (Datalen != 0)
            {
                std::string encoded_content;
                std::string content;

                std::stringstream body;
                Poco::StreamCopier::copyStream(DataIn, body);

                encoded_content = body.str();
                // encoded_content.resize(Datalen);
                // DataIn.read(&encoded_content[0], Datalen);
                // Poco::UTF8::removeBOM(encoded_content);  // modifies the string
                cout << "key:" << encoded_content << std::endl;
                Poco::JSON::Parser parser;
                try
                {
                    Poco::Dynamic::Var result = parser.parse(encoded_content);
                    pObject = result.extract<Poco::JSON::Object::Ptr>();
                    content = pObject->getValue<std::string>("OffWaitTime");
                    cout << endl
                         << "OffWaitTime Request received: " << content << endl;
                }
                catch (Poco::Exception &exc)
                {
                    std::cerr << exc.displayText() << std::endl;
                }
            }
            ThingReq->Thing_Action[ActnIndex]->action_InvokeHandler(ThingId, CmdId, pObject);
        }
    }
}

bool WoTAdapter::WoTAdapterPut(std::string RxURI, Poco::JSON::Object::Ptr JsonMeta)
{
    std::string ThingURI[10];
    int RequestLevel = 0;
    Poco::JSON::Object Temp;
    RequestLevel = ParseThingsURI(RxURI, ThingURI);
    cout << "RequestLevel:" << RequestLevel << std::endl;
    if (RequestLevel == 2)
    {
        // WoTAdapterSetProperty(ThingURI[1],ThingURI[2], JsonMeta);
        return (true);
    }
    else
    {
        return (false);
        // return (WoTAdapterThingHndl(ThingURI[1]));
    }
}

InteractionType WoTAdapter::GetInteractionType(std::string Interaction_Type)
{
    InteractionType Rx_Interaction;
    if (Interaction_Type == PROPERTY_INTERACT)
    {
        Rx_Interaction = ProperyInteraction;
    }
    else if (Interaction_Type == ACTION_INTERACT)
    {
        Rx_Interaction = ActionInteraction;
    }
    else if (Interaction_Type == EVENT_INTERACT)
    {
        Rx_Interaction = EventInteraction;
    }
    return (Rx_Interaction);
}

void WoTAdapter::WoTAdapterGetInteraction(std::string ThingId,
                                          std::string Root_Name,
                                          std::string Interaction_Type,
                                          std::string Interaction_Name,
                                          //    ostream& DataOut)
                                          Poco::JSON::Object *JsonMeta)
{
    InteractionType Rx_Interaction;
    Rx_Interaction = GetInteractionType(Interaction_Type);
    Logger *pLogger = NULL;
    pLogger = Logger::getInstance();
    pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOTADAPTER, Interaction_Type.c_str());
    switch (Rx_Interaction)
    {
    case ProperyInteraction:
    {
        // WoTAdapterGetProperty(ThingId,Root_Name,Interaction_Name,DataOut);
        WoTAdapterGetProperty(ThingId, Root_Name, Interaction_Name, JsonMeta);
    }
    break;
    case ActionInteraction:
    {
    }
    break;
    case EventInteraction:
    {
    }
    break;
    }
}

void WoTAdapter::WoTAdapterSetInteraction(std::string ThingId,
                                          std::string Root_Name,
                                          std::string Interaction_Type,
                                          std::string Interaction_Name,
                                          istream &DataIn,
                                          uint32_t Datalen)
{
    InteractionType Rx_Interaction;
    Rx_Interaction = GetInteractionType(Interaction_Type);
    switch (Rx_Interaction)
    {
    case ProperyInteraction:
    {
        WoTAdapterSetProperty(ThingId, Root_Name, Interaction_Name, DataIn, Datalen);
    }
    break;
    case ActionInteraction:
    {
        WoTAdapterInvokeAction(ThingId, Root_Name, Interaction_Name, DataIn, Datalen);
    }
    break;
    case EventInteraction:
    {
    }
    break;
    }
}

void WoTAdapter::WoTAdapterGetSubscribed(std::string ThingId,
                                         std::string Root_Name,
                                         std::string Interaction_Type,
                                         std::string Interaction_Name,
                                         Poco::JSON::Object *JsonMeta)
{
    InteractionType Rx_Interaction;
    Rx_Interaction = GetInteractionType(Interaction_Type);
    Logger *pLogger = NULL;
    pLogger = Logger::getInstance();
    pLogger->PrintLog(LOG_LEVEL_DEBUG, LOGGER_COMP_WOTADAPTER, Interaction_Type.c_str());
    switch (Rx_Interaction)
    {
    case ProperyInteraction:
    {
        Thing *ThingReq;
        std::string value_string;
        ThingInteractionValueType valueType;
        ThingInteractionValue GetValue;
        cout << "ThingReqID:" << ThingId << ":" << Interaction_Name << std::endl;
        ThingReq = GetExposedThing(ThingId);
        if (ThingReq != NULL)
        {
            // cout << ThingReq->id<< std::endl;
            cout << ThingReq->propertyIndex << std::endl;
            for (int PropIndex = 0; PropIndex < ThingReq->propertyIndex; PropIndex++) //
            {
                std::string KeyName;
                KeyName = ThingReq->Thing_Property[PropIndex]->title;
                cout << "KeyName:" << KeyName << std::endl;
                cout << "PropId:" << Interaction_Name << std::endl;
                if (KeyName == Interaction_Name)
                {
                    ThingReq->Thing_Property[PropIndex]->subscribed = true;
                    // JsonMeta->set("value", "Off");
                    //  if (ThingReq->Thing_Property[PropIndex]->prop_ReadHandler != NULL)
                    //  {
                    //      valueType = ThingReq->Thing_Property[PropIndex]->prop_ReadHandler(ThingId, PropId, &GetValue);
                    //      EncodePayload(valueType, &GetValue, DataOut);
                    //  }
                    //  else
                    //  {
                    //      cout << "prop_ReadHandler:NULL"<<std::endl;
                    //  }
                }
            }
        }
    }
    break;
    case ActionInteraction:
    {
    }
    break;
    case EventInteraction:
    {
        Thing *ThingReq;
        std::string value_string;
        ThingInteractionValueType valueType;
        ThingInteractionValue GetValue;
        cout << "ThingReqID:" << ThingId << ":" << Interaction_Name << std::endl;
        ThingReq = GetExposedThing(ThingId);
        // cout << ThingReq->id<< std::endl;
        if (ThingReq != NULL)
        {
            cout << ThingReq->eventIndex << std::endl;
            for (int PropIndex = 0; PropIndex < ThingReq->eventIndex; PropIndex++) //
            {
                std::string KeyName;
                KeyName = ThingReq->Thing_Event[PropIndex]->title;
                cout << "KeyName:" << KeyName << std::endl;
                cout << "PropId:" << Interaction_Name << std::endl;
                if (KeyName == Interaction_Name)
                {
                    ThingReq->Thing_Event[PropIndex]->subscribed = true;
                    // JsonMeta->set("value", "Off");
                    //  if (ThingReq->Thing_Property[PropIndex]->prop_ReadHandler != NULL)
                    //  {
                    //      valueType = ThingReq->Thing_Property[PropIndex]->prop_ReadHandler(ThingId, PropId, &GetValue);
                    //      EncodePayload(valueType, &GetValue, DataOut);
                    //  }
                    //  else
                    //  {
                    //      cout << "prop_ReadHandler:NULL"<<std::endl;
                    //  }
                }
            }
        }
    }
    break;
    }
}