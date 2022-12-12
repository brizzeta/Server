#pragma once
#include "header.h"

class Client
{
public:
	Client(void);
	~Client(void);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static Client* ptr;
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	HWND hDialog, hEdit[3];
	static const int MAXSTRLEN = 255;
	WSADATA wsaData;
	SOCKET _socket;
	sockaddr_in addr;
	char IP_addr[20];
	char buf[MAXSTRLEN];
};