// Fill out your copyright notice in the Description page of Project Settings.


#include "EvidenceMultiplayerTest.h"

AEvidenceMultiplayerTest::AEvidenceMultiplayerTest()
{

}

void AEvidenceMultiplayerTest::ConvertGenericServerTestInfo_Implementation()
{
	ServerTestInfo = FCustomServerTestInfo(GetGenericServerTestInfo());
}

void AEvidenceMultiplayerTest::ConvertGenericClientTestInfos_Implementation()
{
	for (const FGenericClientTestInfo& ClientInfo : GetGenericClientTestInfos())
	{
		ClientTestInfos.Add(FCustomClientTestInfo(ClientInfo));
	}
}

FCustomClientTestInfo AEvidenceMultiplayerTest::GetClient1TestInfo() const
{
	checkf(GetTestPlayerCount() >= ETestPlayerCount::ServerAnd1Client, TEXT("Test doesn't have 1 client"));

	return ClientTestInfos[0];
}

FCustomClientTestInfo AEvidenceMultiplayerTest::GetClient2TestInfo() const
{
	checkf(GetTestPlayerCount() >= ETestPlayerCount::ServerAnd2Clients, TEXT("Test doesn't have 2 clients"));

	return ClientTestInfos[1];
}

FCustomClientTestInfo AEvidenceMultiplayerTest::GetClient3TestInfo() const
{
	checkf(GetTestPlayerCount() >= ETestPlayerCount::ServerAnd3Clients, TEXT("Test doesn't have 3 clients"));

	return ClientTestInfos[2];
}
