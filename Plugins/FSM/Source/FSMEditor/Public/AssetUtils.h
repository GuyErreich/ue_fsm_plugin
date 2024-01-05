#pragma once

#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetToolsModule.h"

namespace AssetUtils
{
    static void FixUpRedirectors() 
    {
        TArray<UObjectRedirector*> redirectors;

        FAssetRegistryModule& assetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

        FARFilter filter;
        filter.bRecursivePaths = true;
        filter.PackagePaths.Emplace("/Game");
        filter.ClassPaths.Emplace("ObjectRedirector");

        TArray<FAssetData> outRedirectors;

        assetRegistryModule.Get().GetAssets(filter, outRedirectors);

        for (const FAssetData& rediractorData : outRedirectors)
        {
            if (UObjectRedirector* redirector = Cast<UObjectRedirector>(rediractorData.GetAsset()))
            {
                redirectors.Add(redirector);
            }
        }

        FAssetToolsModule& assetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

        assetToolsModule.Get().FixupReferencers(redirectors);
    }
}