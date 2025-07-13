// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/PlayerHeroGameplayAbility.h"
#include "Characters/PlayerCharacter.h"
#include "AbilitySystemGlobals.h"
#include "Controller/MyPlayerController.h"
#include "AbilitySystem/PlayerAbilitySystemComponent.h"
#include "AbilitySystem/PlayerAttributeSet.h"
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

float UPlayerHeroGameplayAbility::GetCurrentHealthFromSpecHandle(const FGameplayEffectSpecHandle& SpecHandle)
{
    if (!SpecHandle.IsValid() || !SpecHandle.Data.IsValid())
    {
        return 0.f;
    }

    // 대상 Actor 가져오기 (타겟 or 이펙트 유발자)
    const FGameplayEffectContextHandle& ContextHandle = SpecHandle.Data->GetEffectContext();
	AActor* TargetActor = ContextHandle.GetOriginalInstigator();
    if (!TargetActor)
    {
        return 0.f;
    }

    // ASC 접근
	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::Get().GetAbilitySystemComponentFromActor(TargetActor);
    if (!ASC)
    {
        return 0.f;
    }

    // AttributeSet 접근
    const UPlayerAttributeSet* Attributes = Cast<UPlayerAttributeSet>(ASC->GetAttributeSet(UPlayerAttributeSet::StaticClass()));
    if (!Attributes)
    {
        return 0.f;
    }

    // CurrentHealth 반환
    return Attributes->GetCurrentHealth();
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

FGameplayEffectSpecHandle UPlayerHeroGameplayAbility::MakePlayerStaminaEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float StaminaChange)
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
		StaminaChange
	);

	return EffectSpecHandle;
}

