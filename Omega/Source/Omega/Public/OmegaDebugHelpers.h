// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include "OmegaDebugHelpers.generated.h"

// Notes: Common type of vector is position vectors that have a starting point at the origin. GetActorLocation() probably uses origin as its starting point
// Notes: Magnitude of a 3d vector is just using the second point and first point and getting sqrt using those two points so  Sqrt((x2 - x1)^2 + (y2-y1)^2 + (z2-z1)^2)
// Notes: distance between 2 vectors is just the magnitude of the vector between 2 points.
// FORCEINLINE float FVector::Dist(const FVector &V1, const FVector &V2)
// {
// 	return FMath::Sqrt(FVector::DistSquared(V1, V2));
// }
// FORCEINLINE float FVector::DistSquared(const FVector &V1, const FVector &V2)
// {
// 	return FMath::Square(V2.X-V1.X) + FMath::Square(V2.Y-V1.Y) + FMath::Square(V2.Z-V1.Z);
// }

USTRUCT()
struct FOmegaPoint
{
public:
	GENERATED_BODY()

	FOmegaPoint() 
	: X(0.f), Y(0.f), Z(0.f)
	{}

	float X;
	float Y;
	float Z;
};

// TODO: DrawDebugVector class?
UCLASS()
class AOmegaDebugVector : public AActor
{
public:
	GENERATED_BODY()
	
	AOmegaDebugVector() 
		: Magnitude(0.f)
	{}

	AOmegaDebugVector(const float& InX, const float& InY, const float& InZ, const float& InMagnitude)
	{
		Direction.X = InX;
		Direction.Y = InY;
		Direction.Z = InZ;

		Magnitude = InMagnitude;
	}

	AOmegaDebugVector(const FOmegaPoint& InPoint, const float& InMagnitude)
	{
		Direction.X = InPoint.X;
		Direction.Y = InPoint.Y;
		Direction.Z = InPoint.Z;

		Magnitude = InMagnitude;
	}

protected:

private:
	FOmegaPoint Direction; 
	float Magnitude;
};


// TODO: Draw Debug Triangle 
// You just need 3 points and a color.
UCLASS()
class AOmegaDebugTriangle : public AActor
{
public:
	GENERATED_BODY()

	AOmegaDebugTriangle() {}

	AOmegaDebugTriangle(const FOmegaPoint& FirstPoint, const FOmegaPoint& SecondPoint, const FOmegaPoint& ThirdPoint, const FColor InColor)
	{
		PointA = FirstPoint;
		PointB = SecondPoint;
		PointC = ThirdPoint;

		LineColor = InColor;
	}
protected:

private:
	FOmegaPoint PointA;
	FOmegaPoint PointB;
	FOmegaPoint PointC;

	FColor LineColor;
};


// VECTORS
// Normalize turns a vector into a unit vector. Makes it easier to work with.
// Divide each component (x,y,z) by the vectors magnitude.
// only true when vector != zero vector at origin/zero velocity standing still

// Dot product is a percentage of similarity / dissimilarity between two vectors.
// The dot product of two vectors can be used to find the angle between two vectors.
// DotProduct(a,b) = magnitude(a)*magnitude(b) * cos(theta)
// if you converted to unit vectors the divisor cancels out ---- theta = acos(DotProduct(a,b) / (Magnitude(a) * Magnitude(b)))

// Using DotProduct to see if player is behind enemy.
// Get Enemy position 
// FVector EnemyPosition = Enemy->GetActorLocation();
// FVector EnemyForwardVector = Enemy->GetForwardVector(); == They are facing forward on the x axis FVector(1, 0, 0);

// Enemy also knows the vector to the player
// FVector PlayerPosition = Player->GetActorLocation();
// FVector EnemyToPlayerVector = EnemyPosition - PlayerPosition; // FVector(-3, 4, 0)
// Also Enemy vision CONE is 90 degrees total so 45 degrees to either side of our forward vector // NEXT TODO: How do I calculate these angles

// So using this information.
// You know which direction the enemy is facing, know the enemys vision cone angles
// You can use the dot product formula to get the angle of the player compared to the enemys forward vector and if that angle is within their vision cone angle they can be fired on 
// You an also use this by checking the players angle, the closer the players angle is to 180 the more the player is directly behind the enemy.


// Cross product of 2 vectors is the vector that is perpendicular to the 2 vectors.
// This vector is the "normal" of those 2 vectors.

// For ue4 example - the normal vector you would get from the crossproduct(x, y); would be the the normal to the x y plane

// CrossProduct(x, y) == CrossProduct(-y, x) 
// If you want to point your normal vector in the opposite direction change the order of the cross product variables.


// REFLECTION
// To do reflection 
// #1 Get the normal of a surface(plane) using cross product
// #2 Pass in the incoming vector and normal vector into FVector.Reflect

// TASK: Find more uses for project onto plane.
// given example was showing how you can prevent a car from stopping completely when it hits a railing
// Output vector = FVector::ProjectOntoPlane(input vector, normal vector)




UCLASS()
class AOmegaDebugCircle : public AActor
{
public:
	GENERATED_BODY()

	AOmegaDebugCircle()
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