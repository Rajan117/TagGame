// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tag/Tests/TagMultiplayerTest.h"
#include "ServerInitialMoveSpeed.generated.h"

UCLASS()
class TAG_API AServerInitialMoveSpeed : public ATagMultiplayerTest
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AServerInitialMoveSpeed();
	virtual void StartTest() override;

private:
	
	
};
