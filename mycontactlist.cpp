#define Pig_Block_Factor 0.2
#define Pig_Bird_Factor 3
#define Pig_Ground_Factor 6.25

#include "mycontactlist.h"
#include "gameitem.h"
#include "enemyitem.h"
#include <QDebug>
#include "game.h"

extern Game * game;

void MyContactList::BeginContact(b2Contact * contact)
{
   b2Fixture * fa = contact->GetFixtureA();
   b2Fixture * fb = contact->GetFixtureB();

   bodyUserData* fa_userdata = (bodyUserData*)fa->GetBody()->GetUserData();
   bodyUserData* fb_userdata = (bodyUserData*)fb->GetBody()->GetUserData();


   //if Bird contact
   if( fa_userdata->ID_NAME == QString("Bird") )
   {
       if(fb_userdata->ID_NAME == QString("Pig"))
       {
           game->pig_delete_SOUND->play();
       }

       if(fb_userdata->ID_NAME == QString("BlackBlock") )
       {
           game->Wood_colli_sound->play();
       }

       if(fb_userdata->ID_NAME == QString("GrayBlock"))
       {
           game->Rock_colli_sound->play();
       }

       if(fb_userdata->ID_NAME == QString("Ground"))
       {
           game->Bird_Bouncing_Sound->setVolume(10*fa->GetBody()->GetLinearVelocity().LengthSquared());
           game->Bird_Bouncing_Sound->play();
       }
   }

   if(fb_userdata->ID_NAME == QString("Bird"))
   {
       if(fa_userdata->ID_NAME == QString("Pig"))
       {
           game->pig_delete_SOUND->play();
       }

       if(fa_userdata->ID_NAME == QString("BlackBlock") )
       {
           game->Wood_colli_sound->play();
       }

       if(fa_userdata->ID_NAME == QString("GrayBlock"))
       {
           game->Rock_colli_sound->play();
       }

       if(fa_userdata->ID_NAME == QString("Ground"))
       {
           game->Bird_Bouncing_Sound->setVolume(10*fb->GetBody()->GetLinearVelocity().LengthSquared());
           game->Bird_Bouncing_Sound->play();
       }
   }

   //check pig contact with ground
   if(fb_userdata->ID_NAME == QString("Pig"))
   {
       if(fa_userdata->ID_NAME == QString("Ground"))
       {
           float groundImpulse = fb->GetBody()->GetLinearVelocity().Length()*Pig_Ground_Factor;
           if(groundImpulse  > 0)  {
              fb_userdata->MaxImpulse = fb_userdata->MaxImpulse - groundImpulse;
              groundImpulse = 0;
           }
       }

   }

}

void MyContactList::EndContact(b2Contact * contact)
{

}

void MyContactList::PreSolve(b2Contact *contact, const b2Manifold *oldManifold)
{

}

void MyContactList::PostSolve(b2Contact *contact, const b2ContactImpulse *Impulse)
{
    b2Fixture * fa = contact->GetFixtureA();
    b2Fixture * fb = contact->GetFixtureB();

    if(fa->GetBody()->GetMass() == 0 || fb->GetBody()->GetMass() == 0 ) { return; }

    bodyUserData* fa_userdata = (bodyUserData*)fa->GetBody()->GetUserData();
    bodyUserData* fb_userdata = (bodyUserData*)fb->GetBody()->GetUserData();


    //solve for Bird impluse on Blocks
    if(fb_userdata->ID_NAME == QString("Bird"))
    {

        if(fa_userdata->ID_NAME == QString("BlackBlock"))
        {
            if(Impulse->normalImpulses[0] > ExertImpulse)
            ExertImpulse = Impulse->normalImpulses[0];

            if(ExertImpulse > 0) {
               ExertImpulse = ExertImpulse/1000000;
               fa_userdata->MaxImpulse = fa_userdata->MaxImpulse - ExertImpulse;

               //display hit score
               Score * DisPlayHit = new Score(true,fb->GetBody()->GetPosition().x,
                                              game->size().height()-fb->GetBody()->GetPosition().y,
                                              (int)ExertImpulse,300);
               ExertImpulse = 0;
            }
        }

        if(fa_userdata->ID_NAME == QString("GrayBlock"))
        {
            if(Impulse->normalImpulses[0] > ExertImpulse)
            ExertImpulse = Impulse->normalImpulses[0];

            if(ExertImpulse > 0) {
               ExertImpulse = ExertImpulse/1000000;
               fa_userdata->MaxImpulse = fa_userdata->MaxImpulse - ExertImpulse;


               //display hit score
               Score * DisPlayHit = new Score(true,fb->GetBody()->GetPosition().x,
                                             game->size().height()-fb->GetBody()->GetPosition().y,
                                             (int)ExertImpulse,300);

               ExertImpulse = 0;
            }
        }

    }




    //Solve for Impulse exert on Pig
    if(fa_userdata->ID_NAME == QString("Pig"))
    {
        if(fb_userdata->ID_NAME == QString("Bird"))
        {
            ExertImpulse = Impulse->normalImpulses[0];
            if(ExertImpulse > 0)
            {
               ExertImpulse = Pig_Bird_Factor*ExertImpulse/1000000;
               fa_userdata->MaxImpulse =
               fa_userdata->MaxImpulse - ExertImpulse;
               ExertImpulse = 0;
            }
        }

    }

    if(fb_userdata->ID_NAME == QString("Pig"))
    {
        if(fa_userdata->ID_NAME == QString("Bird"))
        {
            ExertImpulse = Impulse->normalImpulses[0];
            if(ExertImpulse > 0)
            {
               ExertImpulse = Pig_Bird_Factor*ExertImpulse/1000000;
               fb_userdata->MaxImpulse =
               fb_userdata->MaxImpulse - ExertImpulse;
               ExertImpulse = 0;
            }
        }

    }



    if(fb_userdata->ID_NAME == QString("Pig"))
    {
        if(fa_userdata->ID_NAME == QString("BlackBlock"))
        {
            ExertImpulse = Impulse->normalImpulses[0];

            if(ExertImpulse > 0)
            {
               ExertImpulse = Pig_Block_Factor*ExertImpulse/1000000;
               fb_userdata->MaxImpulse =
               fb_userdata->MaxImpulse - ExertImpulse;
               ExertImpulse = 0;
            }
        }

        if(fa_userdata->ID_NAME == QString("GrayBlock"))
        {
            ExertImpulse = Impulse->normalImpulses[0];

            if(ExertImpulse > 0)
            {
               ExertImpulse = 2*Pig_Block_Factor*ExertImpulse/1000000;
               fb_userdata->MaxImpulse =
               fb_userdata->MaxImpulse - ExertImpulse;
               ExertImpulse = 0;
            }
        }

    }


}


