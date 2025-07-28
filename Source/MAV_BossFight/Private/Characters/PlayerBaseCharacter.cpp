// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/PlayerBaseCharacter.h"
#include "AbilitySystem/PlayerAbilitySystemComponent.h"
#include "AbilitySystem/PlayerAttributeSet.h"
#include "MotionWarpingComponent.h"


// Sets default values
APlayerBaseCharacter::APlayerBaseCharacter()
{
    // Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    GetMesh()->bReceivesDecals = false;

    PlayerAbilitySystemComponent = CreateDefaultSubobject<UPlayerAbilitySystemComponent>(TEXT("PlayerAbilitySystemComponent"));

    PlayerAttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("PlayerAttributeSet"));

    MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));

}

UPawnCombatComponent* APlayerBaseCharacter::GetPawnCombatComponent() const
{
    return nullptr;
}

UAbilitySystemComponent* APlayerBaseCharacter::GetAbilitySystemComponent() const
{
    return GetPlayerAbilitySystemComponent();
}

void APlayerBaseCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    if (PlayerAbilitySystemComponent)
    {
        PlayerAbilitySystemComponent->InitAbilityActorInfo(this, this);

        ensureMsgf(!CharacterStartUpData.IsNull(), TEXT("Forgot to assign start up data to %s"), *GetName());
    }
}
