#include "camera.h"

#include "GodClass.h"


void Camera::ProcessMouseScroll(float yoffset)
{
    GodClass::getInstance().projectionDirty = true;

    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}
