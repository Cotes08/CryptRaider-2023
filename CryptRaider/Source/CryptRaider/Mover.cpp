// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetOwner()->GetActorLocation(); //Guardamos la posicion original del actor
	
	
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector TargetLocation = OriginalLocation;

	if (ShouldMove)
	{
		TargetLocation = OriginalLocation + MoveOffset; //Calculamos la posicion a la que queremos llegar	
	}
	FVector CurrentLocation = GetOwner()->GetActorLocation(); //Guardamos la posicion actual del actor
	float Speed = MoveOffset.Length() / MoveTime; //Calculamos la velocidad que debe llevar, es decir la distancia que se mueve entre el tiempo que tarda en moverse
	
	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);//Calculamos la interpolacion entre la posicion actual y la posicion a la que queremos llegar
	GetOwner()->SetActorLocation(NewLocation);//Movemos el actor a la nueva posicion
}

void UMover::SetShouldMove(bool NewShouldMove)
{
	ShouldMove = NewShouldMove;
}

