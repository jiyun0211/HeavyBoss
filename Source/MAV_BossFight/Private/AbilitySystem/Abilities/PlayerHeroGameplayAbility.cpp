// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PlayerHeroGameplayAbility.h"
#include "Characters/PlayerCharacter.h"
#include "Controller/MyPlayerController.h"
#include "AbilitySystem/PlayerAbilitySystemComponent.h"
#include "MyGameplayTags.h"

APlayerCharacter* UPlayerHeroGameplayAbility::GetPlayerCharacterFromActorInfo()
{   
    if (!CachedPlayerCharacter.IsValid())
    {
        CachedPlayerCharacter = Cast<APlayerCharacter>(CurrentActorInfo->AvatarActor);
    }
   
    return CachedPlayerCharacter.IsValid()? CachedPlayerCharacter.Get() : nullptr;
}

AMyPlayerController* UPlayerHeroGameplayAbility::GetMyControllerFromActorInfo()
{	
    if (!CachedMyPlayerController.IsValid())
    {
        CachedMyPlayerController = Cast<AMyPlayerController>(CurrentActorInfo->PlayerController);
    }

    return CachedMyPlayerController.IsValid()? CachedMyPlayerController.Get() : nullptr;
}

UHeroCombatComponent* UPlayerHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
    return GetPlayerCharacterFromActorInfo()->GetHeroCombatComponent();
}

FGameplayEffectSpecHandle UPlayerHeroGameplayAbility::MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount)
{	
	check(EffectClass);

	FGameplayEffectContextHandle ContextHandle = GetPlayerAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(),GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle EffectSpecHandle = GetPlayerAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		MyGameplayTags::Shared_SetByCaller_BaseDamage,
		InWeaponBaseDamage
	);

	if (InCurrentAttackTypeTag.IsValid())
	{
		EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag,InUsedComboCount);
	}

	return EffectSpecHandle;
}
