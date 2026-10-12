// Copyright 2026 Axiom Toolworks. All Rights Reserved.

#include "ThreeDMaxControlsCommands.h"

#define LOCTEXT_NAMESPACE "FThreeDMaxControlsCommands"

namespace ThreeDMaxControls
{
	FThreeDMaxControlsCommands::FThreeDMaxControlsCommands()
		: TCommands(
			TEXT("ThreeDMaxEditorControlsPlugin"),
			NSLOCTEXT("Contexts", "ThreeDMaxEditorControlsPlugin", "3ds Max Editor Controls"),
			FName("LevelEditor"),
			FName(TEXT("DefaultStyle"))
		)
	{
	}

	void FThreeDMaxControlsCommands::RegisterCommands()
	{
		// --- Session Lifecycle Commands ---
		UI_COMMAND(CommandAcceptAlt, "Accept (Space)", "Accept with Space",
		           EUserInterfaceActionType::Button, FInputChord(EKeys::SpaceBar));
		UI_COMMAND(CommandAccept, "Accept", "Accept the current transformation (Enter)",
		           EUserInterfaceActionType::Button, FInputChord(EKeys::Enter));
		UI_COMMAND(CommandCancel, "Cancel", "Cancel the current transformation (Esc)", EUserInterfaceActionType::Button,
		           FInputChord(EKeys::Escape));

		// --- Transform mode commands ---  
		UI_COMMAND(CommandTranslate, "Move", "Start modal translation (W)",
		           EUserInterfaceActionType::RadioButton, FInputChord(EKeys::W));
		UI_COMMAND(CommandRotate, "Rotate", "Start modal rotation (E)", EUserInterfaceActionType::RadioButton,
		           FInputChord(EKeys::E));
		UI_COMMAND(CommandScale, "Scale", "Start modal scale (R)", EUserInterfaceActionType::RadioButton,
		           FInputChord(EKeys::R));

		// --- Axis constraint commands ---
		UI_COMMAND(CommandAxisX, "X Axis", "Constrain to X axis (X)", EUserInterfaceActionType::RadioButton,
		           FInputChord(EKeys::X));
		UI_COMMAND(CommandAxisY, "Y Axis", "Constrain to Y axis (Y)", EUserInterfaceActionType::RadioButton,
		           FInputChord(EKeys::Y));
		UI_COMMAND(CommandAxisZ, "Z Axis", "Constrain to Z axis (Z)", EUserInterfaceActionType::RadioButton,
		           FInputChord(EKeys::Z));

		// --- Numeric mode helpers ---
		UI_COMMAND(CommandNumericBackspace, "Numeric: Backspace", "Delete last digit",
		           EUserInterfaceActionType::Button, FInputChord(EKeys::BackSpace));
		UI_COMMAND(CommandNumericToggleNegation, "Numeric: Toggle Sign", "Toggle +/-",
		           EUserInterfaceActionType::Button, FInputChord(EKeys::Subtract));
		UI_COMMAND(CommandNumericToggleReciprocal, "Numeric: Reciprocal", "Toggle 1/x",
		           EUserInterfaceActionType::Button, FInputChord(EKeys::Divide));
		UI_COMMAND(CommandNumericCycleSlot, "Numeric: Cycle Slot", "Cycle X/Y/Z slot",
		           EUserInterfaceActionType::Button, FInputChord(EKeys::Tab));

		UI_COMMAND(CommandDuplicateAndMove, "Duplicate and Move",
		           "Duplicates the selection and immediately begins a move operation.",
		           EUserInterfaceActionType::Button, FInputChord(EKeys::D, EModifierKey::Shift));
	}
} // namespace ThreeDMaxControls

#undef LOCTEXT_NAMESPACE
