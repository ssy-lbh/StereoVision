#ifndef __VIEWOBJECT__
#define __VIEWOBJECT__

#include "list.h"
#include "vecmath.h"
#include "mesh.h"

class Transform;
class AViewObject;
class MeshObject;

class Transform {
public:
    enum RotationMode {
        ROT_QUATERNION,
        ROT_EULER_XYZ,
        ROT_EULER_XZY,
        ROT_EULER_YZX,
        ROT_EULER_YXZ,
        ROT_EULER_ZXY,
        ROT_EULER_ZYX
    };

    RotationMode rotationMode = ROT_QUATERNION;

    Vector3 position = Vector3::zero;
    Quaternion rotation = Quaternion::one;
    Vector3 rotationXYZ = Vector3::zero;
    Vector3 scale = Vector3::one;
};

//TODO 3D视口中的可视对象，内部继承后可作为网格体、声源、曲线等
//TODO 正在设计中
class AViewObject {
public:
    Transform transform;

protected:
    AViewObject* parent = NULL;

    List<AViewObject*> children;

public:
    AViewObject();
    virtual ~AViewObject();

    void AddChild(AViewObject* o);
    // 此函数不会回收内存，回收在解构器中进行
    bool DeleteChild(AViewObject* o);
    void EnumChildren(void(*func)(AViewObject*));
    void EnumChildren(void(*func)(AViewObject*, void*), void* user);
    List<AViewObject*>& GetChildren();

    virtual AViewObject* OnSelectObject(Vector3 origin, Vector3 direction, float* distance);
    virtual void OnSelect(Vector3 ori, Vector3 dir);
    virtual void OnSelect(Vector3 camPos, Quaternion camDir, Vector2 zBound, Vector2 p1, Vector2 p2);
    virtual void OnSelectUV(Vector2 uv);
    virtual void OnSelectUV(Vector2 uv1, Vector2 uv2);

    virtual void OnRender();
    virtual void OnTimer(int id);
    virtual void OnChar(char c);
    virtual void OnUnichar(wchar_t c);
    virtual void OnMenuAccel(int id, bool accel);

    virtual void OnAnimationFrame(int frame);
};

class MeshObject : public AViewObject {
private:
    Mesh* mesh;

public:
    MeshObject();
    virtual ~MeshObject() override;

    virtual AViewObject* OnSelectObject(Vector3 origin, Vector3 direction, float* distance) override;
    virtual void OnSelect(Vector3 ori, Vector3 dir) override;
    virtual void OnSelect(Vector3 camPos, Quaternion camDir, Vector2 zBound, Vector2 p1, Vector2 p2) override;
    virtual void OnSelectUV(Vector2 uv) override;
    virtual void OnSelectUV(Vector2 uv1, Vector2 uv2) override;

    virtual void OnRender() override;
    virtual void OnMenuAccel(int id, bool accel) override;
};

#endif