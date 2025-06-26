#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SolarSystem/Planets/Planets.h"

#include "PlanetsManager.generated.h"

UCLASS()
class SOLARSYSTEM_API APlanetsManager : public AActor {
  GENERATED_BODY()

public:
  APlanetsManager();

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Solar System")
  TArray<APlanets*> Planets;
    
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation", meta = (ClampMin = "0.1", ClampMax = "10.0"))
  float TimeScale;
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
  float GravitationalConstant;
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit Visualization")
  bool bShowOrbits;
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit Visualization", meta = (ClampMin = "10", ClampMax = "5000"))
  int32 MaxOrbitPoints;
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit Visualization")
  float MinDistanceBetweenPoints;
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit Visualization")
  float OrbitLineThickness;
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit Visualization")
  float OrbitRecordInterval;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit Preview")
  bool bShowOrbitPrediction;
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit Preview", meta = (ClampMin = "10", ClampMax = "1000"))
  int32 MaxPredictionPoints;
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit Preview", meta = (ClampMin = "0.1", ClampMax = "10.0"))
  float PredictionTimeStep;

protected:
  virtual void BeginPlay() override;

public:
  virtual void Tick(float DeltaTime) override;

private:
  FVector CalculateGravitationalForce(APlanets* Planet1, APlanets* Planet2);
  FVector CalculateTotalForceOnPlanet(APlanets* Planet);
  void UpdateAllVelocities(float DeltaTime);
  void UpdateAllPositions(float DeltaTime);
  
  void RecordOrbitPoints();
  void DrawOrbits();

  void CalculateOrbitPrediction();
  void DrawOrbitPrediction();
  
  TMap<APlanets*, TArray<FVector>> OrbitHistory;
  TMap<APlanets*, TArray<FVector>> OrbitPrediction;
  float OrbitRecordTimer;
};