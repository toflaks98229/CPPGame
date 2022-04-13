#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "CPlayerCamera.generated.h"

UCLASS()
class CPPGAME_API ACPlayerCamera : public ACharacter
{
	GENERATED_BODY()

private: //SceneComp
	UPROPERTY(EditDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly)
		class UCameraComponent* BP_Camera;

private:
	UPROPERTY(EditAnywhere)
		float CameraMoveSpeed = 30.0f;

	UPROPERTY(EditAnywhere)
		float CameraZoomSpeed = 1000.0f;

	UPROPERTY(EditAnywhere)
		float ZoomRangeMin = 300.0f;

	UPROPERTY(EditAnywhere)
		float ZoomRangeMax = 1000.0f;

	UPROPERTY(EditAnywhere)
		float CameraRotateSpeed = 30.0f;

public:
	FVector GetCameraPosition();

	//Move
public:
	void MoveLeft(float DeltaTime);
	void MoveRight(float DeltaTime);
	void MoveUp(float DeltaTime);
	void MoveDown(float DeltaTime);

	void MoveLeftAxis(float InAxis);
	void MoveRightAxis(float InAxis);
	void MoveUpAxis(float InAxis);
	void MoveDownAxis(float InAxis);

	float SetMoveSpeed(bool Inbool);
	FVector2D GetMousePosition();

	void RotateLeft(float InAxis);
	void RotateRight(float InAxis);

	void SetArmLength(float InAxis);

private:
	bool bZoom = true;

public:
	ACPlayerCamera();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
