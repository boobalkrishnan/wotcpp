#include "nlohmann/json-schema.hpp"
#include <fstream>
#include <iostream>
#include <string>

#include "WoTProperty.h"
#include "WoTServient.h"
#include "WoTTd.h"
#include "WoTThing.h"

using nlohmann::json;
using nlohmann::json_uri;
using nlohmann::json_schema::json_validator;

void ThingDescription::TdLoader(const json_uri &uri, json &schema)
{
    std::string filename = "./" + uri.path();
    std::ifstream lf(filename);
    if (!lf.good())
        throw std::invalid_argument("could not open " + uri.url() + " tried with " + filename);
    try
    {
        lf >> schema;
    }
    catch (const std::exception &e)
    {
        throw e;
    }
}

bool ThingDescription::Validate(json Td)
{
    bool result = false;
    /* json-parse the schema */

    std::ifstream file_schema(
        "/02_Cpp_NoScript/WoT/Schema/tm-json-schema-validation.json");
    if (file_schema.is_open())
    {
        // std::cout <<"file schema: "<< file_schema.rdbuf()<<std::endl;
        wot_td_schema = json::parse(file_schema);
        json_validator validator(
            TdLoader, nlohmann::json_schema::default_string_format_check);

        try
        {
            validator.set_root_schema(wot_td_schema); // insert root-schema
        }
        catch (const std::exception &e)
        {
            std::cerr << "Validation of schema failed, here is why: "
                      << e.what() << "\n";
            result = false;
        }
        /* json-parse the people - API of 1.0.0, default throwing error handler
         */

        // std::cout << "About to validate this Td:\n"
        //           << std::setw(2) << Td << std::endl;
        try
        {
            validator.validate(Td); // validate the document - uses the default
                                    // throwing error-handler
            std::cout << "Validation succeeded\n";
            result = true;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Validation failed, here is why: " << e.what() << "\n";
        }
    }
    else
    {
        std::cout << "file schema: not found";
    }

    return result;
}

// Thing ThingDescription::BuildThing(void)
// {
//     if (td_valid)
//     {
//         //Thing mything(wot_td);
//     }
// }

ThingDescription::ThingDescription(std::string Td)
{
    /* Convert to JSON */

    try
    {
        wot_td = json::parse(Td.c_str());
        // std::cout << "Received TD" << Td << std::endl;
    }
    catch (json::parse_error &ex)
    {
        std::cerr << "parse error at byte " << ex.byte << std::endl;
    }

    td_valid = Validate(wot_td);

    // for (const auto& item : wot_td.items())
    // {
    //     std::cout << item.key() << "\n";
    //     for (const auto& val : item.value().items())
    //     {
    //         std::cout << "  " << val.key() << ": " << val.value() << "\n";
    //     }
    // }
}

WoTForms
ThingDescription::ExtractForm(json FormItemFragment)
{
    WoTForms NewForm;
    if (FormItemFragment.contains("href"))
    {
        std::cout << "Extracting href: " << FormItemFragment["href"] << "\n";
        NewForm.href = FormItemFragment["href"];
    }
    else
    {
        NewForm.href = "";
    }
    if (FormItemFragment.contains("type"))
    {
        NewForm.type = FormItemFragment["type"];
    }
    else
    {
        NewForm.type = "";
    }
    if (FormItemFragment.contains("rel"))
    {
        NewForm.rel = FormItemFragment["rel"];
    }
    else
    {
        NewForm.rel = "";
    }
    if (FormItemFragment.contains("anchor"))
    {
        NewForm.anchor = FormItemFragment["anchor"];
    }
    else
    {
        NewForm.anchor = "";
    }
    if (FormItemFragment.contains("sizes"))
    {
        NewForm.sizes = FormItemFragment["sizes"];
    }
    else
    {
        NewForm.sizes = "";
    }
    if (FormItemFragment.contains("hreflang"))
    {
        NewForm.hreflang = FormItemFragment["hreflang"];
    }
    else
    {
        NewForm.hreflang = "";
    }
    return NewForm;
}

ThingProperty *
ThingDescription::build_property(std::string propname, json propItemFragment)
{
    ThingProperty *ThingPropertyNew = new ThingProperty();

    if (propItemFragment["type"] == "object")
    {
        json propObjItem;
        uint8_t propParamCnt = 0;

        propObjItem = propItemFragment["properties"];
        for (const auto &item : propObjItem.items())
        {
            std::string PropertyValType = item.value()["type"];
            std::cout << item.key() << ":" << item.value()["type"] << ":" << item.value()["readOnly"] << "\n";
            ThingPropertyNew->Initialize(item.key(), propname, PropertyValType, item.value()["readOnly"]);
            propParamCnt++;
        }
    }
    else
    {
        json formObjItem;
        uint8_t fromParamCnt = 0;
        std::cout << propItemFragment["title"] << ":" << propItemFragment["type"] << ":" << propItemFragment["readOnly"] << "\n";
        std::string PropertyValType = propItemFragment["type"];
        ThingPropertyNew->Initialize(propItemFragment["title"], PropertyValType, propItemFragment["readOnly"]);
        formObjItem = propItemFragment["forms"];
        for (const auto &item : formObjItem.items())
        {
            ThingPropertyNew->form[fromParamCnt] = ExtractForm(item.value());
            fromParamCnt++;
        }
    }
    return (ThingPropertyNew);
}

ThingAction *
ThingDescription::build_action(std::string actnname, json actnItemFragment)
{
    json actnObjItem;
    uint8_t actnInParamCnt = 0;
    uint8_t actnOutParamCnt = 0;

    ThingAction *ThingActionNew = new ThingAction();
    if (actnItemFragment.contains("input"))
    {
        if (actnItemFragment["input"]["type"] == "object")
        {
            actnObjItem = actnItemFragment["input"]["properties"];
            for (const auto &item : actnObjItem.items())
            {
                std::string ActionValType = item.value()["type"];
                std::cout << item.key() << ":" << item.value()["type"] << ":"
                          << item.value()["readOnly"] << "\n";
                ThingActionNew->InitializeInputs(actnInParamCnt, actnname,
                                                 ActionValType, 4);
                actnInParamCnt++;
            }
        }
        else if (actnItemFragment["input"]["type"] != NULL)
        {
            std::cout << actnItemFragment["input"]["title"] << ":"
                      << actnItemFragment["input"]["type"] << ":"
                      << actnItemFragment["input"]["readOnly"] << "\n";
            std::string ActionValType = actnItemFragment["input"]["type"];
            ThingActionNew->InitializeInputs(actnInParamCnt,
                                             actnItemFragment["input"]["title"],
                                             ActionValType, 4);
            actnInParamCnt = 1;
        }
    }
    else
    {
        actnInParamCnt = 0;
    }
    if (actnItemFragment.contains("output"))
    {
        if (actnItemFragment["output"]["type"] == "object")
        {
            actnObjItem = actnItemFragment["output"]["properties"];
            for (const auto &item : actnObjItem.items())
            {
                std::string ActionValType = item.value()["type"];
                std::cout << item.key() << ":" << item.value()["type"] << ":"
                          << item.value()["readOnly"] << "\n";
                ThingActionNew->InitializeOutputs(actnOutParamCnt, actnname,
                                                  ActionValType, 4);
                actnOutParamCnt++;
            }
        }
        else if (actnItemFragment["output"]["type"] != NULL)
        {
            std::cout << actnItemFragment["output"]["title"] << ":"
                      << actnItemFragment["output"]["type"] << ":"
                      << actnItemFragment["output"]["readOnly"] << "\n";
            std::string ActionValType = actnItemFragment["output"]["type"];
            ThingActionNew->InitializeOutputs(
                actnOutParamCnt, actnItemFragment["output"]["title"],
                ActionValType, actnItemFragment["output"]["readOnly"]);
            actnOutParamCnt = 1;
        }
    }
    else
    {
        actnOutParamCnt = 0;
    }
    ThingActionNew->Initialize(actnname, actnInParamCnt, actnOutParamCnt);
    return (ThingActionNew);
}

ThingEvent *
ThingDescription::build_event(std::string evntname, json evntItemFragment)
{
    json evntObjItem;
    uint8_t evntInParamCnt = 0;
    uint8_t evntOutParamCnt = 0;

    ThingEvent *ThingEventNew = new ThingEvent();

    if (evntItemFragment["input"]["type"] == "object")
    {
        evntObjItem = evntItemFragment["input"]["properties"];
        for (const auto &item : evntObjItem.items())
        {
            std::string ActionValType = item.value()["type"];
            std::cout << item.key() << ":" << item.value()["type"] << ":"
                      << item.value()["readOnly"] << "\n";
            ThingEventNew->InitializeInputs(evntInParamCnt, item.key(),
                                            ActionValType, 4);
            evntInParamCnt++;
        }
    }
    else if (evntItemFragment["input"]["type"] != NULL)
    {
        std::cout << evntItemFragment["input"]["title"] << ":"
                  << evntItemFragment["input"]["type"] << ":"
                  << evntItemFragment["input"]["readOnly"] << "\n";
        std::string ActionValType = evntItemFragment["input"]["type"];
        ThingEventNew->InitializeInputs(evntInParamCnt,
                                        evntItemFragment["input"]["title"],
                                        ActionValType, 4);
        evntInParamCnt = 1;
    }

    ThingEventNew->Initialize(evntname, evntInParamCnt, evntOutParamCnt);
    return (ThingEventNew);
}

ExposedThing *
ThingDescription::build_exposedThing(void)
{
    ExposedThing *ExposedThingTemp = new ExposedThing();
    ExposedThingTemp->Initialize(0, wot_td["id"], wot_td["title"],
                                 wot_td["@type"]);

    for (const auto &item : wot_td["properties"].items())
    {
        std::cout << item.key() << "\n";
        ExposedThingTemp->addProperty(
            build_property(item.key(), item.value()));
    }
    for (const auto &item : wot_td["actions"].items())
    {
        std::cout << item.key() << "\n";
        ExposedThingTemp->addAction(build_action(item.key(), item.value()));
    }
    for (const auto &item : wot_td["events"].items())
    {
        std::cout << item.key() << "\n";
        ExposedThingTemp->addEvent(build_event(item.key(), item.value()));
    }
    return (ExposedThingTemp);
}

ConsumedThing *
ThingDescription::build_consumedThing(void)
{
    ConsumedThing *ConsumedThingTemp = new ConsumedThing();
    ConsumedThingTemp->Initialize(0, wot_td["id"], wot_td["title"],
                                  wot_td["@type"]);
    // ConsumedThingTemp->Servient = servientIn;
    for (const auto &item : wot_td["properties"].items())
    {
        std::cout << item.key() << "\n";
        uint8_t ProIndex = ConsumedThingTemp->addProperty(build_property(item.key(), item.value()));
    }
    for (const auto &item : wot_td["actions"].items())
    {
        std::cout << item.key() << "\n";
        ConsumedThingTemp->addAction(build_action(item.key(), item.value()));
    }
    for (const auto &item : wot_td["events"].items())
    {
        std::cout << item.key() << "\n";
        ConsumedThingTemp->addEvent(build_event(item.key(), item.value()));
    }
    return (ConsumedThingTemp);
}
