// Fill out your copyright notice in the Description page of Project Settings.
#include "WarriorGameInstance.h"
#include "PlayFabClientAPI.h"
#include "PlayFabClientModels.h"
#include "Misc/Guid.h"
#include "Kismet/GameplayStatics.h"

using namespace PlayFab;
using namespace ClientModels;

void UWarriorGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Log, TEXT("WarriorGameInstance Init 실행됨. 로그인 시도..."));
	//LoginWithDeviceID();
}

void UWarriorGameInstance::LoginWithDeviceID()
{
    FLoginWithCustomIDRequest Request;

    FString DeviceId = FPlatformMisc::GetDeviceId();
    if (DeviceId.IsEmpty())
    {
        DeviceId = FGuid::NewGuid().ToString(EGuidFormats::Digits);
    }

    Request.CustomId = DeviceId;
    Request.CreateAccount = true;

    /*UPlayFabClientAPI::LoginWithCustomID(
        Request,
        PlayFab::UPlayFabClientAPI::FLoginWithCustomIDDelegate::CreateUObject(this, &UWarriorGameInstance::OnLoginSuccess),
        PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UWarriorGameInstance::OnLoginError)
    );*/
}

void UWarriorGameInstance::OnLoginSuccess(const FLoginResult& Result)
{
    UE_LOG(LogTemp, Log, TEXT("로그인 성공! PlayFabId: %s"), *Result.PlayFabId);
}

void UWarriorGameInstance::OnLoginError(const FPlayFabCppError& Error)
{
    UE_LOG(LogTemp, Error, TEXT("로그인 실패! Error: %s"), *Error.ErrorMessage);
}

void UWarriorGameInstance::SetStartTime()
{
    StartTime = UGameplayStatics::GetTimeSeconds(GetWorld());
}

void UWarriorGameInstance::SetEndTime()
{
    EndTime = UGameplayStatics::GetTimeSeconds(GetWorld());
}

void UWarriorGameInstance::RecordClearTime()
{
    ClearTime = EndTime - StartTime;
}

void UWarriorGameInstance::SaveClearTime(float Time)
{
    FString TimeString = FString::SanitizeFloat(Time);

    PlayFab::ClientModels::FUpdateUserDataRequest Request;
    Request.Data.Add("ClearTime", TimeString);

    UPlayFabClientAPI::UpdateUserData(Request,
        PlayFab::UPlayFabClientAPI::FUpdateUserDataDelegate::CreateUObject(this, &UWarriorGameInstance::OnSaveSuccess),
        PlayFab::FPlayFabErrorDelegate::CreateUObject(this, &UWarriorGameInstance::OnSaveError)
    );
}