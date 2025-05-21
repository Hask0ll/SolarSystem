#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Planets.generated.h"

UCLASS()
class SOLARSYSTEM_API APlanets : public AActor {
  GENERATED_BODY()

public:
  APlanets();

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
  float Mass;
    
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
  FVector InitialVelocity;
    
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visualization")
  UStaticMeshComponent* BodyMesh;

protected:
  virtual void BeginPlay() override;

public:
  virtual void Tick(float DeltaTime) override;

private:
  FVector Velocity;
};
