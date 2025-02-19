// Copyright 2020 Dan Kestranek.


#include "GAT_ServerWaitForClientTargetData.h"
#include "AbilitySystemComponent.h"

UGAT_ServerWaitForClientTargetData::UGAT_ServerWaitForClientTargetData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

UGAT_ServerWaitForClientTargetData* UGAT_ServerWaitForClientTargetData::ServerWaitForClientTargetData(UGameplayAbility* OwningAbility, FName TaskInstanceName, bool TriggerOnce)
{
	UGAT_ServerWaitForClientTargetData* MyObj = NewAbilityTask<UGAT_ServerWaitForClientTargetData>(OwningAbility, TaskInstanceName);
	MyObj->bTriggerOnce = TriggerOnce;
	return MyObj;
}

void UGAT_ServerWaitForClientTargetData::Activate()
{
	if (!Ability || !Ability->GetCurrentActorInfo()->IsNetAuthority())
	{
		return;
	}

	FGameplayAbilitySpecHandle	SpecHandle = GetAbilitySpecHandle();
	FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
	AbilitySystemComponent->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UGAT_ServerWaitForClientTargetData::OnTargetDataReplicatedCallback);
}

void UGAT_ServerWaitForClientTargetData::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& Data, FGameplayTag ActivationTag)
{
	FGameplayAbilityTargetDataHandle MutableData = Data;
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(MutableData);
	}

	if (bTriggerOnce)
	{
		EndTask();
	}
}

void UGAT_ServerWaitForClientTargetData::OnDestroy(bool AbilityEnded)
{
	if (AbilitySystemComponent.Get())
	{
		FGameplayAbilitySpecHandle	SpecHandle = GetAbilitySpecHandle();
		FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).RemoveAll(this);
	}

	Super::OnDestroy(AbilityEnded);
}
