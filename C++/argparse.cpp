#include <iostream>
#include <string>
#include <stdlib.h>
#include "argparse.h"

int main(int argc, char** argv)
{
    ArgumentParser parser("ArgumentParser test");
    parser.add_argument("help").short_arg("h").flag(true);
    parser.add_argument("name").short_arg("u").required(true);
    parser.add_argument("password").short_arg("p").required(true);
    parser.parse_args();
    
    if(parser["help"])
    {
        parser.help();
        return 1;
    }
    if(parser["name"])
        std::cout << "name: " << parser["name"].value<std::string>();
    if(parser["password"])
        std::cout << "password: " << parser["password"].value<std::string>();
    
    return 0;
}

