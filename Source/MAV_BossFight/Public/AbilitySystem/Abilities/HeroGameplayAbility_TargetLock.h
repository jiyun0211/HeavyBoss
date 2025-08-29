// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PlayerHeroGameplayAbility.h"
#include "HeroGameplayAbility_TargetLock.generated.h"


class APlayerCharacter;

/**
 * 
 */
UCLASS()
class MAV_BOSSFIGHT_API UHeroGameplayAbility_TargetLock : public UPlayerHeroGameplayAbility
{
	GENERATED_BODY()
public:
	// Boss 자동 탐색용 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TargetLock|AutoFind")
	TSubclassOf<AActor> BossClass; // 보스 클래스 지정

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TargetLock|AutoFind")
	FName BossTag = NAME_None;     // 보스 태그 지정

private:
	UPROPERTY()
	AActor* CurrentLockedActor;

	void CancelTargetLockAbility();

	UPROPERTY(EditDefaultsOnly, Category = "Target Lock")
	float TargetLockRotationInterpSpeed = 5.f;


protected:
	UFUNCTION(BlueprintCallable)
	void OnTargetLockTick(float DeltaTime);

	/** Called from ActivateAbility to initiate target locking */
	void TryLockOnTarget();

	/** Returns the pre-registered target actor (boss) */
	AActor* ResolveRegisteredTarget();

	/** The final set of actors available to lock (usually just one in this version) */
	UPROPERTY(BlueprintReadOnly, Category="TargetLock")
	TArray<AActor*> AvailableActorsToLock;

	//~~ Begin UGameplayAbility Interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

};
