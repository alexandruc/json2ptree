#include <iostream>
#include <fstream>
#include <string>
#include "../parser.h"
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>

void display(const int depth, const boost::property_tree::ptree& tree) {
    BOOST_FOREACH( boost::property_tree::ptree::value_type const&v, tree.get_child("") ) {
        boost::property_tree::ptree subtree = v.second;
        std::string nodestr = tree.get<std::string>(v.first);

        // print current node
        std::cout << std::string("").assign(depth*2,' ') << "* ";
        std::cout << v.first;
        if ( nodestr.length() > 0 )
            std::cout << "=\"" << tree.get<std::string>(v.first) << "\"";
        std::cout << std::endl;

        // recursive go down the hierarchy
        display(depth+1,subtree);
    }
}

void parseWithCustomParser(const std::string& json){
    json::Parser jparser;
    boost::property_tree::ptree pt;
    try{
        jparser.parseJson(json, pt);

        std::cout << "Property tree - custom parsed:" << std::endl;
        display(0, pt);
    }
    catch(boost::property_tree::ptree_bad_data ex){
        std::cout << ex.what() << std::endl;
    }
    catch(boost::property_tree::ptree_bad_path ex){
        std::cout << ex.what() << std::endl;
    }
    catch(boost::property_tree::ptree_error ex){
        std::cout << ex.what() << std::endl;
    }
}

void parseWithBoostParser(const std::string& json_file){
    boost::property_tree::ptree pt;
    try{
        boost::property_tree::json_parser::read_json(json_file, pt);

        std::cout << "Property tree - boost parsed:" << std::endl;
        display(0, pt);
    }
    catch(boost::property_tree::ptree_bad_data ex){
        std::cout << ex.what() << std::endl;
    }
    catch(boost::property_tree::ptree_bad_path ex){
        std::cout << ex.what() << std::endl;
    }
    catch(boost::property_tree::ptree_error ex){
        std::cout << ex.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {

    if (argc != 2)
    {
        std::cerr << "Usage: test_json2ptree <json_file>" << std::endl;
        return 1;
    }


    std::string test_file(argv[1]);

    std::string json;
    std::ifstream ifs;
    ifs.open(test_file.c_str(), std::ifstream::in);
    if(ifs.is_open()){
        std::string line;
        while(std::getline(ifs, line)){
            json += line;
        }
        ifs.close();
    }
    else{
        std::cout << "file " << test_file << " not found" << std::endl;
        return 1;
    }

    std::cout << "JSON string: " << json << std::endl;

    parseWithCustomParser(json);
    parseWithBoostParser(test_file);

}
