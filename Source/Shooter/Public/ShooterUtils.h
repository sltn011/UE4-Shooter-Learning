#pragma once

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

};