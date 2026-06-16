#include <fstream>
#include <iostream>
#include <string>
#include "cJSON.h"
#include "WoTProperty.h"
#include "WoTServient.h"
#include "WoTTd.h"
#include "WoTThing.h"

// using nlohmann::json;
// using nlohmann::json_uri;
// using nlohmann::json_schema::json_validator;

void ThingDescription::TdLoader(const std::string uri, cJSON *schema)
{
    std::string filename = "./" + uri;
    std::ifstream lf(filename);
    // if (!lf.good())
    //     throw std::invalid_argument("could not open " + uri + " tried with " + filename);
    // try
    // {
    //     lf >> schema;
    // }
    // catch (const std::exception &e)
    // {
    //     throw e;
    // }
}

bool ThingDescription::Validate(cJSON *Td)
{
    bool result = false;
    /* json-parse the schema */

    // std::ifstream file_schema(
    //     "/02_Cpp_NoScript/WoT/Schema/tm-json-schema-validation.json");
    // if (file_schema.is_open())
    // {
    //     // std::cout <<"file schema: "<< file_schema.rdbuf()<<std::endl;
    //     wot_td_schema = cJSON_Parse(file_schema);
        
    //     json_validator validator(
    //         TdLoader, nlohmann::json_schema::default_string_format_check);

    //     try
    //     {
    //         validator.set_root_schema(wot_td_schema); // insert root-schema
    //     }
    //     catch (const std::exception &e)
    //     {
    //         std::cerr << "Validation of schema failed, here is why: "
    //                   << e.what() << "\n";
    //         result = false;
    //     }
    //     /* json-parse the people - API of 1.0.0, default throwing error handler
    //      */

    //     // std::cout << "About to validate this Td:\n"
    //     //           << std::setw(2) << Td << std::endl;
    //     try
    //     {
    //         validator.validate(Td); // validate the document - uses the default
    //                                 // throwing error-handler
    //         std::cout << "Validation succeeded\n";
    //         result = true;
    //     }
    //     catch (const std::exception &e)
    //     {
    //         std::cerr << "Validation failed, here is why: " << e.what() << "\n";
    //     }
    // }
    // else
    // {
    //     std::cout << "file schema: not found";
    // }
    result = true;
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

    // try
    // {
        wot_td = cJSON_Parse(Td.c_str());
        // std::cout << "Received TD" << Td << std::endl;
    // }
    // catch (...)
    // {
    //     std::cerr << "parse error at byte " << std::endl;
    // }

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

void ThingDescription::ExtractForm(cJSON * FormItemFragment, WoTForms *NewForm)
{
    cJSON *href = cJSON_GetObjectItem(FormItemFragment, "href");
    if (href)
    {
        std::cout << "Extracting href: " << href->valuestring << "\n";
        NewForm->href = href->valuestring;
    }
    else
    {
        NewForm->href = "";
    }
    cJSON *type = cJSON_GetObjectItem(FormItemFragment, "type");
    if (type)
    {
        NewForm->type = type->valuestring;
    }
    else
    {
        NewForm->type = "";
    }
    cJSON *rel = cJSON_GetObjectItem(FormItemFragment, "rel");
    if (rel)
    {
        NewForm->rel = rel->valuestring;
    }
    else
    {
        NewForm->rel = "";
    }
    cJSON *anchor = cJSON_GetObjectItem(FormItemFragment, "anchor");
    if (anchor)
    {
        NewForm->anchor = anchor->valuestring;
    }
    else
    {
        NewForm->anchor = "";
    }
    cJSON *sizes = cJSON_GetObjectItem(FormItemFragment, "sizes");
    if (sizes)
    {
        NewForm->sizes = sizes->valuestring;
    }
    else
    {
        NewForm->sizes = "";
    }
    cJSON *hreflang = cJSON_GetObjectItem(FormItemFragment, "hreflang");
    if (hreflang)
    {
        NewForm->hreflang = hreflang->valuestring;
    }
    else
    {
        NewForm->hreflang = "";
    }
}

ThingProperty *
ThingDescription::build_property(std::string propname, cJSON *propItemFragment)
{
    uint8_t fromParamCnt = 0;
    ThingProperty *ThingPropertyNew = new ThingProperty();

    cJSON *type = cJSON_GetObjectItem(propItemFragment, "type");
    if (type && strcmp(type->valuestring, "object") == 0)
    {
        cJSON *properties = cJSON_GetObjectItem(propItemFragment, "properties");
        uint8_t propParamCnt = 0;

        for (cJSON *item = properties->child; item != NULL; item = item->next)
        {
            cJSON *itemType = cJSON_GetObjectItem(item, "type");
            cJSON *readOnly = cJSON_GetObjectItem(item, "readOnly");
            std::string PropertyValType = itemType ? itemType->valuestring : "";
            std::cout << item->string << ":" << PropertyValType << ":" << (readOnly ? readOnly->valueint : 0) << "\n";
            ThingPropertyNew->Initialize(item->string, propname, PropertyValType, readOnly ? readOnly->valueint : 0);
            propParamCnt++;
        }
    }
    else
    {
        cJSON *title = cJSON_GetObjectItem(propItemFragment, "title");
        cJSON *readOnly = cJSON_GetObjectItem(propItemFragment, "readOnly");
        std::cout << (title ? title->valuestring : "") << ":" << (type ? type->valuestring : "") << ":" << (readOnly ? readOnly->valueint : 0) << "\n";
        std::string PropertyValType = type ? type->valuestring : "";
        ThingPropertyNew->Initialize(title ? title->valuestring : "", PropertyValType, readOnly ? readOnly->valueint : 0);
        cJSON *forms = cJSON_GetObjectItem(propItemFragment, "forms");
        if (forms)
        {
            ExtractForm(forms, &ThingPropertyNew->form[fromParamCnt]);
            fromParamCnt++;
        }
    }
    return (ThingPropertyNew);
}

ThingAction *
ThingDescription::build_action(std::string actnname, cJSON * actnItemFragment)
{
    cJSON *actnObjItem;
    uint8_t actnInParamCnt = 0;
    uint8_t actnOutParamCnt = 0;

    ThingAction *ThingActionNew = new ThingAction();
    if (cJSON_GetObjectItem(actnItemFragment, "input"))
    {
        if (cJSON_GetObjectItem(actnItemFragment, "input") && strcmp(cJSON_GetObjectItem(cJSON_GetObjectItem(actnItemFragment, "input"), "type")->valuestring, "object") == 0)
        {
            actnObjItem = cJSON_GetObjectItem(cJSON_GetObjectItem(actnItemFragment, "input"), "properties");
            for (cJSON *item = actnObjItem->child; item != NULL; item = item->next)
            {
                cJSON *itemType = cJSON_GetObjectItem(item, "type");
                cJSON *readOnly = cJSON_GetObjectItem(item, "readOnly");
                std::string ActionValType = itemType ? itemType->valuestring : "";
                std::cout << item->string << ":" << ActionValType << ":" << (readOnly ? readOnly->valueint : 0) << "\n";
                ThingActionNew->InitializeInputs(actnInParamCnt, actnname,
                                                 ActionValType, 4);
                actnInParamCnt++;
            }
        }
        else if (cJSON_GetObjectItem(cJSON_GetObjectItem(actnItemFragment, "input"), "type"))
        {
            cJSON *input = cJSON_GetObjectItem(actnItemFragment, "input");
            cJSON *title = cJSON_GetObjectItem(input, "title");
            cJSON *type = cJSON_GetObjectItem(input, "type");
            cJSON *readOnly = cJSON_GetObjectItem(input, "readOnly");
            std::cout << (title ? title->valuestring : "") << ":"
                      << (type ? type->valuestring : "") << ":"
                      << (readOnly ? readOnly->valueint : 0) << "\n";
            std::string ActionValType = type ? type->valuestring : "";
            ThingActionNew->InitializeInputs(actnInParamCnt,
                                             title ? title->valuestring : "",
                                             ActionValType, 4);
            actnInParamCnt = 1;
        }
    }
    else
    {
        actnInParamCnt = 0;
    }
    if (cJSON_GetObjectItem(actnItemFragment, "output"))
    {
        if (cJSON_GetObjectItem(cJSON_GetObjectItem(actnItemFragment, "output"), "type") && strcmp(cJSON_GetObjectItem(cJSON_GetObjectItem(actnItemFragment, "output"), "type")->valuestring, "object") == 0)
        {
            actnObjItem = cJSON_GetObjectItem(cJSON_GetObjectItem(actnItemFragment, "output"), "properties");
            for (cJSON *item = actnObjItem->child; item != NULL; item = item->next)
            {
                cJSON *itemType = cJSON_GetObjectItem(item, "type");
                cJSON *readOnly = cJSON_GetObjectItem(item, "readOnly");
                std::string ActionValType = itemType ? itemType->valuestring : "";
                std::cout << item->string << ":" << ActionValType << ":" << (readOnly ? readOnly->valueint : 0) << "\n";
                ThingActionNew->InitializeOutputs(actnOutParamCnt, actnname,
                                                  ActionValType, 4);
                actnOutParamCnt++;
            }
        }
        else if (cJSON_GetObjectItem(cJSON_GetObjectItem(actnItemFragment, "output"), "type"))
        {
            cJSON *output = cJSON_GetObjectItem(actnItemFragment, "output");
            cJSON *title = cJSON_GetObjectItem(output, "title");
            cJSON *type = cJSON_GetObjectItem(output, "type");
            cJSON *readOnly = cJSON_GetObjectItem(output, "readOnly");
            std::cout << (title ? title->valuestring : "") << ":"
                      << (type ? type->valuestring : "") << ":"
                      << (readOnly ? readOnly->valueint : 0) << "\n";
            std::string ActionValType = type ? type->valuestring : "";
            ThingActionNew->InitializeOutputs(
                actnOutParamCnt, title ? title->valuestring : "",
                ActionValType, readOnly ? readOnly->valueint : 0);
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
ThingDescription::build_event(std::string evntname, cJSON *evntItemFragment)
{
    cJSON *evntObjItem;
    uint8_t evntInParamCnt = 0;
    uint8_t evntOutParamCnt = 0;

    ThingEvent *ThingEventNew = new ThingEvent();

    if (cJSON_GetObjectItem(cJSON_GetObjectItem(evntItemFragment, "input"), "type") && strcmp(cJSON_GetObjectItem(cJSON_GetObjectItem(evntItemFragment, "input"), "type")->valuestring, "object") == 0)
    {
        evntObjItem = cJSON_GetObjectItem(cJSON_GetObjectItem(evntItemFragment, "input"), "properties");
        for (cJSON *item = evntObjItem->child; item != NULL; item = item->next)
        {
            cJSON *itemType = cJSON_GetObjectItem(item, "type");
            cJSON *readOnly = cJSON_GetObjectItem(item, "readOnly");
            std::string ActionValType = itemType ? itemType->valuestring : "";
            std::cout << item->string << ":" << ActionValType << ":" << (readOnly ? readOnly->valueint : 0) << "\n";
            ThingEventNew->InitializeInputs(evntInParamCnt, item->string,
                                            ActionValType, readOnly ? readOnly->valueint : 0);
            evntInParamCnt++;
        }
    }
    else if (cJSON_GetObjectItem(cJSON_GetObjectItem(evntItemFragment, "input"), "type"))
    {
        cJSON *input = cJSON_GetObjectItem(evntItemFragment, "input");
        cJSON *title = cJSON_GetObjectItem(input, "title");
        cJSON *type = cJSON_GetObjectItem(input, "type");
        cJSON *readOnly = cJSON_GetObjectItem(input, "readOnly");
        std::cout << (title ? title->valuestring : "") << ":"
                  << (type ? type->valuestring : "") << ":"
                  << (readOnly ? readOnly->valueint : 0) << "\n";
        std::string ActionValType = type ? type->valuestring : "";
        ThingEventNew->InitializeInputs(evntInParamCnt,
                                        title ? title->valuestring : "",
                                        ActionValType, readOnly ? readOnly->valueint : 0);
        evntInParamCnt = 1;
    }

    ThingEventNew->Initialize(evntname, evntInParamCnt, evntOutParamCnt);
    return (ThingEventNew);
}

ExposedThing *
ThingDescription::build_exposedThing(void)
{
    ExposedThing *ExposedThingTemp = new ExposedThing();
    // ExposedThingTemp->Initialize(0, cJSON_GetObjectItem(wot_td, "id"), cJSON_GetObjectItem(wot_td, "title"),
    //                              cJSON_GetObjectItem(wot_td, "@type"));

    // for (const auto &item : cJSON_GetObjectItem(wot_td, "properties")->child)
    // {
    //     std::cout << item->string << "\n";
    //     ExposedThingTemp->addProperty(
    //         build_property(item->string, item));
    // }
    // for (const auto &item : cJSON_GetObjectItem(wot_td, "actions")->child)
    // {
    //     std::cout << item->string << "\n";
    //     ExposedThingTemp->addAction(build_action(item->string, item));
    // }
    // for (const auto &item : cJSON_GetObjectItem(wot_td, "events")->child)
    // {
    //     std::cout << item->string << "\n";
    //     ExposedThingTemp->addEvent(build_event(item->string, item));
    // }
    return (ExposedThingTemp);
}

ConsumedThing *
ThingDescription::build_consumedThing(void)
{
    ConsumedThing *ConsumedThingTemp = new ConsumedThing();
    // ConsumedThingTemp->Initialize(0, cJSON_GetObjectItem(wot_td, "id"), cJSON_GetObjectItem(wot_td, "title"),
    //                               cJSON_GetObjectItem(wot_td, "@type"));
    // // ConsumedThingTemp->Servient = servientIn;
    // for (const auto &item : cJSON_GetObjectItem(wot_td, "properties")->child)
    // {
    //     std::cout << item->string << "\n";
    //     uint8_t ProIndex = ConsumedThingTemp->addProperty(build_property(item->string, item));
    // }
    // for (const auto &item : cJSON_GetObjectItem(wot_td, "actions")->child)
    // {
    //     std::cout << item->string << "\n";
    //     ConsumedThingTemp->addAction(build_action(item->string, item));
    // }
    // for (const auto &item : cJSON_GetObjectItem(wot_td, "events")->child)
    // {
    //     std::cout << item->string << "\n";
    //     ConsumedThingTemp->addEvent(build_event(item->string, item));
    // }
    return (ConsumedThingTemp);
}
