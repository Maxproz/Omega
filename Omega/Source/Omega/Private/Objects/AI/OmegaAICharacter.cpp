// Fill out your copyright notice in the Description page of Project Settings.


#include "Omega/Public/Objects/AI/OmegaAICharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/WidgetComponent.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "../OmegaCharacter.h"

// Sets default values
AOmegaAICharacter::AOmegaAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = GetCapsuleComponent();

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

// Called when the game starts or when spawned
void AOmegaAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOmegaAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//DrawDebugTurretAim(GetActorLocation());

	DrawDebugAimCone();
}

// Called to bind functionality to input
void AOmegaAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AOmegaAICharacter::DrawDebugTurretAim(const FVector& StartLocation)
{
	const FVector TurretStartLocation = StartLocation;
	//UE_LOG(LogTemp, Warning, TEXT("TurretStartLocation: %s "), *TurretStartLocation.ToString());

	const FVector ForwardVector = GetActorForwardVector();
	//UE_LOG(LogTemp, Warning, TEXT("ForwardVector: %s "), *ForwardVector.ToString());

	const float MaxShotDistanceForward = 500; // unreal units

	// This should be something like a vector of 50, 0, 0
	const FVector ForwardShotVector = TurretStartLocation + (ForwardVector * MaxShotDistanceForward);

	//UE_LOG(LogTemp, Warning, TEXT("ForwardShotVector: %s "), *ForwardShotVector.ToString());


	// lets say we can only aim down 30 degrees from our forward vector
	// how do we get the length of our hypotenuse

	// c^2 = a^2 + b^2
	// Cos(30) = 500/H  == H * cos(30) = 50      == h == 50/cos(30)
	const float HypotenuseLength = MaxShotDistanceForward / (UKismetMathLibrary::Cos(UKismetMathLibrary::DegreesToRadians(30.f)));

	const float OppositeSideLength = UKismetMathLibrary::Sqrt((UKismetMathLibrary::Square(HypotenuseLength) - UKismetMathLibrary::Square(MaxShotDistanceForward)));

	// I have the lengths of the sides now I need to get the direction of the vectors. Could I have gotten this earlieR?
	FVector OppositeSideVector = ForwardShotVector - FVector(0, 0, OppositeSideLength);

	// FVector HypotenuseVector = TurretStartLocation + HypotenuseLength;

	DrawDebugLine(GetWorld(), TurretStartLocation, ForwardShotVector, FColor::Red, true, -1.0f, (uint8)'\000', 2);
	DrawDebugLine(GetWorld(), ForwardShotVector, OppositeSideVector, FColor::Red, true, -1.0f, (uint8)'\000', 2);
	DrawDebugLine(GetWorld(), OppositeSideVector, StartLocation, FColor::Red, true, -1.0f, (uint8)'\000', 2);
}

void AOmegaAICharacter::DrawDebugAimCone()
{
	const FVector ActorLocation = GetActorLocation();
	
	AOmegaCharacter* NearestPlayer = nullptr;

	float LastClosestDistance = 0.f;
	bool FoundOneCharacter = false;
	for (TActorIterator<AOmegaCharacter> OmegaCharacterIter(GetWorld()); OmegaCharacterIter; ++OmegaCharacterIter)
	{
		float DistanceToUs = FVector::Distance(ActorLocation, (*(*OmegaCharacterIter)).GetActorLocation());
		
		if (!FoundOneCharacter)
		{
			LastClosestDistance = DistanceToUs;
			NearestPlayer = *OmegaCharacterIter;
		}

		FoundOneCharacter = true;

		if (DistanceToUs < LastClosestDistance && LastClosestDistance != 0.f)
		{
			LastClosestDistance = DistanceToUs;
			NearestPlayer = *OmegaCharacterIter;
		}
	}

	if (NearestPlayer == nullptr)
	{
		return;
	}

	// Now we have a safe pointer to our closest player
	//UE_LOG(LogTemp, Warning, TEXT("Closest Player Location: %s "), *NearestPlayer->GetActorLocation().ToString());

	const FVector VectorToNearestPlayer = ActorLocation - (ActorLocation - NearestPlayer->GetActorLocation());
	

	DrawDebugLine(GetWorld(), ActorLocation, VectorToNearestPlayer, FColor::Red, false, 0.1f, (uint8)'\000', 2);
	DrawDebugLine(GetWorld(), ActorLocation, ActorLocation + (GetActorForwardVector() * 300), FColor::Blue, false, 0.1f, (uint8)'\000', 2);



}