#include "CPaperCharacter.h"
#include "Global.h"
#include "PaperFlipbook.h"
#include "AIController.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "Commander_And_Units.h"
#include "CEquipment.h"

#include "Tasks/AITask_MoveTo.h"
#include "Math/UnrealMathUtility.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CActorComponent_Flipbook.h"
#include "Components/CActorComponent_Action.h"
#include "Components/StaticMeshComponent.h"

ACPaperCharacter::ACPaperCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;	

	CHelpers::CreateActorComponent(this, &Flipbooks, "Flipbook");
	CHelpers::CreateActorComponent(this, &Action, "Action");

	CHelpers::CreateComponent(this, &SelectUI, "SelectUI", GetCapsuleComponent());
	CHelpers::CreateComponent(this, &Weapon, "Weapon", GetCapsuleComponent());
	CHelpers::CreateComponent(this, &Shield, "Shield", GetCapsuleComponent());

	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACPaperCharacter::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ACPaperCharacter::OnOverlapEnd);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ACPaperCharacter::OnHitBegin);

	GetSprite()->SetRelativeScale3D(FVector(8,8,8));

	SelectUI->SetVisibility(false);

	Weapon->SetCastShadow(false);
	Shield->SetCastShadow(false);
}

void ACPaperCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (tickRate == 0.0f)
		tickRate = 1.0f;

	SetActorTickInterval(1 / tickRate);

	LocationMoveTo = GetActorLocation();
	formation_Direction = GetActorRotation();

	/*
	AI 컨트롤러 소환
	*/

	SpawnDefaultController();
	AIController = Cast<AAIController>(GetController());
}

void ACPaperCharacter::Tick(float DeltaTime)
{
	switch (unitState.UnitTstate())
	{
	case EUnitState::E_Move:		
		AddSpeed(DeltaTime);
		if (AIController->GetMoveStatus() == EPathFollowingStatus::Idle)
		{
			Idle();
		}
		break;

	case EUnitState::E_Idle:
		RotationIdle(DeltaTime);
		break;

	case EUnitState::E_Battle:
		RotationBattle(DeltaTime);
		break;

	case EUnitState::E_Dead:
		break;

	}

	/*
	카메라 방향과 보는 방햐에 따른 플릭
	*/

	/*
	공격
	*/


	/*
	받은 힘 만큼 밀리는 거 계산
	*/
	SimulatePulse(DeltaTime);
}

void ACPaperCharacter::StartMove()
{
	unitState.UnitMove();

	current_Rotation = GetActorRotation();
	currentTime = 0.0f;

	look_Direction = (LocationMoveTo - GetActorLocation()).Rotation();

	/*
	AI 컨트롤러 쓰는 버전
	*/

	//GetSprite()->SetFlipbook(Flipbooks->SetMoveFlipbook());
	AIController->MoveToLocation(LocationMoveTo, 0.1f);
}

void ACPaperCharacter::StartMove(FVector Location, FRotator rotation)
{
	unitState.UnitMove();

	current_Rotation = GetActorRotation();
	currentTime = 0.0f;

	float angle = unitState.GetVectorDegrees(GetActorForwardVector(), rotation.Vector());
	float loseSpeed = unitState.GetSpeedCurrent() * angle / 180.0f;

	LoseSpeed(loseSpeed);

	/*
	AI 컨트롤러 쓰는 버전
	*/

	formation_Direction = rotation;
	look_Direction = (Location - GetActorLocation()).Rotation();

	//GetSprite()->SetFlipbook(Flipbooks->SetMoveFlipbook());
	LocationMoveTo = Location;

	GetSprite()->SetFlipbook(Flipbooks->SetMoveFlipbook());
	AIController->MoveToLocation(LocationMoveTo, 0.1f);

	/*
	*/
}

void ACPaperCharacter::StartMoveActor(AActor* actor, FRotator rotation)
{
	unitState.UnitMove();

	current_Rotation = GetActorRotation();
	currentTime = 0.0f;

	float angle = unitState.GetVectorDegrees(GetActorForwardVector(), rotation.Vector());
	float loseSpeed = unitState.GetSpeedCurrent() * angle / 180.0f;

	LoseSpeed(loseSpeed);

	/*
	AI 컨트롤러 쓰는 버전
	*/

	formation_Direction = rotation;
	look_Direction = (actor->GetActorLocation() - GetActorLocation()).Rotation();

	//GetSprite()->SetFlipbook(Flipbooks->SetMoveFlipbook());

	GetSprite()->SetFlipbook(Flipbooks->SetMoveFlipbook());

	/*
	
	*/

	AIController->MoveToActor(actor, 0.1f,false);
}

void ACPaperCharacter::Moving(float DeltaTime)
{
	/*if (unitState.IsMove())
	{
		FVector direction = LocationMoveTo - GetActorLocation();
		direction.Z = 0.0f;

		if (direction.Size() > 5.0f)
		{
			direction = direction.GetSafeNormal() * unitState.GetSpeedCurrent() * DeltaTime * 1.0f;
			AddActorWorldOffset(direction);
		}
		else
		{
			unitState.UnitIdle();
		}
	}*/
}

void ACPaperCharacter::Charge()
{


}

void ACPaperCharacter::Idle()
{
	unitState.UnitIdle();

	LoseSpeed(unitState.GetSpeedMax());
	current_Rotation = GetActorRotation();
	GetSprite()->SetFlipbook(Flipbooks->SetIdleFlipbook());
}

void ACPaperCharacter::Attack()
{
	target->CheckDamage(unitState.GetDamage(), unitState.GetMeleeAttack(),GetActorForwardVector());
	GetSprite()->SetFlipbook(Flipbooks->SetAttackFlipbook());
}

void ACPaperCharacter::StartDead()
{

}

void ACPaperCharacter::EndDead()
{

}

void ACPaperCharacter::StartHited()
{

}

void ACPaperCharacter::EndHited()
{
	
}

void ACPaperCharacter::Rotation(float DeltaTime)
{

}

void ACPaperCharacter::RotationIdle(float DeltaTime)
{
	if (!FMath::IsNearlyEqual(GetActorRotation().Yaw, formation_Direction.Yaw, 10.0f))
	{
		SetActorRotation(FMath::Lerp(current_Rotation, formation_Direction, currentTime));

		currentTime += DeltaTime * unitState.GetSpeedRotation() / 100.0f;
	}
	else
	{
		currentTime = 0.0f;
	}
}

void ACPaperCharacter::RotationBattle(float DeltaTime)
{
	if (bAttack && !!target)
	{
		FRotator targetdirection = (target->GetActorLocation() - GetActorLocation()).Rotation();

		if (!FMath::IsNearlyEqual(GetActorRotation().Yaw, targetdirection.Yaw, 10.0f))
		{
			SetActorRotation(FMath::Lerp(GetActorRotation(), targetdirection, currentTime));

			currentTime += DeltaTime * unitState.GetSpeedRotation() / 100.0f;
		}
		else
		{
			currentTime = 0.0f;
		}
	}
}

void ACPaperCharacter::PlayFlipBook()
{ 
	GetSprite()->Play(); 
}

void ACPaperCharacter::StopPlayFlipBook()
{
	GetSprite()->Stop();
}

void ACPaperCharacter::FlipFlipBook(FVector InVector)
{
	FVector cross = FVector::CrossProduct(GetActorForwardVector(), InVector);

	if (cross.Z < 0.0f)
	{
		GetSprite()->SetWorldRotation(FRotator(-90, 0, 0));
	}
	else if (cross.Z >= 0.0f)
	{
		GetSprite()->SetWorldRotation(FRotator(90, 0, 0));
	}
}

/*
이동속도 변경
*/

void ACPaperCharacter::AddSpeed(float DeltaTime)
{
	float addSpeed = unitState.GetSpeedAcceleration() * DeltaTime;
	unitState.AddSpeedCurrent(addSpeed);
	GetCharacterMovement()->MaxWalkSpeed = unitState.GetSpeedCurrent();
}

void ACPaperCharacter::LoseSpeed(float InFloat)
{
	unitState.ReduceSpeedCurrent(InFloat);

	//GetCharacterMovement()->MaxWalkSpeed = unitState.GetSpeedCurrent();
}
/*
공격당할 시 확인해야 할 것들
*/
void ACPaperCharacter::CheckDamage(float InDamage, float InMelee_attack, FVector InAttack_Direction, FVector InForce)
{
	AddForce(InForce);

	CheckDamage(InDamage, InMelee_attack, InAttack_Direction);
}

void ACPaperCharacter::CheckDamage(float InDamage, float InMelee_attack, FVector InAttack_Direction)
{
	unitState.CheckDamage(InDamage, InMelee_attack, InAttack_Direction, GetActorForwardVector());
}

void ACPaperCharacter::CheckDead()
{
	unitState.CheckDead();
}

/*
유닛 충돌 
*/

void ACPaperCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACPaperCharacter* paperCharacter = Cast<ACPaperCharacter>(OtherActor);

	if (!!paperCharacter)
	{
		FVector forcedirection = OtherActor->GetActorLocation() - GetActorLocation();

		FVector distanceVector = forcedirection.GetSafeNormal() * 2.0f;
		distanceVector.Z = 0.0f;

		paperCharacter->AddForce(distanceVector);
		AddForce(distanceVector * -1.0f);

		if (paperCharacter->GetCNU()->IsPlayers())
		{

		}
		else
		{
			GetCNU()->AddAttackableCNU(paperCharacter->GetCNU());
		}
	}
}

void ACPaperCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ACPaperCharacter::OnOverlapBeginWeapon(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void ACPaperCharacter::OnOverlapBeginShield(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void ACPaperCharacter::OnHitBegin(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

}

	/*
	피직스 시뮬레이트
	*/

void  ACPaperCharacter::AddForce(FVector InVector)
{
	force += InVector;
}

void ACPaperCharacter::SimulatePulse(float Deltatime)
{
	FVector forcedirection = force.GetSafeNormal() * Deltatime * 100.0f;


	if (force.Size() > forcedirection.Size())
	{
		force -= forcedirection;
		AddActorWorldOffset(forcedirection);
		bSimulatePulse = true;
	}
	else
	{
		AddActorWorldOffset(force);
		force = FVector(0,0,0);
		bSimulatePulse = false;
	}
}

float ACPaperCharacter::GetSize()
{
	return unitState.GetSize();
}

void ACPaperCharacter::CheckInLocationMoveTo()
{
	if (!bSimulatePulse)
	{
		StartMove();
	}
}

void ACPaperCharacter::SetTarget(ACPaperCharacter* InPaperCharacter)
{
	if (InPaperCharacter->IsDead())
		return;

	if (InPaperCharacter->GetDistanceTo(this) <= AttackAbleRange)
	{
		if (!!target)
		{
			if (GetDistanceTo(target) > GetDistanceTo(InPaperCharacter))
			{
				target = InPaperCharacter;
			}
		}
	}
	else
	{
		target = InPaperCharacter;
		return;
	}
}

void ACPaperCharacter::CheckState(FVector InVector, bool InBool)
{
	FlipFlipBook(InVector);
	if (InBool)CheckInLocationMoveTo();
}

void ACPaperCharacter::SetUnitState(FUnitState InunitState)
{
	unitState = InunitState;

	/*
	장비 소환
	*/

	Weapon->SetStaticMesh(unitState.GetWeaponMesh());
	Shield->SetStaticMesh(unitState.GetShieldMesh());
}

void ACPaperCharacter::SetCNU(class ACommander_And_Units* InCNU)
{
	CNU = InCNU;
	/*
	콜리전 타입 설정
	*/
	if (CNU->IsPlayers())
	{
		GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
		//GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
		//GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
	}
	else
	{
		GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
		//GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
		//GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
	}
}

void ACPaperCharacter::BillbordMaker()
{
	ACharacter* character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!!character)
	{
		USpringArmComponent* springAm =  CHelpers::GetComponent<USpringArmComponent>(character);
		FRotator rotation = springAm->GetComponentRotation();
		rotation = FRotator(rotation.Roll, rotation.Yaw + 90.0f, rotation.Pitch);
		GetSprite()->SetWorldRotation(rotation);
		SetActorRotation(rotation);
	}
}

void ACPaperCharacter::OnOffSelectUI(bool Inbool)
{
	SelectUI->SetVisibility(Inbool);
}


