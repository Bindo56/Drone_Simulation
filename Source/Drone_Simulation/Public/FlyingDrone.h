#pragma once

#include "CoreMinimal.h"
#include "CustomActorDataBase.h"
#include "GameFramework/Pawn.h"
#include "MotionControllerComponent.h"
#include "DroneInfoWidget.h"
#include "Components/WidgetComponent.h"

#include "FlyingDrone.generated.h"

class AActorCustomData;

UCLASS()
class DRONE_SIMULATION_API AFlyingDrone : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFlyingDrone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Components
	UPROPERTY(VisibleAnywhere)
	USceneComponent* VROrigin;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	FVector BaseCameraLocation;
	bool bBaseLocationSet = false;

	UPROPERTY(VisibleAnywhere)
	class UMotionControllerComponent* LeftController;

	UPROPERTY(VisibleAnywhere)
	class UMotionControllerComponent* RightController;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DroneMesh;

	// Input variables
	FVector2D MoveInput;      // X = Forward/Backward, Y = Right/Left
	FVector2D RotateInput;    // X = LookUpDown, Y = Turn(Yaw)
	float VerticalInput;      // Ascend/Descend

	// Drone physics
	UPROPERTY(EditAnywhere, Category = "Drone Movement")
	float MoveAcceleration = 800.0f;

	UPROPERTY(EditAnywhere, Category = "Drone Movement")
	float MaxSpeed = 600.0f;

	UPROPERTY(EditAnywhere, Category = "Drone Movement")
	float Drag = 4.0f;

	UPROPERTY(EditAnywhere, Category = "Drone Movement")
	float RotateSpeed = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Drone Movement|Visuals")
	float TiltAngle = 15.0f;

	UPROPERTY(EditAnywhere, Category = "Drone Movement|Visuals")
	float TiltInterpSpeed = 4.0f;

	UPROPERTY(EditAnywhere, Category = "Drone Movement|Camera")
	float LookSensitivity = 2.0f;

	// Current velocity for movement
	FVector Velocity;

	ACustomActorDataBase* LastHitActor = nullptr;

	// Input functions
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUpDown(float Value);
	void MoveUp(float Value);
	void PerformInteraction();// const;
	void CloseInfoWidget();

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UDroneInfoWidget> InfoWidgetClass;

	/*UPROPERTY(VisibleAnywhere)
	UWidgetComponent* InfoWidgetComponent;*/
	
	UPROPERTY()
	UDroneInfoWidget* ActiveWidget = nullptr;
};