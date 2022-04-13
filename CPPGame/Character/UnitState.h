#pragma once

#include "UnitState.generated.h"

UENUM(BlueprintType)
enum class EUnitState : uint8
{
	E_Move UMETA(DisplayName = "Move"),
	E_Idle UMETA(DisplayName = "Idle"),
	E_Battle UMETA(DisplayName = "Battle"),
	E_Dead UMETA(DisplayName = "Dead")
};


USTRUCT()
struct FUnitState
{
	GENERATED_BODY()

public:
	float GetDamage()
	{
		return damage;
	}

	void AddDamage(float inFloat)
	{
		damage += inFloat;
	}

	void ReduceDamage(float inFloat)
	{
		damage -= inFloat;
	}

	float GetHealth()
	{
		return health;
	}

	void AddHealth(float inFloat)
	{
		health += inFloat;
	}

	void ReduceHealth(float inFloat)
	{
		health -= inFloat;
	}

	float GetSize()
	{
		return size;
	}

	void AddSize(float inFloat)
	{
		size += inFloat;
	}

	void ReduceSize(float inFloat)
	{
		size -= inFloat;
	}

	float GetArmor()
	{
		return armour;
	}

	void AddArmor(float inFloat)
	{
		armour += inFloat;
	}

	void ReduceArmor(float inFloat)
	{
		armour -= inFloat;
	}

	float GetChargeBonus()
	{
		return charge_Bonus;
	}

	void AddChargeBonus(float inFloat)
	{
		charge_Bonus += inFloat;
	}

	void ReduceChargeBonus(float inFloat)
	{
		charge_Bonus -= inFloat;
	}

	float GetSpeedCurrent()
	{
		return speed_Current;
	}

	void AddSpeedCurrent(float inFloat)
	{
		speed_Current += inFloat;

		if (speed_Current > speed_Max)
			speed_Current = speed_Max;
	}

	void ReduceSpeedCurrent(float inFloat)
	{
		speed_Current -= inFloat;

		if (speed_Current < 0.0f)
			speed_Current = 0.0f;
	}

	float GetSpeedMax()
	{
		return speed_Max;
	}

	void AddSpeedMax(float inFloat)
	{
		speed_Max += inFloat;
	}

	void ReduceSpeedMax(float inFloat)
	{
		speed_Max -= inFloat;
	}

	float GetSpeedAcceleration()
	{
		return speed_Acceleration;
	}

	void AddSpeedAcceleration(float inFloat)
	{
		speed_Acceleration += inFloat;
	}

	void ReduceSpeedAcceleration(float inFloat)
	{
		speed_Acceleration -= inFloat;
	}

	float GetSpeedRotation()
	{
		return speed_Rotation;
	}

	void AddSpeedRotation(float inFloat)
	{
		speed_Rotation += inFloat;
	}

	void ReduceSpeedRotation(float inFloat)
	{
		speed_Rotation -= inFloat;
	}

	float GetMass()
	{
		return mass;
	}

	void AddMass(float inFloat)
	{
		mass += inFloat;
	}

	void ReduceMass(float inFloat)
	{
		mass -= inFloat;
	}

	float GetMeleeAttack()
	{
		return melee_Attack;
	}

	void AddMeleeAttack(float inFloat)
	{
		melee_Attack += inFloat;
	}

	void ReduceMeleeAttack(float inFloat)
	{
		melee_Attack -= inFloat;
	}

	float GetMeleeDefense()
	{
		return melee_Defense;
	}

	void AddMeleeDefense(float inFloat)
	{
		melee_Defense += inFloat;
	}

	void ReduceMeleeDefense(float inFloat)
	{
		melee_Defense -= inFloat;
	}

	float GetMeleeRange()
	{
		return melee_Range;
	}

	void AddMeleeRange(float inFloat)
	{
		melee_Range += inFloat;
	}

	void ReduceMeleeRange(float inFloat)
	{
		melee_Range -= inFloat;
	}

	float GetAttackPeriod()
	{
		return attack_Period;
	}

	void AddAttackPeriod(float inFloat)
	{
		attack_Period += inFloat;
	}

	void ReduceAttackPeriod(float inFloat)
	{
		attack_Period -= inFloat;
	}

	float GetCurrentAttackPeriod()
	{
		return current_Attack_Period;
	}

	void AddCurrentAttackPeriod(float inFloat)
	{
		current_Attack_Period += inFloat;
	}

	void ReduceCurrentAttackPeriod(float inFloat)
	{
		current_Attack_Period -= inFloat;
	}

	bool GetAttackAble()
	{
		return bAttackAble;
	}

	EUnitState GetUnitState()
	{
		return state;
	}

	void UnitMove()
	{
		state = EUnitState::E_Move;
	}

	void UnitIdle()
	{
		state = EUnitState::E_Idle;
	}

	void UnitDead()
	{
		state = EUnitState::E_Dead;
	}

	EUnitState UnitTstate()
	{
		return state;
	}

	void LookTagert(bool InBool)
	{
		bLookTarget = InBool;
	}

	bool IsLookTarget()
	{
		return bLookTarget;
	}

	UStaticMesh* GetWeaponMesh()
	{
		return WeaponMesh;
	}

	UStaticMesh* GetShieldMesh()
	{
		return ShieldMesh;
	}

public:
	void UnitStateTicK(float detaTime)
	{
		if (!bAttackAble)
		{
			current_Attack_Period = current_Attack_Period + detaTime;

			if (current_Attack_Period > attack_Period)
			{
				bAttackAble = true;
				current_Attack_Period = attack_Period;
			}
		}
		else
		{

		}
	}

	void CheckDamage(float InDamage, float InMelee_attack, FVector InAttack_Direction, FVector InDefence_Direction)
	{
		float melee = 0.0f;
		melee += InMelee_attack;

		float angle = GetVectorDegrees(InAttack_Direction, InDefence_Direction);

		if (angle <= 45.0f)
		{
			melee -= melee_Defense + 35.0f;
		}
		else if (angle <= 135.0f)
		{
			melee -= 0.5f * (melee_Defense + 35.0f);
		}
		else
		{
			melee -= 0.0f * (melee_Defense + 35.0f);
		}

		int rand = FMath::RandRange(0, 100);

		if (rand <= melee)
		{
			health -= InDamage;
			CheckDead();
		}
		else
		{

		}
	}

	void CheckDead()
	{
		if (health <= 0)
		{
			bDead = true;
		}
		else
		{

		}
	}

	void Attack()
	{
		current_Attack_Period = 0.0f;
	}


public:
	static FORCEINLINE float GetVectorDegrees(FVector const& InVector1, FVector const& InVector2)
	{
		return FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(InVector1, InVector2)));
	}

private:
	UPROPERTY(EditAnywhere)
		float damage = 100.0f;

	UPROPERTY(EditAnywhere)
		float health = 100.0f;

	UPROPERTY(EditAnywhere)
		float size = 160.0f;

	UPROPERTY(EditAnywhere)
		float armour = 100.0f;

	UPROPERTY(EditAnywhere)
		float charge_Bonus = 100.0f;

	UPROPERTY(EditAnywhere)
		float speed_Current = 0.0f;

	UPROPERTY(EditAnywhere)
		float speed_Max = 300.0f;

	UPROPERTY(EditAnywhere)
		float speed_Acceleration = 5.0f;

	UPROPERTY(EditAnywhere)
		float speed_Rotation = 1;

	UPROPERTY(EditAnywhere)
		float mass = 100.0f;

	UPROPERTY(EditAnywhere)
		int melee_Attack = 100.0f;

	UPROPERTY(EditAnywhere)
		int melee_Defense = 100.0f;

	UPROPERTY(EditAnywhere)
		float melee_Range = 50.0f;

	UPROPERTY(EditAnywhere)
		float attack_Period = 10.0f;

	UPROPERTY(EditAnywhere)
		float current_Attack_Period = 10.0f;

	UPROPERTY(EditAnywhere)
		bool bAttackAble = true;

	UPROPERTY(EditAnywhere)
		bool bDead = false;

	UPROPERTY(EditAnywhere)
		bool bLookTarget = false;

	UPROPERTY(EditAnywhere)
		UStaticMesh* WeaponMesh;

	UPROPERTY(EditAnywhere)
		UStaticMesh* ShieldMesh;

	UPROPERTY(EditAnywhere)
		EUnitState state = EUnitState::E_Idle;
};
