// PlayerCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "Characters/PlayerBaseCharacter.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Abilities/PlayerHeroGameplayAbility.h"
#include "AbilitySystem/PlayerAbilitySystemComponent.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UDataAsset_InputConfig;
struct FInputActionValue;

class UHeroCombatComponent;

UCLASS()
class MAV_BOSSFIGHT_API APlayerCharacter : public APlayerBaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	//~ Begin PawnCombatInterface Interface.
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End PawnCombatInterface Interface
	
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ✅ 락온 상태/기본 타겟(복제)
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated, Category="TargetLock")
	AActor* DefaultTargetToLock = nullptr;

	UPROPERTY(ReplicatedUsing=OnRep_LockedTarget, BlueprintReadOnly, Category="TargetLock")
	AActor* LockedTarget = nullptr;

	UFUNCTION()
	void OnRep_LockedTarget();

	// 화면 회전 파라미터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TargetLock|Camera")
	float TargetLockInterpSpeed = 8.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TargetLock|Camera")
	float SoftZoneHalfAngle = 6.f;          // 중앙에서 이 각도 이상 벗어나면 회전 시작

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TargetLock|Camera")
	float BreakAngle = 130.f;               // 거의 뒤로 가면 락 해제

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TargetLock|Camera")
	bool bYawOnly = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TargetLock|Camera")
	float PitchClampMin = -20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TargetLock|Camera")
	float PitchClampMax = 35.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TargetLock|Camera")
	FName TargetAimSocket = "head";         // 타겟 스켈레톤 소켓명(없으면 바운드 센터)

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;        // ✅ 추가

	UPROPERTY(BlueprintReadWrite, Category = "Warrior")
	int32 ComboMode = 0;

	FORCEINLINE UHeroCombatComponent* GetHeroCombatComponent() const { return HeroCombatComponent; }

protected:
	virtual void PossessedBy(AController* NewController) override;

private:
#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
 	UHeroCombatComponent* HeroCombatComponent;
#pragma endregion

#pragma region Inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_AbilityInputPressed(FGameplayTag InInputTag);
 	void Input_AbilityInputReleased(FGameplayTag InInputTag);
#pragma endregion

	// ✅ 락온 보조
	void UpdateTargetLockCamera(float DeltaSeconds);
	FVector GetTargetAimLocation(AActor* Target) const;
	void ApplyLockedMovementFlags(bool bLocked);
};
