// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "UnitState.h"
#include "CPaperCharacter.generated.h"


/**
 * 
 */

UCLASS()
class CPPGAME_API ACPaperCharacter : public APaperCharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCActorComponent_Flipbook* Flipbooks;

	UPROPERTY(VisibleDefaultsOnly)
		class UCActorComponent_Action* Action;

private:
	UPROPERTY(VisibleAnywhere)
		class UPaperSpriteComponent* SelectUI;

	UPROPERTY(VisibleAnywhere)
		class AAIController* AIController;

private:
	/*
	���
	*/
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Shield;

	//==========================================================

	/*
	�ִϸ��̼� ��ȯ
	*/

public:
	virtual	void StartMove(FVector Location, FRotator rotation);
	virtual	void StartMoveActor(AActor* actor, FRotator rotation);
	virtual	void StartMove();
	virtual	void Moving(float DeltaTime);
	virtual	void Charge();

	virtual	void Idle();
	virtual void Attack();
	virtual void StartDead();
	virtual void EndDead();

	virtual void StartHited();
	virtual void EndHited();

	virtual void Rotation(float DeltaTime);
	virtual void RotationIdle(float DeltaTime);
	virtual void RotationBattle(float DeltaTime);

	virtual void PlayFlipBook();
	virtual void StopPlayFlipBook();

	virtual void FlipFlipBook(FVector InVector);


	/*
	�̵��ӵ� ����
	*/
	virtual void AddSpeed(float DeltaTime);
	virtual void LoseSpeed(float InFloat);

	//===========================================================

	virtual void OnOffSelectUI(bool Inbool);
	virtual float GetSize();

	virtual void CheckInLocationMoveTo();


	/*
	���ݴ��� �� Ȯ���ؾ� �� �͵�
	*/
public:

	virtual void CheckDamage(float InDamage, float InMelee_attack, FVector InAttack_Direction);
	virtual void CheckDamage(float InDamage, float InMelee_attack, FVector InAttack_Direction, FVector InForce);
	virtual void CheckDead();

	//========================================================

	/*
	���� �浹
	*/
private:
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnOverlapBeginWeapon(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void OnOverlapBeginShield(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnHitBegin(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	/*
	������ �ùķ���Ʈ
	*/
	void AddForce(FVector InVector);
	void SetForceZero() { force = FVector(0, 0, 0); }
	FVector GetForce() { return force; }

	void SimulatePulse(float Deltatime);
	bool bSimulatePulse = false;
	FVector force = FVector(0, 0, 0);

private:
	bool bAttack = false;



public:
	void SetLocationMoveTo(FVector InLocation) { LocationMoveTo = InLocation; }

private:
	FVector LocationMoveTo;

	/*
	��� Ȯ��
	*/

public:
	bool IsDead() { return unitState.GetUnitState() == EUnitState::E_Dead; }

public:
	void CheckState(FVector InVector, bool InBool);
	void SetCNU(class ACommander_And_Units* InCNU);
	ACommander_And_Units* GetCNU() { return CNU; }

public:
	void BillbordMaker();

public:
	ACPaperCharacter();
	virtual void Tick(float DeltaTime) override;

public:
	virtual void BeginPlay() override;

private:
	float timer = 0.0f;
	float AttackAbleRange = 100.0f;

private:
	UPROPERTY(EditAnywhere)
		float tickRate = 1.0f;

	/*
	���� Ÿ�� ����
	*/


public:
	void SetTarget(ACPaperCharacter* InPaperCharacter);

private:
	class ACPaperCharacter* target;
	class ACommander_And_Units* CNU;

	/*
	ĳ���� ����
	*/
public:
	void SetUnitState(FUnitState InunitState);

private:
	UPROPERTY(EditAnywhere)
	 FUnitState unitState;


	/*
	ĳ���� ����
	*/

	FRotator look_Direction;
	FRotator formation_Direction;
	FRotator current_Rotation;
	float currentTime = 0.0f;




public:
	void Setdirection(FRotator Indirection) { formation_Direction = Indirection; }
	/*
	Ÿ�Ӷ��ο�
	*/

};
