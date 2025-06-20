#include "Planets.h"

APlanets::APlanets()
{
  PrimaryActorTick.bCanEverTick = true;

  BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("BodyMesh");
  RootComponent = BodyMesh;

  Mass = 1.0f;
  Velocity = FVector::ZeroVector;
}

void APlanets::BeginPlay()
{
  Super::BeginPlay();

  // TODO: Initialiser la vélocité avec la vélocité initiale
  Velocity = InitialVelocity;
}

void APlanets::Tick(float DeltaTime)
{ 
  Super::Tick(DeltaTime);
  
}

void APlanets::AddAcceleration(const FVector& Acceleration, float DeltaTime)
{
  // TODO: Appliquer la formule Vélocité = Vélocité + A * DeltaT
  Velocity += Acceleration * DeltaTime;
}

void APlanets::UpdatePosition(float DeltaTime)
{
  // TODO: Appliquer la formule Position = Position + Vélocité * DeltaT
  FVector NewPosition = GetActorLocation() + (Velocity * DeltaTime);
  SetActorLocation(NewPosition);
}