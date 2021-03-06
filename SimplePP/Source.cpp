#include "stdafx.h"

#include "GSM.h"
#include "Sim_900.h"

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
	// ������ ������
	// ������� ���������
	// ������������� �� � ������ ���� ��� ��������
	// ���������
	// ��������� �����
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
		cout << "�� ����� �������� ������" << endl;
		return answer;
	}
	return answer;
}

bool _get_cmd_and_data(int* cmd, int* data)
{
	int _cmd = 0, _data = 0;
	while (true)
	{
		_cmd = _get_console_data("������� ����� �������:\n");
		_data = _get_console_data("������� ������\n");
		if (_cmd == -1 || _data == -1)
		{
			cout << "������ �� ���������" << endl;
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
	if (m_ovRx.hEvent)	CloseHandle(m_ovRx.hEvent);
	if (m_ovTx.hEvent)	CloseHandle(m_ovTx.hEvent);
	return true;
}

