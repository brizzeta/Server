#pragma once
#include "header.h"

class Server
{
public:
	Server(void);
	~Server(void);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static Server* ptr;
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	HWND hDialog, hEdit[2];
	static const int MAXSTRLEN = 255;
	WSADATA wsaData;//��������� ��� �������� ����������
	//� ������������� �������
	SOCKET _socket; //���������� ���������� ������
	SOCKET acceptSocket;//���������� ������, ������� ������ � �������� 
	sockaddr_in addr; //��������� ����� � ����
	char buf[MAXSTRLEN];
};