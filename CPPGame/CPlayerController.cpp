#include "CPlayerController.h"
#include "Global.h"
#include "CHUD.h"

#include "Character/Commander_And_Units.h"
#include "Character/CNUSizeCheck.h"

#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/CPlayerCamera.h"
#include "Math/UnrealMathUtility.h"



ACPlayerController::ACPlayerController()
{
	EnableInput(this);
	bShowMouseCursor = true;
	PrimaryActorTick.bCanEverTick = true;
	bDrawDecal = false;


}

void ACPlayerController::BeginPlay()
{
	InputComponent->BindAction("LeftMouseClick", EInputEvent::IE_Pressed, this, &ACPlayerController::LeftMouse_Pressed);
	InputComponent->BindAction("LeftMouseClick", EInputEvent::IE_Released, this, &ACPlayerController::LeftMouse_Released);

	InputComponent->BindAction("RightMouseClick", EInputEvent::IE_Pressed, this, &ACPlayerController::RightMouse_Pressed);
	InputComponent->BindAction("RightMouseClick", EInputEvent::IE_Released, this, &ACPlayerController::RightMousee_Released);

	PlayerCamera = Cast<ACPlayerCamera>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	FInputModeGameAndUI inputMode;
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	inputMode.SetHideCursorDuringCapture(false);
	SetInputMode(inputMode);

	HUD = Cast<ACHUD>(GetHUD());

	GetAllUnitsInField();

}

void ACPlayerController::Tick(float DeltaTime)
{
	GetHitResult();
	DrawDecal();
	//GetAllUnitsInField();
}

/*
	마우스 사용
	*/
void ACPlayerController::LeftMouse_Pressed()
{
	if (bDrawDecal == true)
	{
		bDrawDecal = false;
	}

	if (bhitResultLeft)
	{
		ECollisionChannel collisionChannel;
		collisionChannel = hitResultLeft->GetComponent()->GetCollisionObjectType();

		switch (collisionChannel)
		{
		case ECC_WorldStatic:
			break;
		case ECC_GameTraceChannel1:
			break;
		case ECC_GameTraceChannel2:
			break;
		}

		bSelect = true;
		StartSelect();
	}
	else
	{

	}

	DrawHUD(false);

}

void ACPlayerController::LeftMouse_Released()
{
	if (bSelect)
	{
		EndSelect();
	}
}

void ACPlayerController::RightMouse_Pressed()
{

	//CLog::Print(EObjectTypeQuery::ObjectTypeQuery8);

	if (bhitResultRight)
	{
		ECollisionChannel collisionChannel;
		collisionChannel = hitResultRight->GetComponent()->GetCollisionObjectType();
		ACPaperCharacter* paperCharacter;

		switch (collisionChannel)
		{
		case ECC_WorldStatic:
			bDrawDecal = true;
			StartDrawDecal();
			break;
		case ECC_GameTraceChannel2:
			paperCharacter = Cast<ACPaperCharacter>(hitResultRight->GetActor());
			if (!!paperCharacter)
			{
				AttackCommand(paperCharacter);
			}
			break;
		default:
			break;
		}
	}
}



void ACPlayerController::RightMousee_Released()
{
	if (bDrawDecal)
	{
		EndDrawDecal();
	}
}

/*
Units Controller
*/

void ACPlayerController::AddCNU(class ACommander_And_Units* InCNU)
{
	if (InCNU->IsPlayers())
	{
		for (ACommander_And_Units* CUN : PlayersCNUs)
		{
			if (CUN == InCNU)
			{
				CUN->Selected();
			}
		}
		//PlayersCNUsSelected.AddUnique(InCNU);
		InCNU->Selected();
	}
}
void ACPlayerController::ReMoveCNU(class ACommander_And_Units* InCNU)
{
	//PlayersCNUsSelected.Remove(InCNU);
	InCNU->UnSelected();
}

void ACPlayerController::ReMoveAllCNU()
{
	for (ACommander_And_Units* cnu : PlayersCNUs)
	{
		cnu->UnSelected();
	}
	//PlayersCNUsSelected.Empty();
}

void ACPlayerController::StartSelect()
{
	HUD->StartSelect();
}

void ACPlayerController::EndSelect()
{
	HUD->EndSelect();
}

void ACPlayerController::NormalMove()
{
	FVector targetLocation = DecalStartPosition;

	FVector direction = FVector(0,0,0);

	int num = 0;

	for (ACommander_And_Units* CUN : PlayersCNUs)
	{
		if (CUN->IsSelected())
		{
			direction += CUN->GetCommanderPosition();
			num++;
		}
	}

	direction = direction / num;
	direction = targetLocation - direction;
	direction.Z = 0;

	for (ACommander_And_Units* CUN : PlayersCNUs)
	{
		if (CUN->IsSelected())
		{
			CUN->NormalMove(direction);
			float CNUlength = CUN->GetunitsFormationData().length;
			targetLocation += direction.GetSafeNormal() * CNUlength;
		}
	}

	//for (ACommander_And_Units* CUN : PlayersCNUsSelected)
	//{
	//	if (CUN->IsSelected())
	//		CUN->NormalMove(direction);
	//}
}

void ACPlayerController::FormationMove()
{
	TArray<CNUSizeCheck*> decalSizeCheck;

	float current_length = 0.0f;
	float decalDirectionLength = DecalDirection.Size();
	bool bCheckLenth = true;

	for (ACommander_And_Units* CNU : PlayersCNUs)
	{
		if (CNU->IsSelected())
		{
			decalSizeCheck.Add(new CNUSizeCheck(CNU, DecalStartPosition));
		}
	}

	if (decalSizeCheck.Num() < 1)
		return;

	while (bCheckLenth)
	{
		int num = 0;

		for (CNUSizeCheck* decalsizecheck : decalSizeCheck)
		{
			if (decalsizecheck->IsOverMaxUnitLineCount())
			{
				num++;
			}
			else
			{
				current_length += decalsizecheck->GetSize();
				decalsizecheck->AddCunLengthNum();
			}

			if (current_length > decalDirectionLength || num >= decalSizeCheck.Num())
			{
				bCheckLenth = false;
				break;
			}
		}
	}

	FVector location = DecalStartPosition;

	for (CNUSizeCheck* decalsizecheck : decalSizeCheck)
	{
		decalsizecheck->SetCnuLocation(location);

		location = location + DecalDirection.GetSafeNormal() * decalsizecheck->GetCnuLength();
	}

	decalSizeCheck = Hungarian(decalSizeCheck);
	DrawHUD(bFormationMove);

	for (CNUSizeCheck* decalsizecheck : decalSizeCheck)
	{
		decalsizecheck->FormationMove(DecalDirection);
	}
}

void ACPlayerController::StartDrawDecal()
{
	DecalStartPosition = hitResultRight->Location;

	bDrawDecal = true;
}

void ACPlayerController::EndDrawDecal()
{
	bDrawDecal = false;
	if (bNormalMove == true)
	{
		NormalMove();
		bNormalMove = false;
		bFormationMove = false;
	}
	else if (bFormationMove == true)
	{
		FormationMove();
		bNormalMove = false;
		bFormationMove = false;
	}

	//HUD 지우기
	DrawHUD(false);
}

void ACPlayerController::DrawHUD(bool inBool)
{
	for (ACommander_And_Units* CUN : PlayersCNUs)
	{
		if (CUN->IsSelected())
		{
			CUN->DrawHUD(inBool);
		}
		else
		{
			CUN->DrawHUD(false);
		}
	}
}



void ACPlayerController::DrawDecal()
{
	if (bDrawDecal)
	{
		DecalEndPosition = hitResultRight->Location;
		DecalDirection = DecalEndPosition - DecalStartPosition;

		if (DecalDirection.Size() > FormationMoveLength)
		{
			bNormalMove = false;
			bFormationMove = true;
		}
		else if(!bFormationMove)
		{
			bNormalMove = true;
			return;
		}

		TArray<CNUSizeCheck*> decalSizeCheck;

		float current_length = 0.0f;
		float decalDirectionLength = DecalDirection.Size();
		bool bCheckLenth = true;

		for (ACommander_And_Units* CNU : PlayersCNUs)
		{
			if (CNU->IsSelected())
			{
				decalSizeCheck.Add(new CNUSizeCheck(CNU, DecalStartPosition));
			}
		}

		if (decalSizeCheck.Num() < 1)
			return;

		while (bCheckLenth)
		{
			int num = 0;

			for (CNUSizeCheck* decalsizecheck : decalSizeCheck)
			{
				if (decalsizecheck->IsOverMaxUnitLineCount())
				{
					num++;
				}
				else
				{
					current_length += decalsizecheck->GetSize();
					decalsizecheck->AddCunLengthNum();
				}

				if (current_length > decalDirectionLength || num >= decalSizeCheck.Num())
				{
					bCheckLenth = false;
					break;
				}
			}
		}

		FVector location = DecalStartPosition;

		for (CNUSizeCheck* decalsizecheck : decalSizeCheck)
		{
			decalsizecheck->SetCnuLocation(location);

			location = location + DecalDirection.GetSafeNormal() * decalsizecheck->GetCnuLength();
		}

		decalSizeCheck = Hungarian(decalSizeCheck);
		DrawHUD(bDrawDecal);

		for (CNUSizeCheck* decalsizecheck : decalSizeCheck)
		{
			decalsizecheck->SetFormation(DecalDirection);
		}
	}
}

/*
공격
*/
void ACPlayerController::AttackCommand(class ACPaperCharacter* InPaperCharacter)
{
	for (ACommander_And_Units* cnu : PlayersCNUs)
	{
		if (cnu->IsSelected())
		{
			cnu->AttackCommand(InPaperCharacter->GetCNU());
		}
	}
}

void ACPlayerController::GetHitResult()
{
	bhitResultLeft = false;
	bhitResultRight = false;

	hitResultLeft = new FHitResult();
	TArray<TEnumAsByte<EObjectTypeQuery>> objectType = ObjectTypeQueryLeft;
	GetHitResultUnderCursorForObjects(objectType, false, *hitResultLeft);

	if (!!hitResultLeft)
	{
		if (!!hitResultLeft->GetComponent())
		{
			bhitResultLeft = true;
		}
	}

	hitResultRight = new FHitResult();
	objectType = ObjectTypeQueryRight;
	GetHitResultUnderCursorForObjects(objectType, false, *hitResultRight);

	if (!!hitResultRight)
	{
		if (!!hitResultRight->GetComponent())
		{
			bhitResultRight = true;
		}
	}
}

/*
헝가리안
*/

#define N 200
#define INF 987654321

TArray<CNUSizeCheck*> ACPlayerController::Hungarian(TArray<CNUSizeCheck*> InActors)
{
	int w[N][N];
	int n = InActors.Num();

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

	TArray<CNUSizeCheck*> decalSizeCheck = InActors;

	//각 데이터 계산 이 이후에 0 값은 넣지 않을 것
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			w[i][j] = floor(FVector::Distance(decalSizeCheck[j]->GetCnuLocation(), decalSizeCheck[i]->GetCnuLocation()));
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

	for (int i = 0; i < n; ++i)
	{
		if (match_y[i] == -1)
		{
			//CLog::Print(i);
		}
		else
		{
			decalSizeCheck[i] = InActors[match_y[i]];
		}
	}

	return decalSizeCheck;
}

/*
필드의 유닛들 생성과 가져오기
*/

void ACPlayerController::GetAllUnitsInField()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, Actors);

	for (AActor* actor : Actors)
	{
		ACommander_And_Units* cnu = Cast<ACommander_And_Units>(actor);

		if (!!cnu)
		{
			if (cnu->IsPlayers())
			{
				PlayersCNUs.AddUnique(cnu);
			}
			else
			{
				EnemiesCNUs.AddUnique(cnu);
			}
		}
	}
}

void ACPlayerController::MakeAllUnitsInField()
{

}


