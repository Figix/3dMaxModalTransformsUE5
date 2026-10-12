// Copyright 2026 Axiom Toolworks. All Rights Reserved.

// Style.cpp
#include "Style.h"
#include "Brushes/SlateImageBrush.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

namespace ThreeDMaxControls
{
	TSharedPtr<FSlateStyleSet> FStyle::StyleInstance;
	static FName ThreeDMaxControlsStyleName(TEXT("ThreeDMaxEditorControlsStyle"));
	
	static FSlateVectorImageBrush* MakeSvgBrushPtr(
		const TSharedRef<FSlateStyleSet>& Style, const TCHAR* RelPathNoExt, const FVector2D& Size)
	{
		return new FSlateVectorImageBrush(Style->RootToContentDir(RelPathNoExt, TEXT(".svg")), Size);
	}

	static FSlateImageBrush* MakePngBrushPtr(
		const TSharedRef<FSlateStyleSet>& Style, const TCHAR* RelPathNoExt, const FVector2D& Size)
	{
		return new FSlateImageBrush(Style->RootToContentDir(RelPathNoExt, TEXT(".png")), Size);
	}

	TSharedRef<FSlateStyleSet> FStyle::Create()
	{
		TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet(GetStyleSetName()));

		const FString PluginName = TEXT("3DMaxEditorControlsUE5"); // .uplugin name
		const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(PluginName);
		checkf(Plugin.IsValid(), TEXT("Plugin '%s' not found for style setup"), *PluginName);

		Style->SetContentRoot(FPaths::Combine(Plugin->GetBaseDir(), TEXT("Resources")));

		const FVector2D CursorMoveSize(24, 24);
		const FVector2D CursorArrowsSize(32, 32);
		const FVector2D CursorTrackballSize(32, 32);
		const FVector2D Icon128Size(128, 128);

		// IMPORTANT: pass NON-const brush pointers (no casts)
		Style->Set(TEXT("ThreeDMaxEditorControls.Cursors.Move"),
		           MakeSvgBrushPtr(Style, TEXT("MoveTool_Cursor"), CursorMoveSize));

		Style->Set(TEXT("ThreeDMaxEditorControls.Cursors.DoubleArrow"),
		           MakeSvgBrushPtr(Style, TEXT("DoubleArrow_Cursor"), CursorArrowsSize));

		Style->Set(TEXT("ThreeDMaxEditorControls.Cursors.Trackball"),
		           MakeSvgBrushPtr(Style, TEXT("Trackball_Cursor"), CursorTrackballSize));

		Style->Set(TEXT("ThreeDMaxEditorControls.PluginIcon128"),
		           MakePngBrushPtr(Style, TEXT("Icon128"), Icon128Size));

		return Style;
	}

	void FStyle::Initialize()
	{
		if (!StyleInstance.IsValid())
		{
			StyleInstance = Create();
			FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance.Get());
		}
	}

	void FStyle::Shutdown()
	{
		if (StyleInstance.IsValid())
		{
			FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance.Get());
			ensure(StyleInstance.IsUnique());
			StyleInstance.Reset();
		}
	}

	const ISlateStyle& FStyle::Get()
	{
		if (!StyleInstance.IsValid())
		{
			Initialize();
		}
		return *StyleInstance.Get();
	}

	FName FStyle::GetStyleSetName()
	{
		return ThreeDMaxControlsStyleName;
	}
}
