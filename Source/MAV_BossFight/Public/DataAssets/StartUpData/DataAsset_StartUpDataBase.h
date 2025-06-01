// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UPlayerGameplayAbility;
class UPlayerAbilitySystemComponent;
class UGameplayEffect;

/**
 * 
 */
UCLASS()
class MAV_BOSSFIGHT_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UPlayerAbilitySystemComponent* InASCToGive,int32 ApplyLevel = 1);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray< TSubclassOf < UPlayerGameplayAbility > > ActivateOnGivenAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray< TSubclassOf < UPlayerGameplayAbility > > ReactiveAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray< TSubclassOf < UGameplayEffect > > StartUpGameplayEffects;

	void GrantAbilities(const TArray< TSubclassOf < UPlayerGameplayAbility > >& InAbilitiesToGive,UPlayerAbilitySystemComponent* InASCToGive,int32 ApplyLevel = 1);
	
};
