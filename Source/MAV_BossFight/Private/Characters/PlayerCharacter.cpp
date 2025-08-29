// PlayerCharacter.cpp

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
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkinnedMeshComponent.h"       // USkinnedMeshComponent

#include "MAV_BossFightDebugHelper.h"

APlayerCharacter::APlayerCharacter()
{
    GetCapsuleComponent()->InitCapsuleSize(42.f,96.f);

    bUseControllerRotationPitch = false;
	bUseControllerRotationYaw   = false;
    bUseControllerRotationRoll  = false;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetRootComponent());
    CameraBoom->TargetArmLength = 200.f;
    CameraBoom->SocketOffset    = FVector(0.f,55.f,65.f);
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
 	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
 	FollowCamera->bUsePawnControlRotation = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
 	GetCharacterMovement()->RotationRate               = FRotator(0.f,500.f,0.f);
 	GetCharacterMovement()->MaxWalkSpeed               = 400.f;
 	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("HeroCombatComponent"));
}

UPawnCombatComponent* APlayerCharacter::GetPawnCombatComponent() const
{
	return HeroCombatComponent;
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, LockedTarget);
	DOREPLIFETIME(APlayerCharacter, DefaultTargetToLock);
}

void APlayerCharacter::OnRep_LockedTarget()
{
	const bool bLocked = IsValid(LockedTarget);
	ApplyLockedMovementFlags(bLocked);

	if (bLocked)
	{
		UE_LOG(LogTemp, Log, TEXT("Locked on to: %s"), *LockedTarget->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Lock released"));
	}
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
	if (APlayerController* PC = Cast<APlayerController>(NewController))
    {
        PC->SetViewTarget(this);           // ✅ 내 캐릭터를 뷰타겟으로
    }

	if (PlayerAbilitySystemComponent && PlayerAttributeSet)
 	{
 		const FString ASCText = FString::Printf(TEXT("Owner Actor: %s, AvatarActor: %s"),
			*PlayerAbilitySystemComponent->GetOwnerActor()->GetActorLabel(),
			*PlayerAbilitySystemComponent->GetAvatarActor()->GetActorLabel());
 		// Debug::Print(TEXT("Ability system component valid. ") + ASCText, FColor::Green);
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
 	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as input config"));
 
 	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
 	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
 	check(Subsystem);
 	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);
 
 	UMyEnhancedInputComponent* EnhancedInputComponent = CastChecked<UMyEnhancedInputComponent>(PlayerInputComponent);
 	EnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, MyGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
 	EnhancedInputComponent->BindNativeInputAction(InputConfigDataAsset, MyGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
	EnhancedInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}
 
void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

	if (ensure(CameraBoom))
    {
        CameraBoom->bUsePawnControlRotation = true;  // ✅ 컨트롤러 회전 사용
        CameraBoom->bInheritPitch = true;
        CameraBoom->bInheritYaw   = true;
        CameraBoom->bInheritRoll  = false;
    }
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

    if(IsValid(LockedTarget))
    {
        UpdateTargetLockCamera(DeltaSeconds);
    }
}

void APlayerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{	
 	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
 	const FRotator MovementRotation(0.f, Controller ? Controller->GetControlRotation().Yaw : 0.f, 0.f);
 
 	if (MovementVector.Y != 0.f)
 	{
 		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
 		AddMovementInput(ForwardDirection, MovementVector.Y);
 	}
 
 	if (MovementVector.X != 0.f)
 	{
 		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
 		AddMovementInput(RightDirection, MovementVector.X);
 	}
}
 
void APlayerCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
 	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
 	if (LookAxisVector.X != 0.f) { AddControllerYawInput(LookAxisVector.X); }
 	if (LookAxisVector.Y != 0.f) { AddControllerPitchInput(LookAxisVector.Y); }
}

void APlayerCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
 	PlayerAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}
 
void APlayerCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
 	PlayerAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}

// ======== 락온 보조 ========

void APlayerCharacter::ApplyLockedMovementFlags(bool bLocked)
{
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		if (bLocked)
		{
			// 락온: 캐릭터가 컨트롤러 회전을 따르도록(횡이동 느낌)
			Move->bOrientRotationToMovement = false;
			bUseControllerRotationYaw = true;
		}
		else
		{
			// 기본 상태로 복귀(프로젝트 스타일에 맞춰 조정)
			Move->bOrientRotationToMovement = true;
			bUseControllerRotationYaw = false;
		}
	}
}

FVector APlayerCharacter::GetTargetAimLocation(AActor* Target) const
{
	if (!IsValid(Target)) return FVector::ZeroVector;

	// 1) 스켈레톤 소켓(예: head) 우선
	if (const USceneComponent* RootComp = Target->GetRootComponent())
	{
		if (const USkinnedMeshComponent* Skinned = Cast<USkinnedMeshComponent>(RootComp))
		{
			if (Skinned->DoesSocketExist(TargetAimSocket))
			{
				return Skinned->GetSocketLocation(TargetAimSocket);
			}
		}
	}

	// 2) 바운딩 센터
	FVector Origin, Extent;
	Target->GetActorBounds(false, Origin, Extent);
	return Extent.IsNearlyZero() ? Target->GetActorLocation() : Origin;
}

void APlayerCharacter::UpdateTargetLockCamera(float DeltaSeconds)
{
	if (!Controller) return;

	const FVector CamLoc   = FollowCamera ? FollowCamera->GetComponentLocation() : GetPawnViewLocation();
	const FRotator CurRot  = Controller->GetControlRotation();
	const FVector  AimLoc  = GetTargetAimLocation(LockedTarget);

	// 목표 회전
	FRotator Desired = UKismetMathLibrary::FindLookAtRotation(CamLoc, AimLoc);

	if (bYawOnly)
	{
		Desired.Pitch = CurRot.Pitch;
		Desired.Roll  = 0.f;
	}
	else
	{
		Desired.Roll  = 0.f;
		Desired.Pitch = FMath::ClampAngle(Desired.Pitch, PitchClampMin, PitchClampMax);
	}

	// 각도 계산 (화면 중앙에서 벗어난 정도)
	const FVector CamFwd   = CurRot.Vector();
	const FVector ToTarget = (AimLoc - CamLoc).GetSafeNormal();
	const float   Dot      = FVector::DotProduct(CamFwd, ToTarget);
	const float   Angle    = FMath::RadiansToDegrees(FMath::Acos(FMath::Clamp(Dot, -1.f, 1.f)));

	// 너무 뒤로 가면 해제
	if (Angle > BreakAngle)
	{
		LockedTarget = nullptr;           // 로컬 해제
		ApplyLockedMovementFlags(false);
		return;
	}

	// 소프트존 안이면 굳이 회전 X
	if (Angle < SoftZoneHalfAngle)
		return;

	// 부드러운 회전
	const FRotator Smoothed = FMath::RInterpTo(CurRot, Desired, DeltaSeconds, TargetLockInterpSpeed);
	Controller->SetControlRotation(Smoothed);
}
