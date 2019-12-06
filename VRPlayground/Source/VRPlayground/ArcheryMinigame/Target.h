// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Target.generated.h"

UCLASS()
class VRPLAYGROUND_API ATarget : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Target")
	USceneComponent *Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Target")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Target")
	class UBoxComponent* HitTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Target")
	class UBoxComponent* HitTrigger2;
	
public:	
	ATarget();

	void Initialize(class AArcheryGameController* GameController, class USplineComponent* Spline, float PathTime, float StartTime);

protected:
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;


protected:
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);


public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Target")
	void OnTargetHit();

protected:

	class AArcheryGameController* CurrentGameController;
	class USplineComponent* CurrentSpline;
	float TotalPathTime;
	float StartTime;
	
	bool bIsCollisionEnabled;
	bool bIsMoving;		

	float CurrentDeltaTime;

	// Start Delay
	float StartDelayTime;
	bool bWaitToStartToMove;	

	class AArrow* CurrentArrowHit;
	bool bWaitToEndHit;
};
