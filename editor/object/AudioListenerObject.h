#ifndef __EDITOR_OBJECT_AUDIOLISTENEROBJECT__
#define __EDITOR_OBJECT_AUDIOLISTENEROBJECT__

#include <define.h>

#include <utils/math3d/LinearAlgebra.h>
#include <editor/main/ViewObject.h>

class AudioListenerObject final : public AViewObject {
private:
    Point3 recPos;
    float recTime;
    bool dopplerEffect = true;

public:
    AudioListenerObject();
    virtual ~AudioListenerObject() override;

    // 使用渲染回调获取最新位置信息，并更新AudioListener
    virtual void OnRender() override;

    void SetPosv3(Point3 value);
    void SetVelocityv3(Vector3 value);
    void SetPosAutoVelv3(Point3 value);
    
    void SetDopplerEffect(bool on);
    bool HasDopplerEffect();
};

#endif