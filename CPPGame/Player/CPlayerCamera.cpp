#include "CPlayerCamera.h"
#include "Global.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"

ACPlayerCamera::ACPlayerCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	//Create SceneComponent
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &BP_Camera, "Camera", SpringArm);

	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0, 0, 0));
	SpringArm->TargetArmLength = 200.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = false;

	GetCharacterMovement()->GravityScale = 0.0f;

	BP_Camera->bHiddenInGame = false;
}

void ACPlayerCamera::BeginPlay()
{
	Super::BeginPlay();
}

void ACPlayerCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Wheel", this, &ACPlayerCamera::SetArmLength);
	PlayerInputComponent->BindAxis("RotateLeft", this, &ACPlayerCamera::RotateLeft);
	PlayerInputComponent->BindAxis("RotateRight", this, &ACPlayerCamera::RotateRight);

	PlayerInputComponent->BindAxis("MoveUp", this, &ACPlayerCamera::MoveUpAxis);
	PlayerInputComponent->BindAxis("MoveDown", this, &ACPlayerCamera::MoveDownAxis);
	PlayerInputComponent->BindAxis("MoveLeft", this, &ACPlayerCamera::MoveLeftAxis);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayerCamera::MoveRightAxis);
}

void ACPlayerCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveLeft(DeltaTime);
	MoveRight(DeltaTime);
	MoveUp(DeltaTime);
	MoveDown(DeltaTime);
}

void ACPlayerCamera::MoveLeft(float DeltaTime)
{
	FVector2D position = GetMousePosition();
	position = position * UWidgetLayoutLibrary::GetViewportScale(GetWorld());
	float positionX;
	positionX = UKismetMathLibrary::NormalizeToRange(position.X, 0, UWidgetLayoutLibrary::GetViewportSize(GetWorld()).X * 0.02f);
	positionX = UKismetMathLibrary::Clamp(positionX, 0, 1);
	positionX = 1 - positionX;
	FVector AddVector;
	AddVector = GetActorRightVector() * SetMoveSpeed(false) * positionX * DeltaTime;
	//AddVector = FVector(SetMoveSpeed(false), 0, 0) * positionX;
	AddActorWorldOffset(AddVector);
}

void ACPlayerCamera::MoveRight(float DeltaTime)
{
	FVector2D position = GetMousePosition();
	position = position * UWidgetLayoutLibrary::GetViewportScale(GetWorld());
	float positionX;
	positionX = UKismetMathLibrary::NormalizeToRange(position.X, 0, UWidgetLayoutLibrary::GetViewportSize(GetWorld()).X * 0.98f);
	positionX = UKismetMathLibrary::Clamp(positionX, 0, 1);
	FVector AddVector;
	AddVector = GetActorRightVector() * SetMoveSpeed(true) * positionX * DeltaTime;
	//AddVector = FVector(SetMoveSpeed(true), 0, 0) * positionX;
	AddActorWorldOffset(AddVector);
}

void ACPlayerCamera::MoveUp(float DeltaTime)
{
	FVector2D position = GetMousePosition();
	position = position * UWidgetLayoutLibrary::GetViewportScale(GetWorld());
	float positionY;
	positionY = UKismetMathLibrary::NormalizeToRange(position.Y, 0, UWidgetLayoutLibrary::GetViewportSize(GetWorld()).Y * 0.02f);
	positionY = UKismetMathLibrary::Clamp(positionY, 0, 1);
	positionY = 1 - positionY;
	FVector AddVector;
	AddVector = GetActorForwardVector() * SetMoveSpeed(true) * positionY * DeltaTime;
	//AddVector = FVector(0, SetMoveSpeed(false), 0) * positionX;
	//AddActorWorldOffset(AddVector, false, false);
	AddActorWorldOffset(AddVector);
}

void ACPlayerCamera::MoveDown(float DeltaTime)
{
	FVector2D position = GetMousePosition();
	position = position * UWidgetLayoutLibrary::GetViewportScale(GetWorld());
	float positionY;
	GetActorForwardVector();
	positionY = UKismetMathLibrary::NormalizeToRange(position.Y, 0, UWidgetLayoutLibrary::GetViewportSize(GetWorld()).Y * 0.98f);
	positionY = UKismetMathLibrary::Clamp(positionY, 0, 1);
	FVector AddVector;
	AddVector = GetActorForwardVector() * SetMoveSpeed(false) * positionY * DeltaTime;
	//AddActorWorldOffset(AddVector, false, false);
	AddActorWorldOffset(AddVector);
}

FVector ACPlayerCamera::GetCameraPosition()
{
	FVector location =  GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();

	return location;
}


void ACPlayerCamera::MoveLeftAxis(float InAxis)
{
	FVector AddVector;
	AddVector = GetActorRightVector() * SetMoveSpeed(false) * InAxis * GetWorld()->GetDeltaSeconds();
	AddActorWorldOffset(AddVector);
}

void ACPlayerCamera::MoveRightAxis(float InAxis)
{
	FVector AddVector;
	AddVector = GetActorRightVector() * SetMoveSpeed(true) * InAxis * GetWorld()->GetDeltaSeconds();
	AddActorWorldOffset(AddVector);
}

void ACPlayerCamera::MoveUpAxis(float InAxis)
{
	FVector AddVector;
	AddVector = GetActorForwardVector() * SetMoveSpeed(true) * InAxis * GetWorld()->GetDeltaSeconds();
	AddActorWorldOffset(AddVector);
}

void ACPlayerCamera::MoveDownAxis(float InAxis)
{
	FVector AddVector;
	AddVector = GetActorForwardVector() * SetMoveSpeed(false) * InAxis * GetWorld()->GetDeltaSeconds();
	AddActorWorldOffset(AddVector);
}

float ACPlayerCamera::SetMoveSpeed(bool Inbool)
{
	float Speed;
	/*Speed =	UKismetMathLibrary::NormalizeToRange(SpringArm->TargetArmLength,1000,3000);
	Speed = Speed * 40;*/
	Speed = CameraMoveSpeed;

	if (Inbool == true) {}
	else if (Inbool == false)
		Speed = Speed * -1.0f;

	return Speed;
}

FVector2D ACPlayerCamera::GetMousePosition()
{
	return UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
}

void ACPlayerCamera::SetArmLength(float InAxis)
{
	if (InAxis < 0)
	{
		if (bZoom == false)
		{
			SpringArm->TargetArmLength -= (InAxis * CameraZoomSpeed * GetWorld()->GetDeltaSeconds());
			SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, ZoomRangeMin, ZoomRangeMax);
		}
		bZoom = false;
	}
	else if (InAxis > 0)
	{
		if (bZoom == true)
		{
			SpringArm->TargetArmLength -= (InAxis * CameraZoomSpeed * GetWorld()->GetDeltaSeconds());
			SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, ZoomRangeMin, ZoomRangeMax);
		}
		bZoom = true;
	}
}

void ACPlayerCamera::RotateLeft(float InAxis)
{
	FRotator roatator = FRotator(0, InAxis * CameraRotateSpeed * GetWorld()->GetDeltaSeconds(), 0);
	AddActorWorldRotation(roatator);
}

void ACPlayerCamera::RotateRight(float InAxis)
{
	FRotator roatator = FRotator(0, InAxis * -CameraRotateSpeed * GetWorld()->GetDeltaSeconds(), 0);
	AddActorWorldRotation(roatator);
}