// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine.h"
#include "Templates/SharedPointer.h"

// Sets default values for this component's properties
UStateMachine::UStateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UStateMachine::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UStateMachine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentState->Update();
}

// void UStateMachine::Initialize_Implementation(UState *InitialState)
// {
// 	CurrentState = InitialState;

// 	if (CurrentState)
// 	{
// 		CurrentState->Enter();
// 	}
// }

void UStateMachine::ChangeState_Implementation(UState *NewState)
{
	if (CurrentState)
	{
		CurrentState->Exit();
	}

	CurrentState = NewState;

	if (CurrentState)
	{
		CurrentState->Enter();
	}
}

void UStateMachine::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyChangedEvent.Property && PropertyName == GET_MEMBER_NAME_CHECKED(UStateMachine, StatesNames))
	{




		States.Empty();
		int32 NumEnumValues = StatesNames->NumEnums() - 1;
		for (int32 Index = 0; Index < NumEnumValues; ++Index)
		{
			FString Name = StatesNames->GetDisplayNameTextByIndex(Index).ToString();
			UE_LOG(LogTemp, Log,  TEXT("MyVariable's value is: %s"), *Name);
			States.Add(Name, nullptr);
		} 
	}
}
