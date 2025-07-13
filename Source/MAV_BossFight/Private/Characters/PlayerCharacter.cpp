// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/MyEnhancedInputComponent.h"
#include "MyGamePlayTags.h"
#include "AbilitySystem/PlayerAbilitySystemComponent.h"
#include "DataAssets/StartUpData/DataAsset_HeroStartUpData.h"
#include "Components/Combat/HeroCombatComponent.h"

#include "MAV_BossFightDebugHelper.h"

APlayerCharacter::APlayerCharacter()
{
    GetCapsuleComponent()->InitCapsuleSize(42.f,96.f);

    bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetRootComponent());
    CameraBoom->TargetArmLength = 200.f;
    CameraBoom->SocketOffset = FVector(0.f,55.f,65.f);
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
 	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
 	FollowCamera->bUsePawnControlRotation = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
 	GetCharacterMovement()->RotationRate = FRotator(0.f,500.f,0.f);
 	GetCharacterMovement()->MaxWalkSpeed = 400.f;
 	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	

	HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("HeroCombatComponent"));
}

UPawnCombatComponent* APlayerCharacter::GetPawnCombatComponent() const
{
	return HeroCombatComponent;
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
	if (PlayerAbilitySystemComponent && PlayerAttributeSet)
 	{	
 		const FString ASCText = FString::Printf(TEXT("Owner Actor: %s, AvatarActor: %s"),*PlayerAbilitySystemComponent->GetOwnerActor()->GetActorLabel(),*PlayerAbilitySystemComponent->GetAvatarActor()->GetActorLabel());
 		
 		Debug::Print(TEXT("Ability system component valid. ") + ASCText,FColor::Green);
 		Debug::Print(TEXT("AttributeSet valid. ") + ASCText,FColor::Green);
 	}

	if (HasAuthority())
	{
		if (!CharacterStartUpData.IsNull())
		{
			if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
			{
				UE_LOG(LogTemp, Warning, TEXT("GiveToAbilitySystemComponent"));
				LoadedData->GiveToAbilitySystemComponent(PlayerAbilitySystemComponent);
			}
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
 	checkf(InputConfigDataAsset,TEXT("Forgot to assign a valid data asset as input config"));
 
 	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
 
 	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
 
 	check(Subsystem);
 
 	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext,0);
 
 	UMyEnhancedInputComponent* EnhancedInputComponent = CastChecked<UMyEnhancedInputComponent>(PlayerInputComponent);
 
 	EnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset,MyGameplayTags::InputTag_Move,ETriggerEvent::Triggered,this,&ThisClass::Input_Move);
 	EnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset,MyGameplayTags::InputTag_Look,ETriggerEvent::Triggered,this,&ThisClass::Input_Look);

	EnhancedInputComponent->BindAbilityInputAction(InputConfigDataAsset,this,&ThisClass::Input_AbilityInputPressed,&ThisClass::Input_AbilityInputReleased);
}
 
void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
}


void APlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{	
 	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
 
 	const FRotator MovementRotation(0.f,Controller->GetControlRotation().Yaw,0.f);
 
 	if (MovementVector.Y != 0.f)
 	{
 		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
 
 		AddMovementInput(ForwardDirection,MovementVector.Y);
 	}
 
 	if (MovementVector.X != 0.f)
 	{
 		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
 
 		AddMovementInput(RightDirection,MovementVector.X);
 	}
}
 
void APlayerCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
 	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
 	
 	if (LookAxisVector.X != 0.f)
 	{
 		AddControllerYawInput(LookAxisVector.X);
 	}
 
 	if (LookAxisVector.Y != 0.f)
 	{
 		AddControllerPitchInput(LookAxisVector.Y);
 	}
}

void APlayerCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
 	PlayerAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}
 
void APlayerCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
 	PlayerAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}