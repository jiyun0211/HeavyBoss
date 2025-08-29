// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/HeroGameplayAbility_TargetLock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"    
#include "MAV_BossFightDebugHelper.h"
#include "Kismet/KismetMathLibrary.h"
#include "WarriorFunctionLibrary.h"

#include "Kismet/KismetMathLibrary.h"
#include "MyGamePlayTags.h"


void UHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    TryLockOnTarget();

}

void UHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_TargetLock::OnTargetLockTick(float DeltaTime)
{
	if (!CurrentLockedActor ||
		UWarriorFunctionLibrary::NativeDoesActorHaveTag(CurrentLockedActor,MyGameplayTags::Shared_Status_Dead) ||
		UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(), MyGameplayTags::Shared_Status_Dead)
		)
	{
		CancelTargetLockAbility();
	}


	const bool bShouldOverrideRotation =
	!UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(),MyGameplayTags::Player_Status_Rolling)
	&&
	!UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(),MyGameplayTags::Player_Status_Blocking);

	if (bShouldOverrideRotation)
	{
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(
			GetPlayerCharacterFromActorInfo()->GetActorLocation(),
			CurrentLockedActor->GetActorLocation()
		);

		const FRotator CurrentControlRot = GetPlayerCharacterFromActorInfo()->GetControlRotation();
		const FRotator TargetRot = FMath::RInterpTo(CurrentControlRot,LookAtRot,DeltaTime,TargetLockRotationInterpSpeed);

		GetMyControllerFromActorInfo()->SetControlRotation(FRotator(TargetRot.Pitch,TargetRot.Yaw,0.f));
		GetPlayerCharacterFromActorInfo()->SetActorRotation(FRotator(0.f,TargetRot.Yaw,0.f));
	}

}


void UHeroGameplayAbility_TargetLock::TryLockOnTarget() 
{
    AActor* Target = ResolveRegisteredTarget(); // 위 구현 사용
    if (!IsValid(Target) || Target == GetPlayerCharacterFromActorInfo())
    {
        Debug::Print(TEXT("GetPlayerCharacter Failed."));
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
        return;
    }

    AvailableActorsToLock.Reset();
    AvailableActorsToLock.Add(Target);

    // 실제 락 상태 반영 (예: 캐릭터에 복제 프로퍼티로 보관)
    if (APlayerCharacter* PC = GetPlayerCharacterFromActorInfo())
    {
        CurrentLockedActor = Target; // ReplicatedUsing 권장
    }

}

AActor *UHeroGameplayAbility_TargetLock::ResolveRegisteredTarget()
{
    if (const APlayerCharacter* PC = GetPlayerCharacterFromActorInfo())
    {
        if (IsValid(PC->DefaultTargetToLock))
        {
            return PC->DefaultTargetToLock;
        }
    }

    // (옵션) 클래스/태그로 자동 탐색
    if (BossClass)
    {
        if (AActor* Found = UGameplayStatics::GetActorOfClass(GetWorld(), BossClass))
            return Found;
    }

    if (!BossTag.IsNone())
    {
        TArray<AActor*> Actors;
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), BossTag, Actors);
        if (Actors.Num() > 0) return Actors[0];
    }

    return nullptr;
}

void UHeroGameplayAbility_TargetLock::CancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(),GetCurrentActorInfo(),GetCurrentActivationInfo(),true);
}