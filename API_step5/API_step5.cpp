// API_step5.cpp : 격자게임판
//

#include "framework.h"
#include "API_step5.h"

#define MAX_LOADSTRING 100
#define WIN_WIDTH 1280          // 우리가 사용할 윈도우 창 너비
#define WIN_HEIGHT 800          // 우리가 사용할 윈도우 창 높이

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND hWnd;
int gameMap[10][10] = {};

// 더블버퍼링
HDC bufferHdc;
HBITMAP bufferBitmap;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                CreateBackPage(HWND hWnd, HINSTANCE hInstance, HDC* memDC, HBITMAP* memBitmap);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_APISTEP5, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_APISTEP5));

    MSG msg = {};

    // 기본 메시지 루프입니다:
    while (msg.message != WM_QUIT)
    {
        InvalidateRect(hWnd, NULL, false);

        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

    }

    return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APISTEP5));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_APISTEP5);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    // 이번 프로젝트부터는 창의 위치를 이용해야함
    // 윈도우 창을 만들어 보면 생기는 문제: 테두리, 주소 표시줄, 메뉴 등 기타 공간이 자동으로 배치
    // CreateWindowW는 기타 공간 모두 포함한 값으로 창을 만듬
    // 
    // 하지만 화면이 표기되는 부분은 제목 표시줄이나 외곽선 등으로 지정된 범위를 제하고,
    // 남은 범위에서 왼쪽 위를 0,0으로 판정하기 때문에
    // 실제 창의 크기와 우리가 생각하는 좌표가 어긋나
    // 
    // 그래서 처움부터 기타 공간을 고려하고 초기값을 지정해야
    // 
    // GetClientRect는 우니도우 창이 "만들어진 뒤에" 그 크기를 받아오는 함수
    // 따라서 윈도우 창 만드는 함수인 CreateWindowW에는 사용 불가

    RECT rect = { 0, 0, WIN_WIDTH, WIN_HEIGHT };
    // 윈도우의 크기를 갖고 있는 사각형 구조체

    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
    // 사각형 구조체를 받아서, 형식에 맞게 윈도우 창 생성
    // 구조체의 각 값에서 기타 공간을 추가해줌

    // 전역 변수로 선언해둔 hWnd 초기화 (이거 지역 변수 재선언 하지 마라)
    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        CreateBackPage(hWnd, hInst, &bufferHdc, &bufferBitmap);
        gameMap[4][4] = 1;
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

        HBRUSH myBrush = CreateSolidBrush(RGB(255, 255, 0));
        HBRUSH oldBrush = (HBRUSH)SelectObject(bufferHdc, myBrush);

        // 격자판 만들기
        // 1. 선을 그어서 격자를 만들기
        /*
        for (int i = 0; i <= 10; i++)
        {
            for (int j = 0; j <= 10; j++)
            {
                MoveToEx(bufferHdc, 100, 20 + j * 70, NULL);        // 세로선
                LineTo(bufferHdc, 1100, 20 + j * 70);
            }
            for (int j = 0; j <= 10; j++)
            {
                MoveToEx(bufferHdc, 100 + j * 100, 20, NULL);       // 가로선
                LineTo(bufferHdc, 100 + j * 100, 720);
            }
        }

        // 매 칸마다 지정하여 프로퍼티를 바꾸는게 너무 번거로움 (ex. 특정 칸 색칠)
        */

        // 2. 사각형을 그려서 격자를 만들기
        for (int i = 0; i <= 10; i++)
        {
            for (int j = 0; j <= 10; j++)
            {
                if (gameMap[i][j] == 0)
                    oldBrush = (HBRUSH)SelectObject(bufferHdc, myBrush);

                Rectangle(bufferHdc, 
                    100 + j * 100, 
                    20 + i * 70,
                    100 + 100 + j * 100,
                    70 + 20 + i * 70
                );

                if (gameMap[i][j] == 0)
                    myBrush = (HBRUSH)SelectObject(bufferHdc, oldBrush);
            }
        }
        // 사각형을 그리다 보니 인접한 모서리들은 두꺼워지는 문제가 있음



       // 백 버퍼를 메인 버퍼에 덮어 씌우기
        RECT rect;
        GetClientRect(hWnd, &rect);

        BitBlt(hdc, 0, 0, rect.right, rect.bottom, bufferHdc, 0, 0, SRCCOPY);

        FillRect(bufferHdc, &rect, (HBRUSH)SelectObject(bufferHdc, oldBrush));

        DeleteObject(myBrush);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void CreateBackPage(HWND hWnd, HINSTANCE hInstance, HDC* memDC, HBITMAP* memBitmap)
{
    HDC hdc = GetDC(hWnd);
    // WM_PAINT와 마찬가지로, 현재 만들어진 창과 연동되는 HDC 객체 생성

    *memDC = CreateCompatibleDC(hdc);
    // 새로 만든 hdc 객체 주소 저장

    RECT rect;
    // RECT: 사각형의 정보를 저장할 구조체. 구조는 Rectangle, Ellipse 등과 동일

    GetClientRect(hWnd, &rect);
    // 윈도우 창의 크기를 hWnd에서 받아서 rect에 저장

    *memBitmap = CreateCompatibleBitmap(        // 비트맵을 새로이 생성해 할당하는 함수
        hdc,                // 비트맵의 정보를 받아올 hdc
        rect.right,         // 비트맵의 좌우 크기
        rect.bottom         // 비트맵의 상하 크기
    );

    (HBITMAP)SelectObject(*memDC, *memBitmap);
    // 만들어둔 비트맵을 버퍼의 화면에 할당


    // 특정 구간의 사각형을 채우는 함수
    // 윈도우 창 전체를 입력해, 화면 전체를 해당 색으로 채우라는 의미
    FillRect(*memDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));


    // 여기까지 백 버퍼용 HDC, HBITMAP 생성
    // 새로 할당된 것들은 여기서 할당 해제
    ReleaseDC(hWnd, hdc);
}



/*
    과제

    위에선 칸 하나 노란색으로 칠했다

    노란색 말고 맵의 값에 따라 특정한 색을 칠하도록
    1. 색은 최소 3종 이상
    2. 각 색마다 최저 2칸 이상에 칠하기
    3. 10개 칸만 색을 칠하기
*/

