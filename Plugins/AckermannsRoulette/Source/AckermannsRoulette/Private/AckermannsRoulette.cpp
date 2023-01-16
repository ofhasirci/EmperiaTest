// Copyright Epic Games, Inc. All Rights Reserved.

#include "AckermannsRoulette.h"
#include "AckermannsRouletteStyle.h"
#include "AckermannsRouletteCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "ToolMenus.h"
#include "SARSlateWidget.h"
#include "Engine/AssetManager.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StreamableManager.h"

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

	MeshPtr = TSoftObjectPtr<UStaticMesh>(FSoftObjectPath(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")));
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
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SARSlateWidget)
				.OnGenerateButtonClicked(FOnClicked::CreateRaw(this, &FAckermannsRouletteModule::GetRandomNumber))
			]
		];
}

void FAckermannsRouletteModule::GrantItemsDeferred()
{
	UWorld* World = GEditor->GetEditorWorldContext().World();

	if (AStaticMeshActor* MeshActor = World->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass()))
	{
		if (MeshPtr.Get())
		{
			MeshActor->GetStaticMeshComponent()->SetStaticMesh(MeshPtr.Get());
			MeshActor->SetActorLocation(FVector(0.f, 0.f, 60.f));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("MESH ACTOR COULD NOT LOAD"));
		}
	}
}

FReply FAckermannsRouletteModule::GetRandomNumber()
{
	UE_LOG(LogTemp, Warning, TEXT("ON GENERATE SLATE BUTTON IS CLICKED"));

	TArray<FSoftObjectPath> ItemsToStream;
	ItemsToStream.Add(MeshPtr.ToSoftObjectPath());
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(ItemsToStream, FStreamableDelegate::CreateRaw(this, &FAckermannsRouletteModule::GrantItemsDeferred));

	return FReply::Handled();
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

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAckermannsRouletteModule, AckermannsRoulette)