#include "CActorComponent_Action.h"


UCActorComponent_Action::UCActorComponent_Action()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCActorComponent_Action::BeginPlay()
{
	Super::BeginPlay();
}

void UCActorComponent_Action::SetActionMove()
{
	actionType = EActionType::Move;
}

void UCActorComponent_Action::SetActionIdel()
{
	actionType = EActionType::Idle;
}

void UCActorComponent_Action::SetActionAttack()
{
	actionType = EActionType::Attack;
}

bool UCActorComponent_Action::IsActionMove()
{
	return actionType == EActionType::Move;
}

bool UCActorComponent_Action::IsActionIdle()
{
	return actionType == EActionType::Idle;
}

bool UCActorComponent_Action::IsActionAttack()
{
	return actionType == EActionType::Attack;
}
