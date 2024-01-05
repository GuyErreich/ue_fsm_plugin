// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"

class SAdvancedDeleteTab : public SCompoundWidget 
{
	SLATE_BEGIN_ARGS(SAdvancedDeleteTab) {}

	SLATE_ARGUMENT(FString, TestString)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments &args);
};