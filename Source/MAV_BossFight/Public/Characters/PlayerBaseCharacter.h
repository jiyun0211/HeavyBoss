// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interfaces/PawnCombatInterface.h"
#include "PlayerBaseCharacter.generated.h"

class UPlayerAbilitySystemComponent;
class UPlayerAttributeSet;
class UDataAsset_StartUpDataBase;
class UMotionWarpingComponent;



UCLASS()
class MAV_BOSSFIGHT_API APlayerBaseCharacter : public ACharacter, public IAbilitySystemInterface, public IPawnCombatInterface
{
	GENERATED_BODY()

public:
	
	// Sets default values for this character's properties
	APlayerBaseCharacter();

	//~ Begin IAbilitySystemInterface Interface.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface Interface

	//~ Begin PawnCombatInterface Interface.
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End PawnCombatInterface Interface

protected:

	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UPlayerAbilitySystemComponent* PlayerAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UPlayerAttributeSet* PlayerAttributeSet;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MotionWarping")
	UMotionWarpingComponent* MotionWarpingComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
 	TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;
	
public:
	FORCEINLINE UPlayerAbilitySystemComponent* GetPlayerAbilitySystemComponent() const {return PlayerAbilitySystemComponent;}

	FORCEINLINE UPlayerAttributeSet* GetPlayerAttributeSet() const {return PlayerAttributeSet;}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Level")
	int32 CharacterLevel = 1;

	UFUNCTION(BlueprintCallable, Category="Level")
	virtual int32 GetLv() const { return CharacterLevel; }

	UFUNCTION(BlueprintCallable, Category="Level")
	virtual void SetLv(int32 NewLevel) { CharacterLevel = NewLevel; }
};
