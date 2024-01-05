// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/AdvancedDeleteWidget.h"

void SAdvancedDeleteTab::Construct(const FArguments &args)
{
    bCanSupportFocus = true;

    args._TestString;

    ChildSlot
    [
        SNew(STextBlock)
        .Text(FText::FromString(args._TestString))
    ];
}
