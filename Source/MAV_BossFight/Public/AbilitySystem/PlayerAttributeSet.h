// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/PlayerAbilitySystemComponent.h"
#include "PlayerAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class MAV_BOSSFIGHT_API UPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPlayerAttributeSet();

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Phase")
	FGameplayAttributeData Phase;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet,Phase)

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet,CurrentHealth)
	
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet,MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Recover")
	FGameplayAttributeData RecoveryCool;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet,RecoveryCool)

	UPROPERTY(BlueprintReadOnly, Category = "Recover")
	FGameplayAttributeData MaxCool;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet,MaxCool)
	
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet,AttackPower)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet,DefensePower)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet,DamageTaken)

	UPROPERTY(BlueprintReadOnly, Category = "Level")
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UPlayerAttributeSet,Level)
};
