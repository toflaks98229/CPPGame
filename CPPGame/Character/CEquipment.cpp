#include "CEquipment.h"
#include "Global.h"

ACEquipment::ACEquipment()
{
	PrimaryActorTick.bCanEverTick = false;

	CHelpers::CreateComponent(this, &StaticMesh, "StaticMesh");

	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ACEquipment::OnOverlapBegin);

	StaticMesh->SetCastShadow(false);
}

void ACEquipment::SetStaticMesh(UStaticMesh* staticMesh)
{
	ACEquipment();

	StaticMesh->SetStaticMesh(staticMesh);
}

void ACEquipment::BeginPlay()
{
	Super::BeginPlay();

	StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Overlap);
}

void ACEquipment::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}