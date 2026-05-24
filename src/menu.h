#include <pqxx/pqxx>

class Menu{
    static Menu* Instance;
    static int State;
    pqxx::connection* conn;
    Menu(const Menu& other)= delete;
    Menu operator=(const Menu& other)= delete;
    Menu();
    void connect();
    public:
    static Menu* get_instance();
    void run();
    void freeform();
    ~Menu(){delete Instance;delete conn;}
};


/*
States:
0- Quit
1- Database connection
2- Freeform SQL
*/