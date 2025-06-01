#pragma once
 
#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "MyEnhancedInputComponent.generated.h"  // 항상 마지막!

/**
 * 
 */
UCLASS()
class MAV_BOSSFIGHT_API UMyEnhancedInputComponent : public UEnhancedInputComponent
{
    GENERATED_BODY()
 	
public:
    template<class UserObject, typename CallbackFunc>
    void BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func);

    template<class UserObject, typename CallbackFunc>
    void BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig,UserObject* ContextObject,CallbackFunc InputPressedFunc,CallbackFunc InputRelasedFunc);
};


template<class UserObject, typename CallbackFunc>
inline void UMyEnhancedInputComponent::BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
{
    checkf(InInputConfig, TEXT("Input config data asset is null, can not proceed with binding"));

    if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
    {
        BindAction(FoundAction, TriggerEvent, ContextObject, Func);
    }
}

template<class UserObject, typename CallbackFunc>
inline void UMyEnhancedInputComponent::BindAbilityInputAction(const UDataAsset_InputConfig* InInputConfig, UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputRelasedFunc)
{
 	checkf(InInputConfig,TEXT("Input config data asset is null,can not proceed with binding"));
 
 	for (const FMAV_BossFightInputActionConfig& AbilityInputActionConfig : InInputConfig->AbilityInputActions)
 	{
 		if(!AbilityInputActionConfig.IsValid()) continue;
 
 		BindAction(AbilityInputActionConfig.InputAction,ETriggerEvent::Started,ContextObject,InputPressedFunc,AbilityInputActionConfig.InputTag);
 		BindAction(AbilityInputActionConfig.InputAction,ETriggerEvent::Completed,ContextObject,InputRelasedFunc,AbilityInputActionConfig.InputTag);
 	}
}