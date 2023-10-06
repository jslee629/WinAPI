// API_step2.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "API_step2.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
int mousePosX = 0;      // 마우스 포인터 X좌표
int mousePosY = 0;      // 마우스 포인터 Y좌표
int isClicked = 0;      // 마우스 클릭 여부

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_APISTEP2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_APISTEP2));

    MSG msg = {};

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APISTEP2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_APISTEP2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
LRESULT CALLBACK WndProc(
    HWND hWnd,              // 명령이 수행되는 윈도우 창 
    UINT message,           // 함수에 전달된 메세지. 명령
    WPARAM wParam,          // word parameter. 단문형 매개변수. 단일 자료형 데이터
    LPARAM lParam           // long parameter. 장문형 매개변수. 구조체, 클래스 등의 포인터 데이터
)
{
    switch (message)
    {
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

            Ellipse(hdc, mousePosX - 40, mousePosY - 40, mousePosX + 40, mousePosY + 40);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_LBUTTONDOWN:
        // 마우스 왼쪽 버튼이 눌렸을 경우 시스템에서 보내는 메세지
        {
            isClicked |= 1;
            mousePosX = GET_X_LPARAM(lParam);
            mousePosY = GET_Y_LPARAM(lParam);
            // 위 함수를 사용하기 위해선 windowsx.h 추가 -> framework.h에 include함
            // GET_X_LPARAM: lParam으로 전달된 여러 데이터 중 전반부의 값을 가져오는 매크로 (마우스 x좌표)
            // GET_Y_LPARAM: lParam으로 전달된 여러 데이터 중 후반부의 값을 가져오는 매크로 (마우스 y좌표)

            InvalidateRect(
                hWnd,       // 명령 내릴 창
                nullptr,    // 무효화 영역 (RECT 구조체 포인터), NULL이면 창 화면 전체를 새로 그려라
                true        // 화면을 새로 그릴 때, 원래 그려져 있던 그림은 전부 지우고 새로 그릴 것인지 여부
            );      // 어떤 사각형 영역을 무효화하는 함수
        }
        break;
    case WM_LBUTTONUP:
        // 마우스 왼쪽 버튼을 땠을 경우 시스템에서 보내는 메세지
        {
            mousePosX = GET_X_LPARAM(lParam);
            mousePosY = GET_Y_LPARAM(lParam);
            InvalidateRect(hWnd, nullptr, true);
        }
        break;
    case WM_RBUTTONDOWN:
        {
            mousePosX = GET_X_LPARAM(lParam);
            mousePosY = GET_Y_LPARAM(lParam);
            InvalidateRect(hWnd, nullptr, true);
        }
        break;
    case WM_RBUTTONUP:
        {
            mousePosX = GET_X_LPARAM(lParam);
            mousePosY = GET_Y_LPARAM(lParam);
            InvalidateRect(hWnd, nullptr, true);
        }
        break;
    case WM_MOUSEMOVE:
        // 마우스 포인터가 움직일 경우 시스템에서 보내는 메세지
        {
            switch (wParam)
            {
            case VK_LBUTTON:
                {
                    mousePosX = GET_X_LPARAM(lParam);
                    mousePosY = GET_Y_LPARAM(lParam);
                    InvalidateRect(hWnd, nullptr, true);
                }
                break;
            case VK_RBUTTON:
                {
                    mousePosX = GET_X_LPARAM(lParam);
                    mousePosY = GET_Y_LPARAM(lParam);
                    InvalidateRect(hWnd, nullptr, true);
                }
                break;
            }
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


/*
    마우스 드래그 구현 방법
    1. 마우스 클릭 여부 플래그를 지역 변수에 선언 -> 비트 연산자를 통해 UI적으로 드래그 상태를 구분
        - 비트 연산자를 수행해야 하기 때문에 변수 관리가 난해
        - 멀티프레스(Multipress: 동시 입력) 가능
    2. WM_MOUSEMOVE에서 wParam을 이용
        - wParam을 사용하여 조건문을 짜는 과정이 쉬움
        - 멀티프레스 불가
*/


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


/*
    과제:

    마우스 왼쪽 클릭, 마우스 오른쪽 클릭, 마우스 휠 클릭을 구분하는 코드 작성
    1. 왼쪽, 오른쪽은 그대로 원을 찍되, 색으로 구분
    2. 휠은 원이 아닌 사각형을 출력하여 구분
*/