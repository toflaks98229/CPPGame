// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CGameMode.generated.h"


/**
 * 
 */
UCLASS()
class CPPGAME_API ACGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ACGameMode();

public:
	virtual void BeginPlay() override;
};
