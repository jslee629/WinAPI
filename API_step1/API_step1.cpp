// API_step1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "API_step1.h"

#define MAX_LOADSTRING 100

/*
    여기저기사 자주 사용될 내용은
    define 전처리를 통해 상수를 만들거나
    밑의 전역 변수에 만들어 사용하면
    나중에 값 수정 필요 시 용이

    전역 변수 vs 상수
    - 프로그램 실행 중 바뀌면 안 되거나 어지간하면 바뀔 일이 없는 경우는 define
    - 프로그램 실행 중 바뀔 여지가 있는 경우는 전역 변수
*/
#define WINDOW_WIDTH 800        // 윈도우 창 초기 너비
#define WINDOW_HEIGHT 600       // 윈도우 창 초기 높이

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

/*
    API 프로그램은 다음과 같은 순서대로 프로그램이 작동된다 (기본 코드 기준)
    0. MyRegisterClass: 윈도우 어플리케이션 실행 전 기본적인 환경을 셋팅
    1. InitInstance: 윈도우 어플리케이션 작동에 필요한 초기화
    2. 윈도우에서 시스템 메세지를 받아 이를 처리하는 루프를 계속해서 가동
    3. 메세지를 받았을 경우, 이를 처리하는 콜백 함수를 실행해 메세지에 맞는 동작을 실행
    4. 2,3을 "프로그램 종료 메세지" 받기 전까지 반복
    5. 종료 메세지를 받으면 루프 종료, 프로그램 종료
*/

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,           // 지금 이 프로그램이 구동되고 있는 ID(=핸들)을 받아오는 매개 변수
    _In_opt_ HINSTANCE hPrevInstance,   // 만약 이 프로그램이 이전 프로세스에서 넘겨받은 것을 경우
    _In_ LPWSTR    lpCmdLine,           // 프로그램을 실행했을 때 추가로 입력한 명령어를 받아오는 매개 변수
    _In_ int       nCmdShow             
)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    // 받아온 인스턴스 핸들을 통해 우리가 만들 윈도우 창의 제목을 받아오는 함수
    // szTitle은 위의 전역변수 선언하는 공간에 선언된 문자열
    // 지금 이 함수를 통해 받아올 문자열을 저장할 공간으로서 선언된 것
    LoadStringW(hInstance, IDC_APISTEP1, szWindowClass, MAX_LOADSTRING);
    // 위와 비슷하게 윈도우의 클래스 이름을 받아오는 함수
    MyRegisterClass(hInstance);
    // 제목 등의 문자열을 불러온 뒤, 앞으로 생성할 윈도우 창을 이 함수를 통해 기본 설정

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_APISTEP1));

    MSG msg;
    // 시스템에서 프로그램에 요청된 메세지를 여기다 저장
    // GetMessage()를 통해 여기에 메세지를 입력받으므로
    // 초기화가 되어 있지 않아도 처음 사용하는 순간에 입력되면서 초기화 되므로 당장 건드릴 필요 없음
    // 그래도 앞으로 건드릴 일이 생길 것이니 미리미리 초기화하는 버릇을 들여둡시다
    
    // 우리는 다음에 프로그램을 만들 때 루프문의 구조를 바꿀 예정이므로 나중에 따로 초기화를 할 필요가 생김

    // 여기까지가 프로그램 실행을 위한 초기화
    // 이 다음부터 실질적으로 프로그램이 돌아가는 루프문

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
        // 시스템에서 이 프로그램과 관련된 메세지를 보낸 것이 있는지를 체크
        // 시스템 종료 등의 메세지가 오지 않는 한, 이 구문은 무조건 참
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            // 해당 메세지를 함수가 처리할 수 있는 형태로 변환
        {
            TranslateMessage(&msg);     // 변환된 내용 해석
            DispatchMessage(&msg);      // 해석된 메세지에 맞는 동작 실행 (메세지를 윈도우 프로시저에 전달)
        }
    }
    // 더 이상 시스템 메세지가 오지 않는다는 통보를 받은 상황 : 프로그램이 종료되는 상황

    return (int) msg.wParam;
    // 프로그램 종료와 동시에 어떤 사유로 프로그램이 종료되었는지가 msg에 저장되어있으니 이를 반환
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    // 윈도우 클래스 : 여기에 우리가 만들 윈도우 창에 대한 가종 설정들이 저장됨

    wcex.cbSize = sizeof(WNDCLASSEX);
    // 구조체 전체의 크기를 구조체 안에 멤버로스 등록
    // 나중에 이 구조체의 크기를 참조해야 할 일이 있을 때 사용

    // 왜 굳이 이렇게 저장하는 방식을 사용하냐면
    // WinAPI 버전업에 따라 이 구조체 구성의 자체가 바뀌는 일이 있다 보니
    // 앞으로 버전업을 통해 어떻게 바뀌든 항상 총 용량을 제공할 수 있게 유동적으로 구성


    // 이하의 내용은 실제로 건드릴 일이 거의 없음
    // 우리가 수정할 일이 생길 수 있는 내용은 * 표기
    wcex.style          = CS_HREDRAW | CS_VREDRAW;                              // 윈도우 창의 스타일
    wcex.lpfnWndProc    = WndProc;    // 함수 포인터                            // 윈도우 프로시저 (윈도우 창 제작 작업 함수)
    wcex.cbClsExtra     = 0;                                                    // 윈도우의 클래스 여분 메모리
    wcex.cbWndExtra     = 0;                                                    // 윈도우의 시스템 여분 메모리
    wcex.hInstance      = hInstance;                                            // 윈도우 창이 사용할 인스턴스
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APISTEP1));   // 윈도우 창이 사용할 기본 아이콘*
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);                       // 윈도우 창이 사용할 기본 커서*
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);                             // 윈도우 창이 사용할 기본 배경색
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_APISTEP1);                       // 윈도우 창이 사용할 메뉴 이름을 여기에 등록
    wcex.lpszClassName  = szWindowClass;                                        // 윈도우 클래스의 이름을 등록
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL)); // 윈도우 창이 사용할 소형 아이콘

    return RegisterClassExW(&wcex);
    // 설정이 끝난 윈도우 클래스 정보를 전체 프로그램에 등록
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
      CW_USEDEFAULT, 0, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr, hInstance, nullptr);
   /*
        HWMD = 우리가 실제로 다루게 되는 윈도우 창 그 자체
        CreateWindowW(
            szWindowClass           // 윈도우 클래스 이름
            szTitle                 // 새로이 만들 창의 이름을 여기에 지정 (szTitle 이외에 다른 값 입력 가능)
            WS_OVERAPPEDWINDOW      // 윈도우의 스타일을 결정하는 플래그
            CW_USERDEFAULT          // 창의 초기 위치 중 x좌표 설정
            0                       // 창의 초기 위치 중 y좌표 설정 (디폴트 사용 시 무시)
            CW_USERDEFAULT          // 창의 초기 좌우 너비
            0                       // 창의 초기 상하 높이 (디폴트 사용 시 무시)
            nullptr                 // 만약 만들어지는 창이 다른 창에서 파생된 자식 창이라면 여기다가 부모 창 지정
            nullptr                 // 윈도우에서 사용할 메뉴의 핸들을 지정
            hInstance               // 창과 연결될 모듈 인스턴스
            nullptr                 // "CREATECONSTRUCT"라는 구조체에 이용할 값으로서 남아있는 더미 데이터
        )
   */

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
/*
    hWnd = 위에서도 말했듯 본 프로그램이 구동되고 있는 윈도우 창 그 자체
    message = 위에서 본 프로시저를 호출할 때 같이 동봉한 메세지(msg)
    wParam = 핸들을 제공받거나, 정수 값을 전달받는 역할 (word Parameter) ex. 키보드 입력 정보
    lParam = 정수값으로 해결할 수 없어 구조체 등의 포인터를 받아 전달하는 역할 (long Parameter) ex. 마우스 좌표
*/
{
    switch (message)
    {
        // 각 case를 보면 대부분 명령이 "WM_"로 시작하는 것을 볼 수 있음
        // 이는 Windows Message의 줄임말로, 프로그램에서 이런 메세지가 호출되었다고 알리는 역할
    case WM_COMMAND:
        // 창의 메뉴에서 특정 항목이 선택되었을 때 실행
        {
            int wmId = LOWORD(wParam);
            /*
                창의 각 메뉴마다 할당된 ID 존재
                LOWORD : wParam의 아래쪽 4개 자리의 바이트만을 취급해 가져온다
                wParam : 비트 연산자를 통해 2개 이상의 값을 가져오는 경우가 꽤 있어서 그 중 원하는 값만을 이용
            */
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                // 도움말 항목을 눌렀을 경우
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                // 끝내기 항목을 눌렀을 경우
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        // "화면을 그려라" 메세지
        // 창에 그릴 내용을 결정하고 출력
        {
            PAINTSTRUCT ps;
            // 그림이 그려질 도화지

            HDC hdc = BeginPaint(hWnd, &ps);
            // 그림을 그리는 화가
            
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            
            TextOut(hdc, 
                100,        // 글씨를 표시할 구역의 X좌표 
                100,        // 글씨를 표시할 구역의 Y좌표 
                L"Hello, Desktop Window!",          // 출력할 문자열 
                wcslen(L"Hello, Desktop Window!")   // 출력할 문자열의 총 길이
            );      // 텍스트를 작성하는 함수
            
            const TCHAR* new_string = L"또 한 번 우려먹는 Hello, World!";

            SetTextColor(hdc, RGB(200, 100, 0));    
            // 앞으로 입력할 모든 텍스트의 색을 결정하는 함수
            
            TextOut(hdc, 100, 200, new_string, wcslen(new_string));

            HPEN myPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 51));
            HBRUSH myBrush = CreateSolidBrush(RGB(102, 0, 153));
            // 지정한 색으로 칠하도록 설정된 붓, 펜 생성

            HPEN oldPen = (HPEN)SelectObject(hdc, myPen);
            HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
            // 생성된 붓, 펜을 hdc의 기본 붓, 펜과 교체해 사용하도록 설정

            // CreateSolidBrush, CreatePen : 동적할당으로 새 붓을 만드는 함수
            // 프로그램이 종료되면 해당 붓, 펜은 할당 해제 해줘야 함
            // 그러면 기존에 할당되었던 붓, 펜은 특별한 조취를 취하지 않을 경우
            // hdc는 붓, 펜 대신 nullptr을 가지고 있으므로 오류 발생

            // 이를 막기 위해서 기존 사용하던 붓, 펜을 임시로 저장해두고 있을 객체가 필요
            // SelectObject : 반환되는 기존 붓, 펜 정보를 void pointer로 리턴
            // 적절히 형 변환하여 가지고 있어야 할 필요가 있음

            Rectangle(hdc, 
                320,        // 그릴 사각형의 왼쪽 위 꼭지점 X좌표 
                240,        // 그릴 사각형의 왼쪽 위 꼭지점 Y좌표  
                480,        // 그릴 사각형의 오른쪽 아래 꼭지점 X좌표  
                360         // 그릴 사각형의 오른쪽 아래 꼭지점 Y좌표 
            );     // 사각형을 그리는 함수

            HBRUSH clearBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
            // 윈도우에서 기본적으로 마련해준 도구들이 있는데, 그 중 NULL_BRUSH, 투명색 붓을 가져오라는 함수
            // 이 함수 역시도 동적 할당을 사용하기 때문에 후에 할당 해제 해줘야 함

            oldBrush = (HBRUSH)SelectObject(hdc, clearBrush);

            Ellipse(hdc,
                320,        // 그릴 사각형의 왼쪽 위 꼭지점 X좌표 
                240,        // 그릴 사각형의 왼쪽 위 꼭지점 Y좌표  
                480,        // 그릴 사각형의 오른쪽 아래 꼭지점 X좌표  
                360         // 그릴 사각형의 오른쪽 아래 꼭지점 Y좌표 
            );     // 사각형에 내접하는 타원을 그리는 함수

            clearBrush = (HBRUSH)SelectObject(hdc, oldBrush);
            // 붓의 사용이 끝났다면 기존 붓으로 다시 교체

            BeginPath(hdc);
            // 지금부터 공간을 지정하도록 하겠다

            MoveToEx(hdc, 500, 100, NULL);      // 펜을 대고 있는 위치 설정
            LineTo(hdc, 500, 400);              // 해당 위치까지 선을 그어라
            LineTo(hdc, 700, 400);              // 해당 위치까지 선을 그어라
            LineTo(hdc, 500, 100);              // 해당 위치까지 선을 그어라

            // BeginPath 이후 입력된 좌표들은 전부 영역을 표시하는 데도 사용된다

            myPen = (HPEN)SelectObject(hdc, oldPen);
            myBrush = (HBRUSH)SelectObject(hdc, oldBrush);
            // 붓의 사용이 끝났다면 기존 붓으로 다시 교체

            EndPath(hdc);
            // 공간 지정 끝

            StrokeAndFillPath(hdc);
            // 지정된 영역을 칠해라


            DeleteObject(myBrush);
            DeleteObject(clearBrush);
            DeleteObject(myPen);
            // 사용이 끝난 객체는 DeleteObject라는 함수를 이용해서 안전하게 할당 해제

            EndPaint(hWnd, &ps);
            // 그린 그림을 화면에 출력
        }
        break;
    case WM_DESTROY:
        // "윈도우 창 종료" 메세지
        PostQuitMessage(0);
        // (콘솔이나 로그 등에) 프로그램을 종료한다는 메세지 출력
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


/*
    과제

    정육각형을 만들어올 것

    1. 정육각형 내부는 색으로 채워야 함
    2. 정육각형에 외접하는 원이 있어야 함
    3. 정육각형이 원에 가려져서는 안 됨
*/