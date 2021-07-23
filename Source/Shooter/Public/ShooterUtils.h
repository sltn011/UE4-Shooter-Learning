#pragma once

class APawn;

class ShooterUtils {

public:

    template<typename ComponentT>
    static ComponentT *GetPlayerComponentByClass(
        APawn const *PlayerPawn
    )
    {
        if (!PlayerPawn) {
            return nullptr;
        }

        return Cast<ComponentT>(PlayerPawn->GetComponentByClass(ComponentT::StaticClass()));
    }

};