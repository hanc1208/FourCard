#pragma once

enum class Protocol
{
	// Send : ����(string)
	// Receive : ����� ����(null)
	// Todo : ������ ó���մϴ�.
	DB_QUERY,
	// Send : ����(string)
	// Receive : �� ����(int32_t), �� �̸�(string), ��(string), ...
	// Todo : ������ ó���� �� ������� �����ϴ�.
	DB_RESULT,
	// Send : ����(string)
	// Receive : �� ����(int32_t), �� ����(int32_t), �� �̸�(string), �� (string), ..., �� ����(int32_t), ...
	// Todo : ������ ó���� �� ������� �����ϴ�.
	DB_RESULT_ROWS,
#define PROTOCOL_LOGIN_SUCCESS 1
#define PROTOCOL_LOGIN_FAIL 2
	// Send : ���̵�(string), ��й�ȣ(string)
	// Receive : ���(int32_t), [�޽���(string)]
	// Todo : �α����� ó���� �� ����� �����ϴ�.
	// Todo : ���(int32_t)
	// Todo : PROTOCOL_LOGIN_SUCCESS = ����, PROTOCOL_LOGIN_FAIL = ����
	// Todo : ���н� �޽����� �����ϴ�.
	LOGIN,
	// Send : ���̵�(string)
	// Receive : ����� ����(null)
	// Todo : �α׾ƿ��� ó���մϴ�.
	LOGOUT,
#define PROTOCOL_REGISTER_SUCCESS 1
#define PROTOCOL_REGISTER_FAIL 2
	// Send : �� ����(int32_t), �� �̸�(string), ��(string), ...
	// Receive : ���(int32_t), [�޽���(string)]
	// Todo : ȸ�������� ó���� �� ����� �����ϴ�.
	// Todo : ���(int32_t)
	// Todo : PROTOCOL_REGISTER_SUCCESS = ����, PROTOCOL_REGISTER_FAIL = ����
	// Todo : ���н� �޽����� �����ϴ�.
	REGISTER,
	// Send : ������ �� ����(null)
	// Receive : �� ����(int32_t), ����(string), �·�(float), ...
	// Todo : ��ŷ ����(�·�)�� �����ϴ�. (10�� ����, 30�� �̻� ����)
	RANK_WINRATE,
	// Send : ������ �� ����(null)
	// Receive : �� ����(int32_t), ����(string), �¼�(int32_t), ...
	// Todo : ��ŷ ����(�ٽ�)�� �����ϴ�. (10�� ����)
	RANK_WIN,
	// Send : ������ �� ����(null)
	// Receive : �� ����(int32_t), ����(string), �� ���� ��(int32_t), ...
	// Todo : ��ŷ ����(����)�� �����ϴ�. (10�� ����)
	RANK_TOTAL,
	// Send : ������ �� ����(null)
	// Receive : ����(int32_t)
	// Todo : �ֽ� ������ �����ϴ�.
	VERSION
};