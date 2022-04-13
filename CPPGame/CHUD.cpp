#include "CHUD.h"
#include "Global.h"
#include "CPlayerController.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/Commander_And_Units.h"
#include "Character/CPaperCharacter.h"

ACHUD::ACHUD()
{

}

void ACHUD::BeginPlay()
{
	
}

void ACHUD::DrawHUD()
{
	if (bDraw)
	{
		DrawSelect();	
	}

	if (bGetActor)
	{
		GetActors();
		bGetActor = false;
	}
}

void ACHUD::StartSelect()
{
	characters.Empty();
	CNUs.Empty();

	StartPoint = GetMousePosition();

	bDraw = true;
	bGetActor = false;
}

void ACHUD::EndSelect()
{
	bDraw = false;
	bGetActor = true;
}

void ACHUD::DrawSelect()
{ 
	EndPoint = GetMousePosition();
	DrawRect(BoxColor, StartPoint.X, StartPoint.Y, EndPoint.X - StartPoint.X, EndPoint.Y - StartPoint.Y);
}

void ACHUD::GetActors()
{
	GetActorsInSelectionRectangle<ACPaperCharacter>(StartPoint, EndPoint, characters, false, true);

	ACPlayerController* controller = Cast<ACPlayerController>(GetOwningPlayerController());

	controller->ReMoveAllCNU();

	for (ACPaperCharacter* character : characters)
	{
		CNUs.AddUnique(character->GetCNU());
	}

	for (ACommander_And_Units* cnu : CNUs)
	{
		controller->AddCNU(cnu);
	}
}

FVector2D ACHUD::GetMousePosition()
{
	return UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()) * UWidgetLayoutLibrary::GetViewportScale(GetWorld());
}