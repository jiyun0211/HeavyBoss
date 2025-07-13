// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PlayerGameplayAbility.h"
#include "GameplayEffectTypes.h"
#include "PlayerHeroGameplayAbility.generated.h"

class APlayerCharacter;
class AMyPlayerController;
/**
 * 
 */
UCLASS()
class MAV_BOSSFIGHT_API UPlayerHeroGameplayAbility : public UPlayerGameplayAbility
{
	GENERATED_BODY()

	
public:
 	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
 	APlayerCharacter* GetPlayerCharacterFromActorInfo();
 
 	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
 	AMyPlayerController* GetMyControllerFromActorInfo();
 
 	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
 	UHeroCombatComponent* GetHeroCombatComponentFromActorInfo();

	UFUNCTION(BlueprintCallable, Category = "GAS|Attribute")
    static float GetCurrentHealthFromSpecHandle(const FGameplayEffectSpecHandle& SpecHandle);

	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	FGameplayEffectSpecHandle MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass,float InWeaponBaseDamage,FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount);

	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	FGameplayEffectSpecHandle MakePlayerStaminaEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass,float StaminaChange);

private:
 	TWeakObjectPtr<APlayerCharacter> CachedPlayerCharacter;
 	TWeakObjectPtr<AMyPlayerController> CachedMyPlayerController;
};
