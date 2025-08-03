// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "MyGameplayTags.h"
#include "WarriorFunctionLibrary.h"
#include "AbilitySystem/PlayerAttributeSet.h"


#include "MAV_BossFightDebugHelper.h"

void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}

	OverlappedActors.AddUnique(HitActor);

	//TODO:: Implement block check
	bool bIsValidBlock = false;
	
	const bool bIsPlayerBlocking = UWarriorFunctionLibrary::NativeDoesActorHaveTag(HitActor,MyGameplayTags::Player_Status_Blocking);
	const bool bIsMyAttackUnblockable = false;

	
    if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
    {
        // 🔑 내 Pawn의 ASC 가져오기
        UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningPawn());
        if (ASC)
        {
            // 🔑 내 AttributeSet 가져오기
            const UPlayerAttributeSet* MyAttributes = ASC->GetSet<UPlayerAttributeSet>();
            if (MyAttributes)
            {
                float MyLevel = MyAttributes->GetLevel(); // 매크로로 자동 생성된 Getter 사용

                // Level 조건 검사
                if (MyLevel >= 4.f)
                {
                    UE_LOG(LogTemp, Log, TEXT("Shield Ignored"));
                    bIsValidBlock = false;
                }
                else
                {
                    bIsValidBlock = UWarriorFunctionLibrary::IsValidBlock(GetOwningPawn(), HitActor);
                }
            }
        }
    }

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;


	if (bIsValidBlock)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitActor,
			MyGameplayTags::Player_Event_Blocked,
			EventData
		);
	}
	else
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			MyGameplayTags::Shared_Event_MeleeHit,
			EventData
		);
	}
}