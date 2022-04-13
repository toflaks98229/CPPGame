// Fill out your copyright notice in the Description page of Project Settings.
#include "CHUD_Selected.h"
#include "Global.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"

// Sets default values
ACHUD_Selected::ACHUD_Selected()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent(this, &Scene, "Scene");
	CHelpers::CreateComponent(this, &SelectUI, "SelectUI", Scene);
}

// Called when the game starts or when spawned
void ACHUD_Selected::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACHUD_Selected::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACHUD_Selected::DrawUI(bool inBool)
{
	SelectUI->SetVisibility(inBool);
}