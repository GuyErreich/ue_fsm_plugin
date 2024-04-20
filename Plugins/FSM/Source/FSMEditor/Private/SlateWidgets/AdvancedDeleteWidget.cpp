// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/AdvancedDeleteWidget.h"

void SAdvancedDeleteTab::Construct(const FArguments &args)
{
    bCanSupportFocus = true;

    StoredAssetsData = args._AssetsData;

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

            +SScrollBox::Slot() 
            [
                SNew(SListView<TSharedPtr<FAssetData>>)
                .ItemHeight(24.f)
                .ListItemsSource(&StoredAssetsData)
                .OnGenerateRow(this, &SAdvancedDeleteTab::OnGenerateRowForList)
            ]
        ]

        +SVerticalBox::Slot()
        .AutoHeight()
        [
            SNew(SHorizontalBox)
        ]
    ];
}

TSharedRef<ITableRow> SAdvancedDeleteTab::OnGenerateRowForList(TSharedPtr<FAssetData> AssetData, const TSharedRef<STableViewBase> &OwnerTable)
{
    const FString DisplayAssetName = AssetData->AssetName.ToString();

    TSharedRef<STableRow<TSharedPtr<FAssetData>>> ListViewRowWidget = 
    SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable)
    [
        SNew(STextBlock)
        .Text(FText::FromString(DisplayAssetName))
    ];

    return ListViewRowWidget;
}
