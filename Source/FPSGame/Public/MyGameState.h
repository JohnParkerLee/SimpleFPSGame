// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MyGameState.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API AMyGameState : public AGameState
{
	GENERATED_BODY()
	
	public:
	UPROPERTY(BlueprintReadOnly, Category="Gameplay");
	uint8 FPoints = 0;
	UPROPERTY(BlueprintReadOnly, Category="Gameplay");
	uint8 FPointsSum = 0;
	
	
};

