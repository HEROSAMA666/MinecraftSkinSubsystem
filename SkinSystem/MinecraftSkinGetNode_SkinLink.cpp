// Made by Chen aka Herosama

#include "MinecraftSkinSubsystem.h"
#include "Runtime/Core/Public/Misc/Base64.h"
#include "MinecraftSkinGetNode_SkinLink.h"

UMinecraftSkinGetNode_SkinLink* UMinecraftSkinGetNode_SkinLink::GetMinecraftPlayerSkinLink(const FString& MinecraftPlayerUUID, UObject* InWorldContextObject)
{
	UMinecraftSkinGetNode_SkinLink* MinecraftSkinGetNode_SkinLink = NewObject<UMinecraftSkinGetNode_SkinLink>();

	if(!MinecraftPlayerUUID.IsEmpty())
	{
		MinecraftSkinGetNode_SkinLink->SendPlayerSkinLinkRequest(MinecraftPlayerUUID,InWorldContextObject);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Sending Request"));
	}
	else
	{
		MinecraftSkinGetNode_SkinLink->OnPlayerSkinLinkFailure.Broadcast(TEXT("NONE"), TEXT("Player UUID is empty!"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Request failed sent"));
	}
	return MinecraftSkinGetNode_SkinLink;
}

void UMinecraftSkinGetNode_SkinLink::SendPlayerSkinLinkRequest(const FString& MinecraftPlayerUUID,UObject* InWorldContextObject)
{
	AddToRoot();

	if (InWorldContextObject)
	{
		WorldContextObject = InWorldContextObject; // Set the world context object
	}
	TSharedPtr<IHttpRequest> HttpRequest_SkinLink = FHttpModule::Get().CreateRequest();// Create a new HTTP request

	HttpRequest_SkinLink->SetURL(FString::Printf(TEXT("https://sessionserver.mojang.com/session/minecraft/profile/%s"), *MinecraftPlayerUUID)); // Set the URL for the request
	HttpRequest_SkinLink->SetVerb(TEXT("GET")); // Set the request method to GET
	HttpRequest_SkinLink->OnProcessRequestComplete().BindUObject(this, &UMinecraftSkinGetNode_SkinLink::OnPlayerSkinLinkRequestComplete); // Bind the response handler

	HttpRequest_SkinLink->ProcessRequest(); // Send the request
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Request sent"));
}

void UMinecraftSkinGetNode_SkinLink::OnPlayerSkinLinkRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if(bWasSuccessful && Response.IsValid())
	{
		FString JsonBody = Response->GetContentAsString(); // Get the response body as a string

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Response Success!"));
		
		UMinecraftSkinSubsystem* MinecraftSkinSubsystem = WorldContextObject->GetWorld()->GetGameInstance()->GetSubsystem<UMinecraftSkinSubsystem>();
		if (MinecraftSkinSubsystem)
		{
			FString RawBase64Data = GetBase64DataFromJson(JsonBody); // Get the base64 data from the JSON response
			FString MinecraftPlayerSkinLinkJson = base64_Decode(RawBase64Data); // Decode the base64 data
			FString SkinLink = GetSkinLinkFromJson(MinecraftPlayerSkinLinkJson); // Get the skin link from the decoded JSON

			MinecraftSkinSubsystem->LocalMinecraftPlayerInfo.SkinURL = SkinLink;

			OnPlayerSkinLinkSuccess.Broadcast(MinecraftSkinSubsystem->LocalMinecraftPlayerInfo.SkinURL, TEXT("Link Got!")); // Broadcast the UUID

			RemoveFromRoot(); // Clean object
		}

	}
}

FString UMinecraftSkinGetNode_SkinLink::base64_Decode(const FString& base64String)
{
	FString OutMinecraftPlayerSkinLink;

	FBase64::Decode(base64String, OutMinecraftPlayerSkinLink);

	return OutMinecraftPlayerSkinLink;
}

FString UMinecraftSkinGetNode_SkinLink::GetBase64DataFromJson(const FString& RawJsonData)
{
	FString Base64Data;

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TSharedRef< TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(RawJsonData);

	TArray< TSharedPtr<FJsonValue> > JsonValueDeliver;

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject)) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Deserialize success"));

		JsonValueDeliver= JsonObject->GetArrayField(TEXT("properties")); // Get the properties field from the JSON object
		Base64Data = JsonValueDeliver[0]->AsObject()->GetStringField(TEXT("value")); // Get the first item in that array,usually be the data we want
		
		return Base64Data;
	}
	return TEXT("NONE");
} // This return the base64 crypted data used to decode the skin link

FString UMinecraftSkinGetNode_SkinLink::GetSkinLinkFromJson(const FString& RawJsonData)
{
	FString SkinLink;

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TSharedRef< TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(RawJsonData);

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		SkinLink = JsonObject->GetObjectField(TEXT("textures"))->GetObjectField(TEXT("SKIN"))->GetStringField(TEXT("url")); // Get the skin link from the JSON object
	}

	return SkinLink; // This function returns the skin link used to download the skin
}
