// Copyright Epic Games, Inc. All Rights Reserved.

#include "AckermannsRoulette.h"
#include "AckermannsRouletteStyle.h"
#include "AckermannsRouletteCommands.h"
#include "ARDataStruct.h"
#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "LevelEditor.h"
#include "ResponseStruct.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "ToolMenus.h"
#include "SARSlateWidget.h"
#include "Engine/AssetManager.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StreamableManager.h"
#include "Interfaces/IHttpResponse.h"

static const FName AckermannsRouletteTabName("AckermannsRoulette");

#define LOCTEXT_NAMESPACE "FAckermannsRouletteModule"

void FAckermannsRouletteModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FAckermannsRouletteStyle::Initialize();
	FAckermannsRouletteStyle::ReloadTextures();

	FAckermannsRouletteCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FAckermannsRouletteCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FAckermannsRouletteModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FAckermannsRouletteModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(AckermannsRouletteTabName, FOnSpawnTab::CreateRaw(this, &FAckermannsRouletteModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FAckermannsRouletteTabTitle", "AckermannsRoulette"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FAckermannsRouletteModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FAckermannsRouletteStyle::Shutdown();

	FAckermannsRouletteCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(AckermannsRouletteTabName);
}

TSharedRef<SDockTab> FAckermannsRouletteModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FAckermannsRouletteModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("AckermannsRoulette.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SARSlateWidget)
				.OnGenerateButtonClicked(FOnClicked::CreateRaw(this, &FAckermannsRouletteModule::GetRandomNumber))
				.OnDTSelected(FOnDTSelected::CreateRaw(this, &FAckermannsRouletteModule::OnDataTableSelected))
			]
		];
}

FReply FAckermannsRouletteModule::GetRandomNumber()
{
	if (StaticMeshArray.Num() == 0)
	{
		RandomNumber = -1;
		return FReply::Unhandled();
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	FString min = "0";
	FString max = FString::FromInt(StaticMeshArray.Num() - 1);
	FString count = "1";

	FString URL = "http://www.randomnumberapi.com/api/v1.0/random?min=" + min +"&max=" + max + "&count=" + count;

	Request->OnProcessRequestComplete().BindRaw(this, &FAckermannsRouletteModule::OnRandomNumberAPIResponceReceived);
	Request->SetURL(URL);
	Request->SetVerb("GET");
	Request->ProcessRequest();

	return FReply::Handled();
}

void FAckermannsRouletteModule::OnRandomNumberAPIResponceReceived(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bConnectedSuccessfully)
{
	// The API response is a plain String value,
	// I want to use in-built Json to Struct converter instead of writing a custom parser,
	// So at below a Json string constructed with Response String to convert FResponseStruct
	FString ResponsStr = "{ \"RandomArray\":" + Response->GetContentAsString() +" }";
	//UE_LOG(LogTemp, Warning, TEXT("Response %s, Getcontent type: %s"), *ResponsStr, *Response->GetContentType());
	
	FResponseStruct ResponseStruct;
	if (FJsonObjectConverter::JsonObjectStringToUStruct<FResponseStruct>(ResponsStr, &ResponseStruct))
	{
		RandomNumber = !ResponseStruct.RandomArray.IsEmpty() ? ResponseStruct.RandomArray[0] : -1;

		AddStaticMeshToWorld();
	}
}

void FAckermannsRouletteModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(AckermannsRouletteTabName);
}

void FAckermannsRouletteModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FAckermannsRouletteCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FAckermannsRouletteCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

void FAckermannsRouletteModule::OnDataTableSelected(FSoftObjectPath SoftObjectPath)
{
	DTStaticMesh = TSoftObjectPtr<UDataTable>(SoftObjectPath);

	TArray<FSoftObjectPath> ItemsToStream;
	ItemsToStream.Add(DTStaticMesh.ToSoftObjectPath());
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(ItemsToStream, FStreamableDelegate::CreateRaw(this, &FAckermannsRouletteModule::OnDataTableLoaded));
}

void FAckermannsRouletteModule::OnDataTableLoaded()
{
	StaticMeshArray.Empty();
	if (DTStaticMesh.Get() && DTStaticMesh->GetRowStruct() == FARDataStruct::StaticStruct())
	{
		FString Context;
		DTStaticMesh.Get()->GetAllRows<FARDataStruct>(Context, StaticMeshArray);

		UE_LOG(LogTemp, Warning, TEXT("DAta count: %d"), StaticMeshArray.Num());
	}
}

void FAckermannsRouletteModule::AddStaticMeshToWorld()
{
	if (RandomNumber == -1) return;
	if (StaticMeshArray.Num() == 0) return;

	const auto Mesh = StaticMeshArray[RandomNumber]->StaticMesh;

	if (Mesh.Get())
	{
		SpawnStaticMeshActor(Mesh);
		return;
	}

	TArray<FSoftObjectPath> ItemsToStream;
	ItemsToStream.Add(Mesh.ToSoftObjectPath());
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(ItemsToStream, FStreamableDelegate::CreateRaw(this, &FAckermannsRouletteModule::OnStaticMeshLoaded));

}

void FAckermannsRouletteModule::OnStaticMeshLoaded()
{
	SpawnStaticMeshActor(StaticMeshArray[RandomNumber]->StaticMesh);
}

void FAckermannsRouletteModule::SpawnStaticMeshActor(TSoftObjectPtr<UStaticMesh> Mesh)
{
	if (!Mesh) return;

	UWorld* World = GEditor->GetEditorWorldContext().World();

	if (AStaticMeshActor* MeshActor = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass()))
	{
		if (Mesh.Get())
		{
			MeshActor->GetStaticMeshComponent()->SetStaticMesh(Mesh.Get());
			MeshActor->SetActorLocation(FVector(0.f, 0.f, 60.f));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("MESH ACTOR COULD NOT LOAD!"));
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAckermannsRouletteModule, AckermannsRoulette)