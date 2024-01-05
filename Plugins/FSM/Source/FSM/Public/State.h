// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "State.generated.h"


UCLASS(Blueprintable, ClassGroup=FSM, meta=(BlueprintSpawnableComponent))
class FSM_API UState : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Enter();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Update();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Exit();
};
