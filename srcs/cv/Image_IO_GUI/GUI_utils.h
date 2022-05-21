#pragma once


#include "IO.h"
#include <vector>


//GDI+��ͼ
//׼������ͼƬ�ŵ�������ͬһ��Ŀ¼��

thread_local HINSTANCE GhInstance;
thread_local HBITMAP hbmpBack = NULL;


std::vector<std::thread> thr_arr;



__declspec(dllexport)
void GDIPlusInit()
{
    ULONG_PTR  ulToken;
    GdiplusStartupInput    gdiplusStartupInput;
    ::GdiplusStartup(&ulToken, &gdiplusStartupInput, NULL);
}



static
//  ������Ϣ ������
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    HDC HdcMem;

    PAINTSTRUCT ps;

    switch (uMsg)
    {
    case WM_CLOSE:   //  ��X ���ڹرյ���Ϣ
        ::PostQuitMessage(0);  //  �����˳�����Ϣ
        break;
    case WM_PAINT:
    {
#ifndef BitMapOnly
        hdc = BeginPaint(hWnd, &ps);
        HDC hDCMem = CreateCompatibleDC(hdc);
        HBITMAP hOldBmp = (HBITMAP)SelectObject(hDCMem, hbmpBack);

        BITMAP bmp;
        GetObject(hbmpBack, sizeof(BITMAP), &bmp);

        BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, hDCMem, 0, 0, SRCCOPY);

        SelectObject(hDCMem, hOldBmp);//��ԭ����DC����
        DeleteDC(hDCMem);//ɾ������DC�������ڴ�й©
        EndPaint(hWnd, &ps);
#else
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);

        HDC hdc = GetDC(hWnd);//��ȡ���ڵĻ�ͼhdc
        HDC hMdc = CreateCompatibleDC(hdc);//�����ڴ�dc
        HBITMAP hMbitmap = (HBITMAP)SelectObject(hMdc, hbmpBack);
#endif
    }
    break;
    }
    return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}


static
int CALLBACK _main(const wchar_t* window_name, uchar *src, int width, int height, int channel)
{
    //GhInstance = hInstance;
    //����GDI+��ʼ����  
    ULONG_PTR  ulToken;
    GdiplusStartupInput    gdiplusStartupInput;
    ::GdiplusStartup(&ulToken, &gdiplusStartupInput, NULL);

    Bitmap* lpbmp = new Bitmap(width, height);
    if (channel == 4) {
        TranslateMyCls_4(src, lpbmp);       // �� img �е�����ֵ���ؽ� Bitmap ������Ϊ��ʾ�� buffer
    }
    else if (channel == 1) {
        TranslateMyCls_1(src, lpbmp);
    }

    lpbmp->GetHBITMAP(0, &hbmpBack);    // �õ� HBITMAP 

    //  1.  ���
    WNDCLASSEX wndclass;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;   //  �Ƿ�Ҫ�������Ŀռ�
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.hbrBackground = NULL;   //  ������ɫĬ��
    wndclass.hCursor = NULL; // Ĭ�Ϲ��
    wndclass.hIcon = NULL;  //Ĭ��ͼ��
    wndclass.hIconSm = NULL;  //  ��������Сͼ��
    wndclass.hInstance = GhInstance;    //  ��ǰʵ���ľ��
    wndclass.lpfnWndProc = WndProc;    //  ��Ϣ������
    wndclass.lpszClassName = (LPCWSTR)window_name;   //  ע�ᴰ�������
    wndclass.lpszMenuName = NULL;       //  �˵���
    wndclass.style = CS_HREDRAW | CS_VREDRAW;  //  ���������ʽ

    //  2.  ע��
    if (::RegisterClassEx(&wndclass) == FALSE)
    {
        ::MessageBox(NULL, (LPCWSTR)L"ע��ʧ��", (LPCWSTR)"��ʾ", MB_OK);
        return 0;
    }

    //  3.  ����
    // x, y, nW, nH
    HWND hWnd = ::CreateWindow(wndclass.lpszClassName, (LPCWSTR)window_name, WS_OVERLAPPEDWINDOW, 100, 0,
        lpbmp->GetWidth(), lpbmp->GetHeight(), NULL, NULL, GhInstance, NULL);

    if (hWnd == NULL)
    {
        ::MessageBox(NULL, (LPCWSTR)"����ʧ��", (LPCWSTR)"��ʾ", MB_OK);
        return 0;
    }
    //  4.   ��ʾ
    ::ShowWindow(hWnd, SW_SHOW);
    //  5.   ��Ϣѭ��
    MSG msg;
    while (::GetMessage(&msg, 0, 0, 0))
    {
        //  ����
        ::TranslateMessage(&msg);
        //  �ַ�
        ::DispatchMessage(&msg);
    }
    //�ر�GDI+   
    GdiplusShutdown(ulToken);
    return 0;
}




// This DLL
__declspec(dllexport)
void _ShowImage(uchar* src, const wchar_t* window_name, int width, int height, int channel)
{
    thr_arr.push_back(
        std::thread(_main, window_name, src, width, height, channel)
    );
}


__declspec(dllexport)
void _Wait()
{
    for (int i = 0; i < thr_arr.size(); ++i) {
        thr_arr[i].join();
    }
}