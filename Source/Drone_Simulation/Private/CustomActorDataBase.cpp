// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomActorDataBase.h"

// Sets default values
ACustomActorDataBase::ACustomActorDataBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ACustomActorDataBase::SetHighlight(bool bEnable)
{
	
}

// Called when the game starts or when spawned
void ACustomActorDataBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomActorDataBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

