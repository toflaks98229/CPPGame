#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "CPaperCharacter.h"
#include "UnitState.h"
#include "Commander_And_Units.generated.h"

struct UnitsFormationData
{
public:
	UnitsFormationData()
	{
		direction = FVector(0,0,0);
		location = FVector(0,0,0);
		length = 0.0f;
	}

	UnitsFormationData(FVector InVector, float InLength, FVector Inlocation)
	{
		direction = InVector;
		location = Inlocation;
		length = InLength;
	}

public:
	FVector direction;
	FVector location;
	float length;
};

UCLASS()
class CPPGAME_API ACommander_And_Units : public ACharacter
{
	GENERATED_BODY()

public:
	ACommander_And_Units();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsPlayers() { return bPlayers; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE TArray<class ACPaperCharacter*> GetUnits() { return Units; }

public:
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	FVector GetCommanderPosition();

private:
	UPROPERTY(EditAnywhere)
		TArray<class ACPaperCharacter*> Units;

	TArray<class ACHUD_Selected*> UnitsUI;

	TArray<FVector> UnitsPosition;
	TArray<FVector> Current_UnitsPosition;

	UPROPERTY(EditAnywhere)
		class UClass* UnitsActorData;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> UnitsTargetActors;

	UPROPERTY(EditAnywhere)
		class UClass* UnitsData;

	UPROPERTY(EditAnywhere)
		class UClass* CommanderData;

	UPROPERTY(EditAnywhere)
		class UClass* UnitsUIData;

	UPROPERTY(EditAnywhere)
		TArray<ACommander_And_Units*> AttackableCNUs;

	UPROPERTY(EditAnywhere)
		int MaxUnitLineCount = 30;

	UPROPERTY(EditAnywhere)
		int UnitsNumMax = 30;

	UPROPERTY(EditAnywhere)
		int UnitsNum = 0;

private:
	UPROPERTY(EditAnywhere)
		bool bPlayers = true;
	
	/*
	HUD 그리기
	*/

public:
	void DrawHUD(bool inBool);
	void SetHUDPosition();

	bool IsOverMaxUnitLineCount(int InInt);

	/*
	이동
	*/
public:
	void SetCurrentUnitsPosition();
	void NormalMove(FVector location);
	void FormationMove(float InLength, FVector InVector, FVector InTargetLocation);
	void FormationMove(UnitsFormationData InUnitsFormationData);
	void FormationMoveActor(float InLength, FVector InVector, FVector InTargetLocation);
	void FormationMoveActor(UnitsFormationData InUnitsFormationData);
	void NormalMoveActor(FVector location);

	void Rotation();



	//===============================================================

	/*
	공격
	*/
public:

	void AttackCommand(class ACommander_And_Units* InCNU);

private:
	bool bCharge = false;

	//===============================================================

public:
	/*
	유닛 선택됨
	*/
	void Selected();
	void UnSelected();
	bool IsSelected() { return bSelected; }

private:
	bool bSelected = false;

	//===============================================================
	
	/*
	유닛 이동용 함수와 데이터
	*/
public:
	UnitsFormationData GetunitsFormationData() { return unitsFormationData; }

public:
	UnitsFormationData SetFormationStart(float InLength, FVector InVector, FVector InTargetLocation);

	UnitsFormationData SetFormation(float InLength, FVector InVector, FVector InTargetLocation);
	UnitsFormationData SetFormation(UnitsFormationData InUnitsFormationData);

	UnitsFormationData SetFormationNormal(FVector InVector, FVector InTargetLocation);
	UnitsFormationData SetFormationNormal(UnitsFormationData InUnitsFormationData);

	UnitsFormationData SetFormationActor(float InLength, FVector InVector, FVector InTargetLocation);
	UnitsFormationData SetFormationActor(UnitsFormationData InUnitsFormationData);

public:
	FVector SetUnitWorldPostion(FVector inVector);


private:
	UnitsFormationData unitsFormationData;
	TArray<FVector> hungarian();

	/*
	위치 확인 주기
	*/

	UPROPERTY(EditAnywhere)
		float CheckInLocationRate = 3.0f;
		float CheckInLocationTick = 0.0f;

	//===================================================================

public:
	void AddAttackableCNU(ACommander_And_Units* InCNU);
	void RemoveAttackableCNU(ACommander_And_Units* InCNU);

	bool bCheckPlayerUnit(ACommander_And_Units* InCNU);

public:
	void SpawnUnits();

	/*
유닛 사망
*/

public:
	void UnitDead(ACPaperCharacter* InUnit);

	//===============================================================

public:
	void CheckAttackable();

public:
	float GetSize() { return unitState.GetSize(); };

private:
	/*
	캐릭터 스텟
	*/
	UPROPERTY(EditAnywhere)
		FUnitState unitState;

	/*
	플레이어
	*/
	class ACPlayerCamera* player;

};
