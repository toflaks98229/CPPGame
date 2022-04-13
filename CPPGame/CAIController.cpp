#include "CAIController.h"

ACAIController::ACAIController()
{
	if (tickRate == 0.0f)
		tickRate = 1.0f;

	SetActorTickInterval(1 / tickRate);
}

void ACAIController::Tick(float DeltaTime)
{

}
