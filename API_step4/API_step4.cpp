// API_step4.cpp : 더블 버퍼링
//

#include "framework.h"
#include "API_step4.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND hWnd;                      // 윈도우 창을 전역변수에 선언
POINT position = { 800,300 };
int keyboardFlag = 0;
int shapeSpeed = 3;

// 더블 버퍼링에 이용할 버퍼 생성
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
    LoadStringW(hInstance, IDC_APISTEP4, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_APISTEP4));

    MSG msg = {};
    DWORD timeStamp;
    // DWORD: unsigned long(unsigned int)

    timeStamp = timeGetTime();
    // 윈도우를 켠 시각부터 현재까지 경과된 총 시간을 반환하는 함수 (단위 ms, 정수형)

    DWORD frame = 0;
    // 총 경과시간을 기록해줄 변수

    // 기본 메시지 루프입니다:
    while (msg.message != WM_QUIT)
    {
        DWORD curTime = timeGetTime();
        // 매 루프가 시작할 때마다 현재 시각 측정
        
        // 키보드 플래그의 각 비트값 검사
        if (keyboardFlag != 0)
        {
            frame += curTime - timeStamp;
            // 경과 시간 체크

            // TO-DO :: 키보드 입력이 깔끔하게 맞아떨어지지 않는 현상이 발생중
            // 이를 해결해보기

            // 그런데 이 부분이 키보드가 눌리지 않고 있을 때도 동작하는 게 문제
            // 우리가 원하는 건 키보드가 움직이는 동안에만 움직임이 반영되는 것
            // -> 프레임이 경과했다고 해도 키보드가 눌린 적이 없으면 처리되면 안 됨

            // 따라서 프레임을 계산하는 이 부분을 항상 작동되도록 하면 안 됨
            
            if (frame >= 17)
            {
                frame -= 17;

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

                InvalidateRect(hWnd, NULL, false);
                // WM_PAINT 메세지를 발생시키는 함수
                // true는 기존 화면을 전부 지우고 새로 그리라는 의미
                // 더블버퍼링을 사용하는 현재 true는 연산 낭비

                RECT rect;
                GetClientRect(hWnd, &rect);
                // 윈도우 초기 크기를 받아오기 위해 선언

                FillRect(bufferHdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
                // 백 버퍼 초기화
            }
        }
        
        timeStamp = curTime;
        // 타임스탬프 업데이트

        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APISTEP4));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_APISTEP4);
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
    case WM_CREATE:
        // 창이 처음 생성되었을 때 발생하는 메세지
        CreateBackPage(hWnd, hInst, &bufferHdc, &bufferBitmap);
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
            // 그림이 그려질 도화지

            HDC hdc = BeginPaint(hWnd, &ps);
            // 그림을 그리는 화가

            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            TextOut(bufferHdc,
                100,
                100,
                L"Hello, Desktop Window!",
                wcslen(L"Hello, Desktop Window!")
            );      // 텍스트를 작성하는 함수

            const TCHAR* new_string = L"또 한 번 우려먹는 Hello, World!";

            SetTextColor(bufferHdc, RGB(200, 100, 0));
            // 앞으로 입력할 모든 텍스트의 색을 결정하는 함수

            TextOut(bufferHdc, 100, 200, new_string, wcslen(new_string));

            HPEN myPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 51));
            HBRUSH myBrush = CreateSolidBrush(RGB(102, 0, 153));
            // 지정한 색으로 칠하도록 설정된 붓, 펜 생성

            HPEN oldPen = (HPEN)SelectObject(bufferHdc, myPen);
            HBRUSH oldBrush = (HBRUSH)SelectObject(bufferHdc, myBrush);
            // 생성된 붓, 펜을 hdc의 기본 붓, 펜과 교체해 사용하도록 설정

            // CreateSolidBrush, CreatePen : 동적할당으로 새 붓을 만드는 함수
            // 프로그램이 종료되면 해당 붓, 펜은 할당 해제 해줘야 함
            // 그러면 기존에 할당되었던 붓, 펜은 특별한 조취를 취하지 않을 경우
            // hdc는 붓, 펜 대신 nullptr을 가지고 있으므로 오류 발생

            // 이를 막기 위해서 기존 사용하던 붓, 펜을 임시로 저장해두고 있을 객체가 필요
            // SelectObject : 반환되는 기존 붓, 펜 정보를 void pointer로 리턴
            // 적절히 형 변환하여 가지고 있어야 할 필요가 있음

            Rectangle(bufferHdc,320, 240, 480, 360);     // 사각형을 그리는 함수

            HBRUSH clearBrush = (HBRUSH)GetStockObject(NULL_BRUSH);

            oldBrush = (HBRUSH)SelectObject(bufferHdc, clearBrush);

            Ellipse(bufferHdc, 320, 240, 480, 360 );     // 사각형에 내접하는 타원을 그리는 함수

            clearBrush = (HBRUSH)SelectObject(bufferHdc, oldBrush);

            BeginPath(bufferHdc);
            // 지금부터 공간을 지정하도록 하겠다

            MoveToEx(bufferHdc, 500, 100, NULL);      // 펜을 대고 있는 위치 설정
            LineTo(bufferHdc, 500, 400);              // 해당 위치까지 선을 그어라
            LineTo(bufferHdc, 700, 400);              // 해당 위치까지 선을 그어라
            LineTo(bufferHdc, 500, 100);              // 해당 위치까지 선을 그어라

            myPen = (HPEN)SelectObject(bufferHdc, oldPen);
            myBrush = (HBRUSH)SelectObject(bufferHdc, oldBrush);
            // 붓의 사용이 끝났다면 기존 붓으로 다시 교체
            
            // BeginPath 이후 입력된 좌표들은 전부 영역을 표시하는 데도 사용된다

            EndPath(bufferHdc);
            // 공간 지정 끝

            StrokeAndFillPath(bufferHdc);
            // 지정된 영역을 칠해라


            DeleteObject(myBrush);
            DeleteObject(clearBrush);
            DeleteObject(myPen);
            // 사용이 끝난 객체는 DeleteObject라는 함수를 이용해서 안전하게 할당 해제

            HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
            oldBrush = (HBRUSH)SelectObject(bufferHdc, redBrush);

            Ellipse(bufferHdc, position.x, position.y, position.x + 50, position.y + 50);

            redBrush = (HBRUSH)SelectObject(bufferHdc, oldBrush);

            DeleteObject(redBrush);

            RECT rect;
            GetClientRect(hWnd, &rect);
            // 윈도우 초기 창 크기를 가져오기 위한 선언

            // 백버퍼에 그린 그림을 메인 버퍼에 덮어 씌우도록 하자
            BitBlt(
                hdc,                // 메인 hdc
                0,                  // 붙어 넣는 영역의 x좌표
                0,                  // 붙어 넣는 영역의 y좌표
                rect.right,         // 붙어 넣는 영역의 가로
                rect.bottom,        // 붙어 넣는 영역의 세로
                bufferHdc,          // 백 hdc
                0,                  // 붙어 넣을 영역의 x좌표
                0,                  // 붙어 넣을 영역의 y좌표
                SRCCOPY             // 복사 방식을 지정하는 열거형 상수
            );

            EndPaint(hWnd, &ps);
            // 그린 그림을 화면에 출력
        }
        break;
    case WM_KEYDOWN:
        {
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

//
// 예비용 도화지 만드는 함수
// 
// parameters
// - hWnd: 윈도우 창
// - hInstance: 인스턴스
// - memHDC: 백버퍼에서도 그림을 그리기 위해, 예비용 도화지에서 사용할 hdc
// - memBitmap: 그린 그림을 비트맵 데이터로서 저장, 최종적으로 화면에 출력할 데이터를 저장하는 역할의 버퍼 
//
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
