// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PlayerGameplayAbility.h"
#include "AbilitySystem/PlayerAbilitySystemComponent.h"
#include "Characters\PlayerCharacter.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"



 
void UPlayerGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
 	Super::OnGiveAbility(ActorInfo, Spec);
 
 	if (AbilityActivationPolicy == EPlayerAbilityActivationPolicy::OnGiven)
 	{
 		if (ActorInfo && !Spec.IsActive())
 		{
 			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
 	}
}

int32 UPlayerGameplayAbility::GetComboModeFromCharacter() const
{
	APlayerCharacter* PC = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	if (PC)
	{
		return PC->ComboMode;
	}
	return -1;
}

void UPlayerGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
 	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
 	if (AbilityActivationPolicy == EPlayerAbilityActivationPolicy::OnGiven)
 	{
 		if (ActorInfo)
 		{
 			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
 		}
 	}
}

UPawnCombatComponent* UPlayerGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{	
 	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}

UPlayerAbilitySystemComponent* UPlayerGameplayAbility::GetPlayerAbilitySystemComponentFromActorInfo() const
{	
 	return Cast<UPlayerAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

FActiveGameplayEffectHandle UPlayerGameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{	
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (!TargetASC)
	{
		UE_LOG(LogTemp, Error, TEXT("NativeApplyEffectSpecHandleToTarget: TargetASC is null. TargetActor: %s"), *GetNameSafe(TargetActor));
		return FActiveGameplayEffectHandle(); // Invalid handle 반환
	}

	if (!InSpecHandle.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("NativeApplyEffectSpecHandleToTarget: InSpecHandle is invalid in ability %s"), *GetName());
		return FActiveGameplayEffectHandle();
	}


	return GetPlayerAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*InSpecHandle.Data,
		TargetASC
	);
}

FActiveGameplayEffectHandle UPlayerGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EWarriorSuccessType& OutSuccessType)
{
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor,InSpecHandle);

	OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied()? EWarriorSuccessType::Successful : EWarriorSuccessType::Failed;

	return ActiveGameplayEffectHandle;
}