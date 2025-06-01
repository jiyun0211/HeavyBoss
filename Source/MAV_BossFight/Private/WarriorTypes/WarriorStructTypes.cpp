// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorTypes/WarriorStructTypes.h"
#include "AbilitySystem/Abilities/PlayerGameplayAbility.h"

bool FMAV_BossFightAbilitySet::IsValid() const
{
    return InputTag.IsValid() && AbilityToGrant;
}