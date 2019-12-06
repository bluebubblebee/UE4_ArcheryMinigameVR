// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcheryGameController.h"
#include "Components/SplineComponent.h"
#include "Components/TextRenderComponent.h"
#include "Target.h"

// Sets default values
AArcheryGameController::AArcheryGameController()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Spline1 = CreateDefaultSubobject<USplineComponent>(TEXT("Spline1"));
	Spline1->SetupAttachment(Root);
	Spline1->Duration = 1.0f;
	Spline1->bDrawDebug = true;

	Spline2 = CreateDefaultSubobject<USplineComponent>(TEXT("Spline2"));
	Spline2->SetupAttachment(Root);
	Spline2->Duration = 1.0f;
	Spline2->bDrawDebug = true;

	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText"));
	ScoreText->SetupAttachment(Root);
	ScoreText->Text = FText::FromString("Score: 0");

	PrimaryActorTick.bCanEverTick = false;  
}


void AArcheryGameController::BeginPlay()
{
	Super::BeginPlay();

	// Instance each target with each corresponding spline and give different times
	Target1 = GetWorld()->SpawnActor<ATarget>(TargetClass, Spline1->GetComponentTransform());
	if (Target1 != nullptr)
	{
		Target1->Initialize(this, Spline1, TotalTimePath1, 5.0f);
	}

	Target2 = GetWorld()->SpawnActor<ATarget>(TargetClass, Spline1->GetComponentTransform());
	if (Target2 != nullptr)
	{
		Target2->Initialize(this, Spline1, TotalTimePath2, 10.0f);
	}


	Target3 = GetWorld()->SpawnActor<ATarget>(TargetClass, Spline2->GetComponentTransform());
	if (Target3 != nullptr)
	{
		Target3->Initialize(this, Spline2, TotalTimePath2, 8.0f);
	}


	Target4 = GetWorld()->SpawnActor<ATarget>(TargetClass, Spline2->GetComponentTransform());
	if (Target4 != nullptr)
	{
		Target4->Initialize(this, Spline2, TotalTimePath2, 16.0f);
	}
	CurrentScore = 0;
}

void AArcheryGameController::OnTargetHit(class ATarget* Target)
{
	if (Target != nullptr)
	{
		CurrentScore += 1;
		FString Score = "Score: " + FString::FromInt(CurrentScore);
		ScoreText->SetText(FText::FromString(Score));
	}
}

