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
	WSADATA wsaData;//структура для хранения информацию
	//о инициализации сокетов
	SOCKET _socket; //дескриптор слушающего сокета
	SOCKET acceptSocket;//дескриптор сокета, который связан с клиентом 
	sockaddr_in addr; //локальный адрес и порт
	char buf[MAXSTRLEN];
};