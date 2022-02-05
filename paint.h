#ifndef __PAINT__
#define __PAINT__

#include "uimgr.h"
#include "menu.h"

class Texture1D;
class Texture2D;
class Texture3D;
class RenderTexture;

class UVEditWindow;
class PaintWindow;

class UVEditWindow : public IWindow {
private:
    bool focus = false;

    Vector2 cursorPos = Vector2::zero;
    Vector3 cursorDir = Vector3::zero;
    Vector2 cliSize;
    float aspect;

    ITool* curTool;

    class EmptyTool : public ITool {
    private:
        UVEditWindow* window;

    public:
        EmptyTool(UVEditWindow* window);
        ~EmptyTool() override;
        virtual void OnLeftDown() override;
    };

public:
    UVEditWindow();
    ~UVEditWindow();

    virtual bool IsFocus() override;
    virtual void OnRender() override;
    virtual void OnResize(int x, int y) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;
    virtual void OnFocus() override;
    virtual void OnKillFocus() override;
    virtual void OnMenuAccel(int id, bool accel) override;

    void UpdateCursor(int x, int y);
    void UpdateWindowSize(int x, int y);

    void SetTool(ITool* tool);
};

class PaintWindow : public IWindow {
private:
    bool focus;
    UIManager* uiMgr;
    Menu* menu;
    IOperation* curOp;
    ITool* brush;

public:
    PaintWindow();
    virtual ~PaintWindow();

    virtual bool IsFocus() override;
    virtual void OnRender() override;
    virtual void OnCreate() override;
    virtual void OnClose() override;
    virtual void OnTimer(int id) override;
    virtual void OnChar(char c) override;
    virtual void OnUnichar(wchar_t c) override;
    virtual void OnResize(int x, int y) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;
    virtual void OnMouseHover(int key, int x, int y) override;
    virtual void OnMouseLeave() override;
    virtual void OnFocus() override;
    virtual void OnKillFocus() override;
    virtual void OnMouseWheel(int delta) override;
    virtual void OnMenuAccel(int id, bool accel) override;
    virtual void OnDropFileA(const char* path) override;
    virtual void OnDropFileW(const wchar_t* path) override;
};

#endif