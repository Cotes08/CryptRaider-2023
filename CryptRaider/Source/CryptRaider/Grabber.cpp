// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"



// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	//If en modo pro
	if (PhysicsHandle && PhysicsHandle->GrabbedComponent)
	{
		// La posicion del jugador mas la direccion hacia donde mira el jugador por la distancia que queremos que este el objeto
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation()); 
	}

}

void UGrabber::Grab()
{

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if(PhysicsHandle == nullptr){ return;}

	FHitResult HitResult;

	if(GetgrabbableInReach(HitResult))
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();//Despertamos todos los rigidbodies del componente que hemos chocado

		AActor* HitActor = HitResult.GetActor();
		HitActor->Tags.Add(FName("Grabbed"));
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint, 
			GetComponentRotation()
		);
	}
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle && PhysicsHandle->GrabbedComponent)
	{
		//El GrabbedComponent->GetOwner() nos devuelve el actor que tiene el componente que estamos cogiendo y le quitamos el tag de Grabbed
		PhysicsHandle->GrabbedComponent->GetOwner()->Tags.Remove(FName("Grabbed"));
		PhysicsHandle->GrabbedComponent->WakeAllRigidBodies();
		PhysicsHandle->ReleaseComponent();
	}

}

//Busca el componente de tipo UPhysicsHandleComponent en el actor que tiene este componente
UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const 
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if(Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber requieres a UPhysicsHandleComponent"));
	}
	return Result;
}

bool UGrabber::GetgrabbableInReach(FHitResult& OutHitResult) const{

	//Basicamente sacamos el start de nuestro personaje hasta el end de donde podemos coger un objeto
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);//Creamos una esfera de colision con el radio que hemos puesto en el editor
	
	return GetWorld()->SweepSingleByChannel(
		OutHitResult, 
		Start, End, 
		FQuat::Identity, 
		ECC_GameTraceChannel2, 
		Sphere
	);	
}
