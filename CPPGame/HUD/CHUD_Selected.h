// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CHUD_Selected.generated.h"


UCLASS()
class CPPGAME_API ACHUD_Selected : public AActor
{
	GENERATED_BODY()
	

private:
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere)
		class UPaperSpriteComponent* SelectUI;

public:	
	// Sets default values for this actor's properties
	ACHUD_Selected();

	void DrawUI(bool inBool);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
