#pragma once

class UUserWidget;

class UIUtils {

public:

    template<typename ComponentT>
    static ComponentT *GetPlayerPawnComponentByClass(
        APawn const *PlayerPawn
    )
    {
        if (!Player) {
            return nullptr;
        }

        return Cast<ComponentT>(Player->GetComponentByClass(ComponentT::StaticClass()));
    }

};