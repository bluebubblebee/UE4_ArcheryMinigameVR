// Fill out your copyright notice in the Description page of Project Settings.


#include "Target.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Arrow.h"
#include "ArcheryGameController.h"

// Sets default values
ATarget::ATarget()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
	Mesh->SetupAttachment(Root);

	HitTrigger = CreateDefaultSubobject <UBoxComponent>(TEXT("HitTrigger"));
	HitTrigger->SetupAttachment(Root);

	HitTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATarget::BeginOverlap);

	HitTrigger2 = CreateDefaultSubobject <UBoxComponent>(TEXT("HitTrigger2"));
	HitTrigger2->SetupAttachment(Root);

	HitTrigger2->OnComponentBeginOverlap.AddDynamic(this, &ATarget::BeginOverlap);

	PrimaryActorTick.bCanEverTick = true;
}


void ATarget::BeginPlay()
{
	Super::BeginPlay();

	bIsMoving = false;

	bIsCollisionEnabled = false;

	bWaitToEndHit = false;

}

void ATarget::Initialize(class AArcheryGameController* GameController, class USplineComponent* Spline, float PathTime, float StartTime)
{
	CurrentGameController = GameController;
	CurrentSpline = Spline;
	TotalPathTime = PathTime;
	StartDelayTime = StartTime;

	Mesh->SetVisibility(false, false);

	bIsCollisionEnabled = false;	
	bIsMoving = false;

	CurrentDeltaTime = 0.0f;

	// Wait to start to move
	bWaitToStartToMove = true;

	bWaitToEndHit = false;
}


void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// State: Wait to end hit
	if (bWaitToEndHit)
	{
		CurrentDeltaTime += DeltaTime;
		if (CurrentDeltaTime > 0.3f)
		{
			

			if (CurrentArrowHit != nullptr)
			{
				CurrentArrowHit->Destroy();
			}

			bWaitToEndHit = false;
			CurrentDeltaTime = 0.0f;

			// Enable state wait to start move
			bWaitToStartToMove = true;
		}
	}

	// State: Wait to start moving
	if (bWaitToStartToMove)
	{
		CurrentDeltaTime += DeltaTime;
		if (CurrentDeltaTime > StartDelayTime)
		{
			CurrentDeltaTime = 0.0f;
			bWaitToStartToMove = false;

			StartTime = GetWorld()->GetTimeSeconds();

			// Start to move
			Mesh->SetVisibility(true, false);
			bIsCollisionEnabled = true;
			bIsMoving = true;

			bWaitToEndHit = false;
		}
	}
	
	// State: Move the target along the spline
	if (bIsMoving)
	{
		//Returns time in seconds since world was brought up for play, IS stopped when game pauses, IS dilated / clamped
		// https://docs.unrealengine.com/en-US/API/Runtime/Engine/Engine/UWorld/GetTimeSeconds/index.html
		float CurrentTime = (GetWorld()->GetTimeSeconds() - StartTime) / TotalPathTime;

		// Get current distance
		float Distance = CurrentSpline->GetSplineLength() * CurrentTime;
		
		// Get position along spline
		FVector Position = CurrentSpline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		Root->SetWorldLocation(Position);

		// Get rotation
		FVector Direction = CurrentSpline->GetDirectionAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		FRotator Rotator = FRotationMatrix::MakeFromX(Direction).Rotator();
		Root->SetWorldRotation(Rotator);

		// End of path
		if (CurrentTime >= 1.0f)
		{
			bIsCollisionEnabled = false;

			bIsMoving = false;

			Mesh->SetVisibility(false, false);
			
			CurrentDeltaTime = 0.0f;

			bWaitToStartToMove = true;
		}
	}
}

void ATarget::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (!bIsCollisionEnabled) return;

	if (OtherActor != nullptr)
	{
		CurrentArrowHit = Cast<AArrow>(OtherActor);
		
		// Attach arrow to target
		if (CurrentArrowHit != nullptr)
		{
			HitTrigger->AttachToComponent(CurrentArrowHit->GetBase(), FAttachmentTransformRules::KeepWorldTransform, NAME_None);

			bIsCollisionEnabled = false;

			Mesh->SetVisibility(false, false);

			bIsMoving = false;

			CurrentDeltaTime = 0.0f;			

			// Call on target hit
			if (CurrentGameController != nullptr)
			{
				CurrentGameController->OnTargetHit(this); 
			}

			// Call blueprint event
			OnTargetHit();

			CurrentArrowHit->OnHit();

			bWaitToEndHit = true;
		}
	}
}


