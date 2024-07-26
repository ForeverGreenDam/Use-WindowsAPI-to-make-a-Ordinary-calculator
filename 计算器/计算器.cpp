#include "windows.h"
#include <stdio.h>
#include <cmath>
#include <string.h>
#include <wchar.h>
#define MAX 0XFF
double ans1(TCHAR *szBuffer1);
void Trans(int ans, TCHAR *szAns1);
void Answer(TCHAR *szBuffer1, TCHAR *szAns1, TCHAR *szAnsDb);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {

	static TCHAR szAppName[] = TEXT("普通计算器");
	HWND hwnd = 0;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;


	if (!RegisterClass(&wndclass)) {
		MessageBox(hwnd, TEXT("抱歉，本程序需要在Windows NT上运行！"), szAppName, MB_ICONERROR);
		return 0;
	}
	hwnd = CreateWindow(szAppName, TEXT("普通计算器"),
	                    WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
	                    CW_USEDEFAULT, CW_USEDEFAULT,
	                    CW_USEDEFAULT, CW_USEDEFAULT,
	                    NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static HWND  hedit = 0, hforthline[4];
	static int cxChar, cyChar, CharNum = 0;
	static const wchar_t *FTL[] = {TEXT("7"), TEXT("8"), TEXT("9"), TEXT("+") },
	                              *SCL [] = { TEXT("4"), TEXT("5"), TEXT("6"), TEXT("-") },
	                                        * TDL[] = { TEXT("1"), TEXT("2"), TEXT("3"), TEXT("×") },
	                                                * FOL[] = { TEXT("="), TEXT("0"), TEXT("."), TEXT("÷") }; //分别建立四行按钮的文本
	int i, id;
	static TCHAR szBuffer[100] = { '\0' };//算式缓冲区
	static TCHAR szAns[30] = {'\0'};//答案整数缓冲区
	static TCHAR szAnsDouble[30] = { '\0' };//答案小数缓冲区
	switch (message) {
		case WM_CREATE:
			cxChar = LOWORD(GetDialogBaseUnits());
			cyChar = HIWORD(GetDialogBaseUnits());
			MoveWindow(hwnd, 600, 70, 86 * cxChar, 52 * cyChar, TRUE);

			//创建窗口，按钮，微调各组件的像素位置
			//编辑框设为只读
			hedit = CreateWindow(TEXT("edit"), TEXT(""),
			                     WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_RIGHT,
			                     2 * cxChar, 2 * cyChar,
			                     80 * cxChar, 4 * cyChar,
			                     hwnd, (HMENU)10,
			                     ((LPCREATESTRUCT)lParam)->hInstance, NULL);
			for (int i = 0; i < 4; i++)
				CreateWindow(TEXT("button"), FTL[i],
				             WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				             (2 + 20 * i) * cxChar, 8 * cyChar,
				             20 * cxChar, 10 * cyChar,
				             hwnd, (HMENU)100 + i,
				             ((LPCREATESTRUCT)lParam)->hInstance, NULL);
			for (int i = 0; i < 4; i++)
				CreateWindow(TEXT("button"), SCL[i],
				             WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				             (2 + 20 * i) * cxChar, 18 * cyChar,
				             20 * cxChar, 10 * cyChar,
				             hwnd, (HMENU)200 + i,
				             ((LPCREATESTRUCT)lParam)->hInstance, NULL);
			for (int i = 0; i < 4; i++)
				CreateWindow(TEXT("button"), TDL[i],
				             WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				             (2 + 20 * i) * cxChar, 28 * cyChar,
				             20 * cxChar, 10 * cyChar,
				             hwnd, (HMENU)300 + i,
				             ((LPCREATESTRUCT)lParam)->hInstance, NULL);

			for (int i = 0; i < 4; i++)
				hforthline[i] =	CreateWindow(TEXT("button"), FOL[i],
				                             WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				                             (2 + 20 * i) * cxChar, 38 * cyChar,
				                             20 * cxChar, 10 * cyChar,
				                             hwnd, (HMENU)400 + i,
				                             ((LPCREATESTRUCT)lParam)->hInstance, NULL);

			return 0;
		case WM_COMMAND:
			//获取触发按钮的ID值，（暂时不清楚为何相邻两个按钮的ID相差4，理论来讲应该相差1，经调试发现实际相差4）
			//将按钮上的数字以字符的形式保存在算式缓冲区中，并且将算式缓冲区所有内容在编辑框显示
			id = LOWORD(wParam);
			switch (id) {
				case 100:
					szBuffer[CharNum] = '7';
					CharNum++;
					SetWindowText(hedit, szBuffer);
					break;
				case 104:
					szBuffer[CharNum] = '8';
					CharNum++;
					SetWindowText(hedit, szBuffer);
					break;
				case 108:
					szBuffer[CharNum] = '9';
					CharNum++;
					SetWindowText(hedit, szBuffer);
					break;
				case 112:
					szBuffer[CharNum] = '+';
					CharNum++;
					SetWindowText(hedit, szBuffer);
					break;
				case 200:
					szBuffer[CharNum] = '4';
					CharNum++;
					SetWindowText(hedit, szBuffer);
					break;
				case 204:
					szBuffer[CharNum] = '5';
					CharNum++;
					SetWindowText(hedit, szBuffer);
					break;
				case 208:
					szBuffer[CharNum] = '6';
					CharNum++;
					SetWindowText(hedit, szBuffer);
					break;
				case 212:
					szBuffer[CharNum] = '-';
					CharNum++;
					SetWindowText(hedit, szBuffer);
					break;
				case 300:
					szBuffer[CharNum] = '1';
					CharNum++;
					SetWindowText(hedit, szBuffer);
					break;
				case 304:
					szBuffer[CharNum] = '2';
					CharNum++;
					SetWindowText(hedit, szBuffer);
					break;
				case 308:
					szBuffer[CharNum] = '3';
					CharNum++;
					SetWindowText(hedit, szBuffer);
					break;
				case 312:
					szBuffer[CharNum] = '*';
					CharNum++;
					SetWindowText(hedit, szBuffer);
					break;
				//当点击“=”号时，先将之前答案缓冲区清空，再执行计算，计算完成后输出答案，清空算式缓冲区
				case 400:
					szBuffer[CharNum] = '=';
					for ( i = 0; i < 30; i++) {
						szAns[i] = '\0';
						szAnsDouble[i] = '\0';
					}
					CharNum = 0;
					Answer(szBuffer, szAns, szAnsDouble);
					SetWindowText(hedit, szAns);
					for (i = 0; i < 100; i++) {
						szBuffer[i] = '\0';
					}
					break;
				case 404:
					szBuffer[CharNum] = '0';
					CharNum++;
					SetWindowText(hedit, szBuffer);
					break;

				case 408:
					szBuffer[CharNum] = '.';
					CharNum++;
					SetWindowText(hedit, szBuffer);
					break;
				case 412:
					szBuffer[CharNum] = '/';
					CharNum++;
					SetWindowText(hedit, szBuffer);
					break;
				default:
					return DefWindowProc(hwnd, message, wParam, lParam);

			}

			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

//主要的计算函数，将字符串转化为数字计算
double ans0(TCHAR *szBuffer1) {

	int j = 0;
	bool flag = FALSE;
	//判断算式的有效性,防止接下来进入死循环
	for ( j = 0; j < sizeof(szBuffer1); j++) {
		if (szBuffer1[j] == '+' || szBuffer1[j] == '-' || szBuffer1[j] == '*' || szBuffer1[j] == '/')
			flag = TRUE;
	}
	if (!flag)
		return MAX - 1;
	//获取运算符所在位置，以便将前一部分与后一部分运算数转化为int类型
	j = 0;
	while (szBuffer1[j] != '+' && szBuffer1[j] != '-' && szBuffer1[j] != '*' && szBuffer1[j] != '/')
		j++;
	int k, l;
	int point1 = 0, point2 = 0, Lenth = 0;
	double addnum1 = 0, addnum2 = 0;
	double adddouble1 = 0, adddouble2 = 0;
	//判断算式长度
	while (szBuffer1[Lenth] != '=') {
		Lenth++;
	}
	//找到第一个小数点的位置
	while (szBuffer1[point1] != '.' && szBuffer1[point1] != '=') {
		point1++;
	}
	//没找到第一个小数点，就把point1放在运算符处
	if (point1 > j) {
		point1 = j;
		adddouble1 = 0;
	}
	//从第一个小数点起，找到第二个小数点的位置
	point2 = point1 + 1;
	while (szBuffer1[point2] != '.' && szBuffer1[point2] != '=' && point2 <= Lenth) {
		point2++;
	}

	//如果没有第二个小数点，把point2放在‘=’处
	if (point2 >= Lenth) {
		adddouble2 = 0;
		point2 = Lenth;
	}
	//由于之前采用字符保存数字，所以转化时要减去48（ASCII表转化）
	for (k = 0; k < point1 && k < j; k++)

		addnum1 += (szBuffer1[k] - 48) * pow(10, min(point1, j) - k - 1);

	for ( l = j + 1; l < point2; l++)

		addnum2 += (szBuffer1[l] - 48) * pow(10, point2 - l - 1);

	//新增添小数部分计算
	k = point1 + 1;
	while (k < j) {
		adddouble1 += (szBuffer1[k] - 48) * pow(10, -(k - point1));
		k++;
	}
	l = point2 + 1;
	while (l < Lenth) {
		adddouble2 += (szBuffer1[l ] - 48) * pow(10, -(l - point2));
		l++;
	}
	//根据运算符进行运算
	switch (szBuffer1[j]) {
		case '+':
			return (addnum1 + adddouble1) + (addnum2 + adddouble2);
		case '-':
			return (addnum1 + adddouble1) - (addnum2 + adddouble2);
		case '*':
			return (addnum1 + adddouble1) * (addnum2 + adddouble2);
		case '/':
			//除数不能为零，判断
			if (addnum2 != 0)
				return (addnum1 + adddouble1) / (addnum2 + adddouble2);
			else
				return MAX;

		default:
			return 0;
	}

}

//将数字转化为字符数组，存入答案缓冲区
void Trans(int ans, TCHAR *szAns1) {
	if (ans == MAX - 1)
		wsprintfW(szAns1, TEXT("请输入正确的算式"));
	else if (ans == MAX)
		wsprintfW(szAns1, TEXT("error"));
	else
		wsprintfW(szAns1, L"%d", ans);
}

void Answer(TCHAR *szBuffer1, TCHAR *szAns1, TCHAR *szAnsDb) {
	//如果答案是小数采用另外的转化方法。最终我们保留八位小数
	double temp = ans0(szBuffer1) - (int)ans0(szBuffer1);
	if (temp != 0) {
		Trans((int)ans0(szBuffer1), szAns1);
		Trans((int)(temp * pow(10, 8)), szAnsDb);

		TCHAR Dot[] = { '.', '\0'};

		wcscat_s(szAns1, 30, Dot);
		wcscat_s(szAns1, 30, szAnsDb);
	} else
		Trans((int)ans0(szBuffer1), szAns1);
}