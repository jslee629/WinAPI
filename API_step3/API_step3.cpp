// API_step3.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "API_step3.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
POINT position = { 300, 300 };   // POINT: 정수 2개를 저장하는 구조체. 보통 마우스 위치 저장할 때 이 구조체를 이용해 저장
HWND hWnd;                      // 윈도우 창을 전역변수에 선언
int keyboardFlag = 0;           // 입력된 키보드 정보를 저장할 플래그
int shapeSpeed = 3;            // 이동 속도

/*
    키보드플래그 비트 맵핑
    1 : LEFT
    2 : RIGHT
    4 : UP
    8 : DOWN
*/

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
    LoadStringW(hInstance, IDC_APISTEP3, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_APISTEP3));

    MSG msg = {};
    DWORD timeStamp;
    // DWORD: unsigned long(unsigned int)

    timeStamp = timeGetTime();
    // 윈도우를 켠 시각부터 현재까지 경과된 총 시간을 반환하는 함수 (단위 ms, 정수형)

    DWORD frame = 0;
    // 총 경과시간을 기록해줄 변수

    // 기본 메시지 루프입니다:
    while (1)
    {
        // 상시 처리되어야 하는 내용 (ex|특정 메세지를 받으면 예외처리가 필요)
    
        DWORD curTime = timeGetTime();
        // 매 루프가 시작할 때마다 현재 시각 측정

        frame += curTime - timeStamp;
        // 경과 시간 체크

        timeStamp = curTime;
        // 타임스탬프 업데이트

        // 키보드 플래그의 각 비트값 검사
        if (keyboardFlag != 0)
        {
            if (frame > 17)
            {
                frame = 0;

                if ((keyboardFlag & 1) == 1)
                    // 왼쪽 키가 눌려있을 경우
                    position.x -= shapeSpeed;
                if ((keyboardFlag & 2) == 2)
                    // 오른쪽 키가 눌려있을 경우
                    position.x += shapeSpeed;
                if ((keyboardFlag & 4) == 4)
                    // 위쪽 키가 눌려있을 경우
                    position.y -= shapeSpeed;
                if ((keyboardFlag & 8) == 8)
                    // 아래쪽 키가 눌려있을 경우
                    position.y += shapeSpeed;
                InvalidateRect(hWnd, NULL, true);
                // 이걸 사용하기 위해 hWnd를 전역 변수화

                // 1. 눌리는 동안 계속 움직여야 하는데
                // 맨 처음 잠깐 멈추는 현상: GetMessage()는 메세지가 있을때만 실행
                // -> PeekMessage()로 바꿔 메세지가 없을 때도 프로그램이 돌아가도록 조건을 바꿔야
                // 2. 깜빡임 현상
                // -> 더블 버퍼링으로 해결해야

                // feedback
                // 컴퓨터 성능에 따라 육안으로 판별이 힘들 정도로 빨라
                // 1. 정해진 사이클 시간을 설정: winmm.lib
            }
        }

        if (GetMessage(&msg, nullptr, 0, 0))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APISTEP3));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_APISTEP3);
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

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
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
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

            Rectangle(hdc, position.x - 40, position.y - 40, position.x + 40, position.y + 40);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_KEYDOWN:
        // 키보드 키가 눌려있을 경우 발생하는 메세지. 마우스와는 다르게 눌려있는 동안 계속 호출
        {
            /*
            int speed = 30;
            switch (wParam)
                // 마지막으로 입력된 키만이 wParam에 담겨 옴
                // 멀티키 기능 사용 불가
                // 그래서 플래그를 선언하고 플래그에 비트연산을 가하여야 함 (마우스와 비슷)
            {
            case VK_LEFT:
                position.x -= speed;
                break;
            case VK_RIGHT:
                position.x += speed;
                break;
            case VK_UP:
                position.y -= speed;
                break;
            case VK_DOWN:
                position.y += speed;
                break;
            }
            InvalidateRect(hWnd, NULL, true);
            */

            switch (wParam)
            {
            case VK_LEFT:
                keyboardFlag |= 1;
                break;
            case VK_RIGHT:
                keyboardFlag |= 2;
                break;
            case VK_UP:
                keyboardFlag |= 4;
                break;
            case VK_DOWN:
                keyboardFlag |= 8;
                break;
            }
            InvalidateRect(hWnd, NULL, true);
        }
        break;
    case WM_KEYUP:
        {
            switch (wParam)
            {
            case VK_LEFT:
                keyboardFlag &= ~1;
                break;
            case VK_RIGHT:
                keyboardFlag &= ~2;
                break;
            case VK_UP:
                keyboardFlag &= ~4;
                break;
            case VK_DOWN:
                keyboardFlag &= ~8;
                break;
            }
            InvalidateRect(hWnd, NULL, true);
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
