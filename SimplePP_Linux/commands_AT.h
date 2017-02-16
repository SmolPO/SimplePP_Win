#pragma once
#include <string>
using namespace std;

extern const int cnt_AT_cmd;
extern string AT_cmd[17];
extern const int cnt_AT_answer;
extern string AT_answer[7];

//команды
#define AT 0
#define ATE0 1
#define ATE1 2
#define ATV0 3
#define ATV1 4
#define CIPMUX 5
#define CIPSHUT 6
#define IS_TRANSP 7
#define NOT_TRANSP 8
#define CIPSTATUS 9
#define CONNECT_INET 10
#define CIPSTART 11
#define CIICR 12
#define GET_IP 13
#define CIPSEND 14
#define AT_ON_LIGHT 15
#define AT_OFF_LIGHT 16
#define WRITE_MESSAGE 17


//символы
#define END_SEND_SYMBOL_26 26
#define END_SYMBOL_13 13
#define END_SYMBOL_10 10

//ответы

#define OK 0
#define ECHO_OK 1
#define CONN_OK 2
#define ST_INITIAL 3
#define SEND_OK 4
#define SHUT_OK 5
#define OK_CONNECT_OK 6
\
///для терминала
//AT+CSTT="internet.tele2.ru","tele2","tele2"
//AT+CIPSTART="TCP","5.145.160.40","27000"
