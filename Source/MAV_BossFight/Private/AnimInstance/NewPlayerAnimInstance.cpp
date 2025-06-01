// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/NewPlayerAnimInstance.h"
#include "Characters/PlayerBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UNewPlayerAnimInstance::NativeInitializeAnimation()
{
	OwningCharacter = Cast<APlayerBaseCharacter>(TryGetPawnOwner());

	if (OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UNewPlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (!OwningCharacter || !OwningMovementComponent)
	{
		return;
	}

	GroundSpeed = OwningCharacter->GetVelocity().Size2D();

	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D()>0.f;
}