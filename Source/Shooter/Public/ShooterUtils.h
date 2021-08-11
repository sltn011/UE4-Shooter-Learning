#pragma once

#include "Player/ShooterPlayerState.h"

class AActor;

class ShooterUtils {

public:

    template<typename ComponentT>
    static ComponentT *GetPlayerComponentByClass(
        AActor const *PlayerActor
    )
    {
        if (!PlayerActor) {
            return nullptr;
        }

        return Cast<ComponentT>(PlayerActor->GetComponentByClass(ComponentT::StaticClass()));
    }

    static bool AreEnemies(
        AController *Controller1,
        AController *Controller2
    )
    {
        if (!Controller1 || !Controller2 || Controller1 == Controller2) {
            return false;
        }

        AShooterPlayerState *State1, *State2;
        State1 = Controller1->GetPlayerState<AShooterPlayerState>();
        State2 = Controller2->GetPlayerState<AShooterPlayerState>();
        if (!State1 || !State2) {
            return false;
        }

        return State1->TeamID != State2->TeamID;
    }

};