// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"

class SAdvancedDeleteTab : public SCompoundWidget 
{
	SLATE_BEGIN_ARGS(SAdvancedDeleteTab) {}

	SLATE_ARGUMENT(TArray< TSharedPtr<FAssetData> >, AssetsData)

	SLATE_END_ARGS()

public:
	void Construct(const FArguments &args);

private:
	TArray< TSharedPtr<FAssetData> > StoredAssetsData;

	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FAssetData> AssetData, const TSharedRef<STableViewBase>& OwnerTable);
	TSharedRef<SCheckBox> ConstructCheckBox(const TSharedPtr<FAssetData>& AssetData);
	TSharedRef<STextBlock> ConstructTextForRowWidget(const FString& Text, const FSlateFontInfo& Font);
	TSharedRef<SButton> ConstructButtonForRowWidget(const TSharedPtr<FAssetData> &AssetData);

	void OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData);
	FReply OnDeleteButtonClicked(TSharedPtr<FAssetData> AssetData);

#pragma region Getters/Setters
	FSlateFontInfo GetEmbossedTextFont() const { return FCoreStyle::Get().GetFontStyle(FName("EmbossedText")); }
#pragma endregion Getters/Setters

};