// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/Hero/HeroAnimInstance.h"
#include "Characters/PlayerCharacter.h"
 
 void UHeroAnimInstance::NativeInitializeAnimation()
 {
 	Super::NativeInitializeAnimation();
 
 	if (OwningCharacter)
 	{
 		OwningHeroCharacter = Cast<APlayerCharacter>(OwningCharacter);
 	}
 }
 
 void UHeroAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
 {
 	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
 
 	if (bHasAcceleration)
 	{
 		IdleElpasedTime = 0.f;
 		bShouldEnterRelaxState = false;
 	}
 	else
 	{
 		IdleElpasedTime += DeltaSeconds;
 
 		bShouldEnterRelaxState = (IdleElpasedTime >= EnterRelaxtStateThreshold);
 	}
 }
