// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PlayerBaseCharacter.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Abilities/PlayerHeroGameplayAbility.h"
#include "AbilitySystem/PlayerAbilitySystemComponent.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UDataAsset_InputConfig;
struct FInputActionValue;

class UHeroCombatComponent;
/**
 * 
 */
UCLASS()
class MAV_BOSSFIGHT_API APlayerCharacter : public APlayerBaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	//~ Begin PawnCombatInterface Interface.
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End PawnCombatInterface Interface
	
	UPROPERTY(BlueprintReadWrite, Category = "Warrior")
	int32 ComboMode = 0;
	
protected:

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	
private:

#pragma region Components
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
 	UHeroCombatComponent* HeroCombatComponent;
 
#pragma endregion

#pragma region Inputs
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

	void Input_AbilityInputPressed(FGameplayTag InInputTag);
 	void Input_AbilityInputReleased(FGameplayTag InInputTag);
 
#pragma endregion

public:
 	FORCEINLINE UHeroCombatComponent* GetHeroCombatComponent() const { return HeroCombatComponent;}
};
