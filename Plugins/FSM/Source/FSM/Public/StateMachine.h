// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State.h"
#include "Components/ActorComponent.h"
#include "StateMachine.generated.h"

UCLASS( ClassGroup=(FSM), meta=(BlueprintSpawnableComponent) )
class FSM_API UStateMachine : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateMachine();

private:
	UPROPERTY()
    UState* CurrentState;

	UPROPERTY(EditDefaultsOnly)
	UEnum* StatesNames;

	UPROPERTY(EditDefaultsOnly)
    TMap<FString, UObject*> States;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	// void Initialize(UState* InitialState);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void ChangeState(UState* NewState);

	// virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};