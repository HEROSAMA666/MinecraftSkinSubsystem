// Made by Chen aka Herosama

#include "Engine/TextureRenderTarget2D.h"
#include "ImageUtils.h"
#include "IImageWrapper.h"  
#include "IImageWrapperModule.h"  
#include "Runtime/Engine/Classes/Engine/Texture2DDynamic.h"
#include "MinecraftSkinSubsystem.h"

bool UMinecraftSkinSubsystem::SaveSkinImage_RenderTarget(UTextureRenderTarget2D* RenderTargetToSave, const FString& FileDestination, const FString& FileName)
{
	FTextureRenderTargetResource* rtResource = RenderTargetToSave->GameThread_GetRenderTargetResource(); // Get the render target resource
	//FReadSurfaceDataFlags readPixelFlags(RCM_UNorm); // Set the read pixel format to UNorm

	TArray<FColor> outBMP; // Create an array to hold the pixel data
	outBMP.AddUninitialized(RenderTargetToSave->GetSurfaceWidth() * RenderTargetToSave->GetSurfaceHeight()); // Prepare the array size

	rtResource->ReadPixels(outBMP); // Read the pixel data from the render target,data saved in outBMP

	FIntPoint destSize(RenderTargetToSave->GetSurfaceWidth(), RenderTargetToSave->GetSurfaceHeight()); // Get the size of the render target
	TArray64<uint8> CompressedBitmap; // Container for the binary image data

	FImageUtils::PNGCompressImageArray(destSize.X, destSize.Y, outBMP, CompressedBitmap); // Compress the image data from outBMP into CompressedBitmap

	TStringBuilder<256> Builder;
	Builder.Append(FileDestination); // Build the file path
	Builder.Append("/");
	Builder.Append(FileName); // Append the file name to the path

	bool ImageSavedOk = FFileHelper::SaveArrayToFile(CompressedBitmap, Builder.ToString()); // Save the compressed image data to a file

	return ImageSavedOk;

} //  Copy from https://blog.csdn.net/lunweiwangxi3/article/details/87457594 ,and leaved some comments

UTexture2D* UMinecraftSkinSubsystem::TryGetMinecraftSkinFromName(const FString PlayerName)
{
	TStringBuilder<256> Builder_ImagePath;
	Builder_ImagePath.Append(FPaths::ProjectSavedDir());
	Builder_ImagePath.Append("/");
	Builder_ImagePath.Append(FString::Printf(TEXT("%s.png"),*PlayerName));

	if(!FPlatformFileManager::Get().GetPlatformFile().FileExists(Builder_ImagePath.ToString()))
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("not exist  %s"));
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Builder_ImagePath.ToString());
		return nullptr; // File does not exist
	}

	TArray<uint8> ImageRawData; // Create an array to hold the raw image data

	if (!FFileHelper::LoadFileToArray(ImageRawData, Builder_ImagePath.ToString()))
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("load file to array failed"));
		return nullptr; // Failed to load the file into the array
	}

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG); // Create an image wrapper for PNG format,since the skin file we downloaded from Official API is PNG 

	if(ImageWrapper.IsValid()&& ImageWrapper->SetCompressed(ImageRawData.GetData(), ImageRawData.Num()))
	{
		TArray<uint8> UncompressedRGBA; // Create an array to hold the uncompressed RGBA data from the ImageRawData
		UTexture2D* TextureContainer = nullptr; // Initialize the texture container

		if (ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, UncompressedRGBA)) // Check if we can get the raw image data in RGBA format
		{
			TextureContainer = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_R8G8B8A8); // Create a temp texture based on the image size we got from the disk
			
			void* TextureData = TextureContainer->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE); // Filled the new temp texture pixel with the data
			FMemory::Memcpy(TextureData, UncompressedRGBA.GetData(), UncompressedRGBA.Num()); // I don't really fimiliar with those functions. If you are reading this you could search some documentation about them
			TextureContainer->GetPlatformData()->Mips[0].BulkData.Unlock();


			TextureContainer->Filter = TF_Nearest; // Set the texture filter to Nearest so the material will be seen as clean.All Minecraft texture should set this paramret to TF_Nearest
			TextureContainer->SRGB = false;
			TextureContainer->CompressionSettings = TC_HDR;

			TextureContainer->UpdateResource();

			return TextureContainer; // Successfully loaded the skin texture
		}
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Get raw data failed"));
		return nullptr; // Failed to get raw image data
	}
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ImageWrapper is not valid or failed to set compressed data."));
	return nullptr;
}

