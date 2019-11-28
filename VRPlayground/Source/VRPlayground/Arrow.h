// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Arrow.generated.h"

/**
 * 
 */
UCLASS()
class VRPLAYGROUND_API AArrow : public APickup
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Arrow")
	class UStaticMeshComponent* Mesh;

public: 

	AArrow();

public:

	void AttachToObject(USceneComponent* Object, FVector OffsetPosition);

	void UpdateXLocation(float XLocation);

	// Shoot arrow
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Arrow")
	void OnShoot(float Speed);

	void OnShoot_Implementation(float force);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Arrow")
	float DelayToDestroy = 3.0f;	

	// Timer handle for destroying the actor
	FTimerHandle DestroyArrowTimerHandle;

	UFUNCTION()
	void OnDestroyArrow();

	float ForceArrow;
};
