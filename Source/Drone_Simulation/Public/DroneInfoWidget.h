// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DroneInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class DRONE_SIMULATION_API UDroneInfoWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
	UFUNCTION(BlueprintImplementableEvent, Category = "DroneInfo")
	void UpdateInfo(const FString& ElementName, const FString& MaterialType, const FString& InspectionNotes);
	
};


