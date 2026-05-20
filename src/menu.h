

class Menu{
    static Menu* Instance;
    static int State;
    Menu(const Menu& other)= delete;
    Menu operator=(const Menu& other)= delete;
    Menu();
    void connect();
    public:
    static Menu* get_instance();
    void run();
    ~Menu();
};


/*
States:
0- Quit
1- Database connection
2- Freeform SQL
*/