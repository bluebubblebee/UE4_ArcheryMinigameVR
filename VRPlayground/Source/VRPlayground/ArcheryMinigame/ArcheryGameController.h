// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArcheryGameController.generated.h"

UCLASS()
class VRPLAYGROUND_API AArcheryGameController : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Archery Game")
	USceneComponent *Root;

	// Documentation: https://docs.unrealengine.com/en-US/API/Runtime/Engine/Components/USplineComponent/index.html
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Archery Game")
	class USplineComponent* Spline1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Archery Game")
	class USplineComponent* Spline2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Archery Game")
	class UTextRenderComponent* ScoreText;

protected:

	class ATarget* Target1;
	class ATarget* Target2;
	class ATarget* Target3;
	class ATarget* Target4;


	UPROPERTY(EditDefaultsOnly, Category = "Archery Game")
	float TotalTimePath1;

	UPROPERTY(EditDefaultsOnly, Category = "Archery Game")
	float TotalTimePath2;

	// Target to spawn
	UPROPERTY(EditDefaultsOnly, Category = "Archery Game: Target")
	TSubclassOf<class ATarget> TargetClass;
	
	int CurrentScore;	
	
public:	

	AArcheryGameController();

	void OnTargetHit(class ATarget* Target);

protected:

	virtual void BeginPlay() override;

};
