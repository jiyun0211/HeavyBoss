// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/PawnCombatComponent.h"
#include "Items/Weapons/WarriorWeaponBase.h"
#include "Components/BoxComponent.h"


#include "MAV_BossFightDebugHelper.h"
 
void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AWarriorWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon)
{
    if (CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister))
    {
        UE_LOG(LogTemp, Warning, TEXT("[RegisterSpawnedWeapon] Weapon with tag %s already exists. Skipping registration."), *InWeaponTagToRegister.ToString());
        return;
    }

    check(InWeaponToRegister);
    CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);

    InWeaponToRegister->OnWeaponHitTarget.BindUObject(this,&ThisClass::OnHitTargetActor);
	InWeaponToRegister->OnWeaponPulledFromTarget.BindUObject(this,&ThisClass::OnWeaponPulledFromTargetActor);

    if (bRegisterAsEquippedWeapon)
    {
        CurrentEquippedWeaponTag = InWeaponTagToRegister;
    }

    const FString WeaponString = FString::Printf(TEXT("[RegisterSpawnedWeapon] Weapon %s registered with tag %s"), *InWeaponToRegister->GetName(), *InWeaponTagToRegister.ToString());
    //Debug::Print(WeaponString);
}
 
AWarriorWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
 	if (CharacterCarriedWeaponMap.Contains(InWeaponTagToGet))
 	{
 		if (AWarriorWeaponBase* const* FoundWeapon = CharacterCarriedWeaponMap.Find(InWeaponTagToGet))
 		{
 			return *FoundWeapon;
 		}
 	}
 
 	return nullptr;
}
 
void UPawnCombatComponent::ToggleWeaponCollisionByTag(FGameplayTag WeaponTag, bool bEnable)
{
    AWarriorWeaponBase* WeaponToToggle = GetCharacterCarriedWeaponByTag(WeaponTag);

    if (!WeaponToToggle)
    {
        UE_LOG(LogTemp, Warning, TEXT("ToggleWeaponCollisionByTag: Weapon with Tag %s not found"), *WeaponTag.ToString());
        return;
    }

    UPrimitiveComponent* WeaponCollisionBox = WeaponToToggle->GetWeaponCollisionBox();

    if (!WeaponCollisionBox)
    {
        UE_LOG(LogTemp, Warning, TEXT("ToggleWeaponCollisionByTag: Weapon %s has no CollisionBox"), *WeaponToToggle->GetName());
        return;
    }

    if (bEnable)
    {
        WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        WeaponCollisionBox->SetGenerateOverlapEvents(true);
    }
    else
    {
        WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        OverlappedActors.Empty();
    }
}
bool UPawnCombatComponent::DebugCheckWeaponByTag(FGameplayTag WeaponTag)
{
    // 1️⃣ 태그로 무기 검색
    AWarriorWeaponBase* FoundWeapon = GetCharacterCarriedWeaponByTag(WeaponTag);

    // 2️⃣ 존재 여부 검사
    if (!FoundWeapon)
    {
        UE_LOG(LogTemp, Warning, TEXT("[CombatComponent] Weapon with Tag '%s' NOT FOUND."), *WeaponTag.ToString());
        return false;
    }

    // 3️⃣ 디버그 정보 출력
    UE_LOG(LogTemp, Warning, TEXT("[CombatComponent] Weapon FOUND: %s"), *FoundWeapon->GetName());

    // Instigator / Owner 검사
    AActor* Owner = FoundWeapon->GetOwner();
    APawn* Instigator = FoundWeapon->GetInstigator();

    if (!Owner)
    {
        UE_LOG(LogTemp, Warning, TEXT("[CombatComponent] Weapon '%s' has NO OWNER."), *FoundWeapon->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[CombatComponent] Owner: %s"), *Owner->GetName());
    }

    if (!Instigator)
    {
        UE_LOG(LogTemp, Warning, TEXT("[CombatComponent] Weapon '%s' has NO INSTIGATOR."), *FoundWeapon->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[CombatComponent] Instigator: %s"), *Instigator->GetName());
    }

    // 4️⃣ Collision 상태 확인
    const bool bOverlapEnabled = FoundWeapon->GetWeaponCollisionBox()->GetGenerateOverlapEvents();
    const ECollisionEnabled::Type CollisionEnabled = FoundWeapon->GetWeaponCollisionBox()->GetCollisionEnabled();

    UE_LOG(LogTemp, Warning, TEXT("[CombatComponent] Collision Enabled: %s, GenerateOverlapEvents: %s"),
        *UEnum::GetValueAsString(CollisionEnabled),
        bOverlapEnabled ? TEXT("true") : TEXT("false")
    );

    return true;
}
AWarriorWeaponBase* UPawnCombatComponent::GetCharacterCurrentEquippedWeapon() const
{
 	if (!CurrentEquippedWeaponTag.IsValid())
 	{
 		return nullptr;
 	}
 
 	return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

void UPawnCombatComponent::ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType)
{
	if (ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		AWarriorWeaponBase* WeaponToToggle = GetCharacterCurrentEquippedWeapon();

		check(WeaponToToggle);

		if (bShouldEnable)
		{
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            
			OverlappedActors.Empty();
		}		
	}
}

//Handle body collsion box
void UPawnCombatComponent::OnHitTargetActor(AActor* HitActor)
{
}

void UPawnCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
}
