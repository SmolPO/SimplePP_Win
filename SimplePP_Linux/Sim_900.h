#pragma once
#include <string>

#include "structs.h"
// ���������� ������� ������ � �������� Sim900
using namespace std;
int Sim900_conn_to_server();  ///0 - �����������
int Sim900_autotification();  // ������������� �� �������
int Sim900_behaviour_hyber_terminal(); ///����� HyberTerminal


//������ Sim900
void Sim900_menu_thread(); //���� ������ � ������� TODO!!!
									  ///������ �� ������� ������ � �������


void Sim900_recv_handler();

int Sim900_write_cmd(char* cmd);
int Sim900_write(char* buff, int size);
int Sim900_read_COM(char** answerCOM);
int Sim900_read(unsigned char* buf);
int Sim900_cmp_strs(char* NeedAnswer);
int Sim900_cmp_strs_with_time_out(char* answer);
bool Sim900_cipsend(char* buf);

int Sim900_send_msg(stMainHeader* msg);
stMainHeader* Sim900_new_msg(int cmd, BYTE data);
int Sim900_put_char(int ASCIIch);
void Sim900_add_end_symbol(char* cmd);

//int Sim900_�lose_app();
int Sim900_check_CIPSTART();

//����������� ���������
bool Sim900_set_parametrs(stMainHeader* msg);
bool Sim900_on_light();
bool Sim900_off_light();

//������
bool Sim900_set_ATV0();
bool Sim900_set_ATV1();
bool Sim900_set_echo_behaviour();

void Sim900_answer();
void _print_answer(string foo, bool from_thread);

bool _send(const char * Data, const int DataSize);
int _reading_async(char* buf);
