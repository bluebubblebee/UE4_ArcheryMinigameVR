// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"


AArrow::AArrow()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
	Mesh->SetupAttachment(Base);

	bEnablePickup = false;
}

void AArrow::AttachToObject(USceneComponent* Object, FVector OffsetPosition)
{
	// Disable Pickup and Physics simulation
	bEnablePickup = false;
	bSimulatePhysics = false;	

	// Remove collision from base and Mesh
	Base->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Base->SetSimulatePhysics(false); 
	Base->SetEnableGravity(false);

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetSimulatePhysics(false);
	Mesh->SetEnableGravity(false);
		
	Base->SetRelativeLocation(Object->RelativeLocation + OffsetPosition);
	Base->AttachToComponent(Object, FAttachmentTransformRules::KeepWorldTransform, NAME_None);
}

void AArrow::UpdateXLocation(float XLocation)
{
	// Get relative location to its parent and update X
	FVector CurrentBaseLocation = Base->RelativeLocation;
	CurrentBaseLocation.X = XLocation;
	Base->SetRelativeLocation(CurrentBaseLocation);
}


void AArrow::OnShoot_Implementation(float force)
{
	Base->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	
	ForceArrow = force;	

	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetEnableGravity(true);

	// Apply a force to the Mesh, on the forward vector
	Mesh->AddForce(Base->GetForwardVector() * ForceArrow);	

	GetWorld()->GetTimerManager().SetTimer(DestroyArrowTimerHandle, this, &AArrow::OnDestroyArrow, DelayToDestroy, false);
}

void AArrow::OnDestroyArrow()
{
	// Clear the timer
	GetWorld()->GetTimerManager().ClearTimer(DestroyArrowTimerHandle);

	Destroy();
}


void AArrow::OnHit()
{
	// Disable Pickup and Physics simulation
	bEnablePickup = false;
	bSimulatePhysics = false;

	Mesh->SetSimulatePhysics(false);
	Mesh->SetEnableGravity(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}