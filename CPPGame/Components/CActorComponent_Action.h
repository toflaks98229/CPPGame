// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActorComponent_Action.generated.h"

UENUM(BlueprintType)
enum class EActionType : uint8
{
	Move, Idle, Attack
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPPGAME_API UCActorComponent_Action : public UActorComponent
{
	GENERATED_BODY()

public:
	UCActorComponent_Action();

public:
	void SetActionMove();

	void SetActionIdel();

	void SetActionAttack();

public:
	bool IsActionMove();

	bool IsActionIdle();

	bool IsActionAttack();

protected:
	virtual void BeginPlay() override;

public:
	EActionType actionType = EActionType::Idle;
};
