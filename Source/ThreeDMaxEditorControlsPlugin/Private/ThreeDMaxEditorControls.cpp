// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThreeDMaxEditorControls.h"
#include "Modules/ModuleManager.h"
#include "Logging/LogMacros.h"
#include "ThreeDMaxControlsCommands.h"
#include "Input/InputProcessor.h"
#include "Editor.h"
#include "Misc/ConfigCacheIni.h"
#include "Framework/Application/SlateApplication.h"
#include "HAL/IConsoleManager.h"
#include "Interfaces/IMainFrameModule.h"
#include "Misc/App.h"
#include "SWelcomeWindow.h"
DEFINE_LOG_CATEGORY(LogThreeDMaxEditorControls);

namespace ThreeDMaxControls
{
	void FThreeDMaxEditorControlsPluginModule::StartupModule()
	{
		UE_LOG(LogThreeDMaxEditorControls, Log, TEXT("ThreeDMaxEditorControlsPlugin: StartupModule"));

		RegisterCommands();
		RegisterInputProcessor();

		bool bHasSeenWelcome = false;
		if (GConfig)
		{
			GConfig->GetBool(
				TEXT("ThreeDMaxControlsPlugin"),
				TEXT("bHasSeenWelcome"),
				bHasSeenWelcome,
				GEditorPerProjectIni);
		}

		if (!bHasSeenWelcome && !FApp::IsUnattended() && !IsRunningCommandlet())
		{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION <= 2
			// Pass a dummy value of 0.0 since we do not save the delegate handle to unbind it later. 
			FCoreDelegates::OnPostEngineInit.AddLambda([this]() { OnEditorInitialized(0.0); });
#else
			FEditorDelegates::OnEditorInitialized.AddRaw(
				this,
				&FThreeDMaxEditorControlsPluginModule::OnEditorInitialized);
#endif
		}
	}

	void FThreeDMaxEditorControlsPluginModule::ShutdownModule()
	{
		UE_LOG(LogThreeDMaxEditorControls, Log, TEXT("ThreeDMaxEditorControlsPlugin: ShutdownModule"));

		UnregisterInputProcessor();
		UnregisterCommands();
	}

	void FThreeDMaxEditorControlsPluginModule::RegisterCommands()
	{
		FThreeDMaxControlsCommands::Register();
		CommandList = MakeShared<FUICommandList>();
	}

	void FThreeDMaxEditorControlsPluginModule::UnregisterCommands()
	{
		CommandList.Reset();
		FThreeDMaxControlsCommands::Unregister();
	}

	void FThreeDMaxEditorControlsPluginModule::RegisterInputProcessor()
	{
		InputProcessor = MakeShared<FInputProcessor>(CommandList);
		InputProcessor->BindCommands();
		//Ensure that this input-processor receives input first (amongst all input-processors). 
		constexpr int32 Priority = 101;
		FSlateApplication::Get().RegisterInputPreProcessor(InputProcessor, Priority);
	}

	void FThreeDMaxEditorControlsPluginModule::UnregisterInputProcessor()
	{
		if (FSlateApplication::IsInitialized() && InputProcessor.IsValid())
		{
			FSlateApplication::Get().UnregisterInputPreProcessor(InputProcessor);
		}
		InputProcessor.Reset();
	}

	void FThreeDMaxEditorControlsPluginModule::OnEditorInitialized(double InTime)
	{
		IMainFrameModule* MainFrame = FModuleManager::GetModulePtr<IMainFrameModule>("MainFrame");

		if (MainFrame && MainFrame->GetParentWindow().IsValid())
		{
			ShowWelcomeWindow(MainFrame->GetParentWindow());
		}
		else if (MainFrame)
		{
			MainFrame->OnMainFrameCreationFinished().AddRaw(
				this,
				&FThreeDMaxEditorControlsPluginModule::OnMainFrameCreationFinished);
		}
		else
		{
			IMainFrameModule& LoadedMainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
			LoadedMainFrame.OnMainFrameCreationFinished().AddRaw(
				this,
				&FThreeDMaxEditorControlsPluginModule::OnMainFrameCreationFinished);
		}
	}

	void FThreeDMaxEditorControlsPluginModule::OnMainFrameCreationFinished(
		TSharedPtr<SWindow> InRootWindow, bool bIsNewProjectWindow)
	{
		IMainFrameModule* MainFrame = FModuleManager::GetModulePtr<IMainFrameModule>("MainFrame");
		if (MainFrame)
		{
			MainFrame->OnMainFrameCreationFinished().RemoveAll(this);
		}

		ShowWelcomeWindow(InRootWindow);
	}

	void FThreeDMaxEditorControlsPluginModule::ShowWelcomeWindow(TSharedPtr<SWindow> ParentWindow)
	{
		TSharedRef<SWelcomeWindow> WelcomeWindow = SNew(SWelcomeWindow);

		if (ParentWindow.IsValid())
		{
			FSlateApplication::Get().AddWindowAsNativeChild(WelcomeWindow, ParentWindow.ToSharedRef());
		}
		else
		{
			FSlateApplication::Get().AddWindow(WelcomeWindow);
		}

		if (GConfig)
		{
			GConfig->SetBool(TEXT("ThreeDMaxControlsPlugin"), TEXT("bHasSeenWelcome"), true, GEditorPerProjectIni);
			GConfig->Flush(false, GEditorPerProjectIni);
		}
	}
} // namespace ThreeDMaxControls

IMPLEMENT_MODULE(ThreeDMaxControls::FThreeDMaxEditorControlsPluginModule, ThreeDMaxEditorControlsPlugin)
