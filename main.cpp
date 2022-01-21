#include "main.h"

#include "res.h"

#include "log.h"
#include "font.h"
#include "gltools.h"

MainWindow::MoveButton::MoveButton(Vector2 center, float radius, MainWindow* main) : center(center), radius(radius), main(main) {}

MainWindow::MoveButton::~MoveButton(){}

bool MainWindow::MoveButton::Trigger(Vector2 pos){
    return (pos - center).SqrMagnitude() <= radius * radius;
}

void MainWindow::MoveButton::Render(){
    glColor3f(0.3f, 0.3f, 0.3f);
    GLUtils::DrawCorner(center.x, center.y, 0.0f, 360.0f, radius, 0.05f);
}

void MainWindow::MoveButton::Click(){
    DebugLog("MoveButton Click");
    start = main->camLookat;
}

void MainWindow::MoveButton::Drag(Vector2 dir){
    //DebugLog("MoveButton Drag %f %f", dir.x, dir.y);
    main->camLookat = start - (main->camRight * dir.x + main->camUp * dir.y) * main->camDis;
    main->UpdateLookAtLocation();
}

MainWindow::RotateButton::RotateButton(Vector2 center, float radius, MainWindow* main) : center(center), radius(radius), main(main) {}

MainWindow::RotateButton::~RotateButton(){}

bool MainWindow::RotateButton::Trigger(Vector2 pos){
    return (pos - center).SqrMagnitude() <= radius * radius;
}

void MainWindow::RotateButton::Render(){
    glColor3f(0.3f, 0.3f, 0.3f);
    GLUtils::DrawCorner(center.x, center.y, 0.0f, 360.0f, radius, 0.05f);
}

void MainWindow::RotateButton::Click(){
    DebugLog("RotateButton Click");
    start = main->camDir;
}

void MainWindow::RotateButton::Drag(Vector2 dir){
    //DebugLog("RotateButton Drag %f %f", dir.x, dir.y);
    main->camDir = Quaternion::AxisAngle(main->camUp, -dir.x * 100.0f) *
                            Quaternion::AxisAngle(main->camRight, dir.y * 100.0f) * start;
    dragged = true;
}

void MainWindow::RotateButton::Leave(){
    if (dragged){
        DebugLog("RotateButton Leave");
        dragged = false;
        main->UpdateRotation();
    }
}

MainWindow::MoveOperation::MoveOperation(MainWindow* main) : main(main) {}

MainWindow::MoveOperation::~MoveOperation(){}

void MainWindow::MoveOperation::OnEnter(){
    x = y = z = true;
    start = main->cursorPos;
    if (main->selectedPoints.Size() > 0){
        if (main->selectedPoints.Size() > 1){
            DebugLog("MoveOperation OnEnter [Warning]: Selected Multiple Points");
        }
        target = main->selectedPoints.GetFront();
        startPos = target->pos;
    }else{
        target = NULL;
    }
}

void MainWindow::MoveOperation::OnMove(){
    Vector2 mov;
    Vector3 delta;
    if (target){
        mov = (main->cursorPos - start) * main->camDis;
        delta = main->camRight * mov.x + main->camUp * mov.y;
        target->pos = startPos + Vector3(delta.x * x, delta.y * y, delta.z * z);
        DebugLog("MoveOperation OnMove %f %f %f", delta.x * x, delta.y * y, delta.z * z);
    }
}

void MainWindow::MoveOperation::OnCommand(UINT id){
    switch (id){
    case IDM_OP_X: x = true; y = z = false; break;
    case IDM_OP_Y: y = true; z = x = false; break;
    case IDM_OP_Z: z = true; x = y = false; break;
    case IDM_OP_PLANE_X: x = false; y = z = true; break;
    case IDM_OP_PLANE_Y: y = false; z = x = true; break;
    case IDM_OP_PLANE_Z: z = false; x = y = true; break;
    }
}

MainWindow::MainWindow(HINSTANCE hInstance) : hInst(hInstance) {
    uiMgr = new UIManager();
    mesh = new Mesh();

    basicMenu = new Menu();
    basicMenu->AddItem(new MenuItem(L"添加顶点", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* MainWindow){
        MainWindow->AddPoint();
    }, this));
    basicMenu->AddItem(new MenuItem(L"删除顶点", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* MainWindow){
        MainWindow->DeletePoint();
    }, this));
    basicMenu->AddItem(new MenuItem());
    basicMenu->AddItem(new MenuItem(L"保存", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* MainWindow){
        MainWindow->OnInsSave();
    }, this));
    basicMenu->AddItem(new MenuItem());
    basicMenu->AddItem(new MenuItem(L"关于", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* MainWindow){
        MainWindow->AboutBox();
    }, this));
    basicMenu->AddItem(new MenuItem());
    Menu* subMenu = new Menu();
    subMenu->AddItem(new MenuItem(L"Hello World!"));
    basicMenu->AddItem(new MenuItem(L"测试", subMenu));
}

MainWindow::~MainWindow(){
    if(basicMenu) delete basicMenu;
    if(uiMgr) delete uiMgr;
    if(mesh) delete mesh;
    if(curOp) delete curOp;
    if(colorBoard) delete colorBoard;
}

void MainWindow::InitCamera(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, ViewportManager::inst->GetAspect(), 0.1, 100.0);

    Vector3 camPos = camLookat - camDir * Vector3::forward * camDis;
    Vector3 camUp = camDir * Vector3::up;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camPos.x, camPos.y, camPos.z,
            camLookat.x, camLookat.y, camLookat.z,
            camUp.x, camUp.y, camUp.z);
}

void MainWindow::InitLight0(){
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };

    GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };  //最后一个参数为0表示该光源是directional的

    GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void MainWindow::RenderModelView(){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_BLEND);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_ALPHA_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    //glDisable(GL_DITHER); 这几个就一直不用
    //glDisable(GL_FOG);
    //glDisable(GL_LOGIC_OP);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    InitCamera();

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    // xyz坐标轴
    glDisable(GL_LIGHTING);
    glEnable(GL_LINE_SMOOTH);
    glDisable(GL_LINE_STIPPLE);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-camRange, 0.0f, 0.0f); glVertex3f(camRange, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -camRange, 0.0f); glVertex3f(0.0f, camRange, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -camRange); glVertex3f(0.0f, 0.0f, camRange);
    glEnd();
    glDisable(GL_LINE_SMOOTH);

    glLineWidth(1.0f);
    glBegin(GL_LINES);
    glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
    for (float i = 0.02f; i < 1.0f; i += 0.02f){
        glVertex3f(-camRange, i * camRange, 0.0f); glVertex3f(camRange, i * camRange, 0.0f);
        glVertex3f(-camRange, -i * camRange, 0.0f); glVertex3f(camRange, -i * camRange, 0.0f);
        glVertex3f(i * camRange, -camRange, 0.0f); glVertex3f(i * camRange, camRange, 0.0f);
        glVertex3f(-i * camRange, -camRange, 0.0f); glVertex3f(-i * camRange, camRange, 0.0f);
    }
    glEnd();

    InitLight0(); //TODO 后续光照设置法线
    glEnable(GL_LIGHTING);     //开启光照系统
    glEnable(GL_LIGHT0);       //开启GL_LIGHT0光源

    mesh->Render();

    // 已选择点绘制
    glDisable(GL_LIGHTING);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(6.0f);
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_POINTS);
    selectedPoints.Foreach([](Vertex* p){
        glVertex3f(p->pos.x, p->pos.y, p->pos.z);
    });
    glEnd();
    glDisable(GL_POINT_SMOOTH);

    // UI绘制
    uiMgr->Render(ViewportManager::inst->GetAspect());

    // 菜单绘制
    //glFlush();// 仅仅执行drawcall但不阻塞
    //glFinish();// 执行drawcall且阻塞直到执行完毕
    if (menu != NULL){
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glMatrixMode(GL_PROJECTION);
        glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 100.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        menu->Render(menuPos.x, menuPos.y);
    }
}

void MainWindow::OnRender(){
    //TODO 做好Container组件，实现UI尺寸坐标管理
    RenderModelView();
}

void MainWindow::SetMenu(Menu* m){
    if (menu){
        menu->ResetSelect();
    }
    menu = m;
    if (menu != NULL){
        menu->SetClientSize(cliSize);
        menu->CursorMove(cursorPos);
        menuPos = cursorPos;
    }
}

void MainWindow::UpdateWindowSize(int x, int y){
    cliSize.x = x;
    cliSize.y = y;
    cliInvSize.x = 1.0f / cliSize.x;
    cliInvSize.y = 1.0f / cliSize.y;
    aspect = (float)cliSize.x / cliSize.y;
    if (menu != NULL){
        menu->SetClientSize(cliSize);
    }
}

//TODO 添加一些UI范围检测
void MainWindow::UpdateCursor(int x, int y){
    cursorPos.x = 2.0f * x / cliSize.x - 1.0f;
    cursorPos.y = 2.0f * y / cliSize.y - 1.0f;
    cursorDir = camForward + camRight * cursorPos.x * aspect + camUp * cursorPos.y;
    if (menu != NULL){
        menu->CursorMove(cursorPos - menuPos);
        return;
    }
    uiMgr->CursorMove(cursorPos);
    if (curOp){
        curOp->OnMove();
    }
}

void MainWindow::UpdateLookAtLocation(){
    camPos = camLookat - camForward * camDis;
}

void MainWindow::UpdateRotation(){
    camForward = camDir * Vector3::forward;
    camUp = camDir * Vector3::up;
    camRight = camDir * Vector3::right;
    camPos = camLookat - camForward * camDis;
    cursorDir = camForward + camRight * cursorPos.x * aspect + camUp * cursorPos.y;
}

void MainWindow::UpdateDistance(){
    DebugLog("Camera Distance %f", camDis);
    camPos = camLookat - camForward * camDis;
    if (camRange < camDis * 20.0f){
        camRange *= 2.0f;
    }else if (camRange > camDis * 40.0f){
        camRange *= 0.5f;
    }
}

/** synchronized **/
void MainWindow::GetTextInput(){
    static MainWindow* window;
    window = this;
    DialogBox(hInst, MAKEINTRESOURCE(IDD_TEXT), hWnd, [](HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
        switch (uMsg){
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;
        case WM_CLOSE:
            window->inputConfirm = false;
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        case WM_COMMAND:
            switch (LOWORD(wParam)){
            case IDOK:
                window->inputConfirm = true;
                GetDlgItemText(hDlg, IDC_TEXT_EDIT, window->inputText, MAX_PATH);
                EndDialog(hDlg, LOWORD(wParam));
                break;
            case IDCANCEL:
                window->inputConfirm = false;
                EndDialog(hDlg, LOWORD(wParam));
                break;
            }
            return (INT_PTR)TRUE;
        }
        return (INT_PTR)FALSE;
    });
    if (inputConfirm == false){
        *inputText = '\0';
    }
}

void MainWindow::AddPoint(){
    Vector3 pos = camLookat + (camUp * cursorPos.y + camRight * cursorPos.x * aspect) * camDis;
    mesh->AddVertex(pos);
    DebugLog("Point at %f %f %f", pos.x, pos.y, pos.z);
}

void MainWindow::DeletePoint(){
    selectedPoints.Foreach<Mesh*>([](Vertex* v, Mesh* m){
        m->DeleteVertex(v);
    }, mesh);
    selectedPoints.Clear();
}

bool MainWindow::SaveMesh(Mesh* mesh){
    GetTextInput();
    inputText[MAX_PATH] = '\0';
    HANDLE hFile = CreateFile(
        inputText,
        FILE_GENERIC_READ | FILE_GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        CREATE_NEW,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (hFile == INVALID_HANDLE_VALUE){
        DebugError("OpenFile %s Error!", inputText);
        return false;
    }
    mesh->WriteToOBJ(hFile);
    CloseHandle(hFile);
    return true;
}

void MainWindow::AboutBox(){
    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, [](HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
        switch (uMsg){
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;
        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL){
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
        }
        return (INT_PTR)FALSE;
    });
}

void MainWindow::OnCreate(){
    uiMgr->AddButton(new RotateButton(Vector2(0.85f, 0.85f), 0.12f, this));
    uiMgr->AddButton(new MoveButton(Vector2(0.55f, 0.85f), 0.12f, this));
}

void MainWindow::OnClose(){
    PostQuitMessage(0);
}

void MainWindow::OnResize(int x, int y){
    UpdateWindowSize(x, y);
    UpdateCursor(x, y);
}

void MainWindow::OnMouseMove(int x, int y){
    UpdateCursor(x, y);
}

void MainWindow::OnRightDown(int x, int y){
    SetMenu(basicMenu);
}

void MainWindow::OnRightUp(int x, int y){}

void MainWindow::OnInsSave(){
    SaveMesh(mesh);
}

void MainWindow::OnInsMove(){
    curOp = new MoveOperation(this);
    curOp->OnEnter();
}

void MainWindow::OnInsSelectColor(){
    if (colorBoard == NULL){
        colorBoard = new ColorBoard();
    }
    Vector3 color = colorBoard->RunAndGetColor();
    selectedPoints.Foreach<Vector3*>([](Vertex* v, Vector3* c){
        v->color = *c;
    }, &color);
}

void MainWindow::OnInsTopology(){
    switch (selectedPoints.Size()){
    case 0:
        break;
    case 1:
        break;
    case 2:
        mesh->AddEdge(selectedPoints.GetItem(0), selectedPoints.GetItem(1));
        break;
    case 3:
        mesh->AddTriFace(selectedPoints.GetItem(0), selectedPoints.GetItem(1), selectedPoints.GetItem(2));
        break;
    default:
        break;
    }
}

void MainWindow::OnLeftDown(int x, int y){
    // 菜单消失
    if (menu){
        if (menu->InMenu(cursorPos - menuPos)){
            menu->Click();
        }
        SetMenu(NULL);
        return;
    }
    // UI交互
    if (uiMgr->LeftDown()){
        return;
    }
    // 操作
    if (curOp){
        curOp->OnConfirm();
        delete curOp;
        curOp = NULL;
        return;
    }
    // 选择点
    Vertex* v = mesh->Find(camPos, cursorDir);
    if (v == NULL){
        selectedPoints.Clear();
        DebugLog("No Point Selected");
    }else{
        selectedPoints.Add(v);
        DebugLog("Select Point %f %f %f", v->pos.x, v->pos.y, v->pos.z);
    }
}

void MainWindow::OnLeftUp(int x, int y){
    uiMgr->LeftUp();
}

void MainWindow::OnMouseWheel(int delta){
    camDis *= Pow(0.999f, delta);
    UpdateDistance();
}

void MainWindow::OnMenuAccel(int id, bool accel){
    switch (id){
    case IDM_SAVE:
        OnInsSave();
        break;
    case IDM_EXIT:
        PostQuitMessage(0);
        break;
    case IDM_ABOUT:
        AboutBox();
        break;
    case IDM_ROTATE_CCW:
        camDir *= Quaternion::AxisAngle(Vector3::forward, -10.0f);
        UpdateRotation();
        break;
    case IDM_ROTATE_CW:
        camDir *= Quaternion::AxisAngle(Vector3::forward, 10.0f);
        UpdateRotation();
        break;
    case IDM_POINT:
        AddPoint();
        break;
    case IDM_TOPOLOGY:
        OnInsTopology();
        break;
    case IDM_MOVE:
        OnInsMove();
        break;
    case IDM_ROTATE:
        break;
    case IDM_SIZE:
        break;
    case IDM_SELECT_COLOR:
        OnInsSelectColor();
        break;
    case IDM_DELETE:
        DeletePoint();
        break;
    case IDM_MESH_BASIC_PLANE:{
        Vertex* v1 = new Vertex(Vector3(-1.0f, -1.0f, 0.0f));
        Vertex* v2 = new Vertex(Vector3( 1.0f, -1.0f, 0.0f));
        Vertex* v3 = new Vertex(Vector3(-1.0f,  1.0f, 0.0f));
        Vertex* v4 = new Vertex(Vector3( 1.0f,  1.0f, 0.0f));
        mesh->AddVertex(v1);
        mesh->AddVertex(v2);
        mesh->AddVertex(v3);
        mesh->AddVertex(v4);
        mesh->AddTriFace(v1, v2, v4);
        mesh->AddTriFace(v1, v3, v4);
    }
        break;
    case IDM_MESH_BASIC_BLOCK:{
        Vertex* v1 = new Vertex(Vector3(-1.0f, -1.0f, -1.0f));
        Vertex* v2 = new Vertex(Vector3( 1.0f, -1.0f, -1.0f));
        Vertex* v3 = new Vertex(Vector3(-1.0f,  1.0f, -1.0f));
        Vertex* v4 = new Vertex(Vector3( 1.0f,  1.0f, -1.0f));
        Vertex* v5 = new Vertex(Vector3(-1.0f, -1.0f,  1.0f));
        Vertex* v6 = new Vertex(Vector3( 1.0f, -1.0f,  1.0f));
        Vertex* v7 = new Vertex(Vector3(-1.0f,  1.0f,  1.0f));
        Vertex* v8 = new Vertex(Vector3( 1.0f,  1.0f,  1.0f));
        mesh->AddVertex(v1);
        mesh->AddVertex(v2);
        mesh->AddVertex(v3);
        mesh->AddVertex(v4);
        mesh->AddVertex(v5);
        mesh->AddVertex(v6);
        mesh->AddVertex(v7);
        mesh->AddVertex(v8);
        // XY对角
        mesh->AddTriFace(v1, v2, v4);
        mesh->AddTriFace(v1, v3, v4);
        mesh->AddTriFace(v5, v6, v8);
        mesh->AddTriFace(v5, v7, v8);
        // XZ对角
        mesh->AddTriFace(v1, v2, v6);
        mesh->AddTriFace(v1, v5, v6);
        mesh->AddTriFace(v3, v4, v8);
        mesh->AddTriFace(v3, v7, v8);
        // YZ对角
        mesh->AddTriFace(v1, v3, v7);
        mesh->AddTriFace(v1, v5, v7);
        mesh->AddTriFace(v2, v4, v8);
        mesh->AddTriFace(v2, v6, v8);
    }
        break;
    }
    // 当前操作的命令
    if (curOp){
        curOp->OnCommand(id);
    }
}

void MainWindow::OnControl(int inform, int id, HWND hctl){}

void MainWindow::SetFrame(HWND hWnd){
    this->hWnd = hWnd;
}

bool MainWindow::IsFocus(){
    return focus;
}

void MainWindow::OnFocus(){
    focus = true;
}

void MainWindow::OnKillFocus(){
    focus = false;
}

Main::Main(){}

Main::~Main(){}

ATOM Main::RegClass(){
    WNDCLASSEXA wc;

    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
    wc.hIconSm = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
    wc.hInstance = hInst;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = "ModelView.MainWindow";
    wc.lpszMenuName = "";
    wc.style = 0;

    return RegisterClassExA(&wc);
}

HWND Main::CreateWnd(){
    return CreateWindowExA(
        0,
        "ModelView.MainWindow",
        "ModelView",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        750, 750,
        NULL,
        LoadMenu(hInst, MAKEINTRESOURCE(IDC_MENU)),
        hInst,
        NULL
    );
}

void Main::FireEvent(IWindow* window, RECT rect, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    RECT cliRect;
    GetClientRect(hWnd, &cliRect);
    // 事件中鼠标坐标上下需反转
    int x = GET_X_LPARAM(lParam), y = cliRect.bottom - GET_Y_LPARAM(lParam);
    switch (uMsg){
    case WM_CREATE:
        window->SetFrame(hWnd);
        window->OnCreate();
        break;
    case WM_CLOSE:
        window->OnClose();
        break;
    case WM_SIZE:
        OnResize(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;
    case WM_MOUSEMOVE:{
        bool inRect = GLUtils::InRect(x, y, rect);
        if (inRect)
            window->OnMouseMove(x - rect.left, y - rect.bottom);
    }
        break;
    case WM_MOUSEWHEEL:
        if (window->IsFocus())
            window->OnMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
        break;
    case WM_MOUSELEAVE:
        window->OnMouseLeave();
        break;
    case WM_MOUSEHOVER:
        window->OnMouseHover(wParam, x - rect.left, y - rect.bottom);
        break;
    case WM_SETFOCUS:
        break;
    case WM_KILLFOCUS:
        if (window->IsFocus())
            window->OnKillFocus();
        break;
    case WM_LBUTTONDOWN:{
        bool inRect = GLUtils::InRect(x, y, rect);
        if (inRect && !window->IsFocus()){
            window->OnFocus();
        }else if (!inRect && window->IsFocus()){
            window->OnKillFocus();
        }
        if (window->IsFocus())
            window->OnLeftDown(x - rect.left, y - rect.bottom);
    }
        break;
    case WM_LBUTTONUP:
        if (window->IsFocus())
            window->OnLeftUp(x - rect.left, y - rect.bottom);
        break;
    case WM_RBUTTONDOWN:{
        bool inRect = GLUtils::InRect(x, y, rect);
        if (inRect && !window->IsFocus()){
            window->OnFocus();
        }else if (!inRect && window->IsFocus()){
            window->OnKillFocus();
        }
        if (window->IsFocus())
            window->OnRightDown(x - rect.left, y - rect.bottom);
    }
        break;
    case WM_RBUTTONUP:
        if(window->IsFocus())
            window->OnRightUp(x - rect.left, y - rect.bottom);
        break;
    case WM_COMMAND:
        switch (HIWORD(wParam)){
        case 0:
            if (window->IsFocus())
                window->OnMenuAccel(LOWORD(wParam), false);
        case 1:
            if (window->IsFocus())
                window->OnMenuAccel(LOWORD(wParam), true);
        default:
            window->OnControl(HIWORD(wParam), LOWORD(wParam), (HWND)(DWORD_PTR)lParam);
        }
        break;
    }
}

LRESULT Main::LocalWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    FireEvent(mainWnd, mainRect, hWnd, uMsg, wParam, lParam);
    //FireEvent(mainWnd2, mainRect2, hWnd, uMsg, wParam, lParam);
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK Main::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    return Main::inst->LocalWndProc(hWnd, uMsg, wParam, lParam);
}

void Main::OnResize(int x, int y){
    //TODO 这里做子窗口大小管理
    mainRect.left = 0;
    mainRect.right = x;
    mainRect.bottom = 0;
    mainRect.top = y;
    mainWnd->OnResize(mainRect.right - mainRect.left, mainRect.top - mainRect.bottom);

    // mainRect2.left = x >> 1;
    // mainRect2.right = x;
    // mainRect2.bottom = 0;
    // mainRect2.top = y;
    // mainWnd2->OnResize(mainRect2.right - mainRect2.left, mainRect2.top - mainRect2.bottom);
}

void Main::OnRender(){
    ViewportManager::inst->Reset(hWnd);
    ViewportManager::inst->EnableScissor();
    ViewportManager::inst->PushViewport(mainRect);
    mainWnd->OnRender();
    ViewportManager::inst->PopViewport();

    // ViewportManager::inst->PushViewport(mainRect2);
    // mainWnd2->OnRender();
    // ViewportManager::inst->PopViewport();
}

int Main::WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    MSG Msg;

    hInst = hInstance;

    mainWnd = new MainWindow(hInst);
    //mainWnd2 = new MainWindow(hInst);

    RegClass();
    ColorBoard::RegClass(hInstance);

    hWnd = CreateWnd();

    //TODO 更改消息循环位置，使主视口成为组件
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    DebugLog("Main Window Created");

    GLUtils::EnableOpenGL(hWnd, &hDC, &hRC);

    DebugLog("OpenGL Enabled");
    DebugLog("OpenGL Version %s", glGetString(GL_VERSION));
    DebugLog("OpenGL Vendor %s", glGetString(GL_VENDOR));

    glInitASCIIFont();
    glSelectFont(12, GB2312_CHARSET, "楷体_GB2312");

    DebugLog("OpenGL Use Font %s", "GB2312");

    HACCEL hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE(IDC_MENU));

    while (GetMessage(&Msg, NULL, 0, 0)){
        if (!TranslateAccelerator(Msg.hwnd, hAccelTable, &Msg)){
            if (Msg.message == WM_QUIT){
                break;
            } else {
                TranslateMessage(&Msg);
                DispatchMessage(&Msg);
                wglMakeCurrent(hDC, hRC);
                OnRender();
                SwapBuffers(hDC);
            }
        }
    }

    GLUtils::DisableOpenGL(hWnd, hDC, hRC);

    DestroyWindow(hWnd);
    
    return Msg.wParam;
}

HINSTANCE Main::hInst;
Main* Main::inst;
HWND Main::hWnd;
HDC Main::hDC;
HGLRC Main::hRC;

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    Main::inst = new Main();
    int ret = Main::inst->WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    delete Main::inst;
    return ret;
}