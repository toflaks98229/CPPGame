// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CPPGAME_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/*
	���콺 ���
	*/
	void LeftMouse_Pressed();
	void LeftMouse_Released();

	void RightMouse_Pressed();
	void RightMousee_Released();

	/*
	Units Controller
	*/
	void AddCNU(class ACommander_And_Units* InCNU);
	void ReMoveCNU(class ACommander_And_Units* InCNU);
	void ReMoveAllCNU();

	void StartSelect();
	void EndSelect();

	/*
	Move
	*/
	void NormalMove();
	void FormationMove();

	/*
	Decal
	*/
	void StartDrawDecal();
	void EndDrawDecal();
	void DrawDecal();

	/*
	����
	*/
	void AttackCommand(class ACPaperCharacter* InPaperCharacter);

	/*
	�ʵ��� ���ֵ� ������ ��������
	*/
	void GetAllUnitsInField();
	void MakeAllUnitsInField();

	/*
	������ ���ϱ�
	*/

	/*
	HUD �׸���
	*/

	void DrawHUD(bool inBool);

	/*
	�밡����
	*/

	TArray<class CNUSizeCheck*> Hungarian(TArray<class CNUSizeCheck*> InActors);

private:
	UPROPERTY(VisibleDefaultsOnly)
		TArray<class AActor*> Actors;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> ClassToFind;

private:
	bool bNormalMove = true;
	bool bFormationMove = false;

	UPROPERTY(EditAnywhere)
		float FormationMoveLength = 100.0f;

	/*
	EObjectTypeQuery
	*/
	UPROPERTY(EditAnywhere)
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeQueryLeft;

	UPROPERTY(EditAnywhere)
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeQueryRight;

private:
	FHitResult* hitResultLeft;
	FHitResult* hitResultRight;

public:
	void GetHitResult();

private:
	bool bhitResultLeft;
	bool bhitResultRight;
	/*
	���콺 ��� 
	*/
private:
	bool bDrawDecal = false;
	bool bSelect = false;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class ACPlayerCamera* PlayerCamera;

	UPROPERTY(EditAnywhere)
		TArray<class ACommander_And_Units*> PlayersCNUs;

	UPROPERTY(EditAnywhere)
		TArray<class ACommander_And_Units*> EnemiesCNUs;

	/*UPROPERTY(VisibleDefaultsOnly)
		TArray<class ACommander_And_Units*> CNUsSelected;*/

	UPROPERTY(VisibleDefaultsOnly)
		class ACHUD* HUD;

public:
	ACPlayerController();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	FVector DecalStartPosition;
	FVector DecalEndPosition;
	FVector DecalDirection;
};
