// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/AdvancedDeleteWidget.h"

void SAdvancedDeleteTab::Construct(const FArguments &args)
{
    bCanSupportFocus = true;

    FSlateFontInfo TitleTestFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));
    TitleTestFont.Size = 30;

    ChildSlot
    [
        SNew(SVerticalBox)

        +SVerticalBox::Slot()
        .AutoHeight()
        [
            SNew(STextBlock)
            .Text(FText::FromString(TEXT("Advanced Delete")))
            .Font(TitleTestFont)
            .Justification(ETextJustify::Center)
            .ColorAndOpacity(FColor::White)
        ]

        +SVerticalBox::Slot()
        .AutoHeight()
        [
            SNew(SHorizontalBox)
        ]

        +SVerticalBox::Slot()
        .AutoHeight()
        [
            SNew(SScrollBox)
        ]

        +SVerticalBox::Slot()
        .AutoHeight()
        [
            SNew(SHorizontalBox)
        ]
    ];
}
