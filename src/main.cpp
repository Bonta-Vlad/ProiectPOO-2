#include "menu.h"

int main(){
Menu* menu= Menu::get_instance();
menu->run();
delete menu;
return 0;
}