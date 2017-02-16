#include "includes.h"

#include "Sim_900.h"
#include "rs232.h"
#include "GSM.h"

#include "commands.h"
#include "commands_AT.h"
#include "config.h"

//потоки
///пока не используется
void Sim900_menu_thread()
{
    int select_menu = 0;
	while (true)
	{
		cout << "Меню\nВыберите пунк\n1 - соединиться с сервером\n2 - отправить сообщение\n3 - перейти в режим терминала\n4 - Выход\n";

		cin >> select_menu;
		switch (select_menu)
		{
		case 1:
		{
			///соединение с сервером
			if (Sim900_conn_to_server())
				cout << "CONNECT ERROR\n";
			else
                break;
		case 2:
			///отправить сообщение
			if (send_new_command_server() == 0)
				cout << "SEND_ERROR\n";
			else
				cout << "SEND_OK\n";
			break;
		case 3:
			///отправить команду
			Sim900_behaviour_hyber_terminal();
			cout << "режим терминала" << endl;
			break;
		case 4:
			cout << "Выход\n";
         // Sim900_сlose_app();
         // close_programm();
            close_ = true;
			return;
		default:
			cout << "Unknow Command\n";
			break;
		}
		}
	}
}

void Sim900_recv_handler()
{
	///если сообщение состоит из одного сообщения, то принимаем и обрабатываем, иначе... быть и не должно... пока
	char answer_from_COM[SIZE_BUFFER] = { 0 };
    char* pntr = &answer_from_COM[0]; //я знаю это выглядит странно, но без этого не работает)))
    while (!close_)
	{
        if (!Sim900_read_COM(&pntr)) {
			continue;
		}
        //TODO!!!проверка, что пришла именно структура, а не что-то другое!!! добавить блок try
		stMainHeader* MainHeader = new stMainHeader();
        memcpy(MainHeader, pntr, sizeof(stMainHeader));

		switch (MainHeader->cmd) {
            case SET_PARAMETRS: {
				Sim900_set_parametrs(MainHeader);
				break;
			}
			case OFF_LIGHT: 	{
                Sim900_off_light();
				break;
			}
			case ON_LIGHT: 		{
				Sim900_on_light();
				break;
			}
			default:
				cout << "Unknow cmd..." << endl;
				break;
			}
		//очистить буфер
		MEM_SET(answer_from_COM);
	}
	return;
}

//инициализация

//общение по COM порту
int Sim900_read_COM(char** buf)
{
    int n = _reading_async(*buf);
    if(n > 0)
    {
        next_answer();
    }
    return n;
}

int Sim900_read(unsigned char* buf)
{
    int size = 100;
    RS232_PollComport(COM_PORT, buf, size);
    cout << buf;
    return 1;

}

int Sim900_write_cmd(char* cmd)
{
  //  ENTER_CRITICAL_SECTION
    if (RS232_SendBuf(COM_PORT, cmd, strlen(cmd)))
    {
    //    LEAVE_CRITICAL_SECTION
        next_cmd();
        return 0;
    }
    else
    {
   //    LEAVE_CRITICAL_SECTION
        return 1;
    }
}


int Sim900_write(char* buff, int size)
{
    int res = 0;
//	ENTER_CRITICAL_SECTION
    cout << "Sim900_write <- " << buff << "  size: = " << size << endl;

    res = RS232_SendBuf(COM_PORT, buff, size);
 //  LEAVE_CRITICAL_SECTION
    return res; // 0 - верно или 1 - ошибка
}

int Sim900_cmp_strs(char* need_answer)
{
    char answer_from_COM[SIZE_BUFFER] = { 0 };
    char* pntr = &answer_from_COM[0];
    if (is_echo)
        cout << "эхо включено\n"; ///для доработки ^^^^
    Sim900_read_COM(&pntr);

    cout << "Должно прийти: " << need_answer << "$" << endl;
    cout << "Пришло: " << pntr << "$" << endl;

    return strcmp(need_answer, answer_from_COM) == 0 ? true : false;
}

int Sim900_cmp_strs_with_time_out(char* need_answer)
{

    char answer_from_COM[SIZE_BUFFER] = { 0 };
    char* pntr = &answer_from_COM[0];
    if (! need_answer)
	{
        while (*pntr == '\0')
            Sim900_read_COM(&pntr);
		return 1;
	}
    while (*pntr == '\0')
        Sim900_read_COM(&pntr);
    return strcmp( need_answer, answer_from_COM) == 0 ? true : false;
}

void Sim900_answer()
{
    unsigned char answer[SIZE_BUFFER] = { 0 };
    unsigned char* pntr = &answer[0];
    Sim900_read(pntr);
	cout << answer << endl;
}

int Sim900_conn_to_server() ///0 - подключился
{
	cout << "_________________________________\n";
    int step_count = 7;
	int i = 0;

    char* pntr = NULL;
    enum STEP {
        at           = -1,
        cipshut      = 0,
        cipmux       = 1,
		connect_inet = 2, 
        ciicr        = 3,
        get_ip       = 4,
        cipstart     = 5,
        init2server  = 6
	};

    STEP step = at; ///enum на каждый шаг
    char end_symbol_26 = END_SEND_SYMBOL_26;
	
    for (step = at, i = -1; i < step_count; i++)
	{
        step = (STEP)i;
        switch (step)
		{
		case at:
		{
			Sim900_write_cmd(cmd(ATV1));
        //	Sim900_read_COM(&tmp);
		//	cout << "Step 1: " << tmp << endl;
			break;
		}
		case cipshut:
		{
            Sim900_write_cmd(cmd(CIPSHUT));
        //	Sim900_read_COM(&tmp);
		//	cout << "Step 2: " << tmp << endl;
			break;
		}
		case cipmux:
		{
			Sim900_write_cmd(cmd(CIPMUX));
        //	Sim900_read_COM(&tmp);
		//	cout << "Step 3: " << tmp << endl;
			break;
		}
		case connect_inet:
		{
			Sim900_write_cmd(cmd(CONNECT_INET));
        //	Sim900_read_COM(&tmp);
		//	cout << "Step 4: " << tmp << endl;
			break;
		}
		case ciicr:
		{
			Sim900_write_cmd(cmd(CIICR));
        //	Sim900_read_COM(&tmp);
		//	cout << "Step 5: " << tmp << endl;
			break;
		}
		case get_ip:
		{
			Sim900_write_cmd(cmd(GET_IP));
        //	Sim900_read_COM(&tmp);
        //  self_ip = pntr;
		//	cout << "Step 6: " << tmp << endl;
			break;
		}
		case cipstart:
		{
			Sim900_write_cmd(cmd(CIPSTART));
        //	Sim900_read_COM(&tmp);
		//	cout << "Step 7: " << tmp << endl;
			break;
		}
		case init2server:
		{
			////сообщаяем серверу данные о себе
            int res = 0;
            res = Sim900_send_msg(Sim900_new_msg(NEW_PP, 0));
			// TODO разобраться, что и как

            cout << "Step 7: " << res << endl;
			break;
		}
		default:
			return 0;
		}
	}
	if (!Sim900_autotification())
	{
		cout << "Ошибка аутотификации" << endl;
		return 0;
	}
	
	is_connect_to_server = true;

	return 0;
}

int Sim900_autotification()
{
    char pswrd[10] = { 0 };
    char answer[10] = { 0 };
    char* p_answer = &answer[0];
    char* p_pswrd = &pswrd[0];

    strncpy(pswrd, password.c_str(), password.length() + 1);

    Sim900_cipsend(p_pswrd);
    Sim900_read_COM(&p_answer);

	_print_answer(answer, false);
	return 0;
}

bool Sim900_cipsend(char* buf)
{
    ///отправляет сообщение через AT+CIPSEND
    if( Sim900_write_cmd(cmd(CIPSEND)) &&
            Sim900_write(buf, strlen(buf)) &&
            Sim900_write_cmd(cmd(WRITE_MESSAGE)))
        return true;
    else return false;

}

//отправка сообщения
int Sim900_send_msg(stMainHeader* msg)
{
	// узнать какие команды нужны
	// добавить требуемые символы к посылке
	// отправить по COM порту
	// анализ ответа
	return 1;
}

stMainHeader* Sim900_new_msg(int cmd, BYTE data)
{
	//int size_next_msg = len(data) / SIZE_BUFFER;
	stMainHeader* new_msg = new stMainHeader();
    new_msg->id = next_id_;
	new_msg->cmd = cmd;
	new_msg->sender = SERIAL_NUMBER;
	new_msg->size_next_msg = 0;
	new_msg->data = data;
	
    next_id();

	return new_msg;
}

int Sim900_put_char(int ASCIIch)
{
    char ch = (char)ASCIIch;
	cout << "Послать символ: " << ASCIIch << "символ: " << ch << endl;

    int res = RS232_SendBuf(COM_PORT, &ch, sizeof(char));
    return res;
}

void Sim900_add_end_symbol(char* cmd)
{
	char copy_cmd[100] = { 0 };
	copy_cmd[0] = END_SYMBOL_13;
	copy_cmd[1] = END_SYMBOL_10;
	memcpy(&(copy_cmd[2]), cmd, sizeof(char)*strlen(cmd));
	copy_cmd[strlen(copy_cmd)] = END_SYMBOL_13;
	copy_cmd[strlen(copy_cmd)] = END_SYMBOL_10;
	memcpy(cmd, copy_cmd, sizeof(char)*strlen(copy_cmd));
}

int Sim900_check_CIPSTART()
{
	char answerCOM[100] = { 0 };
	char* tmp = &answerCOM[0];
	while (*tmp == '\0')
	{
        Sim900_read_COM(&tmp);
	}
    if (strcmp(answerCOM, answer(OK)) == 0)
	{
		while (*tmp == '\0')
		{
            Sim900_read_COM(&tmp);
		}
        if (strcmp(answerCOM, answer(CONN_OK)) == 0)
		{
			return true;
		}
	}
	char ok_conn_ok[100] = { 0 };
    strcat(&ok_conn_ok[0], answer(OK));
    strcat(&ok_conn_ok[strlen(ok_conn_ok)], answer(CONN_OK));
	if (strcmp(answerCOM, ok_conn_ok) == 0)
	{
		return true;
	}
    if (strcmp(answerCOM, cmd(OK_CONNECT_OK)))
		return true;
	else return false;
}

//прочие
bool Sim900_set_echo_behaviour()
{
    unsigned char* buf = new unsigned char[10];
    int n = 0;
    n = Sim900_write_cmd(cmd(ATE0)); ///команды модулю
	Sim900_answer();
    set_echo(bool(n));
	return true;
}

bool Sim900_set_ATV0()
{
    Sim900_write_cmd(cmd(ATV0));
	return true;
}

bool Sim900_set_ATV1()
{
    Sim900_write_cmd(cmd(ATV1));
    Sim900_answer();
	return true;
}

/*int Sim900_сlose_app()
{
	///удалить списки команд
	for (int i = 0; i < cnt_command; i++)
	{
		free(сommands_AT[i]);
	}
	for (int i = 0; i < cnt_answer; i++)
	{
		free(answer_AT[i]);
	}
	// оправить сообщение на сервер о прекращение работы (если есть связь)
	// закрыть сокет
	// закрыть потоки
	// закрыть COM порт
	cout << "До новых встреч))" << endl;
	return 0;
}*/

// debug
void _print_answer(string str, bool from_thread)
{
	if (!from_thread)
		return;
	cout << "ReadCOM: " << str;
	cout << "*ReadCOM" << endl;
}

int Sim900_behaviour_hyber_terminal()
{
	bool is_end = false;
	char cmd[SIZE_BUFFER] = { 0 };
	char answer[SIZE_BUFFER] = { 0 };
	char* tmp = &answer[0];

	while (!is_end)
	{
		cout << "Введите команду\n";
		cin >> cmd;
		Sim900_add_end_symbol(cmd);
//		ENTER_CRITICAL_SECTION;
        Sim900_write(cmd, strlen(cmd));
		Sim900_answer();
//		LEAVE_CRITICAL_SECTION;
		while (true)
		{
			MEM_SET(answer);
			cout << "Продолжить? (y, n)\n";
			cin >> answer;
			if (strlen(answer) == 1 && answer[0] == 'y') break;
			if (strlen(answer) == 1 && answer[0] == 'n') { is_end = true; break; }
			cout << "Повторите ввод";
		}
	}
	return 0;
}


int _reading_async(char* buf)
{
  int fdesc = Cport[COM_PORT];
  int res = 0;
  flag_async = false;

  while (*buf != '\t')
    {
      /* If there is a data to be read */
      if (flag_async == true)
        {
          /* Read one byte */
          do
            {
              res = read(fdesc, &buf, 1);
              cout << buf << endl;
            }
          while (res > 0);
          flag_async = false;
        }
    }
  return strlen(buf);
}

bool DisconnectController()
{
    RS232_CloseComport(COM_PORT);
    return true;
}
