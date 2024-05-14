// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "ActionMacros.generated.h"

#define RETURN_ACTIONSUBMIT_ALWAYS()	\
		{							\
			this->Submit();			\
			return true;		\
		}

#define RETURN_ACTIONSUBMIT_ISVALIDACTOR(Actor)	\
		{							\
			if (IsValid(Actor))		\
				this->Submit();		\
			else					\
				this->Discard();	\
			return IsValid(Actor);	\
		}
