#ifndef MYCONTACTLIST_H
#define MYCONTACTLIST_H
#include "Box2D/Box2D.h"


class MyContactList : public b2ContactListener{

public:
    void BeginContact(b2Contact *contact);
    void EndContact(b2Contact * contact);
    void PreSolve(b2Contact * contact, const b2Manifold * oldManifold);
    void PostSolve(b2Contact * contact, const b2ContactImpulse * Impulse);

    float ExertImpulse = 0;

};

#endif // MYCONTACTLIST_H
