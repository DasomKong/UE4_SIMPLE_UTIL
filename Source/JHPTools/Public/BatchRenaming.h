// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"

#include "DataTypes.h"

#include "BatchRenaming.generated.h"

/**
 * 
 */
class UTexture2D;

UCLASS()
class JHPTOOLS_API UBatchRenaming : public UAssetActionUtility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(CallInEditor)
	void RenameSelectedAssets(FString SearchPattern, FString ReplacePattern, ESearchCase::Type SearchCase);
};

UCLASS()
class JHPTOOLS_API URGBChannelConvert : public UAssetActionUtility
{
	GENERATED_BODY()

public:
	UFUNCTION(CallInEditor)
		void RGBChannelConvert(const ERGBChannel R_ChannelTo, const ERGBChannel G_ChannelTo, const ERGBChannel B_ChannelTo);
};