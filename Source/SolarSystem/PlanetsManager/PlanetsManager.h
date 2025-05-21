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
    
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation")
  float TimeScale;

protected:
  virtual void BeginPlay() override;

public:
  virtual void Tick(float DeltaTime) override;
};
