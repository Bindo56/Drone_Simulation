// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomActorDataBase.h"
#include "GameFramework/Actor.h"
#include "ActorCustomData5.generated.h"

UCLASS()
class DRONE_SIMULATION_API AActorCustomData5 : public ACustomActorDataBase
{
	GENERATED_BODY()
	
public:	
	AActorCustomData5();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeshComponent;

	virtual void SetHighlight(bool bEnable) override;  // override from base if needed
};
