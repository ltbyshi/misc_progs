#include <iostream>
using namespace std;
#include <string>
#include <stdlib.h>

#include <argparse.h>

int main(int argc, char** argv)
{
    ArgumentParser parser;
    parser.description("ArgumentParser test").progname("argparse");
    
    parser.add_argument("format", "str").short_arg("f").default_value("text")
        .add_choice("html").add_choice("xml").add_choice("text");
    parser.add_argument("alpha", "float").short_arg("a").
        required(false).default_value(3).help("Alpha value");
    parser.add_argument("level", "int").short_arg("l")
        .required(false).default_value(3).help("Grey levels");
    parser.add_argument("name", "str").short_arg("u")
        .required(false).default_value("root").help("Username of the system");
    parser.add_argument("password", "str").short_arg("p")
        .required(false).default_value("pass").help("Password of the system");
    parser.add_positional("dbname", "str")
        .nargs_max(ArgumentParser::NARGS_INF).help("Database name to work on");
    try {
        AnyType strtype("hello");
        cout << "string: " <<  strtype << endl;
    parser.parse_args(argc, argv);
    //parser.help(argv[0]);
    parser.summary();
    
    if(parser["help"].given())
    {
        parser.help();
        return 1;
    }
    if(parser["name"])
        cout << "name: " << parser["name"].value<string>() << endl;
    if(parser["password"])
        cout << "password: " << parser["password"].value<string>() << endl;
    
    } catch(const ArgumentException& e) {
        cerr << e.what() << endl;
        abort();
    }
    
    return 0;
}

