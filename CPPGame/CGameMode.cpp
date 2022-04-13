// Fill out your copyright notice in the Description page of Project Settings.

#include "CGameMode.h"
#include "CHUD.h"
#include "Global.h"
#include "CPlayerController.h"
#include "Player/CPlayerCamera.h"
#include "GameFramework/PlayerController.h"
#include "Input/Reply.h"

ACGameMode::ACGameMode()
{
	CHelpers::GetClass<APawn>(&DefaultPawnClass, "Blueprint'/Game/Player/BP_CPlayerCamera.BP_CPlayerCamera_C'");
	CHelpers::GetClass<APlayerController>(&PlayerControllerClass, "Blueprint'/Game/BP_CPlayerController.BP_CPlayerController_C'");
	CHelpers::GetClass<AHUD>(&HUDClass, "Blueprint'/Game/BP_CHUD.BP_CHUD_C'");
}

void ACGameMode::BeginPlay()
{
	Super::BeginPlay(); 

}
