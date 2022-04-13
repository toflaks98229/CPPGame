#include "Commander_And_Units.h"
#include "Global.h"
#include "AIController.h"
#include "CPaperCharacter.h"
#include "UnitState.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "HUD/CHUD_Selected.h"

#include "Player/CPlayerCamera.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACommander_And_Units::ACommander_And_Units()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACommander_And_Units::BeginPlay()
{
	Super::BeginPlay();

	SpawnDefaultController();
	GetCharacterMovement()->MaxWalkSpeed = unitState.GetSpeedMax();

	//Box->OnComponentBeginOverlap.AddDynamic(this, &ACommander_And_Units::OnBeginOverlap);
	//Box->OnComponentEndOverlap.AddDynamic(this, &ACommander_And_Units::OnEndOverlap);

	UnitsNum = UnitsNumMax;

	FActorSpawnParameters spawnParameters;
	spawnParameters.Owner = this;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int i = 0; i < UnitsNumMax; i++)
	{
		//==========================================================
		//SpawnUnits
		//==========================================================
		ACPaperCharacter* character;

		if (i == 0)
		{
			character = GetWorld()->SpawnActor<ACPaperCharacter>
				(CommanderData, GetActorLocation(), GetActorRotation(), spawnParameters);
		}
		else
		{
			character = GetWorld()->SpawnActor<ACPaperCharacter>
				(UnitsData, GetActorLocation(), GetActorRotation(), spawnParameters);
		}
		Units.AddUnique(character);

		character->SetUnitState(unitState);
		character->SetCNU(this);
		//character->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);


	//==========================================================
	//UnitsPosition
	//==========================================================

		UnitsPosition.Add(FVector(0, 0, 0));

		//==========================================================
		//SpawnUnitsUI
		//==========================================================

		ACHUD_Selected* unitUI = GetWorld()->SpawnActor<ACHUD_Selected>
			(UnitsUIData, GetActorLocation(), GetActorRotation(), spawnParameters);
		UnitsUI.AddUnique(unitUI);
		//unitUI->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

		//==========================================================
		//SpawnUnitsActor
		//==========================================================

		AActor* unitActor = GetWorld()->SpawnActor<AActor>
			(UnitsActorData, GetActorLocation(), GetActorRotation(), spawnParameters);
		UnitsTargetActors.AddUnique(unitActor);
		//unitActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	}

	//==========================================================
	//SetPosition
	//==========================================================

	FRotator rotation = GetActorRotation();
	rotation.Yaw += 90.0f;

	unitsFormationData = SetFormationStart(1000, rotation.Vector(), GetActorLocation());

	int a = 0;

	for (ACPaperCharacter* unit: Units)
	{
		unit->SetActorLocation(UnitsPosition[a]);
		unit->SetLocationMoveTo(UnitsPosition[a]);
		unit->Setdirection(GetActorRotation());
		UnitsTargetActors[a]->SetActorLocation(UnitsPosition[a]);

		a++;
	}

	Current_UnitsPosition = UnitsPosition;

	//SetActorLocation(Commander->GetActorLocation());

	//Commander->SetActorLocation(CommanderPosition);

	/*
	플레이어 저장
	*/

	player = Cast<ACPlayerCamera>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ACommander_And_Units::Tick(float DeltaTime)
{
	//SetActorLocation(UnitsPosition[0]);
	//CLog::Print("Commander : " + Commander->GetActorLocation().ToString());
	//CLog::Print("CNU : " + GetActorLocation().ToString());



	if (!!player)
	{
		CheckInLocationTick += DeltaTime;

		if (CheckInLocationTick > CheckInLocationRate)
		{
			for (ACPaperCharacter* unit : Units)
			{
				unit->CheckState(player->GetActorForwardVector(), true);
			}

			CheckInLocationTick = 0.0f;
		}
		else
		{
			for (ACPaperCharacter* unit : Units)
			{
				unit->CheckState(player->GetActorForwardVector(), false);
			}
		}
	}
}

void ACommander_And_Units::Selected()
{
	bSelected = true;

	for (ACPaperCharacter* unit : Units)
	{
		unit->OnOffSelectUI(true);
	}
}
void ACommander_And_Units::UnSelected()
{
	bSelected = false;

	for (ACPaperCharacter* unit : Units)
	{
		unit->OnOffSelectUI(false);
	}
}

void ACommander_And_Units::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACommander_And_Units* cnu = Cast<ACommander_And_Units>(OtherActor);

	if (!!cnu)
	{
		if (bCheckPlayerUnit(cnu) == false)
			AddAttackableCNU(cnu);
	}
}
void ACommander_And_Units::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACommander_And_Units* cnu = Cast<ACommander_And_Units>(OtherActor);

	if (!!cnu)
	{
		if (bCheckPlayerUnit(cnu) == false)
			RemoveAttackableCNU(cnu);
	}
}

FVector ACommander_And_Units::GetCommanderPosition()
{
	FVector position = FVector(0,0,0); 

	if (!!Units[0])
		position = Units[0]->GetActorLocation();

	return position;
}

/*
HUD 그리기
*/

void ACommander_And_Units::DrawHUD(bool inBool)
{
	for (ACHUD_Selected* hud : UnitsUI)
	{
		hud->DrawUI(inBool);
	}
}

void ACommander_And_Units::SetHUDPosition()
{
	for (int i = 0; i < UnitsPosition.Num(); i++)
	{
		UnitsUI[i]->SetActorLocation(UnitsPosition[i]);
	}
}

bool ACommander_And_Units::IsOverMaxUnitLineCount(int InInt)
{
	if (InInt < UnitsNum && InInt < MaxUnitLineCount)
	{
		return false;
	}

	return true;
}

//==========================================================================

void ACommander_And_Units::SetCurrentUnitsPosition()
{
	Current_UnitsPosition.Empty();

	for (ACPaperCharacter* unit : Units)
	{
		Current_UnitsPosition.Add(unit->GetActorLocation());
	}
}

void ACommander_And_Units::NormalMove(FVector direction)
{
	//Cast<AAIController>(GetController())->MoveToLocation(UnitsPosition[0], 2.0f);

	/*for (ACPaperCharacter* unit : Units)
	{
		FVector unitlocation = unit->GetActorLocation() + direction;
		unit->StartMove(unitlocation, CNUdirection);
	}

	FVector targetlocation = direction + GetActorLocation();
	Commander->StartMove(targetlocation, CNUdirection);*/

	SetCurrentUnitsPosition();

	unitsFormationData = SetFormationNormal(direction.GetSafeNormal(), GetCommanderPosition() + direction);

	Current_UnitsPosition = hungarian();

	int a = 0;

	for (ACPaperCharacter* unit : Units)
	{
		unit->StartMove(Current_UnitsPosition[a], direction.Rotation());
		//unit->StartMoveActor(UnitsStandPosition[a], direction.Rotation());
		a++;
	}

	Cast<AAIController>(GetController())->MoveToLocation(unitsFormationData.location);
	//Current_UnitsPosition = UnitsPosition;
}

void ACommander_And_Units::FormationMove(UnitsFormationData InUnitsFormationData)
{
	FormationMove(InUnitsFormationData.length, InUnitsFormationData.direction, InUnitsFormationData.location);
}

void ACommander_And_Units::FormationMove(float InLength, FVector InVector, FVector InTargetLocation)
{
	unitsFormationData = SetFormation(InLength, InVector, InTargetLocation);

	FRotator rotation = InVector.Rotation();
	rotation.Yaw -= 90.0f;

	SetCurrentUnitsPosition();

	Current_UnitsPosition = hungarian();

	int a = 0;

	for (ACPaperCharacter* unit : Units)
	{
		unit->StartMove(Current_UnitsPosition[a], rotation);
		//unit->StartMoveActor(UnitsStandPosition[a], rotation);
		a++;
	}


	Cast<AAIController>(GetController())->MoveToLocation(unitsFormationData.location);
	//Current_UnitsPosition = UnitsPosition;
}

void ACommander_And_Units::FormationMoveActor(float InLength, FVector InVector, FVector InTargetLocation)
{
	unitsFormationData = SetFormationActor(InLength, InVector, InTargetLocation);

	FRotator rotation = InVector.Rotation();
	rotation.Yaw -= 90.0f;

	SetCurrentUnitsPosition();

	Current_UnitsPosition = hungarian();
	FVector position = FVector(0,0,0);

	for (int i = 0; i < Current_UnitsPosition.Num(); i++)
	{
		UnitsTargetActors[i]->SetActorLocation(Current_UnitsPosition[i]);
		position += Current_UnitsPosition[i];
	}

	position = position / Current_UnitsPosition.Num();

	int a = 0;

	for (ACPaperCharacter* unit : Units)
	{
		unit->StartMoveActor(UnitsTargetActors[a], rotation);
		a++;
	}

	Cast<AAIController>(GetController())->MoveToLocation(unitsFormationData.location);
}

void ACommander_And_Units::FormationMoveActor(UnitsFormationData InUnitsFormationData)
{
	FormationMoveActor(InUnitsFormationData.length, InUnitsFormationData.direction, InUnitsFormationData.location);
}

void ACommander_And_Units::NormalMoveActor(FVector location)
{



}

/*
유닛 전형 설정
*/

UnitsFormationData ACommander_And_Units::SetFormationStart(float InLength, FVector InVector, FVector InTargetLocation)
{
	FRotator rotation;

	rotation = InVector.Rotation();
	rotation.Yaw += 90.0f;

	

	/*
	더할 때 쓰는 백터
	*/

	FVector Addvectorwidth = InVector.GetSafeNormal() * unitState.GetSize();
	FVector AddvectorVertical = rotation.Vector().GetSafeNormal() * unitState.GetSize();

	/*
	유닛의 수
	*/

	int UnitsNumInwidth = InLength / unitState.GetSize();

	if (UnitsNumInwidth < Units.Num() / MaxUnitLineCount)
	{
		UnitsNumInwidth = 1;
		while (UnitsNumInwidth < Units.Num() / MaxUnitLineCount)
		{
			UnitsNumInwidth++;
		}
	}
	else if (UnitsNumInwidth > MaxUnitLineCount)
	{
		UnitsNumInwidth = MaxUnitLineCount;
	}

	int UnitsNumInVertical = Units.Num() / UnitsNumInwidth;

	FVector vector = GetActorLocation();
	vector = vector - AddvectorVertical * UnitsNumInVertical / 2;
	vector = vector + Addvectorwidth * ((UnitsNumInwidth - 1) % 2) / 2;

	int a = 0;

	for (ACPaperCharacter* unit : Units)
	{
		FVector position = vector;

		position += AddvectorVertical * floor(a / UnitsNumInwidth);

		float addfloat = a % UnitsNumInwidth;

		if ((int)addfloat % 2 == 0)
		{
			position += Addvectorwidth * addfloat / 2;
		}
		else
		{
			position -= Addvectorwidth * (addfloat + 1) / 2;
		}

		UnitsPosition[a] = SetUnitWorldPostion(position);
		a++;
	}

	return UnitsFormationData(InVector, UnitsNumInwidth * unitState.GetSize(), vector);
}

UnitsFormationData ACommander_And_Units::SetFormation(UnitsFormationData InUnitsFormationData)
{
	return SetFormation(InUnitsFormationData.length, InUnitsFormationData.direction, InUnitsFormationData.location);
}

UnitsFormationData ACommander_And_Units::SetFormation(float InLength, FVector InVector, FVector InTargetLocation)
{
	FRotator rotation;

	rotation = InVector.Rotation();
	rotation.Yaw += 90.0f;

	

	/*
	더할 때 쓰는 백터
	*/

	FVector Addvectorwidth = InVector.GetSafeNormal() * unitState.GetSize();
	FVector AddvectorVertical = rotation.Vector().GetSafeNormal() * unitState.GetSize();

	/*
	유닛의 수
	*/

	int UnitsNumInwidth = InLength / unitState.GetSize();

	if (UnitsNumInwidth < 1)
		UnitsNumInwidth = 1;

	if (UnitsNumInwidth < Units.Num() / MaxUnitLineCount)
	{
		while (UnitsNumInwidth < Units.Num() / MaxUnitLineCount)
		{
			UnitsNumInwidth++;
		}
	}
	else if (UnitsNumInwidth > MaxUnitLineCount)
	{
		UnitsNumInwidth = MaxUnitLineCount;
	}

	int UnitsNumInVertical = Units.Num() / UnitsNumInwidth;

	FVector vector = InTargetLocation + UnitsNumInwidth / 2 * Addvectorwidth;

	int a = 0;
	
	for (ACPaperCharacter* unit : Units)
	{
		FVector position = vector;

		position += AddvectorVertical * floor(a / UnitsNumInwidth);

		float addfloat = a % UnitsNumInwidth;

		if ((int)addfloat % 2 == 0)
		{
			position += Addvectorwidth * addfloat / 2;
		}
		else
		{
			position -= Addvectorwidth * (addfloat + 1) / 2;
		}

		UnitsPosition[a] = SetUnitWorldPostion(position);
		a++;
	}

	return UnitsFormationData(InVector, UnitsNumInwidth * unitState.GetSize(), vector);
}

/*
액터 이동 기반 
*/

UnitsFormationData ACommander_And_Units::SetFormationActor(UnitsFormationData InUnitsFormationData)
{
	return SetFormationActor(InUnitsFormationData.length, InUnitsFormationData.direction, InUnitsFormationData.location);
}

UnitsFormationData ACommander_And_Units::SetFormationActor(float InLength, FVector InVector, FVector InTargetLocation)
{
	FRotator rotation;

	rotation = InVector.Rotation();
	rotation.Yaw += 90.0f;

	

	/*
	더할 때 쓰는 백터
	*/

	FVector Addvectorwidth = InVector.GetSafeNormal() * unitState.GetSize();
	FVector AddvectorVertical = rotation.Vector().GetSafeNormal() * unitState.GetSize();

	/*
	유닛의 수
	*/

	int UnitsNumInwidth = InLength / unitState.GetSize();

	if (UnitsNumInwidth < Units.Num() / MaxUnitLineCount)
	{
		UnitsNumInwidth = 1;
		while (UnitsNumInwidth < Units.Num() / MaxUnitLineCount)
		{
			UnitsNumInwidth++;
		}
	}
	else if (UnitsNumInwidth > MaxUnitLineCount)
	{
		UnitsNumInwidth = MaxUnitLineCount;
	}

	int UnitsNumInVertical = Units.Num() / UnitsNumInwidth;

	FVector vector = GetActorLocation();
	vector = vector - AddvectorVertical * UnitsNumInVertical / 2;
	vector = vector + Addvectorwidth * ((UnitsNumInwidth + 1) % 2) / 2;


	int a = 0;

	for (ACPaperCharacter* unit : Units)
	{
		FVector position = vector;

		position += AddvectorVertical * floor(a / UnitsNumInwidth);

		float addfloat = a % UnitsNumInwidth;

		if ((int)addfloat % 2 == 0)
		{
			position += Addvectorwidth * addfloat / 2;
		}
		else
		{
			position -= Addvectorwidth * (addfloat + 1) / 2;
		}

		UnitsPosition[a] = position;
		a++;
	}

	FVector targetVector = InTargetLocation + Addvectorwidth * UnitsNumInwidth / 2;

	return UnitsFormationData(InVector, UnitsNumInwidth * unitState.GetSize(), targetVector);
}

/*
SetFormationNormal의 위치 백터와 디랙션 백터는 포메이션 백터와 다르다
*/

UnitsFormationData ACommander_And_Units::SetFormationNormal(FVector InVector, FVector InTargetLocation)
{
	FRotator rotation;
	rotation = InVector.Rotation();
	rotation.Yaw += 90.0f;

	

	/*
	더할 때 쓰는 백터
	*/

	FVector Addvectorwidth = rotation.Vector().GetSafeNormal() * unitState.GetSize();
	FVector AddvectorVertical = InVector.GetSafeNormal() * unitState.GetSize() * -1.0f;

	/*
	유닛의 수
	*/

	int UnitsNumInwidth = unitsFormationData.length / unitState.GetSize();

	if (UnitsNumInwidth < Units.Num() / MaxUnitLineCount)
	{
		UnitsNumInwidth = 1;
		while (UnitsNumInwidth < Units.Num() / MaxUnitLineCount)
		{
			UnitsNumInwidth++;
		}
	}
	else if (UnitsNumInwidth > MaxUnitLineCount)
	{
		UnitsNumInwidth = MaxUnitLineCount;
	}

	int UnitsNumInVertical = Units.Num() / UnitsNumInwidth;

	FVector vector = InTargetLocation;
	//FVector vector = GetActorLocation();

	int a = 0;

	for (ACPaperCharacter* unit : Units)
	{
		FVector position = vector;

		position += AddvectorVertical * floor(a / UnitsNumInwidth);

		float addfloat = a % UnitsNumInwidth;

		if ((int)addfloat % 2 == 0)
		{
			position += Addvectorwidth * addfloat / 2;
		}
		else
		{
			position -= Addvectorwidth * (addfloat + 1) / 2;
		}

		UnitsPosition[a] = SetUnitWorldPostion(position);
		a++;
	}

	return UnitsFormationData(InVector, UnitsNumInwidth * Addvectorwidth.Size(), vector);
}

UnitsFormationData ACommander_And_Units::SetFormationNormal(UnitsFormationData InUnitsFormationData)
{
	return SetFormationNormal(InUnitsFormationData.direction, InUnitsFormationData.location);
}

FVector ACommander_And_Units::SetUnitWorldPostion(FVector inVector)
{
	FVector start = inVector;
	FVector end = inVector;

	start.Z += 4000.0f;
	end.Z -= 4000.0f;

	FHitResult hitresult;

	GetWorld()->LineTraceSingleByChannel(hitresult, start, end, ECollisionChannel::ECC_WorldDynamic);

	if(hitresult.bBlockingHit)
	{
		FVector location = hitresult.Location;

		if (location.Z <= 0)
		{
			location.Z = 0;
		}

		location.Z += 44.0f;
		return location;
	}
	else
	{
		return inVector;
	}
}

#define N 200
#define INF 987654321

TArray<FVector> ACommander_And_Units::hungarian()
{
	int w[N][N];
	int n = UnitsPosition.Num();
	
	int match_x[N];
	int match_y[N];

	int l_x[N];
	int l_y[N];

	bool s[N];
	bool t[N];

	int slack[N];
	int slack_x[N];

	int tree_x[N];
	int tree_y[N];

	FVector position1 = FVector(0, 0, 0);
	FVector position2 = FVector(0, 0, 0);
	FVector length = FVector(0, 0, 0);

	//중정계산
	for (int i = 0; i < n; i++)
	{
		position1 += UnitsPosition[i];
		position2 += Current_UnitsPosition[i];
	}

	position1 = position1 / n;
	position2 = position2 / n;
	length = position1 - position2;
	length.Z = 0;

	//각 데이터 계산 이 이후에 0 값은 넣지 않을 것
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			w[i][j] = floor((UnitsPosition[i] - Current_UnitsPosition[j] - length).Size());
		}
	}

	memset(match_x, -1, sizeof(match_x));
	memset(match_y, -1, sizeof(match_y));
	memset(l_x, INF, sizeof(l_x));
	memset(l_y, INF, sizeof(l_y));

	for (int i = 0; i < n; ++i) 
	{
		for (int j = 0; j < n; ++j) 
		{
			l_x[i] = FMath::Min(l_x[i], w[i][j]);
		}
	}

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			l_y[i] = FMath::Min(l_y[i], w[j][i] - l_x[j]);
		}
	}

	int m = 0;
	while (m != n)
	{
		// repeat at most V times
		memset(tree_x, -1, sizeof(tree_x));
		memset(tree_y, -1, sizeof(tree_y));
		memset(s, false, sizeof(s));
		memset(t, false, sizeof(t));

		int s_start;
		for (int i = 0; i < n; ++i)
		{ 
			// O(V)
			if (match_x[i] == -1)
			{
				//CLog::Log(i);
				s[i] = true;
				s_start = i;
				break;
			}
		}

		for (int i = 0; i < n; ++i)
		{ 
			// init slack
			//slack[i] = l_x[s_start] + l_y[i] - w[s_start][i];
			slack[i] = w[s_start][i] - l_x[s_start] - l_y[i];
			slack_x[i] = s_start;
		}

		bool bStop = true;
		while (bStop)
		{
			int y = -1;
			for (int i = 0; i < n; ++i)
			{
				// compare: O(V)
				if (slack[i] == 0 && t[i] == false)
				{
					y = i;
				}
			}

			//=======================
			if (y == -1)
			{ 
				// n_l = t
				// update label

				int alpha = INF;
				for (int i = 0; i < n; ++i)
				{
					// O(V)
					if (t[i] == false)
					{
						alpha = FMath::Min(alpha, slack[i]);
					}
				}
				for (int i = 0; i < n; ++i) 
				{ // O(V)
					if (s[i]) l_x[i] += alpha;
					if (t[i]) l_y[i] -= alpha;
				}
				for (int i = 0; i < n; ++i)
				{ // O(V)
					if (!t[i])
					{
						slack[i] -= alpha;
						if (slack[i] == 0)
						{
							y = i;
						}
					}
				}
			}

			// n_l != t is guaranteed
			if (match_y[y] == -1)
			{ // free
				tree_y[y] = slack_x[y];
				while (y != -1)
				{
					int x = tree_y[y];
					match_y[y] = x;
					int next_y = match_x[x];
					match_x[x] = y;
					y = next_y;
				}
				m++;
				bStop = false;
			}
			else 
			{ // matched
				int z = match_y[y];
				tree_x[z] = y;
				tree_y[y] = slack_x[y];

				s[z] = true;
				t[y] = true;

				// z가 추가되었으므로 slack과 n_l이 update
				for (int i = 0; i < n; ++i) 
				{ // O(V)
					if (slack[i] > w[z][i] - l_x[z] - l_y[i])
					{
						slack[i] = w[z][i] - l_x[z] - l_y[i];
						slack_x[i] = z;
					}
				}
			}
		}
	}

	TArray<FVector> vector = Current_UnitsPosition;

	for (int i = 0; i < n; ++i)
	{
		if(match_y[i] == -1)
		{
			//CLog::Print(i);
		}
		else
		{
			vector[i] = UnitsPosition[match_y[i]];
		}
	}

	return vector;
}

//======================================================================

void ACommander_And_Units::Rotation()
{

}

void ACommander_And_Units::AttackCommand(class ACommander_And_Units* InCNU)
{
	FVector vector = InCNU->GetCommanderPosition() - GetCommanderPosition();

	NormalMove(vector);
}

bool ACommander_And_Units::bCheckPlayerUnit(ACommander_And_Units* InCNU)
{
	return InCNU->bPlayers;
}

void  ACommander_And_Units::UnitDead(ACPaperCharacter* InUnit)
{
	int num = 0;

	for (ACPaperCharacter* unit : Units)
	{
		if (unit == InUnit)
		{
			break;
		}
		else
		{
			num++;
		}
	};

	if (num < Units.Num())
	{
		Units.Remove(InUnit);
		UnitsUI.RemoveAt(num);
		UnitsPosition.RemoveAt(num);
	}
}

void ACommander_And_Units::AddAttackableCNU(ACommander_And_Units* InCNU)
{
	AttackableCNUs.AddUnique(InCNU);
}

void ACommander_And_Units::RemoveAttackableCNU(ACommander_And_Units* InCNU)
{
	AttackableCNUs.Remove(InCNU);
}

void ACommander_And_Units::CheckAttackable()
{
	if (AttackableCNUs.Num() != 0)
	{
		for (ACommander_And_Units* cnu : AttackableCNUs)
		{
			for (ACPaperCharacter* attackableUnit : cnu->GetUnits())
			{
				if (!!attackableUnit)
				{
					for (ACPaperCharacter* unit : Units)
					{
						unit->SetTarget(attackableUnit);
					}
				}
			}
		}
	}
}

void ACommander_And_Units::SpawnUnits()
{
	
}

