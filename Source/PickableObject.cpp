// All rights reserved.

#include "PickableObject.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/Engine.h"
#include "LifeFunctionality.h"
#include "Twalien.h"
#include "Weapon.h"

// Sets default values
APickableObject::APickableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Object Collider"));
	CollisionComp->SetupAttachment(RootComponent);
	CollisionComp->SetSphereRadius(60);
	CollisionComp->SetSimulatePhysics(true);
	CollisionComp->SetEnableGravity(true);
	CollisionComp->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetCollisionProfileName(FName("CoinCollisionDefault"));

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object Mesh"));
	ObjectMesh->SetupAttachment(CollisionComp);

	AllowedTagsToInteract.Add(FName("Player"));

	RechargeAmmount = 0;
}

// Called when the game starts or when spawned
void APickableObject::BeginPlay()
{
	Super::BeginPlay();

	if (ObjectType == EObjectType::Type_Ammunition)
	{
		Tags.Add(FName("Ammo"));
	}
	else if (ObjectType == EObjectType::Type_Health)
	{
		Tags.Add(FName("Health"));
	}
}

// Called every frame
void APickableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickableObject::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	bool IsAllowed = false;

	for (int i = 0; i < OtherActor->Tags.Num() && !IsAllowed; i++)
	{
		IsAllowed = AllowedTagsToInteract.Contains(OtherActor->Tags[i]);
	}

	if (IsAllowed)
	{
		if (ObjectType == EObjectType::Type_Health)
		{
			TSet<UActorComponent *> Components = OtherActor->GetComponents();
			ILifeFunctionality * LifeComponent = nullptr;

			for (UActorComponent * Component : Components)
			{
				LifeComponent = Cast<ILifeFunctionality>(Component);
				if (LifeComponent)
				{
					break;
				}
			}

			if (LifeComponent)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Health Recharge"));
				LifeComponent->SetHitPoints(RechargeAmmount);
			}
		}
		else if(ObjectType == EObjectType::Type_Ammunition)
		{
			//UWeapon* Gun = Cast<UWeapon>(Cast<ATwalien>(OtherActor)->GetTwalienWeapon());

			/*if (Gun)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Ammunition Recharge"));
				Gun->SetAmmunition(round(RechargeAmmount));
			}*/
		}

		this->Destroy();
	}
}
