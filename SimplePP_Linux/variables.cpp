#include "commands.h"
#include "commands_AT.h"
#include "config.h"
#include "GSM.h"
#include "rs232.h"

///commands.h
#define OFF_LIGHT 1;
#define  ON_LIGHT 2;
#define SET_PARAMETRS 3;
#define  NEW_PP 4;
#define  CMD_UNKNOW -2;

///commands AT
const int cnt_AT_cmd = 17;
string AT_cmd[cnt_AT_cmd] =
{
    "\r\nAT\r\n",
    "\r\nATE1\r\n",
    "\r\nATV0\r\n",
    "\r\nATV1\r\n",
    "\r\nAT+CIPMUX=0\r\n",
    "\r\nAT+CIPSHUT\r\n",
    "\r\nAT+CIPMODE=1\r\n",
    "\r\nAT+CIPMODE=0\r\n",
    "\r\nAT+CIPSTATUS\r\n",
    "\r\nAT+CSTT=\"internet.tele2.ru\",\"tele2\",\"tele2\"\r\n", //TODO!!! форматировать строку согласно оператору св€зи
    "\r\nAT+CIPSTART=\"TCP\",\"5.145.160.40\",\"27000\"\r\n",   //TODO!!! форматировать строку порту и ip-адресу
    "\r\nAT+CIICR\r\n",
    "\r\nAT+CIFSR\r\n",
    "\r\nAT+CIPSEND=8\r\n",
    "\r\nAT+CNETLIGHT=1\r\n",
    "\r\nAT+CNETLIGHT=0\r\n",
    "\r\n> ",
};

const int cnt_AT_answer = 7;
string AT_answer[cnt_AT_answer] =
{
    "\r\nOK\r\n",
    "\r\nAT\r\n\r\nOK\r\n",
    "\r\nCONNECT\r\n",
    "\r\nSTATE: IP INITIAL\r\n",
    "\r\nSEND OK\r\n" ,
    "\r\nSHUT OK\r\n",
    "\r\nOK\r\n\r\nCONNECT OK\r\n"
};

///config
string password = "6EQUJ511_1";

int SERIAL_NUMBER = 0;
string server_addr = "128.199.42.73";
int PORT = 27000;
int my_data = 0;

bool is_connect_to_server = false;
bool is_thread_prog = true; // зачем??)))

///счетчики
int cnt_command_ = 0;
int cnt_answer_ = 0;
int cnt_servers_msgs_ = 0;
int next_id_ = 0; //id сообщени€ на сервер. ѕервое поле в посылке, дл€ четкой последовательности команда

///событи€
//extern HANDLE  h_event; /// TODO!!! —ќЅџ“»я
//pthread_mutex_t* critical_section; // TODO!!!  –»“»„≈— »≈ —≈ ÷»»»
//pthread_mutexattr_t* m_attr;

/// потоки
int* id_reciver_th, *id_menu_th, result;
pthread_t* reciver_thread, *menu_thread;

///дл€ COM
File* f_out;
bool flag_async = false;
//string com_port = "/dev/pts/19";

///прочие
bool f_success;
bool is_echo = false;
bool is_init = false;
string self_ip = "0.0.0.0";
bool close_ = false;

///rs232
int Cport[RS232_PORTNR], error;

struct termios new_port_settings,
       old_port_settings[RS232_PORTNR];

char *comports[RS232_PORTNR]={"/dev/ttyS0","/dev/ttyS1","/dev/ttyS2","/dev/ttyS3","/dev/ttyS4","/dev/ttyS5",
                       "/dev/ttyS6","/dev/ttyS7","/dev/ttyS8","/dev/ttyS9","/dev/ttyS10","/dev/ttyS11",
                       "/dev/ttyS12","/dev/ttyS13","/dev/ttyS14","/dev/ttyS15","/dev/ttyUSB0",
                       "/dev/ttyUSB1","/dev/ttyUSB2","/dev/ttyUSB3","/dev/ttyUSB4","/dev/ttyUSB5",
                       "/dev/ttyAMA0","/dev/ttyAMA1","/dev/ttyACM0","/dev/ttyACM1",
                       "/dev/rfcomm0","/dev/rfcomm1","/dev/ircomm0","/dev/ircomm1",
                       "/dev/cuau0","/dev/cuau1","/dev/cuau2","/dev/cuau3",
                       "/dev/cuaU0","/dev/cuaU1","/dev/cuaU2","/dev/cuaU3"};
