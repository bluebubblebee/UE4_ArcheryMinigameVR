// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Bow.generated.h"

/**
 * 
 */
UCLASS()
class VRPLAYGROUND_API ABow : public APickup
{
	GENERATED_BODY()

public:

	ABow();

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

protected:

	// Snap locator helper to spawn the arrow
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Bow")
	USceneComponent* SnapArrowLocator;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Bow")
	USceneComponent* CableHelperLocator;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Bow")
	class UCableComponent* CableTopComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Bow")
	class UCableComponent* CableBottomComponent;	

protected:

	// Arrow Pickup to spawn
	UPROPERTY(EditDefaultsOnly, Category = "Bow Arrow Settings")
	TSubclassOf<class AArrow> ArrowClass;

	class AArrow* CurrentSpawnedArrow;
	bool bCanSpawnArrow;

	// The other motion controller to move the arrow
	class UMotionControllerComponent *OtherMotionController;
	class USphereComponent* OtherMotionControllerSphere = nullptr;

	bool bUpdateArrowPosition;	

	// Arrow movement limits	
	UPROPERTY(EditDefaultsOnly, Category = "Bow Arrow Settings")
	float EndArrowXPosition = -50.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Bow Arrow Settings")
	float StartArrowXPosition = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Bow Arrow Settings")
	float MinimunArrowXPosition = 4.0f;

	float LastArrowXPosition;

	UPROPERTY(EditDefaultsOnly, Category = "Bow Arrow Settings")
	float ArrowBowForce = 100000.0f;

	FVector InitialCableLocation;

	void ShootArrow();

public:

	//// IPickupInterface Implementation ///
	virtual void Drop_Implementation(class UMotionControllerComponent *MotionController) override;

	virtual void Pickup_Implementation(class UMotionControllerComponent *MotionController) override;
	//// IPickupInterface Implementation ///
		
};
