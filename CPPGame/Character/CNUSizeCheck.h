// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

class CPPGAME_API CNUSizeCheck
{
public:
	CNUSizeCheck(class ACommander_And_Units* InCun);
	CNUSizeCheck(class ACommander_And_Units* InCun, FVector InVector);
	CNUSizeCheck();
	~CNUSizeCheck();

public:
	bool IsOverMaxUnitLineCount();

	void AddCunLengthNum();

	float GetSize();

	void SetCnuLocation(FVector InVector);

	float GetCnuLength();

	FVector GetCnuLocation();

	void SetFormation(FVector InVector);

	void FormationMove(FVector InVector);

private:
	ACommander_And_Units* cnu;
	float size;
	int cnulengthNum;
	FVector cnuLocation;
};