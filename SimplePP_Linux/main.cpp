// SimplePP.cpp: определяет точку входа для консольного приложения.
//
#ifdef QT_NO_DEBUG_OUTPUT
#undef QT_NO_DEBUG_OUTPUT
#endif

#include "GSM.h"
using namespace std;

int main()
{
    if (!init())
        return 0;

    start_menu();
    /*while (true)
    {
        int choice = 0;
            // диалоговое меню пользователя
        printf("Что желаете? Для отправки сообщения нажмите 1\n\rЕсли вы хотите отправить сообщение фонарям - жмите 2\n\rДля выхода из программы нажмите 3\n\r");
        choice = _get_console_data("Ваш выбор?\n\r");
        switch (choice)
        {
        case 1:
            send_new_command_server();
            break;
        case 2:
        //	send_new_command_lamps();
            break;
        case 3:
            close_programm();
            return 0;
        default:
            printf("Вы ввели что то неизвестное... Повторите попытку...\n");
            break;
        }

    }*/
    //close_programm();
    return 0;
}

