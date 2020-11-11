// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "OmegaMovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class OMEGA_API AOmegaMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
public:
	AOmegaMovingPlatform();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	float Speed = 10;

	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	FVector TargetLocation;

protected:

private:
	bool bPlatformMovingBack = false;

	FVector OriginalPlatformLocation;
	FVector OriginalTargetLocation;

};
