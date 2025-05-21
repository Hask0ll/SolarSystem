// Fill out your copyright notice in the Description page of Project Settings.

#include "PlanetsManager.h"

// Sets default values
APlanetsManager::APlanetsManager()
{
  PrimaryActorTick.bCanEverTick = true;
  TimeScale = 1.0f;
}

void APlanetsManager::BeginPlay()
{
  Super::BeginPlay();
  
}

void APlanetsManager::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}



