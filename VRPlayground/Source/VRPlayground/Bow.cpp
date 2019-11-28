// Fill out your copyright notice in the Description page of Project Settings.


#include "Bow.h"
#include "Arrow.h"
#include "MotionControllerComponent.h"
#include "HeadMountedDisplay/Public/XRMotionControllerBase.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

#include "CableComponent.h"


ABow::ABow()
{
	SnapArrowLocator = CreateDefaultSubobject<USceneComponent>(TEXT("SnapArrowLocator"));
	SnapArrowLocator->SetupAttachment(Base);

	// Initialize Cable
	CableHelperLocator = CreateDefaultSubobject<USceneComponent>(TEXT("CableHelperLocator"));
	CableHelperLocator->SetupAttachment(Base);

	CableTopComponent = CreateDefaultSubobject<UCableComponent>(TEXT("CableTopComponent"));
	CableTopComponent->SetupAttachment(Base);

	CableBottomComponent = CreateDefaultSubobject<UCableComponent>(TEXT("CableBottomComponent"));
	CableBottomComponent->SetupAttachment(Base);

	bCanSpawnArrow = false;
}

void ABow::BeginPlay()
{
	Super::BeginPlay();

	InitialCableLocation = CableHelperLocator->RelativeLocation;
}

void ABow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update the arrow position after spawned with the other controller
	if ((OtherMotionControllerSphere != nullptr) && bUpdateArrowPosition && (CurrentSpawnedArrow != nullptr))
	{
		// Gets the local position on the sphere and transform to local position for this pickup
		// The other motion controller is not a child
		FVector OtherMCSphereLocalPostion = Base->GetComponentTransform().InverseTransformPosition(OtherMotionControllerSphere->GetComponentLocation());
			
		LastArrowXPosition = FMath::Clamp(OtherMCSphereLocalPostion.X, EndArrowXPosition, StartArrowXPosition);

		// Move cables
		FVector CurrentCableLocation = CableHelperLocator->RelativeLocation;

		CurrentCableLocation.X = FMath::Clamp(OtherMCSphereLocalPostion.X, EndArrowXPosition, StartArrowXPosition);
		CableHelperLocator->SetRelativeLocation(CurrentCableLocation);

		CurrentSpawnedArrow->UpdateXLocation(LastArrowXPosition); 

		// Shoot arrow automatically if reaches the limit
		if (OtherMCSphereLocalPostion.X < EndArrowXPosition)
		{
			OtherMotionController = nullptr;
			ShootArrow();
		}
	}
}


//// IPickupInterface Implementation ///

void ABow::Pickup_Implementation(class UMotionControllerComponent *MotionController)
{
	// Detect which motion controller is using the bow
	if (CurrentMotionController == nullptr)
	{
		// Remove physics		
		Base->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Base->SetSimulatePhysics(false);

		bCanSpawnArrow = false;

		Super::Pickup_Implementation(MotionController);

	}
	else if ((OtherMotionController == nullptr) && (MotionController != CurrentMotionController))
	{
		OtherMotionController = MotionController;

		// Find the Sphere Component on the other motion controller
		TArray<class UActorComponent*> Components;
		OtherMotionController->GetOwner()->GetComponents(Components);
		for (auto Comp : Components)
		{
			OtherMotionControllerSphere = Cast<USphereComponent>(Comp);
			if (OtherMotionControllerSphere)
			{
				break;
			}
		}

		bCanSpawnArrow = true;
	}

	if (bCanSpawnArrow)
	{
		// Spawn an arrow on the snap arrow locator
		if (ArrowClass == nullptr) return;
			
		// Get local position of the sphere motion controller, relative to this pickup 
		FVector OtherMCSphereLocalPostion = Base->GetComponentTransform().InverseTransformPosition(OtherMotionControllerSphere->GetComponentLocation());
		LastArrowXPosition = FMath::Clamp(OtherMCSphereLocalPostion.X, EndArrowXPosition, StartArrowXPosition);

		CurrentSpawnedArrow = GetWorld()->SpawnActor<AArrow>(ArrowClass, SnapArrowLocator->GetComponentTransform());
			
		// Depend on the controller hand, apply offset to the arrow
		FVector OffsetLocation = FVector(0.0f, 0.0f, 0.0f);

		if (OtherMotionController->MotionSource == "EControllerHand::Right")
		{

			OffsetLocation.Y = 1.5f;
		}
		else if (OtherMotionController->MotionSource == "EControllerHand::Left")
		{
			OffsetLocation.Y = -2.0f;
		}	

		CurrentSpawnedArrow->AttachToObject(SnapArrowLocator, OffsetLocation);
		CurrentSpawnedArrow->UpdateXLocation(LastArrowXPosition);

		// Move cable
		FVector CurrentCableLocation = CableHelperLocator->RelativeLocation;
		CurrentCableLocation.X = FMath::Clamp(OtherMCSphereLocalPostion.X, EndArrowXPosition, StartArrowXPosition);			
		CableHelperLocator->SetRelativeLocation(CurrentCableLocation);

		bUpdateArrowPosition = true;
		bCanSpawnArrow = false;
	}
}


void ABow::Drop_Implementation(class UMotionControllerComponent *MotionController)
{
	// Check if it's the other hand, this means shoot arrow
	if ((OtherMotionController != nullptr) && (MotionController == OtherMotionController))
	{
		ShootArrow();
	}
	else
	{
		// Reset everything to it's initial state
		OtherMotionController = nullptr;

		bCanSpawnArrow = true;		

		if (CurrentSpawnedArrow != nullptr)
		{
			CurrentSpawnedArrow->Destroy();
		}

		CurrentSpawnedArrow = nullptr;
		
		Super::Drop_Implementation(MotionController);	

		Base->SetSimulatePhysics(false);

		Base->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

//// IPickupInterface Implementation ///

void ABow::ShootArrow()
{
	float ForceArrow = 0.0f;
	float Distance = (FMath::Abs(LastArrowXPosition) - FMath::Abs(StartArrowXPosition)) / 10.0f;

	if (Distance <= MinimunArrowXPosition) // Minimun distance to apply speed
	{
		ForceArrow = 0.0f;
	}
	else
	{
		ForceArrow = ArrowBowForce * Distance;
	}

	CurrentSpawnedArrow->OnShoot(ForceArrow);

	bUpdateArrowPosition = false;

	bCanSpawnArrow = true;

	CurrentSpawnedArrow = nullptr;

	// Move bow rope to initial location
	CableHelperLocator->SetRelativeLocation(InitialCableLocation);
}

