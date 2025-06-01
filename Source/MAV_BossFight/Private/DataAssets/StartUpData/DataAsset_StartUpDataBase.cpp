// Vince Petrelli All Rights Reserved
 
 
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "AbilitySystem/PlayerAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/PlayerGameplayAbility.h"

void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UPlayerAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
    check(InASCToGive);
    UE_LOG(LogTemp, Warning, TEXT("Abilities Count: %d"), InASCToGive->GetActivatableAbilities().Num());

    GrantAbilities(ActivateOnGivenAbilities,InASCToGive,ApplyLevel);
    GrantAbilities(ReactiveAbilities,InASCToGive,ApplyLevel); 
    if (!StartUpGameplayEffects.IsEmpty())
	{
		for (const TSubclassOf < UGameplayEffect >& EffectClass : StartUpGameplayEffects)
		{
			if(!EffectClass) continue;

			UGameplayEffect* EffectCDO = EffectClass->GetDefaultObject<UGameplayEffect>();

			InASCToGive->ApplyGameplayEffectToSelf(
				EffectCDO,
				ApplyLevel,
				InASCToGive->MakeEffectContext()
			);
		}
	}
}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UPlayerGameplayAbility>>& InAbilitiesToGive, UPlayerAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
    if (InAbilitiesToGive.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("GrantAbilities: No abilities to give"));
        return;
    }

    for (const TSubclassOf<UPlayerGameplayAbility>& Ability : InAbilitiesToGive)
    {
        if(!Ability) continue;

        FGameplayAbilitySpec AbilitySpec(Ability);
        AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
        AbilitySpec.Level = ApplyLevel;

        InASCToGive->GiveAbility(AbilitySpec);
    }
}