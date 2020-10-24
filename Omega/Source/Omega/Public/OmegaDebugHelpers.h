// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include "OmegaDebugHelpers.generated.h"

USTRUCT()
struct FOmegaPoint
{
public:
	GENERATED_BODY()

	FOmegaPoint() {}

	float X;
	float Y;
	float Z;
};

// TODO: Draw Debug Triangle 
// You just need 3 points and a color.

// maybe use this as an actor I attach to a player to draw the debug? IDK

UCLASS()
class AOmegaCircle : public AActor
{
public:
	GENERATED_BODY()

	AOmegaCircle()
	{

	}

	void Initialize(const float& InRadius)
	{
		// If just radius is given set circle to spawn at the origin.
		Origin.X = 0.0f;
		Origin.Y = 0.0f;
		Origin.Z = 0.0f;
		SetActorLocation(FVector(Origin.X, Origin.Y, Origin.Z));
		Radius = InRadius;
		Circumference = 2 * GetPI() * Radius;
		Diameter = 2 * Radius;
	}

	FOmegaPoint Origin;

	// Radius == Circumference / (2 * PI)
	float Radius;

	// Diameter is also called cross measure;
	// Diameter == Circumference / PI or 2 * Radius
	float Diameter;

	// Circumference == 2 * PI * Radius
	float Circumference;

	// TODO: Function that draws debug arc for 2 vectors given an input location
	// TODO: Function that draws debug sector for 2 vectors and given an input location
	// TODO: Degree to radian helper function (Test Convert 120 degrees to radians and test)
	// TODO: Radian to degree helper function (test convert pi/4 radians to degrees and test)

	float GetCircumference() const { return Circumference; }


	// 1 degree is 1/360th
	// Unknown angles usually named theta


private:
	// Circumference (outer edge full loop) / Diameter (cut in half measure) == PI 
	// No matter how big or small the circle is.
	float GetPI() const { return 3.14159; }

};

 // Define that controls debug drawing
#define ENABLE_OMEGA_DRAW_DEBUGG  !(UE_BUILD_SHIPPING || UE_BUILD_TEST)

#if ENABLE_OMEGA_DRAW_DEBUGG



// 	// no debug line drawing on dedicated server
/*if (GEngine->GetNetMode(InWorld) != NM_DedicatedServer)*/



#endif // ENABLE_OMEGA_DRAW_DEBUGG