// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomActorDataBase.h"
#include "GameFramework/Actor.h"
#include "ActorCustomData.generated.h"

UCLASS()
class DRONE_SIMULATION_API AActorCustomData : public ACustomActorDataBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorCustomData();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeshComponent;

	virtual void SetHighlight(bool bEnable) override; 

};
