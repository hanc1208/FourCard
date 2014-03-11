#include "FourCardServer.h"
//?�창??개트롤러

//hanc is real troller

FourCardServer::FourCardServer(void) : m_file_log("FourCardServerLog.txt", ios::app)
{
	m_mysql.connect("127.0.0.1", "root", "root", "fourcard", 3306);
}

FourCardServer::~FourCardServer(void)
{
	m_mysql.disconnect();
}

ClientData* FourCardServer::onAccept(SOCKET socket) // onAccept ���¿����� m_socket�� �ʱ�ȭ�������� ���� �Դϴ�. �ٲܱ�? �ȵ� ��������
{
	m_file_log << stringf("FourCardServer::onAccept > %s", getIPFromSocket(socket)) << endl;
	
	ClientData* client_data = new ClientData;
	client_data->m_socket = socket; // onAccept���� �ƹ��� �ൿ�� �������� ������� �� ���� �����ص� �˴ϴ�.

	return client_data;
}

void FourCardServer::onAcceptFailed(const ClientData* client_data)
{
	m_file_log << stringf("FourCardServer::onAcceptFailed > %s", getIPFromSocket(client_data->m_socket)) << endl;
	delete client_data;
}

void FourCardServer::onClose(const ClientData* client_data)
{
	m_file_log << stringf("FourCardServer::onClose > %s", getIPFromSocket(client_data->m_socket)) << endl;
	delete client_data;
}

void FourCardServer::onRead(const ClientData* client_data, Packet& packet)
{
	switch(packet.getID())
	{
	case Protocol::DB_QUERY:
		{
			int32_t awr;
			string query;

			

			packet >> awr >> query;
			packet.clear();

			if(m_mysql.query(query.c_str())) {
				packet << "query ����";
			}

			 packet << "query ����";

			
			
			/*if(awr==0)
				packet << "no���";

			if(awr==1)
				packet << "yes���";*/

			this->async_send((ClientData*) client_data, packet);
			break;
		}
	case Protocol::DB_RESULT:
		{
			string query;
			packet >> query;

			map<string, string> query_result;
			m_mysql.result(query.c_str(), query_result);

			packet.clear();
			packet << (int32_t) query_result.size();

			for(auto it = query_result.begin(); it != query_result.end(); it++) {
				packet << it->first << it->second;
			}

			this->async_send((ClientData*) client_data, packet);

			break;
		}

	case Protocol::LOGIN:
		{
			string id;
			string pw;
			packet >> id >> pw;
			packet.clear();
			
			packet << "id: " + id + "  pw: " + pw + "!";
			packet << "insert into \'users\' values (\'ID\', \'PW\', \'name\', \'email\', \'address\', \'phone\');";

			this->async_send((ClientData*) client_data, packet);
			
			break;
		}

	case Protocol::REGISTER:
		{
			string id;
			string pw;
			string check;

			break;
		}


	case Protocol::LOGOUT:
		{
			string id;

			packet >> id;

			packet.clear();
			packet << id;

			this->async_send((ClientData*) client_data, packet);

			

			break;
		}
	case -1:
		{
			string content;
			int32_t int_content;

			packet >> content >> int_content;

			packet.clear();
			packet << content + "!" << int_content + 1;

			this->async_send((ClientData*) client_data, packet);

			break;
		}
	}
}

void FourCardServer::onError(const char* message)
{
	m_file_log << message << endl;
}

void FourCardServer::onWarning(const char* message)
{
	m_file_log << message << endl;
}

void FourCardServer::onLog(const char* message)
{
	m_file_log << message << endl;
}
