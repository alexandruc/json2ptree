#ifndef PARSER_H
#define PARSER_H

#include <json-c/json.h>
#include <string>
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/lexical_cast.hpp>

#define FLOG(x) {std::cout << __FILE__ << ":" << __FUNCTION__ << "(" << __LINE__ << "): " << x << std::endl;}

namespace json {

/**
 * @brief Parser for JSON data
 */
class Parser {
public:
    /**
     * @brief parseJson
     * @param data A string containing the JSON data
     * @param out Boost property tree containing the JSON data
     */
    void parseJson(const std::string& data, boost::property_tree::ptree& out);

private:
    /**
     * @brief Handles primitive JSON types
     * @param jobj JSON Object to parse
     * @param out Property tree to fill
     * @param path Path in the property tree where the value will be stored
     */
    void parseJsonValue(json_object *jobj, boost::property_tree::ptree& out, std::string path);
    /**
     * @brief Handles JSON arrays
     * @param jobj JSON object to parse
     * @param key Key of the array object
     * @param out Property tree to fill
     * @param path Path in the property tree where the value will be stored
     */
    void parseJsonArray( json_object *jobj, char *key, boost::property_tree::ptree& out, std::string path);
    /**
     * @brief Handles JSON objects
     * @param jobj JSON Object to parse
     * @param out Property tree to fill
     * @param path Path in the property tree where the value will be stored
     */
    void parseJsonObject(json_object * jobj, boost::property_tree::ptree& out, std::string path);

    //helper functions - duplicate code
    void goBack1Level(std::string& path) const;
    void buildArrayElem(std::string& elem, int elemNum) const;

};

} //namespace json

#endif // PARSER_H
