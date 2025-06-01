// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/Hero/PlayerLinkedAnimLayer.h"
#include "AnimInstance/Hero/HeroAnimInstance.h"
 
UHeroAnimInstance* UPlayerLinkedAnimLayer::GetHeroAnimInstance() const
{   
    return Cast<UHeroAnimInstance>(GetOwningComponent()->GetAnimInstance());
    
}
