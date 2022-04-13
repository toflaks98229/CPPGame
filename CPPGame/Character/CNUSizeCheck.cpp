// Fill out your copyright notice in the Description page of Project Settings.

#include "CNUSizeCheck.h"
#include "Global.h"
#include "Commander_And_Units.h"

CNUSizeCheck::CNUSizeCheck()
{
}

CNUSizeCheck::CNUSizeCheck(class ACommander_And_Units* InCun)
{
	cnu = InCun;
	size = cnu->GetSize();
	cnulengthNum = 0;
	cnuLocation = FVector(0, 0, 0);
}

CNUSizeCheck::CNUSizeCheck(class ACommander_And_Units* InCun, FVector InVector)
{
	cnu = InCun;
	size = cnu->GetSize();
	cnulengthNum = 0;
	cnuLocation = InVector;
}

CNUSizeCheck::~CNUSizeCheck()
{

}

bool CNUSizeCheck::IsOverMaxUnitLineCount()
{
	return cnu->IsOverMaxUnitLineCount(cnulengthNum);
}

void CNUSizeCheck::AddCunLengthNum()
{
	cnulengthNum++;
}

float CNUSizeCheck::GetSize()
{
	return size;
}

void CNUSizeCheck::SetCnuLocation(FVector InVector)
{
	cnuLocation = InVector;
}

float CNUSizeCheck::GetCnuLength()
{
	return size * cnulengthNum;
}

FVector CNUSizeCheck::GetCnuLocation()
{
	return cnu->GetActorLocation();
}

void CNUSizeCheck::SetFormation(FVector InVector)
{
	cnu->SetFormation(cnulengthNum * size, InVector, cnuLocation);
	cnu->SetHUDPosition();
}

void CNUSizeCheck::FormationMove(FVector InVector)
{
	cnu->FormationMove(cnulengthNum * size, InVector, cnuLocation);
	cnu->SetHUDPosition();
}