// Fill out your copyright notice in the Description page of Project Settings.

#include "BatchRenaming.h"
#include "EditorUtilityLibrary.h"

#include "Engine/Texture2D.h"

#include "JHPToolsBPLibrary.h"

void UBatchRenaming::RenameSelectedAssets(FString SearchPattern, FString ReplacePattern, ESearchCase::Type SearchCase)
{
	if (SearchPattern.IsEmpty() || SearchPattern.Equals(ReplacePattern, SearchCase))
	{
		return;
	}

	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();

	for (UObject* SelectedObject : SelectedObjects)
	{
		FString AssetName = SelectedObject->GetName();
		if (AssetName.Contains(SearchPattern, SearchCase))
		{
			FString NewName = AssetName.Replace(*SearchPattern, *ReplacePattern, SearchCase);
			UEditorUtilityLibrary::RenameAsset(SelectedObject, NewName);
		}
	}
}

void URGBChannelConvert::RGBChannelConvert(const ERGBChannel R_ChannelTo, const ERGBChannel G_ChannelTo, const ERGBChannel B_ChannelTo)
{
	TArray<UObject*> SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();

	for (UObject* selectedObj : SelectedObjects)
	{
		UTexture2D* newTexture = Cast<UTexture2D>(selectedObj);

		if (newTexture->IsValidLowLevel())
		{
			UJHPToolsBPLibrary::ColorChannelExchange(newTexture, R_ChannelTo, G_ChannelTo, B_ChannelTo);
		}
	}

	
}
