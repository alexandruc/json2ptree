#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

namespace json {

static const std::string pathSeparator = ".";
static const std::string arrayElementKeyName = "element";

void Parser::parseJson(const std::string& data, boost::property_tree::ptree& out){
    json_object * jobj = json_tokener_parse(data.c_str());
    if(jobj){
        std::string path = "";
        parseJsonObject(jobj, out, path);
    }
    else{
        std::cout <<"json object is NULL - bad JSON format" << std::endl;
    }
}

void Parser::parseJsonValue(json_object *jobj, boost::property_tree::ptree& out, std::string path){
    enum json_type type;
    FLOG("path: " << path);
    type = json_object_get_type(jobj);
    switch (type) {
    case json_type_boolean: {
        FLOG("type: json_type_boolean");
        out.put(path, json_object_get_boolean(jobj) ? true: false);
        FLOG("\t value: " << out.get<bool>(path));
        break;
    }
    case json_type_double: {
        FLOG("type: json_type_double");
        out.put(path, json_object_get_double(jobj));
        FLOG("\t value: " << out.get<double>(path));
        break;
    }
    case json_type_int: {
        FLOG("type: json_type_int");
        out.put(path, json_object_get_int(jobj));
        FLOG("\t value: " << out.get<int>(path));
        break;
    }
    case json_type_string: {
        FLOG("type: json_type_string");
        const char* str = json_object_get_string(jobj);
        out.put(path, str);
        FLOG("\t value: " << str);
        break;
    }
    default:break;
    }
}


void Parser::parseJsonArray( json_object *jobj, char *key, boost::property_tree::ptree& out, std::string path) {
    enum json_type type;

    json_object *jarray = jobj;
    if(key) {
        //getting the array if it is a key value pair
        if(!json_object_object_get_ex(jobj, key, &jarray)){
            FLOG("Error getting array for key: " << key);
            return;
        }
    }
    FLOG("path: " << path);

    //getting the length of the array
    int arraylen = json_object_array_length(jarray);
    FLOG("array Length: " << arraylen);
    int i;
    json_object * jvalue;

    for (i=0; i< arraylen; i++){
        //getting the array element at position i
        jvalue = json_object_array_get_idx(jarray, i);
        type = json_object_get_type(jvalue);
        if (type == json_type_array) {
            parseJsonArray(jvalue, NULL, out, path);
        }
        else if (type != json_type_object) {
            FLOG("value[" << i <<"]:");
            std::string elem;
            buildArrayElem(elem, i);
            path+= pathSeparator + elem;
            parseJsonValue(jvalue, out, path);
            goBack1Level(path);
        }
        else {
            std::string elem;
            buildArrayElem(elem, i);
            path+= pathSeparator + elem;
            parseJsonObject(jvalue, out, path);
            goBack1Level(path);
        }
    }
}

void Parser::parseJsonObject(json_object * jobj, boost::property_tree::ptree& out, std::string path) {
    enum json_type type;
    json_object_object_foreach(jobj, key, val) {
        FLOG("key: "<< key);
        if(!path.empty()){
            path += pathSeparator;
        }
        path += std::string(key);
        type = json_object_get_type(val);

        FLOG("path: " << path);
        switch (type) {
        case json_type_boolean:
        case json_type_double:
        case json_type_int:
        case json_type_string: {
            parseJsonValue(val, out, path);

            goBack1Level(path);
            break;
        }
        case json_type_object: {
            FLOG("type: json_type_object");
            if(!json_object_object_get_ex(jobj, key, &jobj)){
                FLOG("Error getting object from key: " << key);
                return;
            }
            parseJsonObject(jobj, out, path);
            break;
        }
        case json_type_array: {
            FLOG("type: json_type_array ");
            parseJsonArray(jobj, key, out, path);

            goBack1Level(path);
            break;
        }
        }
    }
}

void Parser::goBack1Level(std::string& path) const
{
    //go back 1 level in path
    size_t pos = path.find_last_of(pathSeparator[0]);
    if(pos != std::string::npos){
        path.erase(path.begin() + pos, path.end());
    }
    else{
        path.clear();
    }
}

void Parser::buildArrayElem(std::string& elem, int elemNum) const
{
    //build new array elem
    elem = arrayElementKeyName;
    char strNum[10];
    sprintf(strNum, "%d", elemNum);
    elem += strNum;
}

} //namespace json
