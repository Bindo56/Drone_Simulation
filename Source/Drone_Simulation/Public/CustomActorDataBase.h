// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomActorDataBase.generated.h"

UCLASS()
class DRONE_SIMULATION_API ACustomActorDataBase : public AActor
{
	GENERATED_BODY()
public:
	ACustomActorDataBase();

	// Common info properties for ALL custom data actors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Info")
	FString ElementName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Info")
	FString MaterialType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Info")
	FString InspectionNotes;

	// Highlight function to be overridden if needed
	UFUNCTION(BlueprintCallable, Category = "Visual")
	virtual void SetHighlight(bool bEnable);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
