#include "Client.h"

Client* Client::ptr = NULL;

Client::Client(void)
{
	ptr = this;
}

Client::~Client(void)
{
	closesocket(_socket);
	WSACleanup();
}

BOOL Client::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hDialog = hwnd;
	HINSTANCE hInst = GetModuleHandle(NULL);

	//получааем дескрипторы Edit
	hEdit[0] = GetDlgItem(hwnd, IDC_EDIT1);
	hEdit[1] = GetDlgItem(hwnd, IDC_EDIT2);
	hEdit[2] = GetDlgItem(hwnd, IDC_EDIT3);
	return TRUE;
}

DWORD WINAPI AcceptMessage(LPVOID lp) {
	Client* ObjPtr = (Client*)lp;
	while (true) {
		int ClientMessage = recv(ObjPtr->_socket, ObjPtr->buf, ObjPtr->MAXSTRLEN, 0);
		ObjPtr->buf[ClientMessage] = '\0';
		SendMessage(ObjPtr->hEdit[1], WM_SETTEXT, NULL, (LPARAM)ObjPtr->buf);
	}
	return 0;
}

void Client::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (id == IDC_BUTTON1)
	{		
		GetWindowText(hEdit[0], (LPWSTR)IP_addr, 20);
	}
	else if (id == IDC_BUTTON2)
	{
		TCHAR buff[MAXSTRLEN];
		GetWindowText(hEdit[2], buff, MAXSTRLEN);
		MessageBox(hwnd, buff, 0, MB_OK);
		send(_socket, (char*)buff, wcslen(buff), 0);
	}
	else if (id == IDC_BUTTON3)
	{
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) {
			MessageBox(hwnd, TEXT("Ошибка WSAStartup!"), TEXT("Клиент"), MB_OK);
			WSACleanup();
			exit(1);
		}
		_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (_socket == INVALID_SOCKET) {
			MessageBox(hwnd, TEXT("Ошибка запуска сервера!"), TEXT("Клиент"), MB_OK);
			WSACleanup();
			exit(2);
		}
		addr.sin_family = AF_INET;
		inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
		addr.sin_port = htons(20000);
		if (connect(_socket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
			MessageBox(hwnd, TEXT("Не удалось подключиться к серверу!"), TEXT("Клиент"), MB_OK);
			WSACleanup();
			exit(3);
		}
		
		
		//Получаем сообщение от клиента
		CreateThread(NULL, 0, AcceptMessage, this, 0, NULL);

		MessageBox(hwnd, TEXT("Подключение прошло успешно."), TEXT("Клиент"), MB_OK);
	}
	else if (id == IDC_BUTTON4)
	{
		//Закрываем сокет
		closesocket(_socket);
		WSACleanup();
		EndDialog(hwnd, 0);
	}
}

BOOL CALLBACK Client::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}
