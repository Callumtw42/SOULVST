/*
  ==============================================================================

	parameter.cpp
	Created: 31 Jul 2020 11:46:17pm
	Author:  callu

  ==============================================================================
*/

//#include "soulpatch.cpp"
//#include "SineSynth.h"

struct _SineSynth::Parameter : public juce::AudioProcessorParameterWithID
{
	Parameter(_SineSynth& owner, const MainProcessor::GeneratedClass::ParameterProperties& p)
		: AudioProcessorParameterWithID(p.UID, p.name),
		textValues(parseTextValues(p.textValues)),
		range(p.minValue, p.maxValue, p.step),
		numDecimalPlaces(getNumDecimalPlaces(range)),
		numParametersNeedingUpdate(owner.pimpl->numParametersNeedingUpdate)
	{
		properties[0] = p;
		currentFullRangeValue = properties[0].initialValue;
	}

	float currentFullRangeValue = 0;
	bool needsUpdate = false;

	std::array<MainProcessor::GeneratedClass::ParameterProperties, MAXVOICES> properties;
	const juce::StringArray textValues;
	const juce::NormalisableRange<float> range;
	const int numDecimalPlaces;
	std::atomic<uint32_t>& numParametersNeedingUpdate;

	juce::String getName(int maximumStringLength) const override { return name.substring(0, maximumStringLength); }
	juce::String getLabel() const override { return properties[0].unit; }
	Category getCategory() const override { return genericParameter; }
	bool isDiscrete() const override { return range.interval != 0; }
	bool isBoolean() const override { return properties[0].isBoolean; }
	bool isAutomatable() const override { return properties[0].isAutomatable; }
	bool isMetaParameter() const override { return false; }
	juce::StringArray getAllValueStrings() const override { return textValues; }
	float getDefaultValue() const override { return convertTo0to1(properties[0].initialValue); }
	float getValue() const override { return convertTo0to1(currentFullRangeValue); }
	void setValue(float newValue) override { setFullRangeValue(convertFrom0to1(newValue)); }

	void setFullRangeValue(float newValue)
	{
		if (newValue != currentFullRangeValue)
		{
			currentFullRangeValue = newValue;

			if (!needsUpdate)
			{
				needsUpdate = true;
				++numParametersNeedingUpdate;
			}

			sendValueChangedMessageToListeners(convertTo0to1(newValue));
		}
	}

	void sendUpdate(int index)
	{
		properties[index].setValue(currentFullRangeValue/*+lfos[index].value*/);
	}

	bool sendUpdateIfNeeded()
	{
		if (!needsUpdate)
			return false;

		needsUpdate = false;
		for (size_t i = 0; i < MAXVOICES; i++)
		{
			sendUpdate(i);
		}
		return true;
	}

	juce::String getText(float v, int length) const override
	{
		if (length > 0)              return getText(v, 0).substring(0, length);
		if (textValues.isEmpty())    return juce::String(convertFrom0to1(v), numDecimalPlaces);
		if (textValues.size() == 1)  return preprocessText(textValues[0].toUTF8(), convertFrom0to1(v));

		return textValues[juce::jlimit(0, textValues.size() - 1, juce::roundToInt(v * (textValues.size() - 1.0f)))];
	}

	float getValueForText(const juce::String& text) const override
	{
		for (int i = 0; i < textValues.size(); ++i)
			if (textValues[i] == text)
				return i / (textValues.size() - 1.0f);

		return convertTo0to1(text.upToLastOccurrenceOf(text, false, false).getFloatValue());
	}

	int getNumSteps() const override
	{
		if (!textValues.isEmpty() && std::abs(textValues.size() - (range.end - range.start)) < 0.01f)
			return textValues.size() - 1;

		if (range.interval > 0)
			return static_cast<int> ((range.end - range.start) / range.interval) + 1;

		return AudioProcessor::getDefaultNumParameterSteps();
	}

private:
	float convertTo0to1(float v) const { return range.convertTo0to1(range.snapToLegalValue(v)); }
	float convertFrom0to1(float v) const { return range.snapToLegalValue(range.convertFrom0to1(juce::jlimit(0.0f, 1.0f, v))); }

	static int getNumDecimalPlaces(juce::NormalisableRange<float> r)
	{
		int places = 7;

		if (r.interval != 0.0f)
		{
			if (juce::approximatelyEqual(std::abs(r.interval - (int)r.interval), 0.0f))
				return 0;

			auto v = std::abs(juce::roundToInt(r.interval * pow(10, places)));

			while ((v % 10) == 0 && places > 0)
			{
				--places;
				v /= 10;
			}
		}

		return places;
	}

	static juce::StringArray parseTextValues(const juce::String& text)
	{
		if (text.isNotEmpty())
			return juce::StringArray::fromTokens(text.unquoted(), "|", {});

		return {};
	}

	static juce::String preprocessText(juce::CharPointer_UTF8 text, float value)
	{
		juce::MemoryOutputStream result;

		while (!text.isEmpty())
		{
			auto c = text.getAndAdvance();

			if (c != '%') { result << juce::String::charToString(c); continue; }

			auto format = text;
			bool addSignChar = (*format == '+');
			if (addSignChar) ++format;

			bool isPadded = (*format == '0');
			int numDigits = 0;

			while (format.isDigit())
				numDigits = numDigits * 10 + (format.getAndAdvance() - '0');

			bool isFloat = (*format == 'f');
			bool isInt = (*format == 'd');

			if (!(isInt || isFloat))
			{
				result << '%';
				continue;
			}

			if (addSignChar && value >= 0)
				result << '+';

			if (isInt)
			{
				juce::String s((int64_t)(value + 0.5f));
				result << (isPadded ? s.paddedLeft('0', numDigits) : s);
			}
			else if (numDigits <= 0)
			{
				result << value;
			}
			else if (isPadded)
			{
				result << juce::String(value, numDigits);
			}
			else
			{
				juce::String s(value);
				auto afterDot = s.fromLastOccurrenceOf(".", false, false);

				if (afterDot.containsOnly("0123456789"))
					if (afterDot.length() > numDigits)
						s = s.dropLastCharacters(afterDot.length() - numDigits);

				result << s;
			}

			text = ++format;
		}

		return result.toString();
	}
};
