// Made by Chen aka Herosama

#include "MinecraftSkinSubsystem.h"
#include "MinecraftSkinGetNodes.h"


UMinecraftSkinGetNodes* UMinecraftSkinGetNodes::GetMinecraftPlayerUUID(const FString& MinecraftPlayerName, UObject* InWorldContextObject)
{
	UMinecraftSkinGetNodes* MinecraftSkinGetNodes = NewObject<UMinecraftSkinGetNodes>();

	if(!MinecraftPlayerName.IsEmpty())
	{
		MinecraftSkinGetNodes->SendUUIDRequest(MinecraftPlayerName,InWorldContextObject);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Sending Request"));
	}// Successfully send requst
	else
	{
		MinecraftSkinGetNodes->OnUUIDFailure.Broadcast(TEXT("None"), TEXT("Player name is empty!,Failed"));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Request failed sent"));
	}// Failed to send request
	return MinecraftSkinGetNodes;
}


void UMinecraftSkinGetNodes::SendUUIDRequest(const FString& MinecraftPlayerName, UObject* InWorldContextObject)
{
	AddToRoot(); // In case not got GC
	if(InWorldContextObject)
	{
		WorldContextObject = InWorldContextObject; // Set the world context object
	}
	TSharedPtr<IHttpRequest> HttpRequest_UUID = FHttpModule::Get().CreateRequest();// Create a new HTTP request

	HttpRequest_UUID->SetURL(FString::Printf(TEXT("https://api.mojang.com/users/profiles/minecraft/%s"),*MinecraftPlayerName));
	HttpRequest_UUID->SetVerb("GET");
	HttpRequest_UUID->OnProcessRequestComplete().BindUObject(this, &UMinecraftSkinGetNodes::OnUUIDRequestComplete);// Call back function
	// Set request parameters

	HttpRequest_UUID->ProcessRequest();
}


void UMinecraftSkinGetNodes::OnUUIDRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if(bWasSuccessful && Response.IsValid())
	{
		FString JsonBody = Response->GetContentAsString(); // Get the response body as a string

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		TSharedRef< TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(JsonBody);

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Response Success!"));
		if(FJsonSerializer::Deserialize(JsonReader,JsonObject))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Deserialize success"));
			UMinecraftSkinSubsystem* MinecraftSkinSubsystem = WorldContextObject->GetWorld()->GetGameInstance()->GetSubsystem<UMinecraftSkinSubsystem>();
			if(MinecraftSkinSubsystem)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Get UUID Success!"));

				MinecraftSkinSubsystem->LocalMinecraftPlayerInfo.PlayerName = JsonObject->GetStringField(TEXT("name")); // Save name to local player name form subsystem
				MinecraftSkinSubsystem->LocalMinecraftPlayerInfo.MinecraftPlayerUUID = JsonObject->GetStringField(TEXT("id")); // Save UUID to local player UUID form subsystem

				OnUUIDSuccess.Broadcast(MinecraftSkinSubsystem->LocalMinecraftPlayerInfo.MinecraftPlayerUUID, TEXT("UUID Got!")); // Broadcast the UUID
			}
		}
	}
	else
	{
		OnUUIDFailure.Broadcast(TEXT("None"), TEXT("Http request failed or response is not valid"));
	} // Failed to get UUID
	RemoveFromRoot(); // Clean in next GC
}

