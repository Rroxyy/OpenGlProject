#include "Transform.h"
#include "Object.h"
#include "ModelComponent.h"

void Transform::updateAABB()
{
    
    if (!object)return;
    ModelComponent* ptr = object->GetComponentExact<ModelComponent>();
    if (!ptr)return;
    ptr->resetAABB();
    
}


