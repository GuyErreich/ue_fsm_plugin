// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetAction/QuickAssetAction.h"
#include "DebugHeader.h"
#include "AssetUtils.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"

void UQuickAssetAction::DuplicateAsset(int32 numOfDuplicates)
{
   if (numOfDuplicates <= 0)
   {
      DebugHeader::ShowMsgDialog(EAppMsgType::Ok, "Please enter a valid number", true);
      return;
   }
   
   TArray<FAssetData> selectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();
   uint32 counter = 0;

   for (const FAssetData& selectedAssetData : selectedAssetsData) {
      for (int32 i = 0; i < numOfDuplicates; i++)
      {
         const FString sourceAssetPath = selectedAssetData.GetObjectPathString();
         const FString newDuplicatedAssetName = selectedAssetData.AssetName.ToString() + "_" + FString::FromInt(i + 1);
         const FString newPathName = FPaths::Combine(selectedAssetData.PackagePath.ToString(), newDuplicatedAssetName);

         if(UEditorAssetLibrary::DuplicateAsset(sourceAssetPath, newPathName))
         {
            UEditorAssetLibrary::SaveAsset(newPathName, false);
            ++counter;
         }
      }
   }

   if (counter > 0)
      DebugHeader::ShowNotifyInfo("Successfully duplicated asset " + FString::FromInt(counter) + " files");
}

void UQuickAssetAction::AddPrefixes()
{

   TArray<UObject*> selectedObjects = UEditorUtilityLibrary::GetSelectedAssets();
   uint32 counter = 0;

   for (UObject* selectedObject : selectedObjects)
   {
      if(!selectedObject) continue;

      FString* prefixFound = prefixMap.Find(selectedObject->GetClass());

      if(!prefixFound || prefixFound->IsEmpty())
      {
         DebugHeader::ShowMsgDialog(EAppMsgType::Ok, "Failed to find prefix for class " + selectedObject->GetClass()->GetName(), true);
         continue;
      }

      FString oldName = selectedObject->GetName();

      if (oldName.StartsWith(*prefixFound))
      {
         DebugHeader::ShowNotifyInfo(oldName + " already has prefix added");
         continue;
      }

      int32 separatorIndex;
      if (oldName.FindChar('_', separatorIndex)) {
         FString currentPrefix = oldName.Mid(0, separatorIndex + 1);
         TArray<FString> values;
         prefixMap.GenerateValueArray(values);
         if (values.Contains(currentPrefix))
            oldName.RemoveFromStart(currentPrefix);
      }

      if (oldName.Find("_Inst", ESearchCase::CaseSensitive ,ESearchDir::FromEnd))
         oldName.RemoveFromEnd("_Inst");

      const FString newNameWithPrefix = *prefixFound + oldName;

      UEditorUtilityLibrary::RenameAsset(selectedObject, newNameWithPrefix);

      ++counter;
   }

   if (counter > 0)
      DebugHeader::ShowNotifyInfo("Successfully renamed " + FString::FromInt(counter) + " assets");
}

void UQuickAssetAction::RemoveUnusedAssets()
{
   TArray<FAssetData> selectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();
   TArray<FAssetData> unusedAssetsData;

   AssetUtils::FixUpRedirectors();

   for (const FAssetData& selectedAssetData : selectedAssetsData)
   {
      
      int32 length = UEditorAssetLibrary::FindPackageReferencersForAsset(selectedAssetData.GetObjectPathString()).Num();

      if (length == 0)
         unusedAssetsData.Add(selectedAssetData);
   }

   if (unusedAssetsData.Num() == 0)
   {
      DebugHeader::ShowMsgDialog(EAppMsgType::Ok, "No unused asset found among selected assets", false);
      return;
   }

   bool isDeleted = ObjectTools::DeleteAssets(unusedAssetsData) == 0 ? false : true;

   if (!isDeleted)
   {
      DebugHeader::ShowMsgDialog(EAppMsgType::Ok, "No unused asset was deleted", false);
      return;
   }

   DebugHeader::ShowNotifyInfo("All unused assets have been deleted");
}
