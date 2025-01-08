// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerTest.h"
#include "CustomTestInfo.h"
#include "TagMultiplayerTest.generated.h"

/**
 * 
 */
UCLASS()
class TAG_API ATagMultiplayerTest : public AMultiplayerTest
{
	GENERATED_BODY()

public:
	ATagMultiplayerTest();

protected:
	void ConvertGenericServerTestInfo_Implementation() override;
	void ConvertGenericClientTestInfos_Implementation() override;

	UPROPERTY(BlueprintReadOnly)
	FCustomServerTestInfo ServerTestInfo;

	UFUNCTION(BlueprintCallable)
	FCustomClientTestInfo GetClient1TestInfo() const;

	UFUNCTION(BlueprintCallable)
	FCustomClientTestInfo GetClient2TestInfo() const;

	UFUNCTION(BlueprintCallable)
	FCustomClientTestInfo GetClient3TestInfo() const;

private:
	TArray<FCustomClientTestInfo> ClientTestInfos;
};
