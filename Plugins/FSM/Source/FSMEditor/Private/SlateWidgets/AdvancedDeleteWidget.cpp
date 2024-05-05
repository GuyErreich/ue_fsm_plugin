// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/AdvancedDeleteWidget.h"
#include "DebugHeader.h"

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
    if(!AssetData.IsValid()) return SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable);

    const FString AssetName = AssetData->AssetName.ToString();
    const FString AssetClass = AssetData->GetClass()->GetName();

    FSlateFontInfo AssetClassNameFont = SAdvancedDeleteTab::GetEmbossedTextFont();
    AssetClassNameFont.Size = 10;

    FSlateFontInfo AssetNameFont = SAdvancedDeleteTab::GetEmbossedTextFont();
    AssetNameFont.Size = 16;

    TSharedRef<STableRow<TSharedPtr<FAssetData>>> ListViewRowWidget = 
    SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable)
    .Padding(FMargin(5.f))
    [
        SNew(SHorizontalBox)

        //First slot for checkbox
        +SHorizontalBox::Slot()
        .HAlign(HAlign_Left)
        .VAlign(VAlign_Center)
        .FillWidth(0.05f)
        [
            ConstructCheckBox(AssetData)
        ]

        //Second slot for displaying asset class name
        +SHorizontalBox::Slot()
        .HAlign(HAlign_Center)
        .VAlign(VAlign_Center)
        .FillWidth(0.5f)
        [
            ConstructTextForRowWidget(AssetClass, AssetClassNameFont)
        ]

        //Third slot for displaying asset name
        +SHorizontalBox::Slot()
        .HAlign(HAlign_Left)
        .VAlign(VAlign_Fill)
        [
            ConstructTextForRowWidget(AssetName, AssetNameFont)
        ]

        //Forth slot for a button
        +SHorizontalBox::Slot()
        .HAlign(HAlign_Right)
        .VAlign(VAlign_Fill)
        [
            ConstructButtonForRowWidget(AssetData)
        ]

    ];

    return ListViewRowWidget;
}

TSharedRef<SCheckBox> SAdvancedDeleteTab::ConstructCheckBox(const TSharedPtr<FAssetData>& AssetData)
{
    return SNew(SCheckBox)
    .Type(ESlateCheckBoxType::Type::CheckBox)
    .OnCheckStateChanged(this, &SAdvancedDeleteTab::OnCheckBoxStateChanged, AssetData)
    .Visibility(EVisibility::Visible);
}

TSharedRef<STextBlock> SAdvancedDeleteTab::ConstructTextForRowWidget(const FString& Text, const FSlateFontInfo& Font)
{
    return SNew(STextBlock)
    .Text(FText::FromString(Text))
    .Font(Font)
    .ColorAndOpacity(FColor::White);
}

TSharedRef<SButton> SAdvancedDeleteTab::ConstructButtonForRowWidget(const TSharedPtr<FAssetData>& AssetData)
{
    return  SNew(SButton)
    .Text(FText::FromString(TEXT("Delete")))
    .OnClicked(this, &SAdvancedDeleteTab::OnDeleteButtonClicked, AssetData);
}

void SAdvancedDeleteTab::OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData)
{
    switch (NewState)
    {
    case ECheckBoxState::Checked:
        DebugHeader::ShowNotifyInfo(AssetData->AssetName.ToString() + TEXT(" is checked"));
        break;
    case ECheckBoxState::Unchecked:
        DebugHeader::ShowNotifyInfo(AssetData->AssetName.ToString() + TEXT(" is unchecked"));
        break;
    case ECheckBoxState::Undetermined:
        break;
    default:
        break;
    }
}

FReply SAdvancedDeleteTab::OnDeleteButtonClicked(TSharedPtr<FAssetData> AssetData)
{
    DebugHeader::ShowNotifyInfo(AssetData->AssetName.ToString() + TEXT(" is clicked"));

    return FReply::Handled();
}
