// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerInitialMoveSpeed.h"


AServerInitialMoveSpeed::AServerInitialMoveSpeed()
{
	TestPlayerCount = ETestPlayerCount::ServerAnd1Client;
}

void AServerInitialMoveSpeed::StartTest()
{
	Super::StartTest();
	bool bSuccess = true;
	bSuccess &= ServerTestInfo.ServerCharacter->GetMoveSpeed() == 600.f;
	bSuccess &= GetClient1TestInfo().ServerCharacter->GetMoveSpeed() == 600.f;
	if (bSuccess)
	{
		FinishTest(EFunctionalTestResult::Succeeded, TEXT("Initial Move Speed is correct"));
	}
	else
	{
		FinishTest(EFunctionalTestResult::Failed, TEXT("Initial Move Speed is incorrect"));
	}
}

