// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorCustomData4.h"

// Sets default values
AActorCustomData4::AActorCustomData4()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	//static ConstructorHelpers::FObjectFinder<UMaterialInterface> OverlayMaterial(TEXT("/Game/Material/OutlineShader.OutlineShader"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	
	//  Enable collision for line tracing (visibility channel)
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetCollisionObjectType(ECC_WorldStatic);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	//  Optional: Enable hit events if needed later
	MeshComponent->SetGenerateOverlapEvents(true);

	// Set Cube Mesh from Engine Basic Shapes
	
	if (CubeMesh.Succeeded())
	{
		MeshComponent->SetStaticMesh(CubeMesh.Object);
		MeshComponent->SetRelativeScale3D(FVector(1.f)); // Adjust size if necessary
	}

	ElementName = "Sphere";
	MaterialType = "Blue Material";
	InspectionNotes = "Circle";

}

// Called when the game starts or when spawned
void AActorCustomData4::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ActorCustomData4 BeginPlay Running"));

	

	
	MeshComponent->SetRenderCustomDepth(false); // Start off by default
	MeshComponent->SetCustomDepthStencilValue(1); // Or 2, 3 if using multiple colors
}

// Called every frame
void AActorCustomData4::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Fill out your copyright notice in the Description page of Project Settings.
void AActorCustomData4::SetHighlight(bool bEnable)
{
	UE_LOG(LogTemp, Warning, TEXT("Changing material highlight state..."));

	FString Path = "/Game/Material/OutlineShader.OutlineShader";
	UMaterialInterface* OverlayMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, *Path));
	UMaterialInterface* DefaultMaterial = MeshComponent->GetMaterial(0);

	if (!OverlayMaterial)
	{
		UE_LOG(LogTemp, Error, TEXT("Overlay Material FAILED to Load: %s"), *Path);
		return;
	}

	if (bEnable)
	{
		MeshComponent->SetOverlayMaterial(OverlayMaterial);
		UE_LOG(LogTemp, Warning, TEXT("Overlay Material Applied"));
	}
	else
	{
		MeshComponent->SetOverlayMaterial(nullptr);  // Remove overlay material
		UE_LOG(LogTemp, Warning, TEXT("Overlay Material Removed"));
	}
	//MeshComponent->SetRenderCustomDepth(bEnable);
}
