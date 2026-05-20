#include "menu.h"      
#include "ftxui/component/component.hpp"       
#include "ftxui/component/component_base.hpp"  
#include "ftxui/component/component_options.hpp"  
#include "ftxui/util/ref.hpp"
#include "ftxui/component/app.hpp"
#include <stdexcept>
Menu* Menu::Instance= nullptr;
int Menu::State= 1;



void Menu::connect(){
    using namespace ftxui;
    std::string port;
    std::string password;
    std::string hostname;
    std::string username;

    InputOption password_option;
    password_option.password = true;
    Component input_password = Input(&password, "password", password_option);

    Component input_host= Input(&hostname,"localhost");
    Component input_user= Input(&username,"postgres");
    Component input_port= Input(&port, "5432");

    auto component = Container::Vertical({
    input_host,
    input_port,
    input_user,
    input_password
    });

    auto app = App::TerminalOutput();
    app.Loop(component);
}

void Menu::run(){
    switch (Menu::State) {
    case 0: return;break;
    case 1: connect();break;
    case 2: /*freeform()*/;break;
    default: throw std::runtime_error("Invalid state!"); //TODO: Use custome exception
    }
}