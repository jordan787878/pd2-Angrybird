#ifndef USERDATA_H
#define USERDATA_H
#include "Box2D/Box2D.h"
#include <QString>

struct bodyUserData {
    QString ID_NAME;
    float MaxImpulse;

    //    bodyUserData* udStruct = (bodyUserData*)EnemyItem_body->GetUserData();
};

#endif // USERDATA_H
