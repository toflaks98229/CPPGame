#include "CAttackCommand.h"
#include "Kismet/GameplayStatics.h"
#include "Character/Commander_And_Units.h"

ACAttackCommand::ACAttackCommand()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACAttackCommand::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ACAttackCommand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACAttackCommand::GetAllCNU()
{
	/*
	À¯´Ö Ã£±â
	*/
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, CNUs);
}