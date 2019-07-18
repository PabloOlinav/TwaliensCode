// All rights reserved.

#include "RadialAttackableLocations.h"
#include "Twalien.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "LifeFunctionality.h"

// Sets default values for this component's properties
URadialAttackableLocations::URadialAttackableLocations()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void URadialAttackableLocations::InitData(float Radius, float MaxEnemyRadius, int RingIndex)
{
	//Calculate desired Circumference Lenght
	float CircumferenceLenght = 2 * Radius * 3.14159265359f;

	//Get number of partitions
	int PartitionsAmount = CircumferenceLenght / (MaxEnemyRadius + MaxEnemyRadius);

	//Calculate angle increment each iteration
	const float AngleOffset = 360 / PartitionsAmount;

	//Reserve spaces in Quadrants
	int FirstSpace, SecondSpace, ThirdSpace, FourthSpace;
	FirstSpace = SecondSpace = ThirdSpace = FourthSpace = 0;

	for (float Angle = 0; Angle < 360; Angle += AngleOffset)
	{
		if (Angle >= 0 && Angle < 90)
		{
			//Angle in first Quadrant
			++FirstSpace;
		}
		else if (Angle >= 90 && Angle < 180)
		{
			//Angle in second Quadrant
			++SecondSpace;
		}
		else if (Angle >= 180 && Angle < 270)
		{
			//Angle in third Quadrant
			++ThirdSpace;
		}
		else
		{
			//Angle in fourth Quadrant
			++FourthSpace;
		}
	}

	//Reserve enought space in each quadrant of Ringindex ring
	RelativeAttackableLocation[0].Quadrant[RingIndex].Position.Reserve(FirstSpace);
	RelativeAttackableLocation[1].Quadrant[RingIndex].Position.Reserve(SecondSpace);
	RelativeAttackableLocation[2].Quadrant[RingIndex].Position.Reserve(ThirdSpace);
	RelativeAttackableLocation[3].Quadrant[RingIndex].Position.Reserve(FourthSpace);

	RelativeAttackableLocation[0].Quadrant[RingIndex].InUse.Reserve(FirstSpace);
	RelativeAttackableLocation[1].Quadrant[RingIndex].InUse.Reserve(SecondSpace);
	RelativeAttackableLocation[2].Quadrant[RingIndex].InUse.Reserve(ThirdSpace);
	RelativeAttackableLocation[3].Quadrant[RingIndex].InUse.Reserve(FourthSpace);
	
	//Create locations
	CreateLocations(PartitionsAmount, AngleOffset, RingIndex, Radius);
}

void URadialAttackableLocations::CreateLocations(int PartitionsAmount, float AngleOffset, int RingIndex, float Radius)
{
	//from 0 to PartitionsAmount
	for (float i = 0, Angle = 0; i < PartitionsAmount; ++i, Angle += AngleOffset)
	{
		float Sin, Cos;
		//Calculate Sin and Cos of Angle
		FMath::SinCos(&Sin, &Cos, Angle);
		//Relative location is equal (Cos(Angle), Sin(Angle), 0) * RadialRadius
		if (Angle >= 0 && Angle < 90)
		{
			RelativeAttackableLocation[0].Quadrant[RingIndex].Position.Add(FVector(Cos * Radius, Sin * Radius, 0));
			RelativeAttackableLocation[0].Quadrant[RingIndex].InUse.Add(false);
		}
		else if (Angle >= 90 && Angle < 180)
		{
			RelativeAttackableLocation[1].Quadrant[RingIndex].Position.Add(FVector(Cos * Radius, Sin * Radius, 0));
			RelativeAttackableLocation[1].Quadrant[RingIndex].InUse.Add(false);
		}
		else if (Angle >= 180 && Angle < 270)
		{
			RelativeAttackableLocation[2].Quadrant[RingIndex].Position.Add(FVector(Cos * Radius, Sin * Radius, 0));
			RelativeAttackableLocation[2].Quadrant[RingIndex].InUse.Add(false);
		}
		else
		{
			RelativeAttackableLocation[3].Quadrant[RingIndex].Position.Add(FVector(Cos * Radius, Sin * Radius, 0));
			RelativeAttackableLocation[3].Quadrant[RingIndex].InUse.Add(false);
		}
	}
}

bool URadialAttackableLocations::GetEmptyPosition(int QuadrantIndex, int RingIndex, FVector & Position, FRelativePositionId & RelativePositionAccessInfo)
{
	//Search in all points in Quadrant
	for (size_t i = 0; i < RelativeAttackableLocation[QuadrantIndex].Quadrant[RingIndex].Position.Num(); ++i)
	{
		//If point is not in use
		if (!RelativeAttackableLocation[QuadrantIndex].Quadrant[RingIndex].InUse[i])
		{
			//mark it as true
			RelativeAttackableLocation[QuadrantIndex].Quadrant[RingIndex].InUse[i] = true;

			//assign position
			Position = RelativeAttackableLocation[QuadrantIndex].Quadrant[RingIndex].Position[i];

			//Fill access info
			RelativePositionAccessInfo.QuadrantIndex = QuadrantIndex;
			RelativePositionAccessInfo.RingIndex = RingIndex;
			RelativePositionAccessInfo.PositionIndex = i;

			//return success
			return true;
		}
	}

	//If we reach this point no point is available
	return false;
}

bool URadialAttackableLocations::SearchForPoint(int QuadrantIndex, FVector & Position, FRelativePositionId & RelativePositionAccessInfo)
{
	bool Result = false;

	for (int RingIndex = 0; RingIndex < RingAmount; RingIndex++)
	{
		int Quadrant = QuadrantIndex;

		//Search in queried quadrant
		Result = GetEmptyPosition(Quadrant, RingIndex, Position, RelativePositionAccessInfo);
		if (Result)
		{
			return Result;
		}

		//Do (QuadrantIndex + 1) mod 4
		Quadrant = QuadrantIndex + 1;
		if (Quadrant > 3)
		{
			Quadrant -= 4;
		}

		//search in adjacent quadrant
		Result = GetEmptyPosition(Quadrant, RingIndex, Position, RelativePositionAccessInfo);
		if (Result)
		{
			return Result;
		}

		//Do (QuadrantIndex - 1) mod 4
		Quadrant = QuadrantIndex - 1;
		if (Quadrant < 0)
		{
			Quadrant += 4;
		}

		Result = GetEmptyPosition(Quadrant, RingIndex, Position, RelativePositionAccessInfo);
		if (Result)
		{
			return Result;
		}

		//Do (QuadrantIndex + 2) mod 4
		Quadrant = QuadrantIndex + 2;
		if (Quadrant > 3)
		{
			Quadrant -= 4;
		}

		//seach in last quadrant
		Result = GetEmptyPosition(Quadrant, RingIndex, Position, RelativePositionAccessInfo);
		if (Result)
		{
			return Result;
		}
	}

	//If we reach this point no point is available in all rings
	return false;
}

// Called when the game starts
void URadialAttackableLocations::BeginPlay()
{
	Super::BeginPlay();

	// ...

	//Calculate Maximum enemy collider radius
	float MaxEnemyRadius = 0;
	for (float Radius : EnemiesColliderRadius)
	{
		MaxEnemyRadius = (MaxEnemyRadius < Radius) ? Radius : MaxEnemyRadius;
	}

	//Invariant checkers
	const ATwalien * Player = Cast<ATwalien>(GetOwner());
	check(Player);
	check(FirstRingRadius >= MaxEnemyRadius + Player->GetCapsuleComponent()->GetScaledCapsuleRadius());
	check(RestRingRadiusIncrement >= MaxEnemyRadius + Player->GetCapsuleComponent()->GetScaledCapsuleRadius());

	//Reserve space for 4 Quadrants
	RelativeAttackableLocation.Reserve(4);

	RelativeAttackableLocation.Add(FQuadrants());
	RelativeAttackableLocation.Add(FQuadrants());
	RelativeAttackableLocation.Add(FQuadrants());
	RelativeAttackableLocation.Add(FQuadrants());

	//Reserve enought space for all 4 quadrants
	RelativeAttackableLocation[0].Quadrant.Reserve(RingAmount);
	RelativeAttackableLocation[1].Quadrant.Reserve(RingAmount);
	RelativeAttackableLocation[2].Quadrant.Reserve(RingAmount);
	RelativeAttackableLocation[3].Quadrant.Reserve(RingAmount);

	//Add uninitialized data
	for (size_t i = 0; i < RingAmount; ++i)
	{
		RelativeAttackableLocation[0].Quadrant.Add(FQuadrant());
		RelativeAttackableLocation[1].Quadrant.Add(FQuadrant());
		RelativeAttackableLocation[2].Quadrant.Add(FQuadrant());
		RelativeAttackableLocation[3].Quadrant.Add(FQuadrant());
	}

	//Initialize Data of all rings
	for (int i = 0, Radius = FirstRingRadius; i < RingAmount; ++i, Radius += RestRingRadiusIncrement)
	{
		InitData(Radius, MaxEnemyRadius, i);
	}
}

// Called every frame
void URadialAttackableLocations::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool URadialAttackableLocations::GetFreeRelativePosition(const FVector & AgentPosition, FVector & RelativePosition, FRelativePositionId & RelativePositionAccessInfo)
{
	const FVector OwnerPosition = GetOwner()->GetActorLocation();

	if (AgentPosition.X - OwnerPosition.X >= 0)
	{
		if (AgentPosition.Y - OwnerPosition.Y >= 0)
		{
			//Start search in First Quadrant
			return SearchForPoint(0, RelativePosition, RelativePositionAccessInfo);
		}
		else
		{
			//Start search in Fourth Quadrant
			return SearchForPoint(3, RelativePosition, RelativePositionAccessInfo);
		}
	}
	else
	{
		if (AgentPosition.Y - OwnerPosition.Y >= 0)
		{
			//Start search in Second Quadrant
			return SearchForPoint(1, RelativePosition, RelativePositionAccessInfo);
		}
		else
		{
			//Start search in Third Quadrant
			return SearchForPoint(2, RelativePosition, RelativePositionAccessInfo);
		}
	}
}

void URadialAttackableLocations::GetRelativePosition(const FRelativePositionId & AccessInfo, FVector & RelativePosition)
{
	//check info
	check(AccessInfo.QuadrantIndex < 4);
	check(AccessInfo.RingIndex < RingAmount);
	check(AccessInfo.PositionIndex < RelativeAttackableLocation[AccessInfo.QuadrantIndex].Quadrant[AccessInfo.RingIndex].Position.Num());

	RelativePosition = RelativeAttackableLocation[AccessInfo.QuadrantIndex].Quadrant[AccessInfo.RingIndex].Position[AccessInfo.PositionIndex];
}

void URadialAttackableLocations::FreeRelativePosition(const FRelativePositionId & AccessInfo)
{
	//check info
	check(AccessInfo.QuadrantIndex < 4);
	check(AccessInfo.RingIndex < RingAmount);
	check(AccessInfo.PositionIndex < RelativeAttackableLocation[AccessInfo.QuadrantIndex].Quadrant[AccessInfo.RingIndex].Position.Num());

	//mark it as false
	RelativeAttackableLocation[AccessInfo.QuadrantIndex].Quadrant[AccessInfo.RingIndex].InUse[AccessInfo.PositionIndex] = false;
}

