#ifndef __EDITOR_MAIN_TRANSFORM__
#define __EDITOR_MAIN_TRANSFORM__

#include <define.h>

#include <utils/math3d/LinearAlgebra.h>
#include <utils/math3d/Property.h>

class Transform final : public Object {
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

    RotationMode rotationMode = ROT_EULER_XYZ;

    // 变换顺序: 大小、旋转、位置
    PropertyVector3 position;
    PropertyQuaternion rotation;
    PropertyVector3 rotationXYZ;
    PropertyVector3 scale;
    
    // Object To World
    Matrix4x4 chainMat = Matrix4x4::identity;
    // World To Object
    Matrix4x4 chainInvMat = Matrix4x4::identity;

    Transform();
    ~Transform();

    Quaternion GetRotation();
    Matrix4x4 GetMatrix();
    Matrix4x4 GetInvMatrix();
    void PushMatrix();
    void PushInvMatrix();
    void PopMatrix();

    void SetFrame(float frame);

    void InsertPos(float frame);
    void InsertRot(float frame);
    void InsertScale(float frame);
};

#endif