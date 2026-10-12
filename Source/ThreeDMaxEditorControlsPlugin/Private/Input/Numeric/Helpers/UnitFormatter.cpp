// Copyright 2026 Axiom Toolworks. All Rights Reserved.

#include "Input/Numeric/Helpers/UnitFormatter.h"
#include "Input/Numeric/NumericInputStructs.h"

namespace ThreeDMaxControls
{
	float FUnitFormatter::ConvertOnToolSwitch(float Value, EThreeDMaxNumericContext From,
	                                          EThreeDMaxNumericContext To)
	{
		if (From == To)
		{
			return Value;
		}
		if ((From == EThreeDMaxNumericContext::Distance || From == EThreeDMaxNumericContext::Scale) && To ==
			EThreeDMaxNumericContext::Angle_Degrees)
		{
			return FMath::RadiansToDegrees(Value);
		}

		if (From == EThreeDMaxNumericContext::Angle_Degrees && (To == EThreeDMaxNumericContext::Distance || To ==
			EThreeDMaxNumericContext::Scale))
		{
			return FMath::DegreesToRadians(Value);
		}

		return Value;
	}

	FString FUnitFormatter::FormatValue(float Value, EThreeDMaxNumericContext Context)
	{
		auto FormatSmart = [](const float V) -> FString
		{
			FNumberFormattingOptions Opt;
			Opt.MinimumFractionalDigits = 0;
			Opt.MaximumFractionalDigits = 3;
			Opt.UseGrouping = false;
			return FText::AsNumber(V, &Opt).ToString();
		};

		switch (Context)
		{
		case EThreeDMaxNumericContext::Angle_Degrees:
			return FormatSmart(Value) + TEXT("°");

		case EThreeDMaxNumericContext::Distance:
			return FormatSmart(Value) + TEXT(" cm");

		default:
			return FormatSmart(Value);
		}
	}
}
