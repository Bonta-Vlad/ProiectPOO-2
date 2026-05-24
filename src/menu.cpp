#include "menu.h"      
#include "ftxui/component/component.hpp"       
#include "ftxui/component/component_base.hpp"  
#include "ftxui/component/component_options.hpp"  
#include "ftxui/dom/elements.hpp"
#include "ftxui/util/ref.hpp"
#include "ftxui/component/app.hpp"
#include "pqxx/internal/result_iter.hxx"
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <stdexcept>
#include <string>
#include <vector>
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

void Menu::freeform(){
    using namespace ftxui;
    std::string sql_query;
    std::string result_text;
    auto app = App::TerminalOutput();
    auto query= Input("SQL Query");
    auto response= text("Response:\n"+result_text) ;

    auto respones_render= Renderer([&]{
        return vbox(response);
    });
    auto query_render= Renderer([&]{
        return vbox(query);
    });
    std::vector<std::string> tabs{
        "Query",
        "Response"
    };
    int tab_index=0;
    auto tab_selection= ftxui::Menu(&tabs, &tab_index);
    auto tab_content= Container::Tab(
        {
            query_render,
            respones_render,
        },
        &tab_index
    );
    auto submit= Button("Submit",[&] {
        result_text= "";
        pqxx::work transaction(*conn);
        pqxx::result result= transaction.exec(sql_query);
        transaction.commit();
        for (auto const &row: result) {
            for (auto const &field: row) result_text+= field.c_str();
            result_text+="\n";
        }
        
    });

    auto main_container= Container::Vertical({
        Container::Horizontal({
            tab_selection,
            submit
        }),
        tab_content
    });

    auto renderer= Renderer(main_container, [&]{
        return vbox({
            text("SQL Requsts")| bold | hcenter,
            hbox({
                tab_selection->Render()| flex,
                submit->Render(),
            }),
            tab_content->Render()| flex,
        });
    });
    app.Loop(renderer);
}



void Menu::run(){
    switch (Menu::State) {
    case 0: return;break;
    case 1: connect();break;
    case 2: freeform();break;
    default: throw std::runtime_error("Invalid state!"); //TODO: Use custome exception
    }
}