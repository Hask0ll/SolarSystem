#include "PlanetsManager.h"

#include "Kismet/GameplayStatics.h"

APlanetsManager::APlanetsManager()
{
  PrimaryActorTick.bCanEverTick = true;
  TimeScale = 1.0f;
  GravitationalConstant = 6740.0f;

  bShowOrbits = true;
  MaxOrbitPoints = 500;
  MinDistanceBetweenPoints = 10.0f;
  OrbitLineThickness = 2.0f;
  OrbitRecordInterval = 0.1f;
  OrbitRecordTimer = 0.0f;

  bShowOrbitPrediction = false;
  MaxPredictionPoints = 200;
  PredictionTimeStep = 0.5f;
}

void APlanetsManager::BeginPlay()
{
  Super::BeginPlay();
  
  TArray<AActor*> FoundActors;
  UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlanets::StaticClass(), FoundActors);
  
  for (AActor* Actor : FoundActors)
  {
      if (APlanets* Planet = Cast<APlanets>(Actor))
      {
          Planets.Add(Planet);
      }
  }
}

void APlanetsManager::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  
  float ScaledDeltaTime = DeltaTime * TimeScale;
  
  UpdateAllVelocities(ScaledDeltaTime);
  UpdateAllPositions(ScaledDeltaTime);
  
  if (bShowOrbits)
  {
    OrbitRecordTimer += DeltaTime;
    if (OrbitRecordTimer >= OrbitRecordInterval)
    {
      RecordOrbitPoints();
      OrbitRecordTimer = 0.0f;
    }
    
    DrawOrbits();
  }
  if (bShowOrbitPrediction)
  {
    CalculateOrbitPrediction();
    DrawOrbitPrediction();
  }
}

FVector APlanetsManager::CalculateGravitationalForce(APlanets* Planet1, APlanets* Planet2)
{
  FVector Direction = Planet2->GetActorLocation() - Planet1->GetActorLocation();
  float Distance = Direction.Size();
  
  if (Distance < 0.1f) {
    return FVector::ZeroVector;
  }
  
  float ForceMagnitude = GravitationalConstant * (Planet1->Mass * Planet2->Mass) / (Distance * Distance);
  
  FVector ForceDirection = Direction.GetSafeNormal();
  
  return ForceMagnitude * ForceDirection;
}

FVector APlanetsManager::CalculateTotalForceOnPlanet(APlanets* Planet)
{
  FVector TotalForce = FVector::ZeroVector;
  
  for (APlanets* OtherPlanet : Planets)
  {
      if (OtherPlanet != Planet)
      {
        TotalForce += CalculateGravitationalForce(Planet, OtherPlanet);
      }
  }
  
  return TotalForce;
}

void APlanetsManager::UpdateAllVelocities(float DeltaTime)
{
  for (APlanets* Planet : Planets) {
    FVector TotalForce = CalculateTotalForceOnPlanet(Planet);
    
    FVector Acceleration = TotalForce / Planet->Mass;
    
    Planet->AddAcceleration(Acceleration, DeltaTime);
  }
}

void APlanetsManager::UpdateAllPositions(float DeltaTime)
{
  for (APlanets* Planet : Planets) {
    Planet->UpdatePosition(DeltaTime);
  }
}

void APlanetsManager::RecordOrbitPoints()
{
  for (APlanets* Planet : Planets)
  {
    FVector CurrentPosition = Planet->GetActorLocation();
    
    if (!OrbitHistory.Contains(Planet))
    {
      OrbitHistory.Add(Planet, TArray<FVector>());
    }
    
    TArray<FVector>& History = OrbitHistory[Planet];
    
    bool bShouldAddPoint = true;
    
    if (History.Num() > 0)
    {
      float DistanceFromLast = FVector::Dist(CurrentPosition, History.Last());
      if (DistanceFromLast < MinDistanceBetweenPoints)
      {
        bShouldAddPoint = false;
      }
    }
    
    if (bShouldAddPoint)
    {
      History.Add(CurrentPosition);
      
      if (History.Num() > MaxOrbitPoints)
      {
        History.RemoveAt(0);
      }
    }
  }
}

void APlanetsManager::DrawOrbits()
{
  for (auto& Pair : OrbitHistory)
  {
    APlanets* Planet = Pair.Key;
    const TArray<FVector>& History = Pair.Value;
    
    if (History.Num() < 2)
      continue;
    
    FColor OrbitColor = FColor::White;
    int32 PlanetIndex = Planets.IndexOfByKey(Planet);
    switch (PlanetIndex)
    {
    case 0: OrbitColor = FColor::Red; break;
    case 1: OrbitColor = FColor::Blue; break;
    case 2: OrbitColor = FColor::Green; break;
    case 3: OrbitColor = FColor::Yellow; break;
    default: OrbitColor = FColor::White; break;
    }
    
    for (int32 i = 0; i < History.Num() - 1; i++)
    {
      DrawDebugLine(
        GetWorld(),
        History[i],
        History[i + 1],
        OrbitColor,
        false,
        -1.0f,
        0,
        OrbitLineThickness
      );
    }
    
    DrawDebugSphere(
      GetWorld(),
      Planet->GetActorLocation(),
      10.0f,
      8,
      OrbitColor,
      false,
      -1.0f
    );
  }
}

void APlanetsManager::CalculateOrbitPrediction()
{
  OrbitPrediction.Empty();
  
  TMap<APlanets*, FVector> TempPositions;
  TMap<APlanets*, FVector> TempVelocities;
  
  for (APlanets* Planet : Planets)
  {
    TempPositions.Add(Planet, Planet->GetActorLocation());
    TempVelocities.Add(Planet, Planet->GetVelocity());
    OrbitPrediction.Add(Planet, TArray<FVector>());
  }
  
  for (int32 Step = 0; Step < MaxPredictionPoints; Step++)
  {
    TMap<APlanets*, FVector> Forces;
    for (APlanets* Planet : Planets)
    {
      FVector TotalForce = FVector::ZeroVector;
      
      for (APlanets* OtherPlanet : Planets)
      {
        if (OtherPlanet != Planet)
        {
          FVector Direction = TempPositions[OtherPlanet] - TempPositions[Planet];
          float Distance = Direction.Size();
          
          if (Distance > 0.1f)
          {
            float ForceMagnitude = GravitationalConstant * (Planet->Mass * OtherPlanet->Mass) / (Distance * Distance);
            FVector ForceDirection = Direction.GetSafeNormal();
            TotalForce += ForceMagnitude * ForceDirection;
          }
        }
      }
      Forces.Add(Planet, TotalForce);
    }
    
    for (APlanets* Planet : Planets)
    {
      FVector Acceleration = Forces[Planet] / Planet->Mass;
      TempVelocities[Planet] += Acceleration * PredictionTimeStep;
      TempPositions[Planet] += TempVelocities[Planet] * PredictionTimeStep;
      
      OrbitPrediction[Planet].Add(TempPositions[Planet]);
    }
  }
}

void APlanetsManager::DrawOrbitPrediction()
{
  for (auto& Pair : OrbitPrediction)
  {
    APlanets* Planet = Pair.Key;
    const TArray<FVector>& Prediction = Pair.Value;
    
    if (Prediction.Num() < 2)
      continue;
    
    FColor PredictionColor = FColor::Orange;
    int32 PlanetIndex = Planets.IndexOfByKey(Planet);
    switch (PlanetIndex)
    {
    case 0: PredictionColor = FColor(255, 100, 100); break; // Rouge clair
    case 1: PredictionColor = FColor(100, 100, 255); break; // Bleu clair
    case 2: PredictionColor = FColor(100, 255, 100); break; // Vert clair
    case 3: PredictionColor = FColor(255, 255, 100); break; // Jaune clair
    default: PredictionColor = FColor::Orange; break;
    }
    
    for (int32 i = 0; i < Prediction.Num() - 1; i++)
    {
      DrawDebugLine(
        GetWorld(),
        Prediction[i],
        Prediction[i + 1],
        PredictionColor,
        false,
        -1.0f,
        0,
        OrbitLineThickness * 0.5f
      );
    }
  }
}