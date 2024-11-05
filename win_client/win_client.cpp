// win_client.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include "network.h"
#include <string>
#include <cstdlib>
#include <thread>
#include <chrono>

#define IP_ADRES "127.0.0.1"

/// <summary>
/// структура входных параметров командной строки
/// </summary>
struct InputParam_t
{
	std::string m_s_name; // имя клиента
	unsigned m_u32_port; // порт подключения
	unsigned m_u32_timeOut; // время переподключения
	InputParam_t() : m_s_name(""), m_u32_port(0), m_u32_timeOut(0)
	{}
};

/// <summary>
/// функция разобра параметров командной строки
/// </summary>
/// <param name="argc"> - количество параметров </param>
/// <param name="argv"> - массив параметров </param>
/// <param name="r_param"> - ссылка на структуру входных параметров </param>
/// <returns> 1 - праметры распознаны </returns>
bool parseParam(int argc, char* argv[], InputParam_t& r_param);

int main(int argc, char* argv[])
{
	printf("run_client\n");
	InputParam_t h_param;

	if (parseParam(argc, argv, h_param))
	{
		log_t h_logger;
		std::string s_msg; // сообщение для передачи серверу
		std::chrono::seconds sec{ h_param.m_u32_timeOut };// задаем время выходя из спящего режима

		while (1)
		{
			// создаем сокет с подключением к серверу
			network::TCP_socketClient_t h_client(IP_ADRES, h_param.m_u32_port, h_logger);
			// формируем сообщение
			s_msg.clear();
			s_msg.append(h_logger.getTime() + ' ');
			s_msg.append(h_param.m_s_name);
			// отправляем серверу
			h_client.Send(s_msg);
			// пререходим в спящий режим
			std::this_thread::sleep_for(sec);
		}
	}
	else
		printf("Invalid parametr's. Please enter thera: client_name number_port time_out_sec\n");

	return EXIT_SUCCESS;
}


/// <summary>
/// функция разобра параметров командной строки
/// </summary>
/// <param name="argc"> - количество параметров </param>
/// <param name="argv"> - массив параметров </param>
/// <param name="r_param"> - ссылка на структуру входных параметров </param>
/// <returns> 1 - праметры распознаны </returns>
bool parseParam(int argc, char* argv[], InputParam_t& r_param)
{
	bool b_result = false;

	if (argc == 4)
	{
		r_param.m_s_name.assign(argv[1]);
		r_param.m_u32_port = std::strtoul(argv[2], NULL, 10);
		r_param.m_u32_timeOut = std::strtoul(argv[3], NULL, 10);
		b_result = !r_param.m_s_name.empty() && r_param.m_u32_port != 0 && r_param.m_u32_timeOut != 0
			&& r_param.m_u32_port != ULONG_MAX && r_param.m_u32_timeOut != ULONG_MAX;
	}

	return b_result;
}
