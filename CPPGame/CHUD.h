// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CHUD.generated.h"

/**
 * 
 */
UCLASS()
class CPPGAME_API ACHUD : public AHUD
{
	GENERATED_BODY()



public:
	void StartSelect();
	void EndSelect();

public:
	ACHUD();

	virtual void BeginPlay() override;

	virtual void DrawHUD() override;

	void GetActors();

	FVector2D GetMousePosition();

public:
	void DrawSelect();

private:
	FVector2D StartPoint;
	FVector2D EndPoint;

	TArray<class ACommander_And_Units*> CNUs;
	TArray<class ACPaperCharacter*> characters;

	UPROPERTY(VisibleDefaultsOnly)
		FLinearColor BoxColor = FLinearColor(0, 0, 1, 0.2f);

	bool bDraw = false;
	bool bGetActor = false;
};
