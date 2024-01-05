#pragma once

#include "CoreMinimal.h"
#include "Misc/MessageDialog.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"

namespace DebugHeader 
{
    static void Print(const FString& message, const FColor& color)
    {
        if(GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 80.f, color, message);
        }
    }

    static void PrintLog(const FString& message)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *message);
    }

    static EAppReturnType::Type ShowMsgDialog(EAppMsgType::Type msgType, const FString& message, bool bShowMsdAsWarning = false)
    {
        if(bShowMsdAsWarning)
        {
            FText title = FText::FromString("Warning");
            
            return FMessageDialog::Open(msgType, FText::FromString(message), title);
        }
        else
        {
            return FMessageDialog::Open(msgType, FText::FromString(message));
        }
    }       

    static void ShowNotifyInfo(const FString& message)
    {
        FNotificationInfo NotifyInfo(FText::FromString(message));
        NotifyInfo.bUseLargeFont = true;
        NotifyInfo.FadeOutDuration = 7.f;

        FSlateNotificationManager::Get().AddNotification(NotifyInfo);
    }
}