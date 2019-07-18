// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetTauntActor.h"
#include "BasicWarriorController.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

class USphereComponent;


// Sets default values
ATargetTauntActor::ATargetTauntActor() : TauntTimeEffect(20), TauntRadio(300)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Taunt Mesh Collider"));
	RootComponent = MeshCollider;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Taunt Mesh"));
	Mesh->SetupAttachment(RootComponent);

	TauntArea = CreateDefaultSubobject<USphereComponent>(TEXT("TauntArea"));
	TauntArea->SetSphereRadius(TauntRadio);
	TauntArea->SetupAttachment(RootComponent);

	FName TagEnemy = FName("Enemy");
	AllowedTagsToInteract.Add(TagEnemy);

}

// Called when the game starts or when spawned
void ATargetTauntActor::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle DestroyHandler;
	GetWorldTimerManager().SetTimer(DestroyHandler, this, &ATargetTauntActor::DestroyTaunt, 1, false, TauntTimeEffect);
}


void ATargetTauntActor::UnTaunt()
{
	for (AActor * ValidActor : ActorsInArea)
	{
		if (IsEnemy(ValidActor))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("DesTaunteando enemigo"));
			ABasicWarriorController* CurrentController = Cast<ABasicWarriorController>((Cast<APawn>(ValidActor)->GetController()));
			if (CurrentController)
			{
				CurrentController->RemoveAgentTarget(this);
			}
		}
	}
}

// Called every frame
void ATargetTauntActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateOverlaps();
	TauntSkill();

}

void ATargetTauntActor::TauntSkill() 
{	
	for (AActor * ValidActor : ActorsInArea)
	{
		if (IsEnemy(ValidActor))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Taunteando enemigo"));
			ABasicWarriorController* CurrentController = Cast<ABasicWarriorController>((Cast<APawn>(ValidActor)->GetController()));
			if (CurrentController) 
			{
				CurrentController->SetAgentTarget(this);
			}
		}
	}
}


void ATargetTauntActor::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (HasActorAnyValidTag(OtherActor) && !ActorsInArea.Contains(OtherActor))
	{
		//Add enemy
		ActorsInArea.Add(OtherActor);
	}
}



bool ATargetTauntActor::HasActorAnyValidTag(AActor * Actor)
{
	bool found = false;

	for (FName Tag : Actor->Tags)
	{
		found = AllowedTagsToInteract.Contains(Tag);
		if (found)
		{
			break;
		}
	}

	return found;
}

void ATargetTauntActor::OnObjectExitToArea(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	//If actor is a coin and it is in Suckarea.
	if (HasActorAnyValidTag(OtherActor) && ActorsInArea.Contains(OtherActor))
	{
		//Remove coin from TArray
		ActorsInArea.Remove(OtherActor);
	}
}

bool ATargetTauntActor::IsEnemy(AActor * Actor) 
{
	return Actor->Tags.Contains("Enemy");
}

void ATargetTauntActor::DestroyTaunt() 
{
	UnTaunt();
	this->Destroy();
}