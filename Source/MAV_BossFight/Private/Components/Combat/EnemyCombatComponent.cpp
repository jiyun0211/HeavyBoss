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
        UAbilitySystemComponent* OpponentASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
        if (ASC)
        {
            const UPlayerAttributeSet* MyAttributes = ASC->GetSet<UPlayerAttributeSet>();
            const UPlayerAttributeSet* OpponentAttributes = OpponentASC->GetSet<UPlayerAttributeSet>();
            if (MyAttributes)
            {
                float MyLevel = MyAttributes->GetLevel();
                float OpponentLevel = OpponentAttributes->GetLevel();
                
                // Level 조건 검사
                if (MyLevel >= OpponentLevel)
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
        
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
            GetOwningPawn(),
            MyGameplayTags::Player_Event_Blocked,
            EventData
        );
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnHit"));
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
            GetOwningPawn(),
            MyGameplayTags::Shared_Event_MeleeHit,
            EventData
        );
        
    }
}