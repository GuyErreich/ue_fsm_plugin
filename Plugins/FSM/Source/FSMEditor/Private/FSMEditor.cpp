// Copyright Epic Games, Inc. All Rights Reserved.

#include "FSMEditor.h"
#include "ContentBrowserModule.h"
#include "DebugHeader.h"
#include "AssetUtils.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"
#include "AssetAction/QuickAssetAction.h"
#include "SlateWidgets/AdvancedDeleteWidget.h"

#define LOCTEXT_NAMESPACE "FFSMEditorModule"

void FFSMEditorModule::StartupModule()
{
	InitCBMenuExtention();

	RegisterAdvancedDeleteTab();
}

void FFSMEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#pragma region ContentBrowserMenuExtension

void FFSMEditorModule::InitCBMenuExtention()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	TArray<FContentBrowserMenuExtender_SelectedPaths>& ContentBrowserModuleMenuExtenders = ContentBrowserModule.GetAllPathViewContextMenuExtenders();

	// FContentBrowserMenuExtender_SelectedPaths CustomCBMenuDelegate;
	// CustomCBMenuDelegate.BindRaw(this, &FFSMEditorModule::CustomCBMenuExtender);

	// ContentBrowserModuleMenuExtenders.Add(CustomCBMenuDelegate);

	ContentBrowserModuleMenuExtenders.Add(FContentBrowserMenuExtender_SelectedPaths::CreateRaw(this, &FFSMEditorModule::CustomCBMenuExtender));
}

TSharedRef<FExtender> FFSMEditorModule::CustomCBMenuExtender(const TArray<FString> &SelectedPaths)
{
	TSharedRef<FExtender> MenuExtender (new FExtender());

	if (SelectedPaths.Num() > 0)
	{
		MenuExtender->AddMenuExtension(
			"Delete", 
			EExtensionHook::After, 
			TSharedPtr<FUICommandList>(),
			FMenuExtensionDelegate::CreateRaw(this, &FFSMEditorModule::AddCBMenuEntry)
		);

		FolderPathsSelected = SelectedPaths;
	}
    return MenuExtender;
}

void FFSMEditorModule::AddCBMenuEntry(FMenuBuilder &MenuBuilder)
{
	MenuBuilder.AddMenuEntry(
		FText::FromString("Delete Unused Assets"),
		FText::FromString("Safely delete all unused assets under folder"),
		FSlateIcon(),
		FExecuteAction::CreateRaw(this, &FFSMEditorModule::OnDeleteUnusedAssetsButtonClicked)
	);

	MenuBuilder.AddMenuEntry(
		FText::FromString("Delete Empty Folders"),
		FText::FromString("Safely delete all empty folder"),
		FSlateIcon(),
		FExecuteAction::CreateRaw(this, &FFSMEditorModule::OnDeleteEmptyFoldersButtonClicked)
	);

	MenuBuilder.AddMenuEntry(
		FText::FromString("Advanced Delete"),
		FText::FromString("Delete files with UI"),
		FSlateIcon(),
		FExecuteAction::CreateRaw(this, &FFSMEditorModule::OnAdvancedDeleteButtonClicked)
	);
}

void FFSMEditorModule::OnDeleteUnusedAssetsButtonClicked()
{
	if (FolderPathsSelected.Num() > 1)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, "You can only do this to one folder");
		return;
	}

	TArray<FString> AssetsPathNames = UEditorAssetLibrary::ListAssets(FolderPathsSelected[0]);

	if (AssetsPathNames.Num() == 0)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, "No assets found");
		return;
	}

	EAppReturnType::Type Response = DebugHeader::ShowMsgDialog(EAppMsgType::YesNo, "A total of " + FString::FromInt(AssetsPathNames.Num()) + " assets found. \nWould you like to proceed?");

	if (Response == EAppReturnType::No) return;

	AssetUtils::FixUpRedirectors();

	TArray<FAssetData> UnusedAssetsData;

	for (const FString& AssetPathName : AssetsPathNames)
	{
		//Don't touch root folder
		if (AssetPathName.Contains(TEXT("Developers")) || 
			AssetPathName.Contains(TEXT("Collections")) ||
			AssetPathName.Contains(TEXT("__ExternalActors__")) ||
			AssetPathName.Contains(TEXT("__ExternalObjects__")))
		{
			continue;
		}

		if (!UEditorAssetLibrary::DoesAssetExist(AssetPathName)) continue;

		TArray<FString> AssetReferencers = UEditorAssetLibrary::FindPackageReferencersForAsset(AssetPathName);

		if (AssetReferencers.Num() == 0)
		{
			const FAssetData UnusedAssetData = UEditorAssetLibrary::FindAssetData(AssetPathName);
			UnusedAssetsData.Add(UnusedAssetData);
		}
	}

	if (UnusedAssetsData.Num() == 0)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, "No unused assets found");
		return;
	}

	ObjectTools::DeleteAssets(UnusedAssetsData);
}

void FFSMEditorModule::OnDeleteEmptyFoldersButtonClicked()
{
	AssetUtils::FixUpRedirectors();

	TArray<FString> Folders = UEditorAssetLibrary::ListAssets(FolderPathsSelected[0], true, true);
	uint32 Counter = 0;

	// FString EmptyFolderPathNames;
	TArray<FString> EmptyFolders;

	for (const FString& Folder : Folders)
	{
		if (Folder.Contains(TEXT("Developers")) || 
			Folder.Contains(TEXT("Collections")) ||
			Folder.Contains(TEXT("__ExternalActors__")) ||
			Folder.Contains(TEXT("__ExternalObjects__")))
		{
			continue;
		}

		if(!UEditorAssetLibrary::DoesDirectoryExist(Folder)) continue;

		if(!UEditorAssetLibrary::DoesDirectoryHaveAssets(Folder)) EmptyFolders.Add(Folder);
	}

	if (EmptyFolders.Num() == 0)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, "No empty folders were found", false);
		return;
	}

	EAppReturnType::Type Response = DebugHeader::ShowMsgDialog(
		EAppMsgType::OkCancel, 
		"Empty folders found:\n\t- " + FString::Join(EmptyFolders, TEXT("\n\t- ")) + "\n\nWould you like to proceed?\n\n",
		false
	);

	if (Response == EAppReturnType::Cancel) return;

	for (const FString& EmptyFolder : EmptyFolders)
	{
		if (UEditorAssetLibrary::DeleteDirectory(EmptyFolder))
			++Counter;
		else
			DebugHeader::PrintLog("Failed to delete " + EmptyFolder);
	}

	if (Counter > 0)
	{
		DebugHeader::ShowNotifyInfo("Successfully deleted " + FString::FromInt(Counter) + "/" + FString::FromInt(EmptyFolders.Num()) + " empty folders");
	}
}

void FFSMEditorModule::OnAdvancedDeleteButtonClicked()
{
	FGlobalTabmanager::Get() -> TryInvokeTab(FName("AdvancedDelete"));
}

#pragma endregion


#pragma region CustomEditorTab

void FFSMEditorModule::RegisterAdvancedDeleteTab()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		FName("AdvancedDelete"),
		FOnSpawnTab::CreateRaw(this, &FFSMEditorModule::OnSpawnAdvancedDeleteTab)
	).SetDisplayName(FText::FromString("Advanced Delete"));
}


TSharedRef<SDockTab> FFSMEditorModule::OnSpawnAdvancedDeleteTab(const FSpawnTabArgs &args)
{
	return
	SNew(SDockTab).TabRole(ETabRole::NomadTab)
	[
		SNew(SAdvancedDeleteTab)
		.AssetsData(GetAllAssetDataUnderSelectedFolder())
	];
}

TArray<TSharedPtr<FAssetData>> FFSMEditorModule::GetAllAssetDataUnderSelectedFolder()
{
	TArray<TSharedPtr<FAssetData>> AvailableAssetData;

	TArray<FString> AssetsPathNames = UEditorAssetLibrary::ListAssets(FolderPathsSelected[0]);

	for (const FString& AssetPathName : AssetsPathNames)
	{
		//Don't touch root folder
		if (AssetPathName.Contains(TEXT("Developers")) || 
			AssetPathName.Contains(TEXT("Collections")) ||
			AssetPathName.Contains(TEXT("__ExternalActors__")) ||
			AssetPathName.Contains(TEXT("__ExternalObjects__")))
		{
			continue;
		}

		if (!UEditorAssetLibrary::DoesAssetExist(AssetPathName)) continue;

		const FAssetData Data = UEditorAssetLibrary::FindAssetData(AssetPathName);

		AvailableAssetData.Add(MakeShared<FAssetData>(Data));
	}

	return AvailableAssetData;
}

#pragma endregion
#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFSMEditorModule, FSMEditor)