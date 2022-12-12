#include "Server.h"

Server* Server::ptr = NULL;

Server::Server(void)
{
	ptr = this;
}

Server::~Server(void)
{
	closesocket(_socket);
	closesocket(acceptSocket);
	WSACleanup();
}

BOOL Server::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hDialog = hwnd;
	HINSTANCE hInst = GetModuleHandle(NULL);

	//получааем дескрипторы Edit
	hEdit[0] = GetDlgItem(hwnd, IDC_EDIT1);
	hEdit[1] = GetDlgItem(hwnd, IDC_EDIT2);
	return TRUE;
}

DWORD WINAPI AcceptMessage(LPVOID lp) {
	Server* ObjPtr = (Server*)lp;
	while (true) {
		ObjPtr->acceptSocket = accept(ObjPtr->_socket, NULL, NULL);
		int ClientMessage = recv(ObjPtr->acceptSocket, (char*)ObjPtr->buf, ObjPtr->MAXSTRLEN, 0);
		ObjPtr->buf[ClientMessage] = '\0';
		SendMessage(ObjPtr->hEdit[0], WM_SETTEXT, NULL, (LPARAM)ObjPtr->buf);
		MessageBox(ObjPtr->hDialog, (LPCWSTR)ObjPtr->buf, TEXT("Клиент"), MB_OK);
	}
	return 0;
}

void Server::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (id == IDC_BUTTON1)
	{
		char str[MAXSTRLEN];
		GetWindowText(hEdit[0], (LPWSTR)str, MAXSTRLEN);
		send(acceptSocket, str, strlen(str), 0);
	}
	else if (id == IDC_BUTTON2)
	{
		//инициализация сокетов
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) {
			MessageBox(hwnd, TEXT("Ошибка WSAStartup!"), TEXT("Сервер"), MB_OK);
			WSACleanup();
			exit(1);
		}
		_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (_socket == INVALID_SOCKET) {
			MessageBox(hwnd, TEXT("Ошибка запуска сервера!"), TEXT("Сервер"), MB_OK);
			WSACleanup();
			exit(2);
		}
		addr.sin_family = AF_INET;
		inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);
		addr.sin_port = htons(20000);
		if (bind(_socket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
			MessageBox(hwnd, TEXT("Ошибка биндинга сервера!"), TEXT("Сервер"), MB_OK);
			WSACleanup();
			exit(3);
		}
		listen(_socket, 1);
		if (listen(_socket, 1) == SOCKET_ERROR) {
			MessageBox(hwnd, TEXT("Ошибка прослушки сервера!"), TEXT("Сервер"), MB_OK);
			WSACleanup();
			exit(4);
		}
		MessageBox(hwnd, TEXT("Сервер запустился."), TEXT("Сервер"), MB_OK);
		//Получаем сообщение от клиента				
		CreateThread(NULL, 0, AcceptMessage, this, 0, NULL);
	}
	else if (id == IDC_BUTTON3)
	{
		//Закрываем сокет
		closesocket(_socket);
		closesocket(acceptSocket);
		WSACleanup();
		EndDialog(hwnd, 0);
	}
}

BOOL CALLBACK Server::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}
