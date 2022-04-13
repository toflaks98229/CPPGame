// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttackCommand.generated.h"

UCLASS()
class CPPGAME_API ACAttackCommand : public AActor
{
	GENERATED_BODY()

public:
	void GetAllCNU();

public:
	ACAttackCommand();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	/*
	ƽ �ӵ� ����
	*/
	UPROPERTY(EditAnywhere)
		float MaxtickRate;
	float tickRate;

private:
	/*
	CNU ���͵�
	*/
	UPROPERTY(EditAnywhere)
		TArray<AActor*> CNUs;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> ClassToFind;

};
