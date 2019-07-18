// All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CrypteckBall.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCrypteckBall : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for all four Crypteck balls. Constains basic functionality common by all four balls. Caution, you MUST use this interface only for Crypteck balls!!!.
 */
class TWALIENS_API ICrypteckBall
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	ICrypteckBall();

	virtual void Action(float DeltaSeconds) { unimplemented(); };

	virtual void Activate() = 0;

protected:
	
	enum BALL_STATE 
	{ 
		DEACTIVATED, 
		DISSOLVE_PRE_GOTO_ORBIT, 
		TELEPORT_TO_ORBIT, 
		IDLE, 
		DISSOLVE_PRE_ATTACK, 
		TELEPORT_TO_ATTACK, 
		DISSOLVE_POST_ATTACK, 
		ONTHEWAY, 
		ONACTION, 
		DISSOLVE_PRE_RETURN, 
		RETURNING
	};

	BALL_STATE State;
};