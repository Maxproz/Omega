// Fill out your copyright notice in the Description page of Project Settings.


#include "Omega/Public/Objects/Props/OmegaMovingPlatform.h"

AOmegaMovingPlatform::AOmegaMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);
}

void AOmegaMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicatingMovement(true);

		OriginalPlatformLocation = GetActorLocation();
		OriginalTargetLocation = GetTransform().TransformPosition(TargetLocation);
	}
}

void AOmegaMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Actor Location is %s"), *GetActorLocation().ToString());
		//UE_LOG(LogTemp, Warning, TEXT("OriginalTargetLocation %s"), *OriginalTargetLocation.ToString());

		if (!bPlatformMovingBack)
		{
			//UE_LOG(LogTemp, Warning, TEXT("bPlatformMovingBack == false") );

			FVector PlatformLocationWorld = GetActorLocation();
			FVector TargetLocationWorld = OriginalTargetLocation;
			FVector Direction = (TargetLocationWorld - PlatformLocationWorld).GetSafeNormal();
			PlatformLocationWorld += Speed * Direction * DeltaTime;
			SetActorLocation(PlatformLocationWorld);

			if (FVector::Distance(GetActorLocation(), OriginalTargetLocation) <= 1.f)
			{
				bPlatformMovingBack = true;
			}
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("bPlatformMovingBack == True") );

			FVector PlatformLocationWorld = GetActorLocation();
			FVector TargetLocationWorld = OriginalPlatformLocation;
			FVector Direction = (TargetLocationWorld - PlatformLocationWorld).GetSafeNormal();
			PlatformLocationWorld += Speed * Direction * DeltaTime;
			SetActorLocation(PlatformLocationWorld);

			if (FVector::Distance(GetActorLocation(), OriginalPlatformLocation) <= 1.f)
			{
				bPlatformMovingBack = false;
			}
		}
	}
}
