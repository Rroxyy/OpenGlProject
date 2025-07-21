#include "Transform.h"
#include "Object.h"
#include "model.h"

void Transform::updateAABB()
{
    
    if (!object)return;
    Model* ptr = object->GetComponentExact<Model>();
    if (!ptr)return;
    ptr->resetAABB();
    
}


