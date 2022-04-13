#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CEquipment.generated.h"

UCLASS()
class CPPGAME_API ACEquipment : public AActor
{
	GENERATED_BODY()
	
public:	
	ACEquipment();
	

protected:
	virtual void BeginPlay() override;

public:
	void SetStaticMesh(UStaticMesh* staticMesh);

private:
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* StaticMesh;

};
