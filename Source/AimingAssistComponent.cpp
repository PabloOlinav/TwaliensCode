// All rights reserved.

#include "AimingAssistComponent.h"
#include "ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UAimingAssistComponent::UAimingAssistComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh> Cone(TEXT("/Engine/BasicShapes/Cone"));

	if(Cone.Object)
	{
		SetStaticMesh(Cone.Object);
	}

	bVisible = false;
	SetCollisionProfileName(FName("AimingAssist"));

	BeginOverlapDelegate.BindUFunction(this, FName("OnBeginOverlap"));
	OnComponentBeginOverlap.Add(BeginOverlapDelegate);

	EndOverlapDelegate.BindUFunction(this, FName("OnEndOverlap"));
	OnComponentEndOverlap.Add(EndOverlapDelegate);
	// ...
}


// Called when the game starts
void UAimingAssistComponent::BeginPlay()
{
	Super::BeginPlay();

	Target.Reserve(100);

	// ...
	
}


// Called every frame
void UAimingAssistComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...

}

void UAimingAssistComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->Tags.Contains("Enemy"))
	{
		Target.AddUnique(OtherActor);
	}
}

void UAimingAssistComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->Tags.Contains("Enemy"))
	{
		Target.Remove(OtherActor);
	}
}



void UAimingAssistComponent::GetTargets(TArray<AActor*>& Targets)
{
	Targets = Target;
}

AActor* UAimingAssistComponent::GetTarget()
{
	TArray<float> Value;
	float Dot;
	float Distance;
	float Result;

	for (int i = 0; i < Target.Num(); ++i)
	{
		Dot = FVector::DotProduct(GetAttachmentRootActor()->GetActorRotation().Vector(), Target[i]->GetActorLocation());
		Distance = FVector::Distance(GetAttachmentRootActor()->GetActorLocation(), Target[i]->GetActorLocation());
		Result = (Dot * 0.6f) + (0.4 * (1 / FMath::Pow(Distance, 2)));
		Value.Add(Result);
	}

	if(Value.Num() > 0)
	{
		int index;
		FMath::Max<float>(Value, &index);
		return Target[index];
	}

	return  NULL;
}

FRotator UAimingAssistComponent::AimToTarget(FVector Start)
{
	FRotator NewRotator;
	FVector Direction;

	if (GetTarget())
	{
		NewRotator = FRotationMatrix::MakeFromX(GetTarget()->GetActorLocation() - Start).Rotator();

		return NewRotator;
	}
	
	return  NewRotator.ZeroRotator;
}


