#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActorComponent_Flipbook.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPPGAME_API UCActorComponent_Flipbook : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		class UPaperFlipbook* AttackFlipbook;

	UPROPERTY(EditAnywhere)
		class UPaperFlipbook* MoveFlipbook;

	UPROPERTY(EditAnywhere)
		class UPaperFlipbook* IdleFlipbook;

public:
	UPaperFlipbook* SetAttackFlipbook() { return AttackFlipbook; }
	UPaperFlipbook* SetMoveFlipbook() { return MoveFlipbook; }
	UPaperFlipbook* SetIdleFlipbook() { return IdleFlipbook; }

public:
	UCActorComponent_Flipbook();

protected:
	virtual void BeginPlay() override;
};
