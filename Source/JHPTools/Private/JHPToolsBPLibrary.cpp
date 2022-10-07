// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "JHPToolsBPLibrary.h"
#include "JHPTools.h"

#include "Engine/Texture2D.h"

UJHPToolsBPLibrary::UJHPToolsBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

/*
CompressionSettings set to VectorDisplacementmap
MipGenSettings to NoMipmaps
SRGB to false
*/
void UJHPToolsBPLibrary::ColorChannelExchange(UTexture2D* targetTexture, const ERGBChannel R_ChannelTo, const ERGBChannel G_ChannelTo, const ERGBChannel B_ChannelTo)
{
#if WITH_EDITOR
	if (!targetTexture->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Warning, TEXT("texture invalid low level!"));
		return;
	}

	// save original settings of texture for bulk read.
	TextureCompressionSettings OldCompressionSettings = targetTexture->CompressionSettings;
	TextureMipGenSettings OldMipGenSettings = targetTexture->MipGenSettings;
	bool OldSRGB = targetTexture->SRGB;

	// change texture data to be readable
	targetTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	targetTexture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
	targetTexture->SRGB = false;
	targetTexture->UpdateResource();

	// get width & height
	const int32 width = targetTexture->GetSizeX();
	const int32 height = targetTexture->GetSizeY();

	TArray<FColor> textureRGBA;
	textureRGBA.Reserve(height * width);
	textureRGBA.SetNumZeroed(height * width);

	// texture data lock read & write
	FColor* FormatedImageData = (FColor*)(targetTexture->Source.LockMip(0));
	const int32 TextureDataSize = targetTexture->Source.CalcMipSize(0);

	for (int32 X = 0; X < width; X++)
	{
		for (int32 Y = 0; Y < height; Y++)
		{
			switch (R_ChannelTo)
			{
			case ERGBChannel::G: textureRGBA[Y * width + X].R = FormatedImageData[Y * width + X].G; break;
			case ERGBChannel::B: textureRGBA[Y * width + X].R = FormatedImageData[Y * width + X].B; break;
			}

			switch (G_ChannelTo)
			{
			case ERGBChannel::R: textureRGBA[Y * width + X].G = FormatedImageData[Y * width + X].R; break;
			case ERGBChannel::B: textureRGBA[Y * width + X].G = FormatedImageData[Y * width + X].B; break;
			}

			switch (B_ChannelTo)
			{
			case ERGBChannel::R: textureRGBA[Y * width + X].B = FormatedImageData[Y * width + X].R; break;
			case ERGBChannel::G: textureRGBA[Y * width + X].B = FormatedImageData[Y * width + X].G; break;
			}

			textureRGBA[Y * width + X].A = FormatedImageData[Y * width + X].A;
		}
	}

	// copy to target channel
	FMemory::Memcpy(FormatedImageData, textureRGBA.GetData(), TextureDataSize);

	//targetTexture->PlatformData->Mips[0].BulkData.Unlock();
	targetTexture->Source.UnlockMip(0);

	// revert changed settings for bulk read.
	targetTexture->CompressionSettings = OldCompressionSettings;
	targetTexture->MipGenSettings = OldMipGenSettings;
	targetTexture->SRGB = OldSRGB;
	targetTexture->UpdateResource();
	targetTexture->MarkPackageDirty();
#endif
}