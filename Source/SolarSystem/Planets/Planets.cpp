#include "Planets.h"

APlanets::APlanets() {
  PrimaryActorTick.bCanEverTick = true;

  BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("BodyMesh");
  RootComponent = BodyMesh;

  Mass = 1.0f;
  Velocity = FVector::ZeroVector;
}

void APlanets::BeginPlay() {
  Super::BeginPlay();

  Velocity = InitialVelocity;
}

void APlanets::Tick(float DeltaTime) { Super::Tick(DeltaTime); }
