#pragma once
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <regex>
#include <cstdint>

#pragma warning(disable: 4996)
#pragma warning(disable: 4005)

using namespace std;
//#define REALIZE_LIBRARY_USING_MYSQL

#define USING_REALIZE_LIBRARY using namespace Realize;
#define USING_REALIZE_LIBRARY_SOCKET using namespace Realize::Socket;

#ifdef WIN32
	#include <WinSock2.h>
	#include <Windows.h>
	#include <process.h>
	#pragma comment(lib, "ws2_32.lib")
	typedef int				socklen_t;
#else
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <arpa/inet.h>
	#include <errno.h>
	typedef int				SOCKET;

	//#pragma region define win32 const variable in linux
	#define INVALID_SOCKET	-1
	#define SOCKET_ERROR	-1
	//#pragma endregion
#endif

namespace Realize {
	// stringf : sprintf�� ���� ����������, ������� �������ݴϴ�.
	// format : sprintf�� ���˰� �����ϴ�
	string stringf(const char* format, ...);
	
	string _itotstring(int i);

	int stringtoi(const string& lpszString);
#ifdef WIN32
	class CFont
	{
	private:
		HFONT hOldFont;
	public:
		HDC hDC;
		HFONT hFont;
		LOGFONT LogFont;

		// ��Ʈ�� �ٲ� �ִ� Ŭ���� �Դϴ�.
		// �Ҹ��ڿ� ���� �ڵ����� �ڵ��� �Ҹ�Ǵ� �޸� ������ ���� ������ �����ϴ�.
		// ��, ���������� �����ϰ� �����쿡 �ڵ��� �ѱ��� DeleteObject�� ����ǹ����� ���� �����Ϳ� �Ҵ��Ͽ� �����츦 �ı��Ҷ� �� Ŭ������ delete�Ͻø� �˴ϴ�.
		CFont() { }
		// ��Ʈ�� �����մϴ�.
		// int cHeight : ��Ʈ�� ũ�� (�ȼ�����)
		// lpszFacename : ��Ʈ �̸� (����, ...)
		// cWeight : ��Ʈ ���� (FW_ �� ����)
		// bItalic : �����, bUndeline : ����, bStrikeOut : ��Ҽ�
		CFont(int cHeight, LPCTSTR lpszFacename, int cWeight = FW_NORMAL, BOOL bItalic = FALSE, BOOL bUnderline = FALSE, BOOL bStrikeOut = FALSE);
		// HDC�� ��Ʈ�� �ڵ����� ����ϵ��� �մϴ�.
		// int cHeight : ��Ʈ�� ũ�� (�ȼ�����)
		// lpszFacename : ��Ʈ �̸� (����, ...)
		// cWeight : ��Ʈ ���� (FW_ �� ����)
		// bItalic : �����, bUndeline : ����, bStrikeOut : ��Ҽ�
		CFont(HDC hDC, int cHeight, LPCTSTR lpszFacename, int cWeight = FW_NORMAL, BOOL bItalic = FALSE, BOOL bUnderline = FALSE, BOOL bStrikeOut = FALSE);
		// ���� ��Ʈ Ŭ������ ��Ʈ�� hWnd�� �����մϴ�.
		void SetFont(HWND hWnd);
		// ���� Ŭ������ ��Ʈ�� �����ϰ� �� ��Ʈ�� �����մϴ�.
		void NewFont(int cHeight, LPCTSTR lpszFacename, int cWeight = FW_NORMAL, BOOL bItalic = FALSE, BOOL bUnderline = FALSE, BOOL bStrikeOut = FALSE);
		~CFont();
	};
#endif
	int send(SOCKET s, const char* buf, int len);
	int recv(SOCKET s, char* buf, int len);
#ifdef WIN32
	void SetConsoleColor(int fontcolor, int bgcolor);

	void ErrorMessage(HWND hWnd, const string& lpszMessage);
	void PrintMessage(HWND hWnd, const string& lpszMessage);

	void AppendWindowText(HWND hWnd, LPCTSTR lpszString);

	void WriteLog(const string& lpszLog, HWND hEdit, fstream& hLogFile);
#endif
	void WriteLog(const string& lpszLog, fstream& hLogFile);

#define Socket_BufferSize 1024
#define Socket_Timeout 60000
#define Socket_Timeout_usec 0

#define Realize_Library_Socket_Read_Listener_Bind(__listener__) std::bind(&__listener__, this, std::placeholders::_1, std::placeholders::_2)

#define CheckSocketError(retval) ((retval) < (int)(Socket_BufferSize * sizeof(char)))
#define HandleThreadSocketError(retval, socket) if(CheckSocketError((retval))) { delete (socket); return 0; }
#ifdef WIN32
	void HandleError(HWND hWnd, const string& lpszString, HWND hEdit, fstream& hLogFile);
	inline bool HandleSocketError(int retval, HWND hWnd, const string& lpszString, HWND hEdit, fstream& hLogFile);
#endif
	string getpeername(SOCKET Socket);

	namespace Socket
	{
#define IO_SEND 1
#define IO_RECV 2
		/*
		��Ŷ ����

		[ ��� ���� ]
		4����Ʈ : ��Ŷ ũ�⸦ �����ϴ� �� �κ��� ������ ������ ��Ŷ�� ũ��
		4����Ʈ : �ĺ���
		4����Ʈ : �Ű������� ����

		[ �Ű����� ���� ]
		4����Ʈ : �Ű������� ũ��
		n����Ʈ : �Ű�����
		*/
		class Packet
		{
		public:
			char* packet;
			long size_of_packet;

			Packet();
			Packet(const int32_t id, const int32_t number_of_data, ...);
			Packet(const Packet& SrcPacket);
			Packet& operator=(const Packet& SrcPacket);
			void Build(const int32_t id, const int32_t number_of_data, ...);
			int Build(SOCKET Socket);
			const int32_t GetID() const;
			vector<string> GetData() const;
			const char* GetPacket() const;
			string ToString() const;
			~Packet();
		};
		struct SOCKET_INFO
		{
			SOCKET Socket;
			void* Info;
		};
#ifdef WIN32
		struct IO_DATA : WSAOVERLAPPED
		{
			WSABUF WSABuf;
			int code;
		};
#endif
		class Client
		{
		private:
			SOCKET Socket;

			fstream& hLogFile;

			function<void(const char*)> listener_error;
		public:
			// ���� ���� Ŭ���̾�Ʈ�� �����մϴ�.
			// hWnd : ���� �޽����� ����� ������ �ڵ� �Դϴ�.
			Client(const char* ip, int port, fstream& hLogFile, function<void(const char*)> listener_error);
			int GetError();
			int Close();
			int Clean();
			int Send(const Packet& );
			Packet Receive(int* lpError = NULL);
			void setErrorListener(const std::function<void(const char*)>& listener);
			~Client();
		};
#ifdef WIN32
		class Server 
		{
		private:
			WSADATA Wsa;
			SOCKADDR_IN SocketAddress;
			SOCKET Socket;

			HWND hWnd;

			HWND hLog;
			fstream& hLogFile;

			HANDLE* hThreads;
			int dwNumberOfThreads;

			HANDLE hIOCP;
			list<SOCKET_INFO*> Socket_Info_List;
			list<IO_DATA*> IO_Data_List;

			CRITICAL_SECTION CS;

			map<int32_t, std::function<void(SOCKET_INFO*, const Packet&)>> listener_read;
			std::function<void(SOCKET_INFO*)> listener_accept;
			std::function<void(SOCKET_INFO*)> listener_close;
		public:
			void* ParameterEx;

			// ���� ���� ������ �����մϴ�.
			// hWnd : ���� �޽����� ����� ������ �ڵ� �Դϴ�.
			// hLog : �α׸� ���� Edit �ڵ� (NULL ����)
			// lpszLogFileName : �α׸� ���� ������ �̸� (NULL ����)
			// lpOnRead : ������ ��Ŷ�� �޾��� �� ȣ��Ǵ� �ݹ� �Լ� �Դϴ�. [] (void*) -> unsigned int __stdcall { } ������ lambda�� �ѱ�� �˴ϴ�.
			// lambda�� �Ű������� ��Ŷ�� ���� Socket_Info �� �־����ϴ�. �� Socket_Info�� �� delete���ֽñ� �ٶ��ϴ�.
			Server(char* ip, int port, HWND hWnd, HWND hLog, fstream& hLogFile);

			static unsigned int __stdcall IOCPThreadRoutine(void* Parameter);
			static unsigned int __stdcall AcceptThreadRoutine(void* Parameter);
			int Send(SOCKET Socket, const Packet& Packet);
			static int Receive(SOCKET Socket, IO_DATA* IO_Data);
			void RemoveSocket(SOCKET_INFO** Socket_Info, IO_DATA** IO_Data);
			void Start();

			void addReadListener(const int32_t id, const std::function<void(SOCKET_INFO*, const Packet&)>& listener);
			void setAcceptListener(const std::function<void(SOCKET_INFO*)>& listener);
			void setCloseListener(const std::function<void(SOCKET_INFO*)>& listener);

			~Server();
		};
#endif
	}

#ifdef REALIZE_LIBRARY_USING_MYSQL

#include "C:\xampp\mysql\include\mysql.h"
#pragma comment(lib, "C:\\xampp\\mysql\\lib\\libmysql.lib")

#define _tmysql_error(MySQL) mysql_error(MySQL).c_str()

#define MySQL_Host "127.0.0.1"
#define MySQL_User "root"
#define MySQL_Password "root"
#define MySQL_DB "SilenceStory"
#define MySQL_Port 3306

	class SilenceStoryMySQL
	{
	private:
		HWND hWnd;

		HWND hLog;
		fstream& hLogFile;

		MYSQL* MySQL_Connection;
	public:
		bool isError;

		SilenceStoryMySQL(HWND hWnd, HWND hLog, fstream& hLogFile);
		vector<map<string, string>> Select(string MySQL_Table, string MySQL_Column, string MySQL_Where, string MySQL_Description = string());
		string Result(string Query);
		bool Update(string MySQL_Table, string MySQL_Set, string MySQL_Where, string MySQL_Description);
		bool Delete(string MySQL_Table, string MySQL_Where, string MySQL_Description);
		bool Insert(string MySQL_Table, string MySQL_Columns, string MySQL_Values);
		~SilenceStoryMySQL();
	};

#endif

}
