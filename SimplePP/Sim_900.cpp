//#include "stdafx.h"

#include "stdafx.h"

#include "Sim_900.h"
#include "GSM.h"
#include "commands.h"
#include "commands_AT.h"
#include "config.h"

//������
///���� �� ������������
VOID WINAPI thr_menu(PVOID* dummy)
{
	
	int select_menu = 0;
	while (true)
	{
		cout << "����\n�������� ����\n1 - ����������� � ��������\n2 - ��������� ���������\n3 - ������� � ����� ���������\n4 - �����\n";

		cin >> select_menu;
		switch (select_menu)
		{
		case 1:
		{
			///���������� � ��������
			if (Sim900_conn_to_server())
				cout << "CONNECT ERROR\n";
			else
			break;
		case 2:
			///��������� ���������
			if (send_new_command_server() == 0)
				cout << "SEND_ERROR\n";
			else
				cout << "SEND_OK\n";
			break;
		case 3:
			///��������� �������
			Sim900_behaviour_hyber_terminal();
			cout << "����� ���������" << endl;
			break;
		case 4:
			cout << "�����\n";
			close = true;
			SetEvent(h_event);
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
	///���� ��������� ������� �� ������ ���������, �� ��������� � ������������, �����... ���� � �� ������... ����
	char answer_from_COM[SIZE_BUFFER] = { 0 };
	char* tmp = &answer_from_COM[0]; //� ���� ��� �������� �������, �� ��� ����� �� ��������)))
	while (!close) 
	{
		if (!Sim900_read_COM(&tmp, true)) {
			continue;
		}
			
		stMainHeader* MainHeader = new stMainHeader();
		memcpy(MainHeader, tmp, sizeof(MainHeader));

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
		//�������� �����
		MEM_SET(answer_from_COM);
	}
	return;
}

//�������������
void init_cmds_and_answs_AT()
{
	add_cmd(AT);
	add_cmd(ATE0);
	add_cmd(ATE1);
	add_cmd(ATV0);
	add_cmd(ATV1);
	add_cmd(CIPMUX);
	add_cmd(CIPSTATUS);
	add_cmd(CIPSEND);
	add_cmd(CIPSTART);
	add_cmd(CIPSHUT);
	add_cmd(CIICR);
	add_cmd(CONNECT_INET);
	add_cmd(GET_IP);
	add_cmd(AT_ON_LIGHT);
	add_cmd(AT_OFF_LIGHT);
	add_cmd(IS_TRANSP);
	add_cmd(NOT_TRANSP);

	add_answer(WRITE_MESSAGE);
	add_answer(OK);
	add_answer(ECHO_OK);
	add_answer(CONN_OK);
	add_answer(ST_INITIAL);
	add_answer(SEND_OK);
	add_answer(SHUT_OK);

}

void add_cmd(char* cmd)
{
	char* next_command = new char[SIZE_CMD_OR_ANSWER];
	memset(next_command, 0, sizeof(char)*SIZE_CMD_OR_ANSWER);
	memcpy(next_command, cmd, sizeof(char)*strlen(cmd));
	�ommands_AT[cnt_command] = next_command;
	cnt_command++;
}

void add_answer(char* answer)
{
	char* next_answer = new char[SIZE_CMD_OR_ANSWER];
	memset(next_answer, 0, sizeof(char)*SIZE_CMD_OR_ANSWER);
	memcpy(next_answer, answer, sizeof(char)*strlen(answer));
	answer_AT[cnt_answer] = next_answer;
	cnt_answer++;
}

//���������������
char* cmd(char* buffer)
{
	char cmd[SIZE_BUFFER] = { 0 };
	memcpy(cmd, buffer, STR_LEN(buffer));
	//Sim900_add_end_symbol(cmd);
	////�������������
	for (int i = 0; i < strlen(cmd); i++)
	{
		if (cmd[i] == '"')
		{
			cmd[i] = '\"';
		}
	}
	for (int i = 0; i < cnt_command; i++)
	{
		///� ������ ������� ���� \r\n, ������� ������� �� ������� �������
		///���������� ������ �� ������� �������!
		if (strncmp(cmd, �ommands_AT[i], STR_LEN(cmd)) == 0)
			return �ommands_AT[i];
	}

	return '\0';
}

char* answer(char* buffer)
{
	char answ[SIZE_BUFFER] = { 0 };
	memcpy(answ, buffer, STR_LEN(buffer));
	Sim900_add_end_symbol(answ);
	////�������������
	for (int i = 0; i < strlen(answ); i++)
	{
		if (answ[i] == '"')
		{
			answ[i] = '\"';
		}
	}
	for (int i = 0; i < cnt_answer; i++)
	{
		///���������� ������ �� ������� �������!
		if (strncmp(answ, answer_AT[i], STR_LEN(answ)) == 0)
			return answer_AT[i];
	}
	cout << "����� �� ������: " << buffer << endl;
	return '\0';
}

//������� �� COM �����
int Sim900_read_COM(char** answerCOM, bool flag)
{
	int i = 0;
	
	char answer_from_COM[SIZE_BUFFER] = { 0 };
	memset(answer_from_COM, 0, sizeof(char) * SIZE_BUFFER);
	if (!_recieve_func(answer_from_COM))
		return false;

	/*while (ReadFile(h_com, help, 1, &bytes_read, ))
	{
		if (bytes_read <= 0)
			break;
		answer_from_COM[i] = help[0];
		i++;
	}*/

	_print_answer(answer_from_COM, flag);
	strcpy(*answerCOM, answer_from_COM);
	return answer_from_COM[0] == '\0' ? false : true;
	
}

int Sim900_write_cmd(char* cmd)
{
	if (_send(cmd, strlen(cmd)))
	{
		cout << "������� ����������\n";
		return true;
	}
	cout << "������ Sim900_write_cmd\n";
	return MY_MY_ERROR;
	/*cout << cmd;
	WriteFile(h_com, cmd, strlen(cmd), &bWritten, NULL);
	return bWritten == strlen(cmd) ? true : false;*/
}

bool _send(const char * Data, const int DataSize)
{
	DWORD dw_bytes_written;
	DWORD err = GetLastError();
	//ENTER_CRITICAL_SECTION

	if (!h_com)
	{
		//LEAVE_CRITICAL_SECTION
		return false;
	}

	PurgeComm(h_com, PURGE_TXCLEAR);	//�������� ���������� ����� �����	
	
	BOOL write_res = WriteFile(h_com, Data, DataSize, &dw_bytes_written, &m_ovTx); 
	DWORD signal = WaitForSingleObject(m_ovTx.hEvent, INFINITE);	  
	if (signal == WAIT_TIMEOUT)
	{
		
		//LEAVE_CRITICAL_SECTION
		return false;
	}
	else
	{
		BOOL OverlappedRes = GetOverlappedResult(h_com, &m_ovTx, &dw_bytes_written, true);

		if ((signal != WAIT_OBJECT_0) || (!write_res && (err != ERROR_IO_PENDING)) || !OverlappedRes)
		{
			
			//LEAVE_CRITICAL_SECTION
			return false;
		}
		//else
			//LEAVE_CRITICAL_SECTION
	}

	/* Error check */
	return (dw_bytes_written == DataSize);
}

bool DisconnectController()
{
	// ������� com-����
	return true;
}

bool _recieve_func(char* buffer)
{
	BOOL		status;
	DWORD		dw_bytes_read = 0;
	DWORD		dw_errors;
	COMSTAT		ab_comstat;
	DWORD		dummy, mask, signal;
	int			btr;

	if (!h_com)
		return false;

	status = WaitCommEvent(h_com, &mask, &m_ovRx);	
	dw_errors = GetLastError();
	if (status || (!status && dw_errors == ERROR_IO_PENDING))
	{
		do
		{
			signal = WaitForSingleObject(m_ovRx.hEvent, INFINITE);
			switch (signal)
			{
			case WAIT_OBJECT_0: 
				if (GetOverlappedResult(h_com, &m_ovRx, &dummy, true)) //���������, ������� �� 
				{
					if (mask & EV_RXCHAR)  
					{	
						if (ClearCommError(h_com, &dw_errors, &ab_comstat) != FALSE)
						{
							btr = ab_comstat.cbInQue;
							if (btr)
							{	
								do
								{
									dw_bytes_read = min(btr, sizeof(buffer) / sizeof(buffer[0]));
									ReadFile(h_com, buffer, dw_bytes_read, NULL, &m_ovRx);

									btr -= sizeof(buffer) / sizeof(buffer[0]);
								} while (btr > 0);
							}
						}
						else
							return false;
					}
				}
				else
					return false;
				break;

			default:
				return false;
			}
		} while (signal == WAIT_TIMEOUT);
	}
	else
		return false;

	return true;
}

int Sim900_write_with_size(char* buff, int size)
{
	ENTER_CRITICAL_SECTION
	cout << "Buff:= " << buff << "Size: = " << size << endl;

	WriteFile(h_com, buff, size, &bWritten, NULL);
	LEAVE_CRITICAL_SECTION
	return bWritten == size ? true : false;
}

int Sim900_cmp_strs(char* NeedAnswer)
{
	char AnswerCom[SIZE_BUFFER] = { 0 };
	char* tmp = &AnswerCom[0];
	if (is_echo) cout << "��� ��������\n"; ///��� ��������� ^^^^
	Sim900_read_COM(&tmp, false);

	cout << "������ ������: " << NeedAnswer << "$" << endl;
	cout << "������: " << tmp << "$" << endl;

	return strcmp(NeedAnswer, AnswerCom) == 0 ? true : false;
}

int Sim900_cmp_strs_with_time_out(char* NeedAnswer)
{

	char answer_COM[SIZE_BUFFER] = { 0 };
	char* tmp = &answer_COM[0];
	if (!NeedAnswer)
	{
		while (*tmp == '\0')
			Sim900_read_COM(&tmp, false);
		return 1;
	}
	while (*tmp == '\0')
		Sim900_read_COM(&tmp, false);
	return strcmp(NeedAnswer, answer_COM) == 0 ? true : false;
}

void Sim900_answer()
{
	char answer[SIZE_BUFFER] = { 0 };
	char* tmp = &answer[0];
	Sim900_read_COM(&tmp, false);
	cout << answer << endl;
}

int Sim900_conn_to_server() ///0 - �����������
{
	cout << "_________________________________\n";
	int stepCount = 7;
	int i = 0;

	char* tmp = NULL;
	enum STEP { at = -1, 
		cipshut = 0, 
		cipmux = 1, 
		connect_inet = 2, 
		ciicr = 3, 
		get_ip = 4, 
		cipstart = 5, 
		init2server = 6 
	};
	STEP Step = at; ///enum �� ������ ���
	char EndSymbol26 = END_SEND_SYMBOL_26;
	
	for (Step = at, i = -1; i < stepCount; i++)
	{
		Step = (STEP)i;
		switch (Step)
		{
		case at:
		{
			Sim900_write_cmd(cmd(ATV1));
		//	Sim900_read_COM(&tmp, false);
		//	cout << "Step 1: " << tmp << endl;
			break;
		}
		case cipshut:
		{
			Sim900_write_cmd(cmd(CIPSHUT));
		//	Sim900_read_COM(&tmp, false);
		//	cout << "Step 2: " << tmp << endl;
			break;
		}
		case cipmux:
		{
			Sim900_write_cmd(cmd(CIPMUX));
		//	Sim900_read_COM(&tmp, false);
		//	cout << "Step 3: " << tmp << endl;
			break;
		}
		case connect_inet:
		{
			Sim900_write_cmd(cmd(CONNECT_INET));
		//	Sim900_read_COM(&tmp, false);
		//	cout << "Step 4: " << tmp << endl;
			break;
		}
		case ciicr:
		{
			Sim900_write_cmd(cmd(CIICR));
		//	Sim900_read_COM(&tmp, false);
		//	cout << "Step 5: " << tmp << endl;
			break;
		}
		case get_ip:
		{
			Sim900_write_cmd(cmd(GET_IP));
		//	Sim900_read_COM(&tmp, false);
		//	cout << "Step 6: " << tmp << endl;
			break;
		}
		case cipstart:
		{
			Sim900_write_cmd(cmd(CIPSTART));
		//	Sim900_read_COM(&tmp, false);
		//	cout << "Step 7: " << tmp << endl;
			break;
		}
		case init2server:
		{
			////��������� ������� ������ � ����
			int tmp2 = 0;
			tmp2 = Sim900_send_msg(Sim900_new_msg(NEW_PP, 0));
			// TODO �����������, ��� � ���

			cout << "Step 7: " << tmp2 << endl;
			break;
		}
		default:
			return 0;
		}
	}
	if (!Sim900_autotification())
	{
		cout << "������ �������������" << endl;
		return 0;
	}
	
	is_connect_to_server = true;

	return 0;
}

int Sim900_autotification()
{
	char _tmp[10] = { 0 };
	char answer[10] = { 0 };
	char* _answ = &answer[0];

	strncpy(_tmp, password.c_str(), password.length() + 1);
	Sim900_write_cmd(_tmp);
	Sim900_read_COM(&_answ, false);

	_print_answer(answer, false);
	return 0;
}

//�������� ���������
int Sim900_send_msg(stMainHeader* msg)
{
	// ������ ����� ������� �����
	// �������� ��������� ������� � �������
	// ��������� �� COM �����
	// ������ ������
	return 1;
}

stMainHeader* Sim900_new_msg(int cmd, BYTE data)
{
	//int size_next_msg = len(data) / SIZE_BUFFER;
	stMainHeader* new_msg = new stMainHeader();
	new_msg->id = next_id;
	new_msg->cmd = cmd;
	new_msg->sender = SERIAL_NUMBER;
	new_msg->size_next_msg = 0;
	new_msg->data = data;
	
	next_id++;

	return new_msg;
}

int Sim900_put_char(int ASCIIch)
{
	char ch = ASCIIch;
	cout << "������� ������: " << ASCIIch << "������: " << ch << endl;

	WriteFile(h_com, &ch, 1, &bWritten, NULL);
	return bWritten == 1 ? true : false;
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
		Sim900_read_COM(&tmp, false);
	}
	if (strcmp(answerCOM, OK) == 0)
	{
		while (*tmp == '\0')
		{
			Sim900_read_COM(&tmp, false);
		}
		if (strcmp(answerCOM, CONN_OK) == 0)
		{
			return true;
		}
	}
	char ok_conn_ok[100] = { 0 };
	strcat(&ok_conn_ok[0], OK);
	strcat(&ok_conn_ok[strlen(ok_conn_ok)], CONN_OK);
	if (strcmp(answerCOM, ok_conn_ok) == 0)
	{
		return true;
	}
	if (strcmp(answerCOM, "\r\nOK\r\n\r\nCONNECT OK\r\n"))
		return true;
	else return false;
}

//������
bool Sim900_set_echo_behaviour()
{
	Sim900_write_cmd(cmd(ATE0)); ///������� ������
	Sim900_answer();
	return true;
}

bool Sim900_set_ATV0()
{
	Sim900_write_cmd(cmd(ATV0));
	Sim900_answer();
	return true;
}

bool Sim900_set_ATV1()
{
	Sim900_write_cmd(cmd(ATV1));
	Sim900_answer();
	return true;
}

int Sim900_�lose_app()
{
	///������� ������ ������
	for (int i = 0; i < cnt_command; i++)
	{
		free(�ommands_AT[i]);
	}
	for (int i = 0; i < cnt_answer; i++)
	{
		free(answer_AT[i]);
	}
	// �������� ��������� �� ������ � ����������� ������ (���� ���� �����)
	// ������� �����
	// ������� ������
	// ������� COM ����
	cout << "�� ����� ������))" << endl;
	return 0;
}

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
		cout << "������� �������\n";
		cin >> cmd;
		Sim900_add_end_symbol(cmd);
		ENTER_CRITICAL_SECTION;
		Sim900_write_cmd(cmd);
		Sim900_answer();
		LEAVE_CRITICAL_SECTION;
		while (true)
		{
			MEM_SET(answer);
			cout << "����������? (y, n)\n";
			cin >> answer;
			if (strlen(answer) == 1 && answer[0] == 'y') break;
			if (strlen(answer) == 1 && answer[0] == 'n') { is_end = true; break; }
			cout << "��������� ����";
		}
	}
	return 0;
}

