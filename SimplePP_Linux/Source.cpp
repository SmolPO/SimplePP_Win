#include "includes.h"

#include "GSM.h"
#include "Sim_900.h"
#include "config.h"

bool listen_server()
{
	Sim900_recv_handler();
	return true;
}

bool listen_lamps()
{
	cout << "listen lamps..." << endl;
	return true;
}

bool send_new_command_server()
{
	// ввести данные
	// создать сообщения
	// преобразовать ее к пакету байт для отправки
	// отправить
	// проверить ответ
	int cmd = 0;
	int data = 0;
	stMainHeader* new_msg = new stMainHeader();

	BYTE* new_msg_in_bytes = NULL;
	_get_cmd_and_data(&cmd, &data);
	new_msg = Sim900_new_msg(cmd, BYTE(data));
	//*new_msg_in_bytes = BYTE(&new_msg);
	/*if (!Sim900_send_mess(char(new_msg_in_bytes)))
	{
		return true;
	}
	else
	{
		return false;
	}
	*/


	return true;
}

int _get_console_data(const char* mess)
{
	char* _answer = new char(10);
	int answer = -1;
	cout << mess << endl;
	cin >> _answer;
	try
	{
		answer = atoi(_answer);
	}
	catch (...)
	{
		cout << "Вы ввели неверные данные" << endl;
		return answer;
	}
	return answer;
}

bool _get_cmd_and_data(int* cmd, int* data)
{
	int _cmd = 0, _data = 0;
	while (true)
	{
		_cmd = _get_console_data("Введите номер команды:\n");
		_data = _get_console_data("Введите данные\n");
		if (_cmd == -1 || _data == -1)
		{
			cout << "Данные не корректны" << endl;
			_cmd = -1;
			_data = -1;
			continue;
		}
		else
			break;
	}
	return true;
}

bool close_programm()
{
  //  delete critical_section, m_attr;
    RS232_CloseComport(COM_PORT);
//    pthread_mutex_destroy (&critical_section);
	return true;
}


int start_menu()
{
    /*int result = pthread_create(&menu_thread, NULL, menu_thread, &id_menu_th);
    if (result != 0) {
       perror("Создание потока прослушки порта!");
       return MY_MY_ERROR;
    }

    result = pthread_join(menu_thread, NULL);
    if (result != 0) {
        perror("Ждём поток меню");
        return EXIT_FAILURE;
    }*/
}

//изменение глобальных данных
void next_cmd()
{
    cnt_command_++;
}

void next_answer()
{
    cnt_answer_++;
}

void next_msg()
{
    cnt_servers_msgs_++;
}

void set_echo(bool status)
{
    is_echo = status;
}

void next_id()
{
    next_id_++;
}
