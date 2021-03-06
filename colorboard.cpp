#include "colorboard.h"

#include "gltools.h"
#include "log.h"

ColorBoard::ColorBoard(){
    DebugLog("ColorBoard Created %p", this);
    //TODO 添加属性WS_POPUP时SetProp失效
    hWnd = CreateWindowExA(
        0,
        "ModelView.ColorBoard",
        "ColorBoard",
        WS_CAPTION,
        CW_USEDEFAULT, CW_USEDEFAULT,
        300, 230,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );
    GLUtils::EnableOpenGL(hWnd, &hDC, &hRC);
    SetProp(hWnd, "PROP_THIS", (HANDLE)this);
}

ColorBoard::~ColorBoard(){
    GLUtils::DisableOpenGL(hWnd, hDC, hRC);
    DestroyWindow(hWnd);
}

ATOM ColorBoard::RegClass(HINSTANCE hInstance){
    WNDCLASSEXA wc;

    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
    wc.hIcon = LoadIconA(hInstance, IDI_APPLICATION);
    wc.hIconSm = LoadIconA(hInstance, IDI_APPLICATION);
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = "ModelView.ColorBoard";
    wc.lpszMenuName = "";
    wc.style = 0;

    return RegisterClassExA(&wc);
}

LRESULT CALLBACK ColorBoard::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    ColorBoard* board = (ColorBoard*)GetProp(hWnd, "PROP_THIS");
    if (board == NULL){
        DebugError("ColorBoard::NullPointerException");
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return board->LocalWndProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK ColorBoard::LocalWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch (uMsg){
    case WM_CREATE:
        GetClientRect(hWnd, &cliRect);
        break;
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        switch (wParam){
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        }
    case WM_SIZE:
        GetClientRect(hWnd, &cliRect);
        cursorPos.x = 2.0f * GET_X_LPARAM(lParam) / (cliRect.right - cliRect.left) - 1.0f;
        cursorPos.y = 1.0f - 2.0f * GET_Y_LPARAM(lParam) / (cliRect.bottom - cliRect.top);
        break;
    case WM_MOUSEMOVE:
        cursorPos.x = 2.0f * GET_X_LPARAM(lParam) / (cliRect.right - cliRect.left) - 1.0f;
        cursorPos.y = 1.0f - 2.0f * GET_Y_LPARAM(lParam) / (cliRect.bottom - cliRect.top);
        break;
    case WM_LBUTTONDOWN:
        if (cursorPos.x >= 0.4f && cursorPos.x <= 0.93333333f){
            white = (cursorPos.y + 1.0f) * 0.5f;
            break;
        }
        if (cursorPos.x <= 0.33333333f){
            float x = (cursorPos.x + 0.33333333f) * 1.5f;
            if (x * x + cursorPos.y * cursorPos.y <= 1.0f){
                glReadPixels(GET_X_LPARAM(lParam), (cliRect.bottom - cliRect.top) - GET_Y_LPARAM(lParam),
                                1, 1, GL_RGB, GL_FLOAT, (GLvoid*)&color);
                DebugLog("ColorBoard Select Color %f %f %f", color.x, color.y, color.z);
                PostQuitMessage(0);
                break;
            }
        }
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void ColorBoard::Render(){
    wglMakeCurrent(hDC, hRC);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, 200, 200);

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(white, white, white);
    glVertex2f(0.0f, 0.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    for (float i = 0.0f; i < PI * 0.33333333f; i += 0.01f){
        glColor3f(1.0f, i / (PI * 0.33333333f), 0.0f);
        glVertex2f(Cos(i), Sin(i));
    }
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex2f(Cos(PI * 0.33333333f), Sin(PI * 0.33333333f));
    for (float i = PI * 0.33333333f; i < PI * 0.66666667f; i += 0.01f){
        glColor3f((PI * 0.66666667f - i) / (PI * 0.33333333f), 1.0f, 0.0f);
        glVertex2f(Cos(i), Sin(i));
    }
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(Cos(PI * 0.66666667f), Sin(PI * 0.66666667f));
    for (float i = PI * 0.66666667f; i < PI; i += 0.01f){
        glColor3f(0.0f, 1.0f, (i - PI * 0.66666667f) / (PI * 0.33333333f));
        glVertex2f(Cos(i), Sin(i));
    }
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex2f(-1.0f, 0.0f);
    for (float i = PI; i < PI * 1.33333333f; i += 0.01f){
        glColor3f(0.0f, (PI * 1.33333333f - i) / (PI * 0.33333333f), 1.0f);
        glVertex2f(Cos(i), Sin(i));
    }
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(Cos(PI * 1.33333333f), Sin(PI * 1.33333333f));
    for (float i = PI * 1.33333333f; i < PI * 1.66666667f; i += 0.01f){
        glColor3f((i - PI * 1.33333333f) / (PI * 0.33333333f), 0.0f, 1.0f);
        glVertex2f(Cos(i), Sin(i));
    }
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex2f(Cos(PI * 1.66666667f), Sin(PI * 1.66666667f));
    for (float i = PI * 1.66666667f; i < PI * 2.0f; i += 0.01f){
        glColor3f(1.0f, 0.0f, (PI * 2.0f - i) / (PI * 0.33333333f));
        glVertex2f(Cos(i), Sin(i));
    }
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glEnd();

    glViewport(200, 0, 100, 200);

    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 1.0f, 1.0f); glVertex2f(-0.8f, 1.0f);
    glColor3f(1.0f, 1.0f, 1.0f); glVertex2f(0.8f, 1.0f);
    glColor3f(0.0f, 0.0f, 0.0f); glVertex2f(0.8f, -1.0f);
    glColor3f(1.0f, 1.0f, 1.0f); glVertex2f(-0.8f, 1.0f);
    glColor3f(0.0f, 0.0f, 0.0f); glVertex2f(-0.8f, -1.0f);
    glColor3f(0.0f, 0.0f, 0.0f); glVertex2f(0.8f, -1.0f);
    glEnd();

    SwapBuffers(hDC);
}

Vector3 ColorBoard::RunAndGetColor(){
    MSG Msg;

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    while (GetMessageA(&Msg, NULL, 0, 0)){
        if (Msg.message == WM_QUIT){
            break;
        } else {
            TranslateMessage(&Msg);
            DispatchMessageA(&Msg);
            Render();
        }
    }

    ShowWindow(hWnd, SW_HIDE);

    return color;
}