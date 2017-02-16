#pragma once
#include <vector>
#include <queue>
#include <stddef.h>
#include <pthread.h>
#include "structs.h"
#include <string>

///для интернета
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

///типы
typedef unsigned char BYTE;
typedef int File;
typedef int SOCKET;
using namespace std;

///переменные модуля
extern bool is_connect_to_server; // = false;
///прием

//--------------------------//
///глобальные переменные
extern bool is_thread_prog; // = true; // зачем??)))

//Прочие
#define MY_MY_ERROR -1

#define ENTER_CRITICAL_SECTION pthread_mutex_lock (&critical_section);
#define LEAVE_CRITICAL_SECTION pthread_mutex_unlock (&critical_section);

#define char_END_SYMBOL_26 char EndSymbol = END_SEND_SYMBOL_26;
#define SIZE_CMD_OR_ANSWER 100

///счетчики
extern int cnt_command_;// = 0;
extern int cnt_answer_; // = 0;
extern int cnt_servers_msgs_; // = 0;
extern int next_id_; // = 0; //id сообщения на сервер. Первое поле в посылке, для четкой последовательности команда

///события
//extern HANDLE  h_event; /// TODO!!! СОБЫТИЯ
extern pthread_mutex_t critical_section; // TODO!!! КРИТИЧЕСКИЕ СЕКЦИИИ
extern pthread_mutexattr_t m_attr;

/// потоки
extern int* id_reciver_th, *id_menu_th, result;
extern pthread_t* reciver_thread, *menu_thread;

///для COM
extern File* f_out;
extern bool flag_async; // = false;
//extern string com_port;

///прочие
extern bool f_success;
extern bool is_echo; // = false;
extern bool is_init; // = false;
extern string self_ip; // = "0.0.0.0";
extern bool close_; // = false;


///модуль init
int init();
bool init_critical_section();
char* cmd(int);
char* answer(int);

///графический интерфейс
bool send_new_command_server();
//bool send_new_command_lamps();

/// общие функции
//bool connect_to_server();
int start_menu();
bool listen_server();
bool listen_lamps();
//bool close_programm(); // TODO!
int _get_console_data(const char* mess);
bool _get_cmd_and_data(int* cmd, int* data);

///изменение глобальных данных
void next_cmd();
void next_answer();
void next_msg();
void set_echo(bool status);
void next_id();
//структуры данных


///Вспомогательные
#define ZERO_STRUCT_MEMORY(pst) ZeroMemory(pst, sizeof(*(pst)))
#define ZERO_STRUCT_ARRAY_MEMORY(pst, count) ZeroMemory(pst, count*sizeof(pst[0]))
#define COPY_STRUCTS_ARRAY(pDst, pSrc, count) CopyMemory(pDst, pSrc, count*sizeof(pDst[0]))
#define SAFE_DELETE_POINTER(p) { if (p) delete p; p = NULL; }
#define SAFE_DELETE_ARRAY_POINTER(p) { if (p) delete [] p; p = NULL; }
#define MY_ERROR  perror("Ошибка");
#define RETURN_SOCK_ERROR return -1;
#define S_ERROR -1
#define STR_LEN(x) sizeof(char)*strlen(x)
#define GL_MEM_SET(x, y, z) MEM_SET(x) MEM_SET(y) MEM_SET(z)
#define MEM_SET(x) memset(x, 0, sizeof(char) * SIZE_BUFFER);
