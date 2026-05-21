#include "menu.h"      
#include "ftxui/component/component.hpp"       
#include "ftxui/component/component_base.hpp"  
#include "ftxui/component/component_options.hpp"  
#include "ftxui/dom/elements.hpp"
#include "ftxui/util/ref.hpp"
#include "ftxui/component/app.hpp"
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <stdexcept>
Menu* Menu::Instance= nullptr;
int Menu::State= 1;

Menu* Menu::get_instance(){
    if (Menu::Instance == nullptr) {
        Instance= new Menu;
    }
    return Instance;
}
void Menu::connect(){
    using namespace ftxui;
    auto app = App::TerminalOutput();
    std::string port;
    std::string password;
    std::string hostname;
    std::string username;
    std::string dbname;

    InputOption password_option;
    password_option.password = true;
    Component input_password = Input(&password, "password", password_option);

    Component input_host= Input(&hostname,"localhost");
    Component input_user= Input(&username,"postgres");
    Component input_port= Input(&port, "5432");
    Component input_dbname= Input(&dbname, "postgres");

    Component submit_bttn= Button("Submit", [&]{
        std::string con_arg=
        "host="+hostname+
        " port="+port+
        " user="+username+
        " password="+password+
        " dbname="+dbname;
        conn= new pqxx::connection(con_arg);
        Menu::State=2;
        app.Exit();});

    auto component = Container::Vertical({
    input_host,
    input_port,
    input_user,
    input_password,
    input_dbname,
    submit_bttn
    });
    auto renderer= Renderer(component,[&]{
        return vbox({
            hbox({text("Host:"),input_host->Render()}),
            hbox({text("Port:"),input_port->Render()}),
            hbox({text("User:"),input_user->Render()}),
            hbox({text("Password:"),input_password->Render()}),
            hbox({text("Databasse:"),input_dbname->Render()}),
            separator(),
            submit_bttn->Render()
        });
    });

    app.Loop(renderer);
}

void Menu::run(){
    switch (Menu::State) {
    case 0: return;break;
    case 1: connect();break;
    case 2: /*freeform()*/;break;
    default: throw std::runtime_error("Invalid state!"); //TODO: Use custome exception
    }
}