// Fill out your copyright notice in the Description page of Project Settings.

#include "TriggerComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Mover == nullptr) { return; }

	AActor* Actor = GetAcceptableActor();

	if (Actor != nullptr)
	{
		//Esto sirve para convereir un actor con fisicas a un componente primitivo
		UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		if (Component != nullptr)
		{
			//Con esto desactivamos las fisicas del componente
			Component->SetSimulatePhysics(false);
		}

		//Hace que el actor(la gargola) se pege al componente que tiene esta clase, es decir el muro secreto
		Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
		
		Mover->SetShouldMove(true);
	}else{
		
		Mover->SetShouldMove(false);
	}
}

//Esto se setea en el blueprint
void UTriggerComponent::SetMover(UMover* NewMover)
{
    Mover = NewMover;
}

AActor* UTriggerComponent::GetAcceptableActor() const{

	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	//Va a recorres el array de actores por cada AAactor* y se va a guardar en actor en dicha iteracion
	for (AActor* Actor : Actors)
	{
		if (Actor->ActorHasTag(AcceptableActorTag) && !Actor->ActorHasTag("Grabbed"))
		{
			return Actor;
		}
	}

	return nullptr;
}
