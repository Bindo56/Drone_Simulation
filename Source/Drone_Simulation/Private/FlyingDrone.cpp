#include "FlyingDrone.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "MotionControllerComponent.h"
//#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "ActorCustomData.h"
#include "ActorCustomData2.h"
#include "ActorCustomData3.h"
#include "ActorCustomData4.h"
#include "ActorCustomData5.h"
#include "DroneInfoWidget.h"
#include "Components/WidgetComponent.h"

//#include "Kismet/GameplayStatics.h"

// Sets default values
AFlyingDrone::AFlyingDrone()
{
    PrimaryActorTick.bCanEverTick = true;

    VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));
    RootComponent = VROrigin;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(VROrigin);
    Camera->SetRelativeLocation(FVector(0.f, 0.f, -25.f)); 

    LeftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftController"));
    LeftController->SetupAttachment(VROrigin);
    LeftController->SetTrackingSource(EControllerHand::Left);

    RightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightController"));
    RightController->SetupAttachment(VROrigin);
    RightController->SetTrackingSource(EControllerHand::Right);

    DroneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DroneMesh"));
    DroneMesh->SetupAttachment(VROrigin);

    // Optional: Set default mesh in code (if desired)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/GEO_Main_Body.GEO_Main_Body"));  //"/Engine/BasicShapes/Cube.Cube"
   // static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Game/Materials/MyMaterial.MyMaterial"));
    if (MeshAsset.Succeeded())
    {
        DroneMesh->SetStaticMesh(MeshAsset.Object);
        DroneMesh->SetupAttachment(RootComponent);
        DroneMesh->SetRelativeScale3D(FVector(0.7f));
    }

    static ConstructorHelpers::FClassFinder<UDroneInfoWidget> WidgetBPClass(TEXT("/Game/Widget/MyDroneInfoWidget"));
    if (WidgetBPClass.Succeeded())
    {
        InfoWidgetClass = WidgetBPClass.Class;
        UE_LOG(LogTemp, Warning, TEXT("InfoWidgetClass successfully loaded."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find InfoWidgetClass! Check the path."));
    }

    /*InfoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InfoWidgetComponent"));
    InfoWidgetComponent->SetupAttachment(VROrigin);
    InfoWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
    InfoWidgetComponent->SetDrawSize(FVector2D(500, 300));
    InfoWidgetComponent->SetRelativeLocation(FVector(200.f, 0.f, 50.f));
    InfoWidgetComponent->SetVisibility(false);*/

    
    /*if (MaterialAsset.Succeeded())
    {
        DroneMesh->SetMaterial(0, MaterialAsset.Object); 
    }*/

    // Tuning parameters
    MoveAcceleration = 4000.0f;
    MaxSpeed = 3000.0f;
    Drag = 2.5f;
    RotateSpeed = 90.0f;
    TiltAngle = 15.0f;
    TiltInterpSpeed = 4.0f;
    LookSensitivity = 5.0f;

    MoveInput = FVector2D::ZeroVector;
    RotateInput = FVector2D::ZeroVector;
    VerticalInput = 0.0f;
}

void AFlyingDrone::BeginPlay()
{
    Super::BeginPlay();
    
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->Possess(this);   //  Make sure this drone is possessed
        EnableInput(PC);     //  Explicitly enable input
        PC->SetInputMode(FInputModeGameOnly());  //  Set correct input mode
    }

    /*if (InfoWidgetClass)
    {
        InfoWidgetComponent->SetWidgetClass(InfoWidgetClass);
    }*/
}

void AFlyingDrone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Calculate desired acceleration direction
    FVector DesiredDirection = FVector::ZeroVector;
    DesiredDirection += GetActorForwardVector() * MoveInput.X;  
    DesiredDirection += GetActorRightVector() * MoveInput.Y;   
    DesiredDirection += FVector::UpVector * VerticalInput;
    //DesiredDirection.Normalize();

    if (!DesiredDirection.IsNearlyZero())
    {
        DesiredDirection.Normalize();
    }

    if (!bBaseLocationSet)
    {
        BaseCameraLocation = Camera->GetRelativeLocation();
        bBaseLocationSet = true;
    }

    float Time = GetWorld()->TimeSeconds;

    FVector Oscillation;
    Oscillation.X = FMath::Sin(Time * 1.2f) * 3.0f;  // Gentle forward/back sway
    Oscillation.Y = FMath::Cos(Time * 0.9f) * 2.0f;  // Gentle side-to-side
    Oscillation.Z = FMath::Sin(Time * 1.5f) * 1.0f;  // Slight up/down float

    // Reset camera to base location + oscillation ? prevents drift over time
    Camera->SetRelativeLocation(BaseCameraLocation + Oscillation);

    if (MoveInput.IsNearlyZero() && FMath::IsNearlyZero(VerticalInput, 0.01f))
    {
        Velocity = FMath::VInterpTo(Velocity, FVector::ZeroVector, DeltaTime, 1.5f);
    }


    // Apply acceleration to velocity
    Velocity += DesiredDirection * MoveAcceleration * DeltaTime;

    // Apply drag for damping
    Velocity = Velocity - (Velocity * Drag * DeltaTime);
    Velocity = Velocity.GetClampedToMaxSize(MaxSpeed);

    // Move the actor
    AddActorWorldOffset(Velocity * DeltaTime, true);

    // Smooth Yaw Rotation (turning)
    FRotator NewRotation = GetActorRotation();
    NewRotation.Yaw += RotateInput.Y * RotateSpeed * DeltaTime;
    SetActorRotation(NewRotation);

    // Tilt the drone based on input direction for visual feedback
    FRotator TargetTilt = FRotator();
    TargetTilt.Pitch = MoveInput.X * -TiltAngle;
    TargetTilt.Roll = MoveInput.Y * -TiltAngle;

    FRotator NewTilt = FMath::RInterpTo(DroneMesh->GetRelativeRotation(), TargetTilt, DeltaTime, TiltInterpSpeed);
    DroneMesh->SetRelativeRotation(NewTilt);
}

void AFlyingDrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AFlyingDrone::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AFlyingDrone::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AFlyingDrone::Turn);
    PlayerInputComponent->BindAxis(TEXT("LookUpDown"), this, &AFlyingDrone::LookUpDown);
    PlayerInputComponent->BindAxis(TEXT("MoveUp"), this, &AFlyingDrone::MoveUp);
    PlayerInputComponent->BindAction(TEXT("Check"), IE_Pressed, this, &AFlyingDrone::PerformInteraction);
    PlayerInputComponent->BindAction(TEXT("Close"), IE_Pressed, this, &AFlyingDrone::CloseInfoWidget);

}

void AFlyingDrone::MoveForward(float Value)
{
    MoveInput.X = Value;
}

void AFlyingDrone::MoveRight(float Value)
{
    MoveInput.Y = Value;
}

void AFlyingDrone::Turn(float Value)
{
    RotateInput.Y = Value;
}

void AFlyingDrone::LookUpDown(float Value)
{
    if (!Camera) return;

    FRotator CurrentRotation = Camera->GetRelativeRotation();
    CurrentRotation.Pitch = FMath::Clamp(CurrentRotation.Pitch + (Value * LookSensitivity), -85.0f, 85.0f);

    Camera->SetRelativeRotation(CurrentRotation);
}

void AFlyingDrone::MoveUp(float Value)
{
    VerticalInput = Value;
}

void AFlyingDrone::PerformInteraction() //const
{
    UE_LOG(LogTemp, Warning, TEXT("PerformInteraction() called"));

    FVector Start = Camera->GetComponentLocation();         // Start FROM the camera position (world space)
    FVector Direction = Camera->GetForwardVector();         //  Direction → camera's *forward* vector
    FVector End = Start + (Direction * 400.f);             //  5000 units forward

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);


    DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.f);

    if (bHit && Hit.GetActor())
    {

        ACustomActorDataBase* Info = Cast<ACustomActorDataBase>(Hit.GetActor()); 

        LastHitActor = Info;
        if (Info)
        {
            if (!ActiveWidget && InfoWidgetClass)
            {
                ActiveWidget = CreateWidget<UDroneInfoWidget>(GetWorld(), InfoWidgetClass);
                if (ActiveWidget)
                {
                    ActiveWidget->AddToViewport();
                }
            }

            if (ActiveWidget)
            {
                Info->SetHighlight(true);
                ActiveWidget->UpdateInfo(Info->ElementName, Info->MaterialType, Info->InspectionNotes);
            }
        }
    }
}

void AFlyingDrone::CloseInfoWidget()
{
    if (ActiveWidget && ActiveWidget->IsInViewport())
    {
        UE_LOG(LogTemp, Warning, TEXT("Info Widget closed."));
        ActiveWidget->RemoveFromParent();
        if (LastHitActor)
        {
         LastHitActor->SetHighlight(false);
        }
            
        ActiveWidget = nullptr;
    }
    else
    {
        if (!InfoWidgetClass) return;

        ActiveWidget = CreateWidget<UDroneInfoWidget>(GetWorld(), InfoWidgetClass);
        if (ActiveWidget)
        {
            FVector WorldLocation = Camera->GetComponentLocation() + (Camera->GetForwardVector() * 200.f);
            FVector2D ScreenPosition;
            APlayerController* PC = GetWorld()->GetFirstPlayerController();

            if (PC && PC->ProjectWorldLocationToScreen(WorldLocation, ScreenPosition))
            {
                FWidgetTransform WidgetTransform;
                WidgetTransform.Translation = ScreenPosition;
                ActiveWidget->SetRenderTransform(WidgetTransform);
                ActiveWidget->AddToViewport();

                UE_LOG(LogTemp, Warning, TEXT("Info Widget opened at screen position X:%f Y:%f"), ScreenPosition.X, ScreenPosition.Y);
            }
            //ActiveWidget->AddToViewport();
            UE_LOG(LogTemp, Warning, TEXT("Info Widget opened."));
        }
    }
}



