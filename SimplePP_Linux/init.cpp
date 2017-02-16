
#include "includes.h"

#include "GSM.h"
#include "Sim_900.h"
#include "config.h"
#include "rs232.h"
#include "commands_AT.h"
#include <string>
using namespace std;

int init()
{
	setlocale(LC_ALL, "Russian");
	is_echo = false;
    char mode[]={'8','N','1', 0};
    cout << "mode";

    if(RS232_AsyncComPort(1, RASE, PARITY))
    {
        cout << "Can not open comport\n" << endl;
        return 0;
    }
    cout << "COM port " << COM_PORT << " YES!!!\n" << endl;

    ///тест
    char* wow = new char[5];
    strcpy(wow, "wow\0");
   // console.log(wow);
    int t = 8/0;
    int j = RS232_SendBuf(1, wow, strlen(wow));
    cout << wow;
    delete [] wow;
    unsigned char buf[10] = {0};
    int y = RS232_PollComport(1,buf,3);
    ///тест

   // init_critical_section();
    is_init = true;
    Sim900_set_ATV1();
    Sim900_set_echo_behaviour();

    cout << "inited!\n";

	//Подключение к серверу
	if (!Sim900_conn_to_server())
		return 0;
	is_connect_to_server = true;

	// поток прослушки порта
    /*int result = pthread_create(reciver_thread, NULL, &Sim900_recv_handler, id_reciver_th);
    if (result != 0) {
       perror("Создание потока прослушки порта!");
       return MY_MY_ERROR;
    }*/

	return 0;
}

/*bool init_critical_section()
{
    m_attr = new pthread_mutexattr_t;
    critical_section = new pthread_mutex_t;
    // setup recursive mutex for mutex attribute
    pthread_mutexattr_settype(m_attr, PTHREAD_MUTEX_RECURSIVE_NP);

    // Use the mutex attribute to create the mutex
    pthread_mutex_init(critical_section, m_attr);

    // Mutex attribute can be destroy after initializing the mutex variable
    pthread_mutexattr_destroy(m_attr);
    return true;
}*/

char* cmd(int cmd_)
{
    if(cmd_ > cnt_AT_cmd)
        return NULL;
    char* command = new char[sizeof(strlen(AT_cmd[cmd_].c_str()))];
    memcpy(command, AT_cmd[cmd_].c_str(), strlen(AT_cmd[cmd_].c_str()));
    return command;
}

char* answer(int answ)
{
    if(answ > cnt_AT_answer)
        return NULL;
    char* answer = new char[sizeof(strlen(AT_answer[answ].c_str()))];
    memcpy(answer, AT_answer[answ].c_str(), strlen(AT_answer[answ].c_str()));
    return answer;
}

//вспомогательные

