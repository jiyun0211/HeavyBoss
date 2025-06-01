// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance/PlayerAnimInstance.h"
#include "PlayerLinkedAnimLayer.generated.h"


class UHeroAnimInstance;
/**
 * 
 */
UCLASS()
class MAV_BOSSFIGHT_API UPlayerLinkedAnimLayer : public UPlayerAnimInstance
{
	GENERATED_BODY()

public:
 	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
 	UHeroAnimInstance* GetHeroAnimInstance() const;
};
