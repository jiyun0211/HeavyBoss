// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PlayFab.h"
#include "Core/PlayFabError.h"
#include "Core/PlayFabClientDataModels.h"
#include "PlayFabClientDataModels.h"
#include "PlayFabClientAPI.h"
#include "WarriorGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MAV_BOSSFIGHT_API UWarriorGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	UPROPERTY(BlueprintReadWrite);
	float StartTime = 0.0f;

	UPROPERTY(BlueprintReadWrite);
	float EndTime = 0.0f;

	UPROPERTY(BlueprintReadWrite);
	float ClearTime = 0.0f;

	UFUNCTION(BlueprintCallable)
	void SetStartTime();

	UFUNCTION(BlueprintCallable)
	void SetEndTime();

	UFUNCTION(BlueprintCallable)
	void RecordClearTime();

	UFUNCTION(BlueprintCallable)
	void SaveClearTime(float Time);

private:
	void LoginWithDeviceID();

	void OnLoginSuccess(const PlayFab::ClientModels::FLoginResult& Result);
	void OnLoginError(const PlayFab::FPlayFabCppError& Error);
};
