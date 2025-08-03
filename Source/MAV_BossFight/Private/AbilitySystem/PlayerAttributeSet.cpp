#include "AbilitySystem/PlayerAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "TimerManager.h"

UPlayerAttributeSet::UPlayerAttributeSet()
{
    InitCurrentHealth(1.f);
    InitMaxHealth(1.f);
    InitCurrentRage(1.f);
    InitMaxRage(1.f);
    InitAttackPower(1.f);
    InitDefensePower(1.f);
	InitLevel(1.f);
}

void UPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
    {
        float NewHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());
        SetCurrentHealth(NewHealth);

        if (NewHealth == 0.f)
        {
            if (Data.Target.AbilityActorInfo.IsValid())
            {
                UAbilitySystemComponent* ASC = Data.Target.AbilityActorInfo->AbilitySystemComponent.Get();
                if (ASC)
                {
                    ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.Status.BlockRegen")));
                    FTimerHandle Handle;
                    ASC->GetWorld()->GetTimerManager().SetTimer(Handle,
                        [ASC]()
                        {
                            if (ASC) ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("Player.Status.BlockRegen")));
                        }, 2.0f, false);
                }
            }
        }
    }

    // Rage 처리
    if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
    {
        const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());
        SetCurrentRage(NewCurrentRage);
    }

    // 피해 처리
    if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
    {
        const float OldHealth = GetCurrentHealth();
        const float DamageDone = GetDamageTaken();
        const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());

        SetCurrentHealth(NewCurrentHealth);
    }

	if (Data.EvaluatedData.Attribute == GetLevelAttribute())
	{
		SetLevel(FMath::Clamp(GetLevel(), 1.f, 5.f));
	}
}
