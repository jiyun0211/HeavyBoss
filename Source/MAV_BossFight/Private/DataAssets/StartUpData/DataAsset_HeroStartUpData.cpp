// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUpData/DataAsset_HeroStartUpData.h"
#include "AbilitySystem/PlayerAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/PlayerGameplayAbility.h"

void UDataAsset_HeroStartUpData::GiveToAbilitySystemComponent(UPlayerAbilitySystemComponent* InASCToGive,int32 ApplyLevel)
{
    Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

    for(const FMAV_BossFightAbilitySet& AbilitySet : HeroStartUpAbilitySets)
    {
        if(!AbilitySet.IsValid()) continue;
        
        FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
        AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
        AbilitySpec.Level = ApplyLevel;
        AbilitySpec.FGameplayAbilitySpec::GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);
        
        InASCToGive->GiveAbility(AbilitySpec);
    }
}
