// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "OmegaCharacter.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/WidgetComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "OmegaDebugHelpers.h"

//////////////////////////////////////////////////////////////////////////
// AOmegaCharacter

AOmegaCharacter::AOmegaCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}



void AOmegaCharacter::DrawDebugCircle(const FVector& StartLocation, bool bHorizontal)
{
	// x is forward and back
	// y is left and right
	// z is up and down

	// amount to add to theta each time (degrees)
	float Step = (15 * UKismetMathLibrary::GetPI()) / 180;

	// angle that will be increased each loop
	float Theta;

	FVector CharacterLocation = GetActorLocation();

	int32 h = StartLocation.X;			// x coordinate of circle center
 	int32 k = StartLocation.Y;			// y coordinate of circle center
 	int32 StartingZ = StartLocation.Z;	// z coordinate of circle center
 	int32 radius = 50;

	TArray<FVector> Points;

	for (Theta = 0; Theta < 2 * UKismetMathLibrary::GetPI(); Theta = Theta + Step)
	{
		float DesiredX;
		float DesiredY;
		float DesiredZ; 

		if (!bHorizontal)
		{
			DesiredX = h + radius * UKismetMathLibrary::Cos(Theta);
			DesiredY = k - radius * UKismetMathLibrary::Sin(Theta);
			DesiredZ = StartingZ;
		}
		else
		{
			DesiredX = h;
			DesiredY = k + radius * UKismetMathLibrary::Cos(Theta);
			DesiredZ = StartingZ - radius * UKismetMathLibrary::Sin(Theta);
		}

		FVector PointToAdd(DesiredX, DesiredY, DesiredZ);

		Points.Push(PointToAdd);
	}

	FVector FirstPoint = Points[0];

	for (int Index = 0; Index < Points.Num(); ++Index)
	{
		FVector CurrentPoint = Points[Index];

		if (Index + 1 == Points.Num())
		{
			DrawDebugLine(GetWorld(), CurrentPoint, FirstPoint, FColor::Red, true, -1.0f, (uint8)'\000', 3.0f);
			break;
		}

		FVector NextPoint = Points[Index + 1];

		DrawDebugLine(GetWorld(), CurrentPoint, NextPoint, FColor::Red, true, -1.0f, (uint8)'\000', 3.0f);
	}
}

void AOmegaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bHasDrawnDebugCircle)
	{
		bHasDrawnDebugCircle = true;

		AOmegaCircle* TempCircle = NewObject<AOmegaCircle>(GetWorld(), AOmegaCircle::StaticClass());
		TempCircle->Initialize(8);

		UE_LOG(LogTemp, Warning, TEXT("Circumference: %f "), TempCircle->GetCircumference());

		FVector InputLocation = GetActorLocation();
		InputLocation.X = InputLocation.X + 50;
		DrawDebugCircle(InputLocation, true);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AOmegaCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AOmegaCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AOmegaCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AOmegaCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AOmegaCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AOmegaCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AOmegaCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AOmegaCharacter::OnResetVR);

	
}


void AOmegaCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AOmegaCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AOmegaCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AOmegaCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AOmegaCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AOmegaCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AOmegaCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
