//      _____ _____ _____ __
//     |   __|     |  |  |  |        Auto-generated C++
//     |__   |  |  |  |  |  |__      SOUL Version 0.9.0
//     |_____|_____|_____|_____|     https://soul.dev
//

#include <vector>
#include <array>
#include <functional>
#include <cmath>
#include <cstddef>
#include <limits>
#include <cstring>
#include <cassert>

#define SOUL_CPP_ASSERT(x) assert (x)

#ifndef SOUL_CPP_ASSERT
 #define SOUL_CPP_ASSERT(x)
#endif

//==============================================================================
// Generated from graph 'SineSynth', source file: SineSynth.soulpatch
//
class SineSynth
{
public:
    SineSynth() = default;
    ~SineSynth() = default;

    //==============================================================================
    static constexpr uint32_t maxBlockSize = 512;
    template <typename Type, int size> struct Vector;
    template <typename Type, int size> struct FixedArray;
    template <typename Type> struct DynamicArray;

    static constexpr const char*  UID           = "dev.soul.examples.sinesynth";
    static constexpr const char*  name          = "SineSynth";
    static constexpr const char*  description   = "SOUL Sine Synth example";
    static constexpr const char*  category      = "synth";
    static constexpr const char*  manufacturer  = "soul.dev";
    static constexpr const char*  version       = "1.0";
    static constexpr const char*  URL           = "";
    static constexpr bool         isInstrument  = true;
    static constexpr const char*  manifest      = "{\n"
    "    \"soulPatchV1\": \n"
    "    {\n"
    "        \"ID\":               \"dev.soul.examples.sinesynth\",\n"
    "        \"version\":          \"1.0\",\n"
    "        \"name\":             \"SineSynth\",\n"
    "        \"description\":      \"SOUL Sine Synth example\",\n"
    "        \"category\":         \"synth\",\n"
    "        \"manufacturer\":     \"soul.dev\",\n"
    "        \"isInstrument\":     true,\n"
    "\n"
    "        \"source\":           [\n"
    "                            \"Voice.soul\",\"SineSynth.soul\", \"polyblep.soul\", \"controller.soul\",\n"
    "                            \"_NoteHandler.soul\", \"adsrEnvelope.soul\", \"detuner.soul\", \"filter.soul\",\n"
    "                            \"envelope.soul\", \"logger.soul\"\n"
    "                            ]\n"
    "    }\n"
    "\n"
    "}";

    //==============================================================================
    struct _RenderStats;
    struct _SparseStreamStatus;
    struct _Event_in_struct_Message_1;
    struct _Event_in_f32_1;
    struct _Stream_out_vec_2_f32_512;
    struct NoteHandler__NoteInfo;
    struct NoteHandler__NoteHandler;
    struct Controller___State;
    struct ADSREnvelope___State;
    struct Filter__FilterTerms;
    struct Filter__FrequencySettings;
    struct Filter__PrecalcTerms;
    struct Filter___State;
    struct Envelope___State;
    struct Detuner___State;
    struct PolyBlep___State;
    struct PolyBlep_2___State;
    struct PolyBlep_3___State;
    struct PolyBlep_4___State;
    struct PolyBlep_5___State;
    struct PolyBlep_6___State;
    struct PolyBlep_7___State;
    struct PolyBlep_8___State;
    struct PolyBlep_9___State;
    struct PolyBlep_10___State;
    struct PolyBlep_11___State;
    struct PolyBlep_12___State;
    struct PolyBlep_13___State;
    struct PolyBlep_14___State;
    struct PolyBlep_15___State;
    struct PolyBlep_16___State;
    struct Voice___State;
    struct soul__midi__MPEParser___State;
    struct soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator__VoiceInfo;
    struct soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___State;
    struct soul__gain__DynamicGain___for__root__SineSynth_gainProcessor___State;
    struct soul__gain__SmoothedGainParameter___for__root__SineSynth_smoothedGain___State;
    struct _State;
    struct soul__midi__Message;
    struct soul__note_events__NoteOn;
    struct soul__note_events__NoteOff;
    struct soul__note_events__PitchBend;
    struct soul__note_events__Pressure;
    struct soul__note_events__Slide;
    struct soul__note_events__Control;
    struct soul__gain__SmoothedGainParameter___for__root__SineSynth_smoothedGain___IO;
    struct soul__gain__DynamicGain___for__root__SineSynth_gainProcessor___IO;
    struct Voice___IO;
    struct PolyBlep___IO;
    struct Controller___IO;
    struct ADSREnvelope___IO;
    struct Detuner___IO;
    struct Filter___IO;
    struct Envelope___IO;
    struct PolyBlep_2___IO;
    struct PolyBlep_3___IO;
    struct PolyBlep_4___IO;
    struct PolyBlep_5___IO;
    struct PolyBlep_6___IO;
    struct PolyBlep_7___IO;
    struct PolyBlep_8___IO;
    struct PolyBlep_9___IO;
    struct PolyBlep_10___IO;
    struct PolyBlep_11___IO;
    struct PolyBlep_12___IO;
    struct PolyBlep_13___IO;
    struct PolyBlep_14___IO;
    struct PolyBlep_15___IO;
    struct PolyBlep_16___IO;
    struct StringLiteral;

    //==============================================================================
    // The following methods provide basic initialisation and control for the processor

    void init (double newSampleRate, int sessionID)
    {
        memset (&state, 0, sizeof (state));
        sampleRate = newSampleRate;
        _initialise (state, sessionID);
        initialisedState = state;
    }

    void reset() noexcept
    {
        state = initialisedState;
    }

    uint32_t getNumXRuns() noexcept
    {
        return static_cast<uint32_t> (_get_num_xruns (state));
    }

    //==============================================================================
    // These classes and functions provide a high-level rendering method that
    // presents the processor as a set of standard audio and MIDI channels.

    static constexpr uint32_t numAudioInputChannels  = 0;
    static constexpr uint32_t numAudioOutputChannels = 2;

    struct MIDIMessage
    {
        uint32_t frameIndex = 0;
        uint8_t byte0 = 0, byte1 = 0, byte2 = 0;
    };

    struct MIDIMessageArray
    {
        const MIDIMessage* messages = nullptr;
        uint32_t numMessages = 0;
    };

    template <typename FloatType = float>
    struct RenderContext
    {
        std::array<const FloatType*, 0> inputChannels;
        std::array<FloatType*, 2> outputChannels;
        MIDIMessageArray  incomingMIDI;
        uint32_t          numFrames = 0;
    };

    //==============================================================================
    template <typename FloatType>
    void render (RenderContext<FloatType> context)
    {
        uint32_t startFrame = 0, startMIDIIndex = 0;

        while (startFrame < context.numFrames)
        {
            auto framesRemaining = context.numFrames - startFrame;
            auto numFramesToDo = framesRemaining < maxBlockSize ? framesRemaining : maxBlockSize;
            auto endMIDIIndex = startMIDIIndex;

            while (endMIDIIndex < context.incomingMIDI.numMessages)
            {
                auto eventTime = context.incomingMIDI.messages[endMIDIIndex].frameIndex;

                if (eventTime > startFrame)
                {
                    auto framesUntilEvent = eventTime - startFrame;

                    if (framesUntilEvent < numFramesToDo)
                        numFramesToDo = framesUntilEvent;

                    break;
                }

                ++endMIDIIndex;
            }

            prepare (numFramesToDo);

            while (startMIDIIndex < endMIDIIndex)
            {
                auto midi = context.incomingMIDI.messages[startMIDIIndex++];
                auto packed = (static_cast<uint32_t> (midi.byte0) << 16) | (static_cast<uint32_t> (midi.byte1) << 8) | static_cast<uint32_t> (midi.byte2);
                _addInputEvent_midiIn_struct_Message (state, { static_cast<int32_t> (packed) });
            }

            advance();

            copyFromInterleaved (&context.outputChannels[0], startFrame, _getOutputFrameArrayRef_audioOut (state).elements, numFramesToDo);
            startFrame += numFramesToDo;
        }
    }

    //==============================================================================
    // The following methods provide low-level access for read/write to all the
    // endpoints directly, and to run the prepare/advance loop.

    void prepare (uint32_t numFramesToBeRendered)
    {
        SOUL_CPP_ASSERT (numFramesToBeRendered != 0);
        framesToAdvance = numFramesToBeRendered;
        _prepare (state, static_cast<int32_t> (numFramesToBeRendered));
    }

    void advance()
    {
        SOUL_CPP_ASSERT (framesToAdvance != 0); // you must call prepare() before advance()!
        auto framesRemaining = framesToAdvance;

        while (framesRemaining > 0)
        {
            auto framesThisCall = framesRemaining < maxBlockSize ? framesRemaining : maxBlockSize;

            run (state, static_cast<int32_t> (framesThisCall));

            totalFramesElapsed += framesThisCall;
            framesRemaining -= framesThisCall;
        }
    }

    void addInputEvent_midiIn (const soul__midi__Message& eventValue)
    {
        _addInputEvent_midiIn_struct_Message (state, eventValue);
    }

    void addInputEvent_volume (float eventValue)
    {
        _addInputEvent_volume_f32 (state, eventValue);
    }

    void addInputEvent_ampAttack (float eventValue)
    {
        _addInputEvent_ampAttack_f32 (state, eventValue);
    }

    void addInputEvent_ampDecay (float eventValue)
    {
        _addInputEvent_ampDecay_f32 (state, eventValue);
    }

    void addInputEvent_ampSustain (float eventValue)
    {
        _addInputEvent_ampSustain_f32 (state, eventValue);
    }

    void addInputEvent_ampRelease (float eventValue)
    {
        _addInputEvent_ampRelease_f32 (state, eventValue);
    }

    void addInputEvent_detune (float eventValue)
    {
        _addInputEvent_detune_f32 (state, eventValue);
    }

    void addInputEvent_voiceCount (float eventValue)
    {
        _addInputEvent_voiceCount_f32 (state, eventValue);
    }

    void addInputEvent_cutoff (float eventValue)
    {
        _addInputEvent_cutoff_f32 (state, eventValue);
    }

    void addInputEvent_x (float eventValue)
    {
        _addInputEvent_x_f32 (state, eventValue);
    }

    DynamicArray<const Vector<float, 2>> getOutputStreamFrames_audioOut()
    {
        return { &(_getOutputFrameArrayRef_audioOut (state).elements[0]), static_cast<int32_t> (framesToAdvance) };
    }

    //==============================================================================
    // The following methods provide a fixed interface for finding out about
    // the input/output endpoints that this processor provides.

    using EndpointID = const char*;
    enum class EndpointType     { value, stream, event };

    struct EndpointDetails
    {
        const char* name;
        EndpointID endpointID;
        EndpointType endpointType;
        const char* frameType;
        uint32_t numAudioChannels;
        const char* annotation;
    };

    std::vector<EndpointDetails> getInputEndpoints() const
    {
        return
        {
            { "midiIn",     "in:midiIn",     EndpointType::event, "Message { int32 midiBytes }", 0, ""                                                                                                                         },
            { "volume",     "in:volume",     EndpointType::event, "float32",                     0, "{ \"label\": \"Volume\", \"min\": -85, \"max\": 6, \"init\": -6, \"step\": 1, \"unit\": \"dB\" }"                         },
            { "ampAttack",  "in:ampAttack",  EndpointType::event, "float32",                     0, "{ \"label\": \"Attack\", \"min\": 0.009999999776482582, \"max\": 100.0, \"init\": 0.009999999776482582, \"step\": 1.0 }"  },
            { "ampDecay",   "in:ampDecay",   EndpointType::event, "float32",                     0, "{ \"label\": \"Decay\", \"min\": 0.009999999776482582, \"max\": 100.0, \"init\": 0.009999999776482582, \"step\": 1.0 }"   },
            { "ampSustain", "in:ampSustain", EndpointType::event, "float32",                     0, "{ \"label\": \"Sustain\", \"min\": 0.009999999776482582, \"max\": 100.0, \"init\": 100.0, \"step\": 1.0 }"                },
            { "ampRelease", "in:ampRelease", EndpointType::event, "float32",                     0, "{ \"label\": \"Release\", \"min\": 0.009999999776482582, \"max\": 100.0, \"init\": 0.009999999776482582, \"step\": 1.0 }" },
            { "detune",     "in:detune",     EndpointType::event, "float32",                     0, "{ \"label\": \"Detune\", \"min\": 0.0, \"max\": 50.0, \"init\": 0.0, \"step\": 1.0 }"                                     },
            { "voiceCount", "in:voiceCount", EndpointType::event, "float32",                     0, "{ \"label\": \"voiceCount\", \"min\": 1, \"max\": 16, \"init\": 1, \"step\": 1 }"                                         },
            { "cutoff",     "in:cutoff",     EndpointType::event, "float32",                     0, "{ \"label\": \"cutoff\", \"min\": 0.0, \"max\": 127.0, \"init\": 127.0, \"step\": 1.0 }"                                  },
            { "x",          "in:x",          EndpointType::event, "float32",                     0, ""                                                                                                                         }
        };
    }

    std::vector<EndpointDetails> getOutputEndpoints() const
    {
        return
        {
            { "audioOut", "out:audioOut", EndpointType::stream, "float32<2>", 2, "" }
        };
    }

    //==============================================================================
    // The following methods provide help in dealing with the processor's endpoints
    // in a format suitable for traditional audio plugin channels and parameters.

    struct ParameterProperties
    {
        const char* UID;
        const char* name;
        const char* unit;
        float minValue, maxValue, step, initialValue;
        bool isAutomatable, isBoolean, isHidden;
        const char* group;
        const char* textValues;
        float currentValue;
        std::function<void(float)> applyValue;

        void setValue (float f)
        {
            currentValue = snapToLegalValue (f);
            applyValue (f);
        }

        float getValue() const
        {
            return currentValue;
        }

    private:
        float snapToLegalValue (float v) const
        {
            if (step > 0)
                v = minValue + step * std::floor ((v - minValue) / step + 0.5f);

            return v < minValue ? minValue : (v > maxValue ? maxValue : v);
        }
    };

    struct AudioBus
    {
        const char* name;
        uint32_t numChannels;
    };

    static constexpr bool      hasMIDIInput = true;
    static constexpr uint32_t  numParameters = 9;

    std::vector<ParameterProperties> getParameterProperties()
    {
        return
        {
            {  "volume",      "volume",      "dB",  -85.0f,  6.0f,    1.0f,    -6.0f,   true,  false,  false,  "",  "",  -6.0f,   [this] (float v) { addInputEvent_volume (v); }      },
            {  "ampAttack",   "ampAttack",   "",    0.01f,   100.0f,  1.0f,    0.01f,   true,  false,  false,  "",  "",  0.01f,   [this] (float v) { addInputEvent_ampAttack (v); }   },
            {  "ampDecay",    "ampDecay",    "",    0.01f,   100.0f,  1.0f,    0.01f,   true,  false,  false,  "",  "",  0.01f,   [this] (float v) { addInputEvent_ampDecay (v); }    },
            {  "ampSustain",  "ampSustain",  "",    0.01f,   100.0f,  1.0f,    100.0f,  true,  false,  false,  "",  "",  100.0f,  [this] (float v) { addInputEvent_ampSustain (v); }  },
            {  "ampRelease",  "ampRelease",  "",    0.01f,   100.0f,  1.0f,    0.01f,   true,  false,  false,  "",  "",  0.01f,   [this] (float v) { addInputEvent_ampRelease (v); }  },
            {  "detune",      "detune",      "",    0.0f,    50.0f,   1.0f,    0.0f,    true,  false,  false,  "",  "",  0.0f,    [this] (float v) { addInputEvent_detune (v); }      },
            {  "voiceCount",  "voiceCount",  "",    1.0f,    16.0f,   1.0f,    1.0f,    true,  false,  false,  "",  "",  1.0f,    [this] (float v) { addInputEvent_voiceCount (v); }  },
            {  "cutoff",      "cutoff",      "",    0.0f,    127.0f,  1.0f,    127.0f,  true,  false,  false,  "",  "",  127.0f,  [this] (float v) { addInputEvent_cutoff (v); }      },
            {  "x",           "x",           "",    0.0f,    1.0f,    0.001f,  0.0f,    true,  false,  false,  "",  "",  0.0f,    [this] (float v) { addInputEvent_x (v); }           }
        };
    }

    std::vector<AudioBus> getInputBuses() const   { return {}; }

    std::vector<AudioBus> getOutputBuses() const   { return { { "audioOut", 2 } }; }

    //==============================================================================
    struct ZeroInitialiser
    {
        template <typename Type>
        operator Type() const   { return {}; }
    };

    //==============================================================================
    template <typename Type>
    struct DynamicArray
    {
        using ElementType = Type;
        ElementType* elements = nullptr;
        int32_t numElements = 0;

        constexpr ElementType& operator[] (int i) noexcept                   { return elements[i]; }
        constexpr const ElementType& operator[] (int i) const noexcept       { return elements[i]; }
        constexpr int getElementSizeBytes() const noexcept                   { return sizeof (ElementType); }
    };

    //==============================================================================
    template <typename Type, int32_t size>
    struct FixedArray
    {
        using ElementType = Type;
        ElementType elements[size];
        static constexpr int32_t numElements = size;

        static constexpr FixedArray fromRepeatedValue (ElementType value)
        {
            FixedArray a;

            for (auto& element : a.elements)
                element = value;

            return a;
        }

        static size_t elementOffset (int i) noexcept               { return sizeof (ElementType) * i; }
        ElementType& operator[] (int i) noexcept                   { return elements[i]; }
        const ElementType& operator[] (int i) const noexcept       { return elements[i]; }
        int getElementSizeBytes() const noexcept                   { return sizeof (ElementType); }
        DynamicArray<ElementType> toDynamicArray() const noexcept  { return { const_cast<ElementType*> (&elements[0]), size }; }
        operator ElementType*() const noexcept                     { return const_cast<ElementType*> (&elements[0]); }

        FixedArray& operator= (ZeroInitialiser)
        {
            for (auto& e : elements)
                e = {};

            return *this;
        }

        template <int start, int end>
        constexpr FixedArray<Type, end - start> slice() const noexcept
        {
            FixedArray<Type, end - start> newSlice;

            for (int i = 0; i < end - start; ++i)
                newSlice.elements[i] = elements[start + i];

            return newSlice;
        }
    };

    //==============================================================================
    template <typename Type, int32_t size>
    struct Vector
    {
        using ElementType = Type;
        ElementType elements[size] = {};
        static constexpr int32_t numElements = size;

        constexpr Vector() = default;
        constexpr Vector (const Vector&) = default;
        constexpr Vector& operator= (const Vector&) = default;

        explicit constexpr Vector (Type value)
        {
            for (auto& element : elements)
                element = value;
        }

        constexpr Vector (std::initializer_list<Type> i)
        {
            int n = 0;

            for (auto e : i)
                elements[n++] = e;
        }

        static constexpr Vector fromRepeatedValue (Type value)
        {
            return Vector (value);
        }

        constexpr Vector operator+ (const Vector& rhs) const                { return apply<Vector> (rhs, [] (Type a, Type b) { return a + b; }); }
        constexpr Vector operator- (const Vector& rhs) const                { return apply<Vector> (rhs, [] (Type a, Type b) { return a - b; }); }
        constexpr Vector operator* (const Vector& rhs) const                { return apply<Vector> (rhs, [] (Type a, Type b) { return a * b; }); }
        constexpr Vector operator/ (const Vector& rhs) const                { return apply<Vector> (rhs, [] (Type a, Type b) { return a / b; }); }
        constexpr Vector operator% (const Vector& rhs) const                { return apply<Vector> (rhs, [] (Type a, Type b) { return a % b; }); }
        constexpr Vector operator-() const                                  { return apply<Vector> ([] (Type n) { return -n; }); }

        Vector& operator= (ZeroInitialiser)
        {
            for (auto& e : elements)
                e = {};

            return *this;
        }

        constexpr Vector<bool, size> operator== (const Vector& rhs) const   { return apply<Vector<bool, size>> (rhs, [] (Type a, Type b) { return a == b; }); }
        constexpr Vector<bool, size> operator!= (const Vector& rhs) const   { return apply<Vector<bool, size>> (rhs, [] (Type a, Type b) { return a != b; }); }

        template <typename ReturnType, typename Op>
        constexpr ReturnType apply (const Vector& rhs, Op&& op) const noexcept
        {
            ReturnType v;

            for (int i = 0; i < size; ++i)
                v.elements[i] = op (elements[i], rhs.elements[i]);

            return v;
        }

        template <typename ReturnType, typename Op>
        constexpr ReturnType apply (Op&& op) const noexcept
        {
            ReturnType v;

            for (int i = 0; i < size; ++i)
                v.elements[i] = op (elements[i]);

            return v;
        }

        template <int start, int end>
        constexpr Vector<Type, end - start> slice() const noexcept
        {
            Vector<Type, end - start> newSlice;

            for (int i = 0; i < end - start; ++i)
                newSlice.elements[i] = elements[start + i];

            return newSlice;
        }

        constexpr const Type& operator[] (int i) const noexcept  { return elements[i]; }
        constexpr Type& operator[] (int i) noexcept              { return elements[i]; }
    };

    //==============================================================================
    struct StringLiteral
    {
        constexpr StringLiteral (int32_t h) noexcept : handle (h) {}
        StringLiteral() = default;
        StringLiteral (const StringLiteral&) = default;
        StringLiteral& operator= (const StringLiteral&) = default;

        const char* toString() const       { return lookupStringLiteral (handle); }
        operator const char*() const       { return lookupStringLiteral (handle); }

        bool operator== (StringLiteral other) const noexcept    { return handle == other.handle; }
        bool operator!= (StringLiteral other) const noexcept    { return handle != other.handle; }

        int32_t handle = 0;
    };


    //==============================================================================
    //==============================================================================
    //
    //    All the code that follows this point should be considered internal.
    //    User code should rarely need to refer to anything beyond this point..
    //
    //==============================================================================
    //==============================================================================

    template <typename Vec>  static Vec _vec_sqrt  (Vec a)         { return a.template apply<Vec> ([]  (typename Vec::ElementType x) { return std::sqrt (x); }); }
    template <typename Vec>  static Vec _vec_pow   (Vec a, Vec b)  { return a.template apply<Vec> ([&] (typename Vec::ElementType x) { return std::pow (x, b); }); }
    template <typename Vec>  static Vec _vec_exp   (Vec a)         { return a.template apply<Vec> ([]  (typename Vec::ElementType x) { return std::exp (x); }); }
    template <typename Vec>  static Vec _vec_log   (Vec a)         { return a.template apply<Vec> ([]  (typename Vec::ElementType x) { return std::log (x); }); }
    template <typename Vec>  static Vec _vec_log10 (Vec a)         { return a.template apply<Vec> ([]  (typename Vec::ElementType x) { return std::log10 (x); }); }
    template <typename Vec>  static Vec _vec_sin   (Vec a)         { return a.template apply<Vec> ([]  (typename Vec::ElementType x) { return std::sin (x); }); }
    template <typename Vec>  static Vec _vec_cos   (Vec a)         { return a.template apply<Vec> ([]  (typename Vec::ElementType x) { return std::cos (x); }); }
    template <typename Vec>  static Vec _vec_tan   (Vec a)         { return a.template apply<Vec> ([]  (typename Vec::ElementType x) { return std::tan (x); }); }
    template <typename Vec>  static Vec _vec_sinh  (Vec a)         { return a.template apply<Vec> ([]  (typename Vec::ElementType x) { return std::sinh (x); }); }
    template <typename Vec>  static Vec _vec_cosh  (Vec a)         { return a.template apply<Vec> ([]  (typename Vec::ElementType x) { return std::cosh (x); }); }
    template <typename Vec>  static Vec _vec_tanh  (Vec a)         { return a.template apply<Vec> ([]  (typename Vec::ElementType x) { return std::tanh (x); }); }
    template <typename Vec>  static Vec _vec_asinh (Vec a)         { return a.template apply<Vec> ([]  (typename Vec::ElementType x) { return std::asinh (x); }); }
    template <typename Vec>  static Vec _vec_acosh (Vec a)         { return a.template apply<Vec> ([]  (typename Vec::ElementType x) { return std::acosh (x); }); }
    template <typename Vec>  static Vec _vec_atanh (Vec a)         { return a.template apply<Vec> ([]  (typename Vec::ElementType x) { return std::atanh (x); }); }
    template <typename Vec>  static Vec _vec_asin  (Vec a)         { return a.template apply<Vec> ([]  (typename Vec::ElementType x) { return std::asin (x); }); }
    template <typename Vec>  static Vec _vec_acos  (Vec a)         { return a.template apply<Vec> ([]  (typename Vec::ElementType x) { return std::acos (x); }); }
    template <typename Vec>  static Vec _vec_atan  (Vec a)         { return a.template apply<Vec> ([]  (typename Vec::ElementType x) { return std::atan (x); }); }
    template <typename Vec>  static Vec _vec_atan2 (Vec a, Vec b)  { return a.template apply<Vec> ([&] (typename Vec::ElementType x) { return std::atan2 (x, b); }); }

    static constexpr int32_t _intrin_clamp (int32_t n, int32_t low, int32_t high)  { return n < low ? low : (n > high ? high : n); }
    static constexpr int32_t _intrin_wrap (int32_t n, int32_t range)   { if (range == 0) return 0; auto x = n % range; return x < 0 ? x + range : x; }

    static constexpr float  _nan32 = std::numeric_limits<float>::quiet_NaN();
    static constexpr double _nan64 = std::numeric_limits<double>::quiet_NaN();

    static constexpr float  _inf32 = std::numeric_limits<float>::infinity();
    static constexpr double _inf64 = std::numeric_limits<double>::infinity();

    static constexpr float  _ninf32 = -_inf32;
    static constexpr double _ninf64 = -_inf64;

    template <typename SourceFloatType, typename DestFloatType>
    static inline void copyToInterleaved (DestFloatType* monoDest, const SourceFloatType* const* sourceChannels, uint32_t sourceStartFrame, uint32_t numFrames)
    {
        auto source = *sourceChannels + sourceStartFrame;

        for (uint32_t i = 0; i < numFrames; ++i)
            monoDest[i] = static_cast<DestFloatType> (source[i]);
    }

    template <typename SourceFloatType, typename DestFloatType, int numChannels>
    static inline void copyToInterleaved (Vector<DestFloatType, numChannels>* vectorDest, const SourceFloatType* const* sourceChannels, uint32_t sourceStartFrame, uint32_t numFrames)
    {
        for (uint32_t i = 0; i < numFrames; ++i)
            for (uint32_t chan = 0; chan < numChannels; ++chan)
                vectorDest[i].elements[chan] = static_cast<DestFloatType> (sourceChannels[chan][sourceStartFrame + i]);
    }

    template <typename SourceFloatType, typename DestFloatType>
    static inline void copyFromInterleaved (DestFloatType* const* destChannels, uint32_t destStartFrame, const SourceFloatType* monoSource, uint32_t numFrames)
    {
        auto dest = *destChannels + destStartFrame;

        for (uint32_t i = 0; i < numFrames; ++i)
            dest[i] = static_cast<DestFloatType> (monoSource[i]);
    }

    template <typename SourceFloatType, typename DestFloatType, int numChannels>
    static inline void copyFromInterleaved (DestFloatType* const* destChannels, uint32_t destStartFrame, const Vector<SourceFloatType, numChannels>* vectorSource, uint32_t numFrames)
    {
        for (uint32_t i = 0; i < numFrames; ++i)
            for (uint32_t chan = 0; chan < numChannels; ++chan)
                destChannels[chan][destStartFrame + i] = static_cast<DestFloatType> (vectorSource[i].elements[chan]);
    }

    //==============================================================================
    struct _RenderStats
    {
        int32_t m_underrunCount, m_underrunFrames, m_overrunCount, m_overrunFrames;
    };

    struct _SparseStreamStatus
    {
        int32_t m_activeRamps;
        FixedArray<int32_t, 10> m_rampArray;
    };

    struct _Event_in_struct_Message_1
    {
        int32_t m_numFrames;
    };

    struct _Event_in_f32_1
    {
        int32_t m_numFrames;
    };

    struct _Stream_out_vec_2_f32_512
    {
        FixedArray<Vector<float, 2>, 512> m_buffer;
    };

    struct NoteHandler__NoteInfo
    {
        int32_t m_channel;
        float m_bendSemitones, m_note, m_pressure, m_slide;
    };

    struct NoteHandler__NoteHandler
    {
        float m_currentVelocity, m_currentNote, m_currentBendSemitones, m_currentPressure, m_currentSlide;
        int32_t m_maxNotes;
        FixedArray<NoteHandler__NoteInfo, 16> m_noteArray;
        int32_t m_activeNotes;
    };

    struct Controller___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
        NoteHandler__NoteHandler m_noteHandler;
        float m_multiplier;
        int32_t m_sampleCount;
    };

    struct ADSREnvelope___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
        double m_attackRamp, m_decayMultiplier, m_sustainLevel, m_releaseMultiplier;
        float m_velocitySensitivity;
        double m_targetValue, m_value;
    };

    struct Filter__FilterTerms
    {
        float m_a, m_b, m_c;
    };

    struct Filter__FrequencySettings
    {
        FixedArray<Filter__FilterTerms, 101> m_resonance;
    };

    struct Filter__PrecalcTerms
    {
        FixedArray<Filter__FrequencySettings, 128> m_frequency;
    };

    struct Filter___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
        float m_cutoff;
        Filter__PrecalcTerms m_precalcTerms;
        float m_factor_a1, m_factor_a2, m_factor_b1, m_factor_b2;
        int32_t m_counter_3;
        float m_in_1, m_in_2, m_out_1, m_out_2;
    };

    struct Envelope___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
        float m_paramVal, m_modAmt;
        double m_attackRamp, m_decayMultiplier, m_sustainLevel, m_releaseMultiplier;
        float m_velocitySensitivity;
        double m_targetValue, m_value;
    };

    struct Detuner___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
        Vector<float, 16> m_detuneVals, m_active;
        float m_voiceCount, m_detune;
    };

    struct PolyBlep___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
        float m_normalisedPhase, m_normalisedPhaseIncrement, m_waveshapeValue, m_detuneSemitonesValue, m_detuneValue, m_frequencyMultiplier;
    };

    struct PolyBlep_2___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
        float m_normalisedPhase, m_normalisedPhaseIncrement, m_waveshapeValue, m_detuneSemitonesValue, m_detuneValue, m_frequencyMultiplier;
    };

    struct PolyBlep_3___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
        float m_normalisedPhase, m_normalisedPhaseIncrement, m_waveshapeValue, m_detuneSemitonesValue, m_detuneValue, m_frequencyMultiplier;
    };

    struct PolyBlep_4___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
        float m_normalisedPhase, m_normalisedPhaseIncrement, m_waveshapeValue, m_detuneSemitonesValue, m_detuneValue, m_frequencyMultiplier;
    };

    struct PolyBlep_5___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
        float m_normalisedPhase, m_normalisedPhaseIncrement, m_waveshapeValue, m_detuneSemitonesValue, m_detuneValue, m_frequencyMultiplier;
    };

    struct PolyBlep_6___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
        float m_normalisedPhase, m_normalisedPhaseIncrement, m_waveshapeValue, m_detuneSemitonesValue, m_detuneValue, m_frequencyMultiplier;
    };

    struct PolyBlep_7___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
        float m_normalisedPhase, m_normalisedPhaseIncrement, m_waveshapeValue, m_detuneSemitonesValue, m_detuneValue, m_frequencyMultiplier;
    };

    struct PolyBlep_8___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
        float m_normalisedPhase, m_normalisedPhaseIncrement, m_waveshapeValue, m_detuneSemitonesValue, m_detuneValue, m_frequencyMultiplier;
    };

    struct PolyBlep_9___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
        float m_normalisedPhase, m_normalisedPhaseIncrement, m_waveshapeValue, m_detuneSemitonesValue, m_detuneValue, m_frequencyMultiplier;
    };

    struct PolyBlep_10___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
        float m_normalisedPhase, m_normalisedPhaseIncrement, m_waveshapeValue, m_detuneSemitonesValue, m_detuneValue, m_frequencyMultiplier;
    };

    struct PolyBlep_11___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
        float m_normalisedPhase, m_normalisedPhaseIncrement, m_waveshapeValue, m_detuneSemitonesValue, m_detuneValue, m_frequencyMultiplier;
    };

    struct PolyBlep_12___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
        float m_normalisedPhase, m_normalisedPhaseIncrement, m_waveshapeValue, m_detuneSemitonesValue, m_detuneValue, m_frequencyMultiplier;
    };

    struct PolyBlep_13___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
        float m_normalisedPhase, m_normalisedPhaseIncrement, m_waveshapeValue, m_detuneSemitonesValue, m_detuneValue, m_frequencyMultiplier;
    };

    struct PolyBlep_14___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
        float m_normalisedPhase, m_normalisedPhaseIncrement, m_waveshapeValue, m_detuneSemitonesValue, m_detuneValue, m_frequencyMultiplier;
    };

    struct PolyBlep_15___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
        float m_normalisedPhase, m_normalisedPhaseIncrement, m_waveshapeValue, m_detuneSemitonesValue, m_detuneValue, m_frequencyMultiplier;
    };

    struct PolyBlep_16___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
        float m_normalisedPhase, m_normalisedPhaseIncrement, m_waveshapeValue, m_detuneSemitonesValue, m_detuneValue, m_frequencyMultiplier;
    };

    struct Voice___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
        Controller___State m_voiceController_state;
        ADSREnvelope___State m_amplitudeEnvelope_state;
        Filter___State m_filter_state;
        Envelope___State m_filterEnv_state;
        Detuner___State m_detuner_state;
        PolyBlep___State m_osc1_state;
        PolyBlep_2___State m_osc2_state;
        PolyBlep_3___State m_osc3_state;
        PolyBlep_4___State m_osc4_state;
        PolyBlep_5___State m_osc5_state;
        PolyBlep_6___State m_osc6_state;
        PolyBlep_7___State m_osc7_state;
        PolyBlep_8___State m_osc8_state;
        PolyBlep_9___State m_osc9_state;
        PolyBlep_10___State m_osc10_state;
        PolyBlep_11___State m_osc11_state;
        PolyBlep_12___State m_osc12_state;
        PolyBlep_13___State m_osc13_state;
        PolyBlep_14___State m_osc14_state;
        PolyBlep_15___State m_osc15_state;
        PolyBlep_16___State m_osc16_state;
    };

    struct soul__midi__MPEParser___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
    };

    struct soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator__VoiceInfo
    {
        bool m_active;
        int32_t m_channel;
        float m_note;
        int32_t m_voiceAge;
    };

    struct soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId, m_nextAllocatedVoiceAge, m_nextUnallocatedVoiceAge;
        FixedArray<soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator__VoiceInfo, 8> m_voiceInfo;
    };

    struct soul__gain__DynamicGain___for__root__SineSynth_gainProcessor___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
    };

    struct soul__gain__SmoothedGainParameter___for__root__SineSynth_smoothedGain___State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId;
        float m_targetGain, m_currentGain, m_increment;
        int32_t m_remainingRampSamples;
    };

    struct _State
    {
        int32_t m__resumePoint, m__frameCount, m__arrayEntry, m__sessionID, m__processorId, m__framesToAdvance;
        _RenderStats m__renderStats;
        _SparseStreamStatus m__sparseStreamStatus;
        _Event_in_struct_Message_1 m__in_midiIn;
        _Event_in_f32_1 m__in_volume, m__in_ampAttack, m__in_ampDecay, m__in_ampSustain, m__in_ampRelease, m__in_detune, m__in_voiceCount, m__in_cutoff, m__in_x;
        _Stream_out_vec_2_f32_512 m__out_audioOut;
        FixedArray<Voice___State, 8> m_voices_state;
        soul__midi__MPEParser___State m_midiParser_state;
        soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___State m_voiceAllocator_state;
        soul__gain__DynamicGain___for__root__SineSynth_gainProcessor___State m_gainProcessor_state;
        soul__gain__SmoothedGainParameter___for__root__SineSynth_smoothedGain___State m_smoothedGain_state;
    };

    struct soul__midi__Message
    {
        int32_t m_midiBytes;
    };

    struct soul__note_events__NoteOn
    {
        int32_t m_channel;
        float m_note, m_velocity;
    };

    struct soul__note_events__NoteOff
    {
        int32_t m_channel;
        float m_note, m_velocity;
    };

    struct soul__note_events__PitchBend
    {
        int32_t m_channel;
        float m_bendSemitones;
    };

    struct soul__note_events__Pressure
    {
        int32_t m_channel;
        float m_pressure;
    };

    struct soul__note_events__Slide
    {
        int32_t m_channel;
        float m_slide;
    };

    struct soul__note_events__Control
    {
        int32_t m_channel, m_control;
        float m_value;
    };

    struct soul__gain__SmoothedGainParameter___for__root__SineSynth_smoothedGain___IO
    {
        float m__out_gain;
    };

    struct soul__gain__DynamicGain___for__root__SineSynth_gainProcessor___IO
    {
        Vector<float, 2> m__in_in;
        float m__in_gain;
        Vector<float, 2> m__out_out;
    };

    struct Voice___IO
    {
        Vector<float, 2> m__out_audioOut;
    };

    struct PolyBlep___IO
    {
        float m__in_activeIn, m__in_frequencyIn, m__in_gainIn, m__in_detuneIn;
        Vector<float, 2> m__out_audioOut;
    };

    struct Controller___IO
    {
        float m__out_note1_noteOut, m__out_note1_frequencyOut, m__out_note1_velocity, m__out_note1_active, m__out_note2_frequencyOut, m__out_note1_pressure, m__out_note1_slide;
    };

    struct ADSREnvelope___IO
    {
        float m__in_triggerLevel, m__out_envelopeLevel;
    };

    struct Detuner___IO
    {
        float m__out_logOut, m__out_activeOut1, m__out_activeOut2, m__out_activeOut3, m__out_activeOut4, m__out_activeOut5, m__out_activeOut6, m__out_activeOut7, m__out_activeOut8, m__out_activeOut9, m__out_activeOut10, m__out_activeOut11, m__out_activeOut12, m__out_activeOut13, m__out_activeOut14, m__out_activeOut15,
         m__out_activeOut16, m__out_detuneOut1, m__out_detuneOut2, m__out_detuneOut3, m__out_detuneOut4, m__out_detuneOut5, m__out_detuneOut6, m__out_detuneOut7, m__out_detuneOut8, m__out_detuneOut9, m__out_detuneOut10, m__out_detuneOut11, m__out_detuneOut12, m__out_detuneOut13, m__out_detuneOut14, m__out_detuneOut15,
         m__out_detuneOut16;
    };

    struct Filter___IO
    {
        float m__in_gainIn, m__in_resonanceIn;
        Vector<float, 2> m__in_audioIn, m__out_audioOut;
    };

    struct Envelope___IO
    {
        float m__in_triggerLevel, m__out_envelopeLevel;
    };

    struct PolyBlep_2___IO
    {
        float m__in_activeIn, m__in_frequencyIn, m__in_gainIn, m__in_detuneIn;
        Vector<float, 2> m__out_audioOut;
    };

    struct PolyBlep_3___IO
    {
        float m__in_activeIn, m__in_frequencyIn, m__in_gainIn, m__in_detuneIn;
        Vector<float, 2> m__out_audioOut;
    };

    struct PolyBlep_4___IO
    {
        float m__in_activeIn, m__in_frequencyIn, m__in_gainIn, m__in_detuneIn;
        Vector<float, 2> m__out_audioOut;
    };

    struct PolyBlep_5___IO
    {
        float m__in_activeIn, m__in_frequencyIn, m__in_gainIn, m__in_detuneIn;
        Vector<float, 2> m__out_audioOut;
    };

    struct PolyBlep_6___IO
    {
        float m__in_activeIn, m__in_frequencyIn, m__in_gainIn, m__in_detuneIn;
        Vector<float, 2> m__out_audioOut;
    };

    struct PolyBlep_7___IO
    {
        float m__in_activeIn, m__in_frequencyIn, m__in_gainIn, m__in_detuneIn;
        Vector<float, 2> m__out_audioOut;
    };

    struct PolyBlep_8___IO
    {
        float m__in_activeIn, m__in_frequencyIn, m__in_gainIn, m__in_detuneIn;
        Vector<float, 2> m__out_audioOut;
    };

    struct PolyBlep_9___IO
    {
        float m__in_activeIn, m__in_frequencyIn, m__in_gainIn, m__in_detuneIn;
        Vector<float, 2> m__out_audioOut;
    };

    struct PolyBlep_10___IO
    {
        float m__in_activeIn, m__in_frequencyIn, m__in_gainIn, m__in_detuneIn;
        Vector<float, 2> m__out_audioOut;
    };

    struct PolyBlep_11___IO
    {
        float m__in_activeIn, m__in_frequencyIn, m__in_gainIn, m__in_detuneIn;
        Vector<float, 2> m__out_audioOut;
    };

    struct PolyBlep_12___IO
    {
        float m__in_activeIn, m__in_frequencyIn, m__in_gainIn, m__in_detuneIn;
        Vector<float, 2> m__out_audioOut;
    };

    struct PolyBlep_13___IO
    {
        float m__in_activeIn, m__in_frequencyIn, m__in_gainIn, m__in_detuneIn;
        Vector<float, 2> m__out_audioOut;
    };

    struct PolyBlep_14___IO
    {
        float m__in_activeIn, m__in_frequencyIn, m__in_gainIn, m__in_detuneIn;
        Vector<float, 2> m__out_audioOut;
    };

    struct PolyBlep_15___IO
    {
        float m__in_activeIn, m__in_frequencyIn, m__in_gainIn, m__in_detuneIn;
        Vector<float, 2> m__out_audioOut;
    };

    struct PolyBlep_16___IO
    {
        float m__in_activeIn, m__in_frequencyIn, m__in_gainIn, m__in_detuneIn;
        Vector<float, 2> m__out_audioOut;
    };

    //==============================================================================
    #if __clang__
     #pragma clang diagnostic push
     #pragma clang diagnostic ignored "-Wunused-label"
     #pragma clang diagnostic ignored "-Wunused-parameter"
     #pragma clang diagnostic ignored "-Wshadow"
    #elif defined(__GNUC__)
     #pragma GCC diagnostic push
     #pragma GCC diagnostic ignored "-Wunused-label"
     #pragma GCC diagnostic ignored "-Wunused-parameter"
     #pragma GCC diagnostic ignored "-Wshadow"
    #elif defined(_MSC_VER)
     #pragma warning (push)
     #pragma warning (disable: 4100 4102 4458)
    #endif

    //==============================================================================
    int32_t run (_State& _state, int32_t maxFrames) noexcept
    {
        int32_t _2 = {};
        FixedArray<Voice___IO, 8> _3 = {};
        soul__gain__SmoothedGainParameter___for__root__SineSynth_smoothedGain___IO _4 = {};
        soul__gain__DynamicGain___for__root__SineSynth_gainProcessor___IO _5 = {};

        _2 = _internal___minInt32 (512, maxFrames);
        _updateRampingStreams (_state, _2);
        _state.m__frameCount = 0;
        _main_loop_check: { if (! (_state.m__frameCount < _2)) goto _exit; }
        _main_loop_body: { _3 = ZeroInitialiser();
                           Voice__run (_state.m_voices_state[0], _3[0]);
                           Voice__run (_state.m_voices_state[1], _3[1]);
                           Voice__run (_state.m_voices_state[2], _3[2]);
                           Voice__run (_state.m_voices_state[3], _3[3]);
                           Voice__run (_state.m_voices_state[4], _3[4]);
                           Voice__run (_state.m_voices_state[5], _3[5]);
                           Voice__run (_state.m_voices_state[6], _3[6]);
                           Voice__run (_state.m_voices_state[7], _3[7]);
                           _4 = ZeroInitialiser();
                           soul__gain__SmoothedGainParameter___for__root__SineSynth_smoothedGain__run (_state.m_smoothedGain_state, _4);
                           _5 = ZeroInitialiser();
                           _5.m__in_in = ((((((_3[0].m__out_audioOut + _3[1].m__out_audioOut) + _3[2].m__out_audioOut) + _3[3].m__out_audioOut) + _3[4].m__out_audioOut) + _3[5].m__out_audioOut) + _3[6].m__out_audioOut) + _3[7].m__out_audioOut;
                           _5.m__in_gain = _4.m__out_gain;
                           soul__gain__DynamicGain___for__root__SineSynth_gainProcessor__run (_state.m_gainProcessor_state, _5);
                           _writeToStream_struct__Stream_out_vec_2_f32_512 (_state.m__out_audioOut, _state.m__frameCount, _5.m__out_out);
                           _state.m__frameCount = _state.m__frameCount + 1;
                           goto _main_loop_check;
        }
        _exit: { _state.m__frameCount = 0;
                 return _2;
        }
    }

    void _initialise (_State& _state, int32_t sessionID) noexcept
    {
        _state.m__sessionID = sessionID;
        _state.m_voices_state[0].m__arrayEntry = 0;
        _state.m_voices_state[0].m__sessionID = _state.m__sessionID;
        _state.m_voices_state[0].m__processorId = 22;
        Voice___initialise (_state.m_voices_state[0]);
        _state.m_voices_state[1].m__arrayEntry = 1;
        _state.m_voices_state[1].m__sessionID = _state.m__sessionID;
        _state.m_voices_state[1].m__processorId = 23;
        Voice___initialise (_state.m_voices_state[1]);
        _state.m_voices_state[2].m__arrayEntry = 2;
        _state.m_voices_state[2].m__sessionID = _state.m__sessionID;
        _state.m_voices_state[2].m__processorId = 24;
        Voice___initialise (_state.m_voices_state[2]);
        _state.m_voices_state[3].m__arrayEntry = 3;
        _state.m_voices_state[3].m__sessionID = _state.m__sessionID;
        _state.m_voices_state[3].m__processorId = 25;
        Voice___initialise (_state.m_voices_state[3]);
        _state.m_voices_state[4].m__arrayEntry = 4;
        _state.m_voices_state[4].m__sessionID = _state.m__sessionID;
        _state.m_voices_state[4].m__processorId = 26;
        Voice___initialise (_state.m_voices_state[4]);
        _state.m_voices_state[5].m__arrayEntry = 5;
        _state.m_voices_state[5].m__sessionID = _state.m__sessionID;
        _state.m_voices_state[5].m__processorId = 27;
        Voice___initialise (_state.m_voices_state[5]);
        _state.m_voices_state[6].m__arrayEntry = 6;
        _state.m_voices_state[6].m__sessionID = _state.m__sessionID;
        _state.m_voices_state[6].m__processorId = 28;
        Voice___initialise (_state.m_voices_state[6]);
        _state.m_voices_state[7].m__arrayEntry = 7;
        _state.m_voices_state[7].m__sessionID = _state.m__sessionID;
        _state.m_voices_state[7].m__processorId = 29;
        Voice___initialise (_state.m_voices_state[7]);
        _state.m_midiParser_state.m__arrayEntry = 0;
        _state.m_midiParser_state.m__sessionID = _state.m__sessionID;
        _state.m_midiParser_state.m__processorId = 30;
        _state.m_voiceAllocator_state.m__arrayEntry = 0;
        _state.m_voiceAllocator_state.m__sessionID = _state.m__sessionID;
        _state.m_voiceAllocator_state.m__processorId = 31;
        soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___initialise (_state.m_voiceAllocator_state);
        _state.m_gainProcessor_state.m__arrayEntry = 0;
        _state.m_gainProcessor_state.m__sessionID = _state.m__sessionID;
        _state.m_gainProcessor_state.m__processorId = 32;
        _state.m_smoothedGain_state.m__arrayEntry = 0;
        _state.m_smoothedGain_state.m__sessionID = _state.m__sessionID;
        _state.m_smoothedGain_state.m__processorId = 33;
        soul__gain__SmoothedGainParameter___for__root__SineSynth_smoothedGain___initialise (_state.m_smoothedGain_state);
    }

    void _addInputEvent_midiIn_struct_Message (_State& _state, const soul__midi__Message& event) noexcept
    {
        soul__midi__MPEParser___parseMIDI_struct_Message (_state.m_midiParser_state, event);
    }

    void _addInputEvent_volume_f32 (_State& _state, const float& event) noexcept
    {
        soul__gain__SmoothedGainParameter___for__root__SineSynth_smoothedGain___volume_f32 (_state.m_smoothedGain_state, event);
    }

    void _addInputEvent_ampAttack_f32 (_State& _state, const float& event) noexcept
    {
        Voice___ampAttack_f32 (_state.m_voices_state[0], event);
        Voice___ampAttack_f32 (_state.m_voices_state[1], event);
        Voice___ampAttack_f32 (_state.m_voices_state[2], event);
        Voice___ampAttack_f32 (_state.m_voices_state[3], event);
        Voice___ampAttack_f32 (_state.m_voices_state[4], event);
        Voice___ampAttack_f32 (_state.m_voices_state[5], event);
        Voice___ampAttack_f32 (_state.m_voices_state[6], event);
        Voice___ampAttack_f32 (_state.m_voices_state[7], event);
    }

    void _addInputEvent_ampDecay_f32 (_State& _state, const float& event) noexcept
    {
        Voice___ampDecay_f32 (_state.m_voices_state[0], event);
        Voice___ampDecay_f32 (_state.m_voices_state[1], event);
        Voice___ampDecay_f32 (_state.m_voices_state[2], event);
        Voice___ampDecay_f32 (_state.m_voices_state[3], event);
        Voice___ampDecay_f32 (_state.m_voices_state[4], event);
        Voice___ampDecay_f32 (_state.m_voices_state[5], event);
        Voice___ampDecay_f32 (_state.m_voices_state[6], event);
        Voice___ampDecay_f32 (_state.m_voices_state[7], event);
    }

    void _addInputEvent_ampSustain_f32 (_State& _state, const float& event) noexcept
    {
        Voice___ampSustain_f32 (_state.m_voices_state[0], event);
        Voice___ampSustain_f32 (_state.m_voices_state[1], event);
        Voice___ampSustain_f32 (_state.m_voices_state[2], event);
        Voice___ampSustain_f32 (_state.m_voices_state[3], event);
        Voice___ampSustain_f32 (_state.m_voices_state[4], event);
        Voice___ampSustain_f32 (_state.m_voices_state[5], event);
        Voice___ampSustain_f32 (_state.m_voices_state[6], event);
        Voice___ampSustain_f32 (_state.m_voices_state[7], event);
    }

    void _addInputEvent_ampRelease_f32 (_State& _state, const float& event) noexcept
    {
        Voice___ampRelease_f32 (_state.m_voices_state[0], event);
        Voice___ampRelease_f32 (_state.m_voices_state[1], event);
        Voice___ampRelease_f32 (_state.m_voices_state[2], event);
        Voice___ampRelease_f32 (_state.m_voices_state[3], event);
        Voice___ampRelease_f32 (_state.m_voices_state[4], event);
        Voice___ampRelease_f32 (_state.m_voices_state[5], event);
        Voice___ampRelease_f32 (_state.m_voices_state[6], event);
        Voice___ampRelease_f32 (_state.m_voices_state[7], event);
    }

    void _addInputEvent_detune_f32 (_State& _state, const float& event) noexcept
    {
        Voice___detune_f32 (_state.m_voices_state[0], event);
        Voice___detune_f32 (_state.m_voices_state[1], event);
        Voice___detune_f32 (_state.m_voices_state[2], event);
        Voice___detune_f32 (_state.m_voices_state[3], event);
        Voice___detune_f32 (_state.m_voices_state[4], event);
        Voice___detune_f32 (_state.m_voices_state[5], event);
        Voice___detune_f32 (_state.m_voices_state[6], event);
        Voice___detune_f32 (_state.m_voices_state[7], event);
    }

    void _addInputEvent_voiceCount_f32 (_State& _state, const float& event) noexcept
    {
        Voice___voiceCountIn_f32 (_state.m_voices_state[0], event);
        Voice___voiceCountIn_f32 (_state.m_voices_state[1], event);
        Voice___voiceCountIn_f32 (_state.m_voices_state[2], event);
        Voice___voiceCountIn_f32 (_state.m_voices_state[3], event);
        Voice___voiceCountIn_f32 (_state.m_voices_state[4], event);
        Voice___voiceCountIn_f32 (_state.m_voices_state[5], event);
        Voice___voiceCountIn_f32 (_state.m_voices_state[6], event);
        Voice___voiceCountIn_f32 (_state.m_voices_state[7], event);
    }

    void _addInputEvent_cutoff_f32 (_State& _state, const float& event) noexcept
    {
        Voice___cutoffIn_f32 (_state.m_voices_state[0], event);
        Voice___cutoffIn_f32 (_state.m_voices_state[1], event);
        Voice___cutoffIn_f32 (_state.m_voices_state[2], event);
        Voice___cutoffIn_f32 (_state.m_voices_state[3], event);
        Voice___cutoffIn_f32 (_state.m_voices_state[4], event);
        Voice___cutoffIn_f32 (_state.m_voices_state[5], event);
        Voice___cutoffIn_f32 (_state.m_voices_state[6], event);
        Voice___cutoffIn_f32 (_state.m_voices_state[7], event);
    }

    void _addInputEvent_x_f32 (_State& _state, const float& event) noexcept
    {
    }

    FixedArray<Vector<float, 2>, 512>& _getOutputFrameArrayRef_audioOut (_State& state) noexcept
    {
        return state.m__out_audioOut.m_buffer;
    }

    void _prepare (_State& state, int32_t frames) noexcept
    {
        state.m__framesToAdvance = frames;
    }

    int32_t _get_num_xruns (_State& state) noexcept
    {
        return state.m__renderStats.m_underrunCount + state.m__renderStats.m_overrunCount;
    }

    //==============================================================================
    bool _updateRampingStream (_State& _state, int32_t streamId, int32_t framesToRender) noexcept
    {
        bool rampComplete = {};

        rampComplete = false;
        return rampComplete;
    }

    void _updateRampingStreams (_State& _state, int32_t framesToRender) noexcept
    {
        bool rampComplete = {};
        int32_t readPos = {}, writePos = {};

        rampComplete = false;
        readPos = 0;
        writePos = 0;
        if (_state.m__sparseStreamStatus.m_activeRamps == 0) goto _exit;
        _loop: { rampComplete = _updateRampingStream (_state, _state.m__sparseStreamStatus.m_rampArray[readPos], framesToRender);
                 if (rampComplete) goto _rampComplete;
        }
        _rampActive: { _state.m__sparseStreamStatus.m_rampArray[writePos] = _state.m__sparseStreamStatus.m_rampArray[readPos];
                       readPos = readPos + 1;
                       writePos = writePos + 1;
                       if (readPos == _state.m__sparseStreamStatus.m_activeRamps) goto _loopExit;
                       goto _loop;
        }
        _rampComplete: { readPos = readPos + 1;
                         if (! (readPos == _state.m__sparseStreamStatus.m_activeRamps)) goto _loop;
        }
        _loopExit: { _state.m__sparseStreamStatus.m_activeRamps = writePos; }
        _exit: {}
    }

    void _writeToStream_struct__Stream_out_vec_2_f32_512 (_Stream_out_vec_2_f32_512& stream, int32_t writePos, Vector<float, 2> value) noexcept
    {
        stream.m_buffer[writePos] = value;
    }

    //==============================================================================
    float soul__dBtoGain (float decibels) noexcept
    {
        float _2 = {}, _3 = {}, _4 = {}, _T0 = {};

        if (! (decibels > -100.0f)) goto _ternary_false_0;
        _ternary_true_0: { _2 = std::pow (10.0f, decibels * 0.05f);
                           _T0 = _2;
                           goto _ternary_end_0;
        }
        _ternary_false_0: { _3 = 0;
                            _T0 = _3;
        }
        _ternary_end_0: { _4 = _T0;
                          return _4;
        }
    }

    float soul__noteNumberToFrequency_2 (float note) noexcept
    {
        float _2 = {};

        _2 = std::pow (2.0f, (note - 69.0f) * 0.083333336f);
        return 440.0f * _2;
    }

    //==============================================================================
    float soul__intrinsics___pow_specialised_2_f32_f32 (float a, float b) noexcept
    {
        return 0;
    }

    float soul__intrinsics___sin_specialised_1_f32 (float n) noexcept
    {
        return 0;
    }

    float soul__intrinsics___abs_specialised_1_f32 (float n) noexcept
    {
        float _2 = {}, _3 = {}, _4 = {}, _T5 = {};

        if (! (n < 0)) goto _ternary_false_5;
        _ternary_true_5: { _2 = -n;
                           _T5 = _2;
                           goto _ternary_end_5;
        }
        _ternary_false_5: { _3 = n;
                            _T5 = _3;
        }
        _ternary_end_5: { _4 = _T5;
                          return _4;
        }
    }

    int32_t soul__intrinsics___max_specialised_2_i32_i32 (int32_t a, int32_t b) noexcept
    {
        int32_t _2 = {}, _3 = {}, _4 = {}, _T6 = {};

        if (! (a > b)) goto _ternary_false_6;
        _ternary_true_6: { _2 = a;
                           _T6 = _2;
                           goto _ternary_end_6;
        }
        _ternary_false_6: { _3 = b;
                            _T6 = _3;
        }
        _ternary_end_6: { _4 = _T6;
                          return _4;
        }
    }

    double soul__intrinsics___pow_specialised_2_f64_const_f64 (double a, double b) noexcept
    {
        return 0;
    }

    float soul__intrinsics___pow_specialised_2_f32_const_f32 (float a, float b) noexcept
    {
        return 0;
    }

    double soul__intrinsics___min_specialised_2_f64_f64 (double a, double b) noexcept
    {
        double _2 = {}, _3 = {}, _4 = {}, _T7 = {};

        if (! (a < b)) goto _ternary_false_7;
        _ternary_true_7: { _2 = a;
                           _T7 = _2;
                           goto _ternary_end_7;
        }
        _ternary_false_7: { _3 = b;
                            _T7 = _3;
        }
        _ternary_end_7: { _4 = _T7;
                          return _4;
        }
    }

    float soul__intrinsics___clamp_specialised_3_f32_f32_f32 (float n, float low, float high) noexcept
    {
        float _2 = {}, _3 = {}, _4 = {}, _5 = {}, _6 = {}, _T9 = {}, _T8 = {};

        if (! (n < low)) goto _ternary_false_8;
        _ternary_true_8: { _2 = low;
                           _T8 = _2;
                           goto _ternary_end_8;
        }
        _ternary_false_8: { if (! (n > high)) goto _ternary_false_9; }
        _ternary_true_9: { _3 = high;
                           _T9 = _3;
                           goto _ternary_end_9;
        }
        _ternary_false_9: { _4 = n;
                            _T9 = _4;
        }
        _ternary_end_9: { _5 = _T9;
                          _T8 = _5;
        }
        _ternary_end_8: { _6 = _T8;
                          return _6;
        }
    }

    float soul__intrinsics___tan_specialised_1_f32 (float n) noexcept
    {
        float _2 = {}, _3 = {};

        _2 = std::sin (n);
        _3 = std::cos (n);
        return _2 / _3;
    }

    float soul__intrinsics___cos_specialised_1_f32 (float n) noexcept
    {
        return 0;
    }

    //==============================================================================
    int32_t soul__midi__getByte1 (soul__midi__Message m) noexcept
    {
        return (m.m_midiBytes >> 16) & 255;
    }

    int32_t soul__midi__getByte2 (soul__midi__Message m) noexcept
    {
        return (m.m_midiBytes >> 8) & 255;
    }

    int32_t soul__midi__getByte3 (soul__midi__Message m) noexcept
    {
        return m.m_midiBytes & 255;
    }

    //==============================================================================
    static _State& _stateUpCast (soul__midi__MPEParser___State& s)
    {
        auto offset = static_cast<int32_t> (offsetof (_State, m_midiParser_state));
        return *reinterpret_cast<_State*> (reinterpret_cast<char*> (&s) - offset);
    }

    //==============================================================================
    void soul__midi__MPEParser___parseMIDI_struct_Message (soul__midi__MPEParser___State& _state, soul__midi__Message message) noexcept
    {
        int32_t _2 = {}, _3 = {}, _4 = {};
        int32_t messageByte1 = {}, messageByte2 = {}, messageByte3 = {}, messageType = {}, channel = {};
        soul__note_events__NoteOff _5 = {}, _6 = {};
        float _7 = {}, _8 = {}, _9 = {}, _10 = {}, _11 = {}, _12 = {};
        soul__note_events__NoteOn _13 = {};
        soul__note_events__Slide _14 = {};
        soul__note_events__Control _15 = {};
        soul__note_events__Pressure _16 = {};
        soul__note_events__PitchBend _17 = {};

        _2 = soul__midi__getByte1 (message);
        messageByte1 = static_cast<int32_t> (_2);
        _3 = soul__midi__getByte2 (message);
        messageByte2 = static_cast<int32_t> (_3);
        _4 = soul__midi__getByte3 (message);
        messageByte3 = static_cast<int32_t> (_4);
        messageType = static_cast<int32_t> (static_cast<int32_t> (messageByte1) & 240);
        channel = static_cast<int32_t> (static_cast<int32_t> (messageByte1) & 15);
        if (! (messageType == 128)) goto _ifnot_0;
        _if_0: { _5 = ZeroInitialiser();
                 _5.m_channel = static_cast<int32_t> (channel);
                 _5.m_note = static_cast<float> (messageByte2);
                 _7 = soul__midi__MPEParser__normaliseValue (_state, static_cast<int32_t> (messageByte3));
                 _5.m_velocity = _7;
                 soul__midi__MPEParser___eventOut_struct_NoteOff (_state, _5);
                 goto _ifend_0;
        }
        _ifnot_0: { if (! (messageType == 144)) goto _ifnot_1; }
        _if_1: { if (! (messageByte3 == 0)) goto _ifnot_2; }
        _if_2: { _6 = ZeroInitialiser();
                 _6.m_channel = static_cast<int32_t> (channel);
                 _6.m_note = static_cast<float> (messageByte2);
                 soul__midi__MPEParser___eventOut_struct_NoteOff (_state, _6);
                 goto _ifend_0;
        }
        _ifnot_2: { _13 = ZeroInitialiser();
                    _13.m_channel = static_cast<int32_t> (channel);
                    _13.m_note = static_cast<float> (messageByte2);
                    _8 = soul__midi__MPEParser__normaliseValue (_state, static_cast<int32_t> (messageByte3));
                    _13.m_velocity = _8;
                    soul__midi__MPEParser___eventOut_struct_NoteOn (_state, _13);
                    goto _ifend_0;
        }
        _ifnot_1: { if (! (messageType == 176)) goto _ifnot_3; }
        _if_3: { if (! (messageByte2 == 74)) goto _ifnot_4; }
        _if_4: { _14 = ZeroInitialiser();
                 _14.m_channel = static_cast<int32_t> (channel);
                 _9 = soul__midi__MPEParser__normaliseValue (_state, static_cast<int32_t> (messageByte3));
                 _14.m_slide = _9;
                 soul__midi__MPEParser___eventOut_struct_Slide (_state, _14);
                 goto _ifend_0;
        }
        _ifnot_4: { _15 = ZeroInitialiser();
                    _15.m_channel = static_cast<int32_t> (channel);
                    _15.m_control = static_cast<int32_t> (messageByte2);
                    _10 = soul__midi__MPEParser__normaliseValue (_state, static_cast<int32_t> (messageByte3));
                    _15.m_value = _10;
                    goto _ifend_0;
        }
        _ifnot_3: { if (! (messageType == 208)) goto _ifnot_5; }
        _if_5: { _16 = ZeroInitialiser();
                 _16.m_channel = static_cast<int32_t> (channel);
                 _11 = soul__midi__MPEParser__normaliseValue (_state, static_cast<int32_t> (messageByte2));
                 _16.m_pressure = _11;
                 soul__midi__MPEParser___eventOut_struct_Pressure (_state, _16);
                 goto _ifend_0;
        }
        _ifnot_5: { if (! (messageType == 224)) goto _ifend_0; }
        _if_6: { _17 = ZeroInitialiser();
                 _17.m_channel = static_cast<int32_t> (channel);
                 _12 = soul__midi__MPEParser__translateBendSemitones (_state, static_cast<int32_t> (messageByte3), static_cast<int32_t> (messageByte2));
                 _17.m_bendSemitones = _12;
                 soul__midi__MPEParser___eventOut_struct_PitchBend (_state, _17);
        }
        _ifend_0: {}
    }

    float soul__midi__MPEParser__normaliseValue (soul__midi__MPEParser___State& _state, int32_t i) noexcept
    {
        return static_cast<float> (i) * 0.007874016f;
    }

    float soul__midi__MPEParser__translateBendSemitones (soul__midi__MPEParser___State& _state, int32_t msb, int32_t lsb) noexcept
    {
        int32_t value = {};

        value = static_cast<int32_t> ((msb * 128) + lsb);
        return static_cast<float> (value - 8192) / 170.66667f;
    }

    void soul__midi__MPEParser___eventOut_struct_NoteOff (soul__midi__MPEParser___State& _state, soul__note_events__NoteOff value) noexcept
    {
        auto& _2 = _stateUpCast (_state);
        soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___eventIn_struct_NoteOff (_2.m_voiceAllocator_state, value);
    }

    void soul__midi__MPEParser___eventOut_struct_NoteOn (soul__midi__MPEParser___State& _state, soul__note_events__NoteOn value) noexcept
    {
        auto& _2 = _stateUpCast (_state);
        soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___eventIn_struct_NoteOn (_2.m_voiceAllocator_state, value);
    }

    void soul__midi__MPEParser___eventOut_struct_Slide (soul__midi__MPEParser___State& _state, soul__note_events__Slide value) noexcept
    {
        auto& _2 = _stateUpCast (_state);
        soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___eventIn_struct_Slide (_2.m_voiceAllocator_state, value);
    }

    void soul__midi__MPEParser___eventOut_struct_Pressure (soul__midi__MPEParser___State& _state, soul__note_events__Pressure value) noexcept
    {
        auto& _2 = _stateUpCast (_state);
        soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___eventIn_struct_Pressure (_2.m_voiceAllocator_state, value);
    }

    void soul__midi__MPEParser___eventOut_struct_PitchBend (soul__midi__MPEParser___State& _state, soul__note_events__PitchBend value) noexcept
    {
        auto& _2 = _stateUpCast (_state);
        soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___eventIn_struct_PitchBend (_2.m_voiceAllocator_state, value);
    }

    //==============================================================================
    static _State& _stateUpCast (soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___State& s)
    {
        auto offset = static_cast<int32_t> (offsetof (_State, m_voiceAllocator_state));
        return *reinterpret_cast<_State*> (reinterpret_cast<char*> (&s) - offset);
    }

    //==============================================================================
    void soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___eventIn_struct_NoteOn (soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___State& _state, soul__note_events__NoteOn e) noexcept
    {
        int32_t allocatedVoice = {};
        int32_t allocatedVoiceAge = {}, i = {}, _2 = {}, _3 = {};
        int32_t age = {};
        soul__note_events__NoteOff noteOff = {};

        allocatedVoice = 0;
        allocatedVoiceAge = _state.m_voiceInfo[allocatedVoice].m_voiceAge;
        i = 1;
        _loop_0: { if (! (i < 8)) goto _break_0; }
        _body_0: { age = static_cast<int32_t> (_state.m_voiceInfo[_intrin_wrap (static_cast<int32_t> (i), 8) & 7].m_voiceAge);
                   if (! (age < static_cast<int32_t> (allocatedVoiceAge))) goto _cont_0;
        }
        _if_0: { allocatedVoiceAge = static_cast<int32_t> (age);
                 allocatedVoice = _intrin_wrap (static_cast<int32_t> (i), 8) & 7;
        }
        _cont_0: { _2 = i;
                   i = _2 + 1;
                   goto _loop_0;
        }
        _break_0: { soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___voiceEventOut_struct_NoteOn (_state, static_cast<int32_t> (allocatedVoice) & 7, e);
                    if (! _state.m_voiceInfo[allocatedVoice].m_active) goto _ifnot_1;
        }
        _if_1: { noteOff = ZeroInitialiser();
                 noteOff.m_channel = _state.m_voiceInfo[allocatedVoice].m_channel;
                 noteOff.m_note = _state.m_voiceInfo[allocatedVoice].m_note;
                 soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___voiceEventOut_struct_NoteOff (_state, static_cast<int32_t> (allocatedVoice) & 7, noteOff);
        }
        _ifnot_1: { _state.m_voiceInfo[allocatedVoice].m_active = true;
                    _state.m_voiceInfo[allocatedVoice].m_channel = e.m_channel;
                    _state.m_voiceInfo[allocatedVoice].m_note = e.m_note;
                    _3 = _state.m_nextAllocatedVoiceAge;
                    _state.m_nextAllocatedVoiceAge = _3 + 1;
                    _state.m_voiceInfo[allocatedVoice].m_voiceAge = _3;
        }
    }

    void soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___eventIn_struct_NoteOff (soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___State& _state, soul__note_events__NoteOff e) noexcept
    {
        int32_t voice = {}, _2 = {};
        int32_t counter_1 = {};
        bool _3 = {}, _4 = {}, _5 = {}, _T3 = {};
        int32_t _6 = {};

        voice = 0;
        counter_1 = 8;
        _loop_1: { if (! (counter_1 > 0)) goto _break_1; }
        _body_1: { if (! (_state.m_voiceInfo[voice].m_channel == e.m_channel)) goto _ternary_false_3; }
        _ternary_true_3: { _3 = _state.m_voiceInfo[voice].m_note == e.m_note;
                           _T3 = _3;
                           goto _ternary_end_3;
        }
        _ternary_false_3: { _4 = false;
                            _T3 = _4;
        }
        _ternary_end_3: { _5 = _T3;
                          if (! _5) goto _ifnot_2;
        }
        _if_2: { _state.m_voiceInfo[voice].m_active = false;
                 _6 = _state.m_nextUnallocatedVoiceAge;
                 _state.m_nextUnallocatedVoiceAge = _6 + 1;
                 _state.m_voiceInfo[voice].m_voiceAge = _6;
                 soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___voiceEventOut_struct_NoteOff (_state, static_cast<int32_t> (voice) & 7, e);
        }
        _ifnot_2: { _2 = voice;
                    voice = (_2 + 1) & 7;
                    counter_1 = counter_1 - 1;
                    goto _loop_1;
        }
        _break_1: {}
    }

    void soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___eventIn_struct_PitchBend (soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___State& _state, soul__note_events__PitchBend e) noexcept
    {
        int32_t voice = {}, _2 = {};
        int32_t counter_2 = {};

        voice = 0;
        counter_2 = 8;
        _loop_2: { if (! (counter_2 > 0)) goto _break_2; }
        _body_2: { if (! (_state.m_voiceInfo[voice].m_channel == e.m_channel)) goto _ifnot_4; }
        _if_4: { soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___voiceEventOut_struct_PitchBend (_state, static_cast<int32_t> (voice) & 7, e); }
        _ifnot_4: { _2 = voice;
                    voice = (_2 + 1) & 7;
                    counter_2 = counter_2 - 1;
                    goto _loop_2;
        }
        _break_2: {}
    }

    void soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___eventIn_struct_Pressure (soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___State& _state, soul__note_events__Pressure p) noexcept
    {
        int32_t voice = {}, _2 = {};
        int32_t counter_3 = {};

        voice = 0;
        counter_3 = 8;
        _loop_3: { if (! (counter_3 > 0)) goto _break_3; }
        _body_3: { if (! (_state.m_voiceInfo[voice].m_channel == p.m_channel)) goto _ifnot_5; }
        _if_5: { soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___voiceEventOut_struct_Pressure (_state, static_cast<int32_t> (voice) & 7, p); }
        _ifnot_5: { _2 = voice;
                    voice = (_2 + 1) & 7;
                    counter_3 = counter_3 - 1;
                    goto _loop_3;
        }
        _break_3: {}
    }

    void soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___eventIn_struct_Slide (soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___State& _state, soul__note_events__Slide s) noexcept
    {
        int32_t voice = {}, _2 = {};
        int32_t counter_4 = {};

        voice = 0;
        counter_4 = 8;
        _loop_4: { if (! (counter_4 > 0)) goto _break_4; }
        _body_4: { if (! (_state.m_voiceInfo[voice].m_channel == s.m_channel)) goto _ifnot_6; }
        _if_6: { soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___voiceEventOut_struct_Slide (_state, static_cast<int32_t> (voice) & 7, s); }
        _ifnot_6: { _2 = voice;
                    voice = (_2 + 1) & 7;
                    counter_4 = counter_4 - 1;
                    goto _loop_4;
        }
        _break_4: {}
    }

    void soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___initialise (soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___State& _state) noexcept
    {
        _state.m_nextAllocatedVoiceAge = 1000000000;
        _state.m_nextUnallocatedVoiceAge = 1;
        _state.m_voiceInfo = ZeroInitialiser();
    }

    void soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___voiceEventOut_struct_NoteOn (soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___State& _state, int32_t element, soul__note_events__NoteOn value) noexcept
    {
        if (element == 0) goto _element_0;
        _block_1: { if (element == 1) goto _element_1; }
        _block_2: { if (element == 2) goto _element_2; }
        _block_3: { if (element == 3) goto _element_3; }
        _block_4: { if (element == 4) goto _element_4; }
        _block_5: { if (element == 5) goto _element_5; }
        _block_6: { if (element == 6) goto _element_6; }
        _block_7: { if (element == 7) goto _element_7; }
        _block_8: { return; }
        _element_0: { auto& _2 = _stateUpCast (_state);
                      Voice___eventIn_struct_NoteOn (_2.m_voices_state[0], value);
                      return;
        }
        _element_1: { auto& _3 = _stateUpCast (_state);
                      Voice___eventIn_struct_NoteOn (_3.m_voices_state[1], value);
                      return;
        }
        _element_2: { auto& _4 = _stateUpCast (_state);
                      Voice___eventIn_struct_NoteOn (_4.m_voices_state[2], value);
                      return;
        }
        _element_3: { auto& _5 = _stateUpCast (_state);
                      Voice___eventIn_struct_NoteOn (_5.m_voices_state[3], value);
                      return;
        }
        _element_4: { auto& _6 = _stateUpCast (_state);
                      Voice___eventIn_struct_NoteOn (_6.m_voices_state[4], value);
                      return;
        }
        _element_5: { auto& _7 = _stateUpCast (_state);
                      Voice___eventIn_struct_NoteOn (_7.m_voices_state[5], value);
                      return;
        }
        _element_6: { auto& _8 = _stateUpCast (_state);
                      Voice___eventIn_struct_NoteOn (_8.m_voices_state[6], value);
                      return;
        }
        _element_7: { auto& _9 = _stateUpCast (_state);
                      Voice___eventIn_struct_NoteOn (_9.m_voices_state[7], value);
        }
    }

    void soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___voiceEventOut_struct_NoteOff (soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___State& _state, int32_t element, soul__note_events__NoteOff value) noexcept
    {
        if (element == 0) goto _element_0;
        _block_1: { if (element == 1) goto _element_1; }
        _block_2: { if (element == 2) goto _element_2; }
        _block_3: { if (element == 3) goto _element_3; }
        _block_4: { if (element == 4) goto _element_4; }
        _block_5: { if (element == 5) goto _element_5; }
        _block_6: { if (element == 6) goto _element_6; }
        _block_7: { if (element == 7) goto _element_7; }
        _block_8: { return; }
        _element_0: { auto& _2 = _stateUpCast (_state);
                      Voice___eventIn_struct_NoteOff (_2.m_voices_state[0], value);
                      return;
        }
        _element_1: { auto& _3 = _stateUpCast (_state);
                      Voice___eventIn_struct_NoteOff (_3.m_voices_state[1], value);
                      return;
        }
        _element_2: { auto& _4 = _stateUpCast (_state);
                      Voice___eventIn_struct_NoteOff (_4.m_voices_state[2], value);
                      return;
        }
        _element_3: { auto& _5 = _stateUpCast (_state);
                      Voice___eventIn_struct_NoteOff (_5.m_voices_state[3], value);
                      return;
        }
        _element_4: { auto& _6 = _stateUpCast (_state);
                      Voice___eventIn_struct_NoteOff (_6.m_voices_state[4], value);
                      return;
        }
        _element_5: { auto& _7 = _stateUpCast (_state);
                      Voice___eventIn_struct_NoteOff (_7.m_voices_state[5], value);
                      return;
        }
        _element_6: { auto& _8 = _stateUpCast (_state);
                      Voice___eventIn_struct_NoteOff (_8.m_voices_state[6], value);
                      return;
        }
        _element_7: { auto& _9 = _stateUpCast (_state);
                      Voice___eventIn_struct_NoteOff (_9.m_voices_state[7], value);
        }
    }

    void soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___voiceEventOut_struct_PitchBend (soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___State& _state, int32_t element, soul__note_events__PitchBend value) noexcept
    {
        if (element == 0) goto _element_0;
        _block_1: { if (element == 1) goto _element_1; }
        _block_2: { if (element == 2) goto _element_2; }
        _block_3: { if (element == 3) goto _element_3; }
        _block_4: { if (element == 4) goto _element_4; }
        _block_5: { if (element == 5) goto _element_5; }
        _block_6: { if (element == 6) goto _element_6; }
        _block_7: { if (element == 7) goto _element_7; }
        _block_8: { return; }
        _element_0: { auto& _2 = _stateUpCast (_state);
                      Voice___eventIn_struct_PitchBend (_2.m_voices_state[0], value);
                      return;
        }
        _element_1: { auto& _3 = _stateUpCast (_state);
                      Voice___eventIn_struct_PitchBend (_3.m_voices_state[1], value);
                      return;
        }
        _element_2: { auto& _4 = _stateUpCast (_state);
                      Voice___eventIn_struct_PitchBend (_4.m_voices_state[2], value);
                      return;
        }
        _element_3: { auto& _5 = _stateUpCast (_state);
                      Voice___eventIn_struct_PitchBend (_5.m_voices_state[3], value);
                      return;
        }
        _element_4: { auto& _6 = _stateUpCast (_state);
                      Voice___eventIn_struct_PitchBend (_6.m_voices_state[4], value);
                      return;
        }
        _element_5: { auto& _7 = _stateUpCast (_state);
                      Voice___eventIn_struct_PitchBend (_7.m_voices_state[5], value);
                      return;
        }
        _element_6: { auto& _8 = _stateUpCast (_state);
                      Voice___eventIn_struct_PitchBend (_8.m_voices_state[6], value);
                      return;
        }
        _element_7: { auto& _9 = _stateUpCast (_state);
                      Voice___eventIn_struct_PitchBend (_9.m_voices_state[7], value);
        }
    }

    void soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___voiceEventOut_struct_Pressure (soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___State& _state, int32_t element, soul__note_events__Pressure value) noexcept
    {
        if (element == 0) goto _element_0;
        _block_1: { if (element == 1) goto _element_1; }
        _block_2: { if (element == 2) goto _element_2; }
        _block_3: { if (element == 3) goto _element_3; }
        _block_4: { if (element == 4) goto _element_4; }
        _block_5: { if (element == 5) goto _element_5; }
        _block_6: { if (element == 6) goto _element_6; }
        _block_7: { if (element == 7) goto _element_7; }
        _block_8: { return; }
        _element_0: { auto& _2 = _stateUpCast (_state);
                      Voice___eventIn_struct_Pressure (_2.m_voices_state[0], value);
                      return;
        }
        _element_1: { auto& _3 = _stateUpCast (_state);
                      Voice___eventIn_struct_Pressure (_3.m_voices_state[1], value);
                      return;
        }
        _element_2: { auto& _4 = _stateUpCast (_state);
                      Voice___eventIn_struct_Pressure (_4.m_voices_state[2], value);
                      return;
        }
        _element_3: { auto& _5 = _stateUpCast (_state);
                      Voice___eventIn_struct_Pressure (_5.m_voices_state[3], value);
                      return;
        }
        _element_4: { auto& _6 = _stateUpCast (_state);
                      Voice___eventIn_struct_Pressure (_6.m_voices_state[4], value);
                      return;
        }
        _element_5: { auto& _7 = _stateUpCast (_state);
                      Voice___eventIn_struct_Pressure (_7.m_voices_state[5], value);
                      return;
        }
        _element_6: { auto& _8 = _stateUpCast (_state);
                      Voice___eventIn_struct_Pressure (_8.m_voices_state[6], value);
                      return;
        }
        _element_7: { auto& _9 = _stateUpCast (_state);
                      Voice___eventIn_struct_Pressure (_9.m_voices_state[7], value);
        }
    }

    void soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___voiceEventOut_struct_Slide (soul__voice_allocators__Basic___for__root__SineSynth_voiceAllocator___State& _state, int32_t element, soul__note_events__Slide value) noexcept
    {
        if (element == 0) goto _element_0;
        _block_1: { if (element == 1) goto _element_1; }
        _block_2: { if (element == 2) goto _element_2; }
        _block_3: { if (element == 3) goto _element_3; }
        _block_4: { if (element == 4) goto _element_4; }
        _block_5: { if (element == 5) goto _element_5; }
        _block_6: { if (element == 6) goto _element_6; }
        _block_7: { if (element == 7) goto _element_7; }
        _block_8: { return; }
        _element_0: { auto& _2 = _stateUpCast (_state);
                      Voice___eventIn_struct_Slide (_2.m_voices_state[0], value);
                      return;
        }
        _element_1: { auto& _3 = _stateUpCast (_state);
                      Voice___eventIn_struct_Slide (_3.m_voices_state[1], value);
                      return;
        }
        _element_2: { auto& _4 = _stateUpCast (_state);
                      Voice___eventIn_struct_Slide (_4.m_voices_state[2], value);
                      return;
        }
        _element_3: { auto& _5 = _stateUpCast (_state);
                      Voice___eventIn_struct_Slide (_5.m_voices_state[3], value);
                      return;
        }
        _element_4: { auto& _6 = _stateUpCast (_state);
                      Voice___eventIn_struct_Slide (_6.m_voices_state[4], value);
                      return;
        }
        _element_5: { auto& _7 = _stateUpCast (_state);
                      Voice___eventIn_struct_Slide (_7.m_voices_state[5], value);
                      return;
        }
        _element_6: { auto& _8 = _stateUpCast (_state);
                      Voice___eventIn_struct_Slide (_8.m_voices_state[6], value);
                      return;
        }
        _element_7: { auto& _9 = _stateUpCast (_state);
                      Voice___eventIn_struct_Slide (_9.m_voices_state[7], value);
        }
    }

    //==============================================================================
    void soul__gain__SmoothedGainParameter___for__root__SineSynth_smoothedGain___volume_f32 (soul__gain__SmoothedGainParameter___for__root__SineSynth_smoothedGain___State& _state, float targetDB) noexcept
    {
        float _2 = {}, _3 = {};
        float maxDelta = {};
        int32_t _4 = {};

        _2 = soul__dBtoGain (targetDB);
        _state.m_targetGain = _2;
        maxDelta = static_cast<float> (static_cast<float> ((1.0 / (sampleRate * 1.0)) / 0.009999999776482582));
        _3 = soul__intrinsics___abs_specialised_1_f32 (_state.m_targetGain - _state.m_currentGain);
        _4 = soul__intrinsics___max_specialised_2_i32_i32 (1, static_cast<int32_t> (_3 / static_cast<float> (maxDelta)));
        _state.m_remainingRampSamples = _4;
        _state.m_increment = (_state.m_targetGain - _state.m_currentGain) / static_cast<float> (_state.m_remainingRampSamples);
    }

    void soul__gain__SmoothedGainParameter___for__root__SineSynth_smoothedGain__run (soul__gain__SmoothedGainParameter___for__root__SineSynth_smoothedGain___State& _state, soul__gain__SmoothedGainParameter___for__root__SineSynth_smoothedGain___IO& _io) noexcept
    {
        float out_value_gain = {};
        int32_t _2 = {}, _3 = {};

        out_value_gain = 0;
        if (! (_state.m_remainingRampSamples != 0)) goto _ifnot_0;
        _if_0: { _2 = _state.m_remainingRampSamples;
                 _3 = _2 - 1;
                 _state.m_remainingRampSamples = _3;
                 if (! (_3 == 0)) goto _ifnot_1;
        }
        _if_1: { _state.m_currentGain = _state.m_targetGain;
                 goto _ifnot_0;
        }
        _ifnot_1: { _state.m_currentGain = _state.m_currentGain + _state.m_increment; }
        _ifnot_0: { out_value_gain = out_value_gain + _state.m_currentGain;
                    _state.m__resumePoint = 1;
                    _io.m__out_gain = out_value_gain;
        }
    }

    void soul__gain__SmoothedGainParameter___for__root__SineSynth_smoothedGain___initialise (soul__gain__SmoothedGainParameter___for__root__SineSynth_smoothedGain___State& _state) noexcept
    {
        _state.m_targetGain = 0;
        _state.m_currentGain = 0;
        _state.m_increment = 0;
        _state.m_remainingRampSamples = 0;
    }

    //==============================================================================
    void soul__gain__DynamicGain___for__root__SineSynth_gainProcessor__run (soul__gain__DynamicGain___for__root__SineSynth_gainProcessor___State& _state, soul__gain__DynamicGain___for__root__SineSynth_gainProcessor___IO& _io) noexcept
    {
        Vector<float, 2> out_value_out = {}, _2 = {};
        float _3 = {};

        out_value_out = ZeroInitialiser();
        _2 = _io.m__in_in;
        _3 = _io.m__in_gain;
        out_value_out = out_value_out + (_2 * Vector<float, 2> (_3));
        _state.m__resumePoint = 1;
        _io.m__out_out = out_value_out;
    }

    //==============================================================================
    void Voice___initialise (Voice___State& _state) noexcept
    {
        _state.m_voiceController_state.m__arrayEntry = 0;
        _state.m_voiceController_state.m__sessionID = _state.m__sessionID;
        _state.m_voiceController_state.m__processorId = 1;
        Controller___initialise (_state.m_voiceController_state);
        _state.m_amplitudeEnvelope_state.m__arrayEntry = 0;
        _state.m_amplitudeEnvelope_state.m__sessionID = _state.m__sessionID;
        _state.m_amplitudeEnvelope_state.m__processorId = 2;
        ADSREnvelope___initialise (_state.m_amplitudeEnvelope_state);
        _state.m_filter_state.m__arrayEntry = 0;
        _state.m_filter_state.m__sessionID = _state.m__sessionID;
        _state.m_filter_state.m__processorId = 3;
        Filter___initialise (_state.m_filter_state);
        _state.m_filterEnv_state.m__arrayEntry = 0;
        _state.m_filterEnv_state.m__sessionID = _state.m__sessionID;
        _state.m_filterEnv_state.m__processorId = 4;
        Envelope___initialise (_state.m_filterEnv_state);
        _state.m_detuner_state.m__arrayEntry = 0;
        _state.m_detuner_state.m__sessionID = _state.m__sessionID;
        _state.m_detuner_state.m__processorId = 5;
        Detuner___initialise (_state.m_detuner_state);
        _state.m_osc1_state.m__arrayEntry = 0;
        _state.m_osc1_state.m__sessionID = _state.m__sessionID;
        _state.m_osc1_state.m__processorId = 6;
        PolyBlep___initialise (_state.m_osc1_state);
        _state.m_osc2_state.m__arrayEntry = 0;
        _state.m_osc2_state.m__sessionID = _state.m__sessionID;
        _state.m_osc2_state.m__processorId = 7;
        PolyBlep_2___initialise (_state.m_osc2_state);
        _state.m_osc3_state.m__arrayEntry = 0;
        _state.m_osc3_state.m__sessionID = _state.m__sessionID;
        _state.m_osc3_state.m__processorId = 8;
        PolyBlep_3___initialise (_state.m_osc3_state);
        _state.m_osc4_state.m__arrayEntry = 0;
        _state.m_osc4_state.m__sessionID = _state.m__sessionID;
        _state.m_osc4_state.m__processorId = 9;
        PolyBlep_4___initialise (_state.m_osc4_state);
        _state.m_osc5_state.m__arrayEntry = 0;
        _state.m_osc5_state.m__sessionID = _state.m__sessionID;
        _state.m_osc5_state.m__processorId = 10;
        PolyBlep_5___initialise (_state.m_osc5_state);
        _state.m_osc6_state.m__arrayEntry = 0;
        _state.m_osc6_state.m__sessionID = _state.m__sessionID;
        _state.m_osc6_state.m__processorId = 11;
        PolyBlep_6___initialise (_state.m_osc6_state);
        _state.m_osc7_state.m__arrayEntry = 0;
        _state.m_osc7_state.m__sessionID = _state.m__sessionID;
        _state.m_osc7_state.m__processorId = 12;
        PolyBlep_7___initialise (_state.m_osc7_state);
        _state.m_osc8_state.m__arrayEntry = 0;
        _state.m_osc8_state.m__sessionID = _state.m__sessionID;
        _state.m_osc8_state.m__processorId = 13;
        PolyBlep_8___initialise (_state.m_osc8_state);
        _state.m_osc9_state.m__arrayEntry = 0;
        _state.m_osc9_state.m__sessionID = _state.m__sessionID;
        _state.m_osc9_state.m__processorId = 14;
        PolyBlep_9___initialise (_state.m_osc9_state);
        _state.m_osc10_state.m__arrayEntry = 0;
        _state.m_osc10_state.m__sessionID = _state.m__sessionID;
        _state.m_osc10_state.m__processorId = 15;
        PolyBlep_10___initialise (_state.m_osc10_state);
        _state.m_osc11_state.m__arrayEntry = 0;
        _state.m_osc11_state.m__sessionID = _state.m__sessionID;
        _state.m_osc11_state.m__processorId = 16;
        PolyBlep_11___initialise (_state.m_osc11_state);
        _state.m_osc12_state.m__arrayEntry = 0;
        _state.m_osc12_state.m__sessionID = _state.m__sessionID;
        _state.m_osc12_state.m__processorId = 17;
        PolyBlep_12___initialise (_state.m_osc12_state);
        _state.m_osc13_state.m__arrayEntry = 0;
        _state.m_osc13_state.m__sessionID = _state.m__sessionID;
        _state.m_osc13_state.m__processorId = 18;
        PolyBlep_13___initialise (_state.m_osc13_state);
        _state.m_osc14_state.m__arrayEntry = 0;
        _state.m_osc14_state.m__sessionID = _state.m__sessionID;
        _state.m_osc14_state.m__processorId = 19;
        PolyBlep_14___initialise (_state.m_osc14_state);
        _state.m_osc15_state.m__arrayEntry = 0;
        _state.m_osc15_state.m__sessionID = _state.m__sessionID;
        _state.m_osc15_state.m__processorId = 20;
        PolyBlep_15___initialise (_state.m_osc15_state);
        _state.m_osc16_state.m__arrayEntry = 0;
        _state.m_osc16_state.m__sessionID = _state.m__sessionID;
        _state.m_osc16_state.m__processorId = 21;
        PolyBlep_16___initialise (_state.m_osc16_state);
    }

    void Voice__run (Voice___State& _state, Voice___IO& _io) noexcept
    {
        Detuner___IO _2 = {};
        Controller___IO _3 = {};
        ADSREnvelope___IO _4 = {};
        PolyBlep___IO _5 = {};
        PolyBlep_2___IO _6 = {};
        PolyBlep_3___IO _7 = {};
        PolyBlep_4___IO _8 = {};
        PolyBlep_5___IO _9 = {};
        PolyBlep_6___IO _10 = {};
        PolyBlep_7___IO _11 = {};
        PolyBlep_8___IO _12 = {};
        PolyBlep_9___IO _13 = {};
        PolyBlep_10___IO _14 = {};
        PolyBlep_11___IO _15 = {};
        PolyBlep_12___IO _16 = {};
        PolyBlep_13___IO _17 = {};
        PolyBlep_14___IO _18 = {};
        PolyBlep_15___IO _19 = {};
        PolyBlep_16___IO _20 = {};
        Filter___IO _21 = {};
        Envelope___IO _22 = {};

        _2 = ZeroInitialiser();
        Detuner__run (_state.m_detuner_state, _2);
        _3 = ZeroInitialiser();
        Controller__run (_state.m_voiceController_state, _3);
        _4 = ZeroInitialiser();
        _4.m__in_triggerLevel = _3.m__out_note1_active;
        ADSREnvelope__run (_state.m_amplitudeEnvelope_state, _4);
        _5 = ZeroInitialiser();
        _5.m__in_activeIn = _2.m__out_activeOut1;
        _5.m__in_frequencyIn = _3.m__out_note1_frequencyOut;
        _5.m__in_gainIn = _4.m__out_envelopeLevel;
        _5.m__in_detuneIn = _2.m__out_detuneOut1;
        PolyBlep__run (_state.m_osc1_state, _5);
        _6 = ZeroInitialiser();
        _6.m__in_activeIn = _2.m__out_activeOut2;
        _6.m__in_frequencyIn = _3.m__out_note1_frequencyOut;
        _6.m__in_gainIn = _4.m__out_envelopeLevel;
        _6.m__in_detuneIn = _2.m__out_detuneOut2;
        PolyBlep_2__run (_state.m_osc2_state, _6);
        _7 = ZeroInitialiser();
        _7.m__in_activeIn = _2.m__out_activeOut3;
        _7.m__in_frequencyIn = _3.m__out_note1_frequencyOut;
        _7.m__in_gainIn = _4.m__out_envelopeLevel;
        _7.m__in_detuneIn = _2.m__out_detuneOut3;
        PolyBlep_3__run (_state.m_osc3_state, _7);
        _8 = ZeroInitialiser();
        _8.m__in_activeIn = _2.m__out_activeOut4;
        _8.m__in_frequencyIn = _3.m__out_note1_frequencyOut;
        _8.m__in_gainIn = _4.m__out_envelopeLevel;
        _8.m__in_detuneIn = _2.m__out_detuneOut4;
        PolyBlep_4__run (_state.m_osc4_state, _8);
        _9 = ZeroInitialiser();
        _9.m__in_activeIn = _2.m__out_activeOut5;
        _9.m__in_frequencyIn = _3.m__out_note1_frequencyOut;
        _9.m__in_gainIn = _4.m__out_envelopeLevel;
        _9.m__in_detuneIn = _2.m__out_detuneOut5;
        PolyBlep_5__run (_state.m_osc5_state, _9);
        _10 = ZeroInitialiser();
        _10.m__in_activeIn = _2.m__out_activeOut6;
        _10.m__in_frequencyIn = _3.m__out_note1_frequencyOut;
        _10.m__in_gainIn = _4.m__out_envelopeLevel;
        _10.m__in_detuneIn = _2.m__out_detuneOut6;
        PolyBlep_6__run (_state.m_osc6_state, _10);
        _11 = ZeroInitialiser();
        _11.m__in_activeIn = _2.m__out_activeOut7;
        _11.m__in_frequencyIn = _3.m__out_note1_frequencyOut;
        _11.m__in_gainIn = _4.m__out_envelopeLevel;
        _11.m__in_detuneIn = _2.m__out_detuneOut7;
        PolyBlep_7__run (_state.m_osc7_state, _11);
        _12 = ZeroInitialiser();
        _12.m__in_activeIn = _2.m__out_activeOut8;
        _12.m__in_frequencyIn = _3.m__out_note1_frequencyOut;
        _12.m__in_gainIn = _4.m__out_envelopeLevel;
        _12.m__in_detuneIn = _2.m__out_detuneOut8;
        PolyBlep_8__run (_state.m_osc8_state, _12);
        _13 = ZeroInitialiser();
        _13.m__in_activeIn = _2.m__out_activeOut9;
        _13.m__in_frequencyIn = _3.m__out_note1_frequencyOut;
        _13.m__in_gainIn = _4.m__out_envelopeLevel;
        _13.m__in_detuneIn = _2.m__out_detuneOut9;
        PolyBlep_9__run (_state.m_osc9_state, _13);
        _14 = ZeroInitialiser();
        _14.m__in_activeIn = _2.m__out_activeOut10;
        _14.m__in_frequencyIn = _3.m__out_note1_frequencyOut;
        _14.m__in_gainIn = _4.m__out_envelopeLevel;
        _14.m__in_detuneIn = _2.m__out_detuneOut10;
        PolyBlep_10__run (_state.m_osc10_state, _14);
        _15 = ZeroInitialiser();
        _15.m__in_activeIn = _2.m__out_activeOut11;
        _15.m__in_frequencyIn = _3.m__out_note1_frequencyOut;
        _15.m__in_gainIn = _4.m__out_envelopeLevel;
        _15.m__in_detuneIn = _2.m__out_detuneOut11;
        PolyBlep_11__run (_state.m_osc11_state, _15);
        _16 = ZeroInitialiser();
        _16.m__in_activeIn = _2.m__out_activeOut12;
        _16.m__in_frequencyIn = _3.m__out_note1_frequencyOut;
        _16.m__in_gainIn = _4.m__out_envelopeLevel;
        _16.m__in_detuneIn = _2.m__out_detuneOut12;
        PolyBlep_12__run (_state.m_osc12_state, _16);
        _17 = ZeroInitialiser();
        _17.m__in_activeIn = _2.m__out_activeOut13;
        _17.m__in_frequencyIn = _3.m__out_note1_frequencyOut;
        _17.m__in_gainIn = _4.m__out_envelopeLevel;
        _17.m__in_detuneIn = _2.m__out_detuneOut13;
        PolyBlep_13__run (_state.m_osc13_state, _17);
        _18 = ZeroInitialiser();
        _18.m__in_activeIn = _2.m__out_activeOut14;
        _18.m__in_frequencyIn = _3.m__out_note1_frequencyOut;
        _18.m__in_gainIn = _4.m__out_envelopeLevel;
        _18.m__in_detuneIn = _2.m__out_detuneOut14;
        PolyBlep_14__run (_state.m_osc14_state, _18);
        _19 = ZeroInitialiser();
        _19.m__in_activeIn = _2.m__out_activeOut15;
        _19.m__in_frequencyIn = _3.m__out_note1_frequencyOut;
        _19.m__in_gainIn = _4.m__out_envelopeLevel;
        _19.m__in_detuneIn = _2.m__out_detuneOut15;
        PolyBlep_15__run (_state.m_osc15_state, _19);
        _20 = ZeroInitialiser();
        _20.m__in_activeIn = _2.m__out_activeOut16;
        _20.m__in_frequencyIn = _3.m__out_note1_frequencyOut;
        _20.m__in_gainIn = _4.m__out_envelopeLevel;
        _20.m__in_detuneIn = _2.m__out_detuneOut16;
        PolyBlep_16__run (_state.m_osc16_state, _20);
        _21 = ZeroInitialiser();
        _21.m__in_gainIn = 0;
        _21.m__in_resonanceIn = 0;
        _21.m__in_audioIn = ((((((((((((((_5.m__out_audioOut + _6.m__out_audioOut) + _7.m__out_audioOut) + _8.m__out_audioOut) + _9.m__out_audioOut) + _10.m__out_audioOut) + _11.m__out_audioOut) + _12.m__out_audioOut) + _13.m__out_audioOut) + _14.m__out_audioOut) + _15.m__out_audioOut) + _16.m__out_audioOut)
        + _17.m__out_audioOut) + _18.m__out_audioOut) + _19.m__out_audioOut) + _20.m__out_audioOut;
        Filter__run (_state.m_filter_state, _21);
        _io.m__out_audioOut = _21.m__out_audioOut;
        _22 = ZeroInitialiser();
        _22.m__in_triggerLevel = _3.m__out_note1_active;
        Envelope__run (_state.m_filterEnv_state, _22);
    }

    void Voice___eventIn_struct_NoteOn (Voice___State& _state, soul__note_events__NoteOn event) noexcept
    {
        Controller___eventIn_struct_NoteOn (_state.m_voiceController_state, event);
    }

    void Voice___eventIn_struct_NoteOff (Voice___State& _state, soul__note_events__NoteOff event) noexcept
    {
        Controller___eventIn_struct_NoteOff (_state.m_voiceController_state, event);
    }

    void Voice___eventIn_struct_PitchBend (Voice___State& _state, soul__note_events__PitchBend event) noexcept
    {
        Controller___eventIn_struct_PitchBend (_state.m_voiceController_state, event);
    }

    void Voice___eventIn_struct_Pressure (Voice___State& _state, soul__note_events__Pressure event) noexcept
    {
        Controller___eventIn_struct_Pressure (_state.m_voiceController_state, event);
    }

    void Voice___eventIn_struct_Slide (Voice___State& _state, soul__note_events__Slide event) noexcept
    {
        Controller___eventIn_struct_Slide (_state.m_voiceController_state, event);
    }

    void Voice___ampAttack_f32 (Voice___State& _state, float event) noexcept
    {
        ADSREnvelope___attack_f32 (_state.m_amplitudeEnvelope_state, event);
    }

    void Voice___ampDecay_f32 (Voice___State& _state, float event) noexcept
    {
        ADSREnvelope___decay_f32 (_state.m_amplitudeEnvelope_state, event);
    }

    void Voice___ampSustain_f32 (Voice___State& _state, float event) noexcept
    {
        ADSREnvelope___sustain_f32 (_state.m_amplitudeEnvelope_state, event);
    }

    void Voice___ampRelease_f32 (Voice___State& _state, float event) noexcept
    {
        ADSREnvelope___release_f32 (_state.m_amplitudeEnvelope_state, event);
    }

    void Voice___detune_f32 (Voice___State& _state, float event) noexcept
    {
        Detuner___detuneIn_f32 (_state.m_detuner_state, event);
    }

    void Voice___voiceCountIn_f32 (Voice___State& _state, float event) noexcept
    {
        Detuner___voiceCountIn_f32 (_state.m_detuner_state, event);
    }

    void Voice___cutoffIn_f32 (Voice___State& _state, float event) noexcept
    {
        Filter___cutoffIn_f32 (_state.m_filter_state, event);
    }

    //==============================================================================
    float PolyBlep__blep (PolyBlep___State& _state, float t2) noexcept
    {
        float t = {};

        t = t2;
        if (! (t < _state.m_normalisedPhaseIncrement)) goto _ifnot_0;
        _if_0: { t = t / _state.m_normalisedPhaseIncrement;
                 return ((t + t) - (t * t)) - 1.0f;
        }
        _ifnot_0: { if (! (t > (1.0f - _state.m_normalisedPhaseIncrement))) goto _ifnot_1; }
        _if_1: { t = (t - 1.0f) / _state.m_normalisedPhaseIncrement;
                 return ((t * t) + (t + t)) + 1.0f;
        }
        _ifnot_1: { return 0; }
    }

    void PolyBlep__run (PolyBlep___State& _state, PolyBlep___IO& _io) noexcept
    {
        Vector<float, 2> out_value_audioOut = {}, _2 = {};
        float _3 = {}, _4 = {}, _5 = {}, _6 = {}, normalisedPhase2 = {}, _7 = {}, _8 = {}, _9 = {}, squareValue = {}, _10 = {}, _11 = {}, _12 = {}, left = {}, right = {}, _13 = {}, _T4 = {};
        float sawValue = {}, amp = {};

        out_value_audioOut = ZeroInitialiser();
        _3 = _io.m__in_activeIn;
        if (! (_3 == 1.0f)) goto _ifnot_2;
        _if_2: { _4 = _io.m__in_detuneIn;
                 _state.m_detuneValue = _4 * 0.01f;
                 _5 = std::pow (2.0f, (_state.m_detuneSemitonesValue + _state.m_detuneValue) / 12.0f);
                 _state.m_frequencyMultiplier = _5;
                 _6 = _io.m__in_frequencyIn;
                 _state.m_normalisedPhaseIncrement = (_6 * (1.0f / static_cast<float> ((sampleRate * 1.0)))) * _state.m_frequencyMultiplier;
        }
        _loop_1: { if (! (_state.m_normalisedPhase >= 1.0f)) goto _break_1; }
        _body_1: { _state.m_normalisedPhase = _state.m_normalisedPhase - 1.0f;
                   goto _loop_1;
        }
        _break_1: { normalisedPhase2 = _state.m_normalisedPhase + 0.5f;
                    if (! (normalisedPhase2 > 1.0f)) goto _ifnot_3;
        }
        _if_3: { normalisedPhase2 = normalisedPhase2 - 1.0f; }
        _ifnot_3: { _7 = PolyBlep__blep (_state, _state.m_normalisedPhase);
                    sawValue = static_cast<float> (((2.0f * _state.m_normalisedPhase) - 1.0f) - _7);
                    _8 = PolyBlep__blep (_state, _state.m_normalisedPhase);
                    _9 = PolyBlep__blep (_state, normalisedPhase2);
                    squareValue = (-_8) + _9;
                    if (! (_state.m_normalisedPhase <= 0.5f)) goto _ternary_false_4;
        }
        _ternary_true_4: { _10 = -1.0f;
                           _T4 = _10;
                           goto _ternary_end_4;
        }
        _ternary_false_4: { _11 = 1.0f;
                            _T4 = _11;
        }
        _ternary_end_4: { _12 = _T4;
                          squareValue = squareValue + _12;
                          left = static_cast<float> (0.5 + static_cast<double> (_state.m_detuneValue));
                          right = static_cast<float> (0.5 - static_cast<double> (_state.m_detuneValue));
                          _13 = _io.m__in_gainIn;
                          amp = static_cast<float> (_13 * ((squareValue * _state.m_waveshapeValue) + static_cast<float> (sawValue * static_cast<float> (1.0f - _state.m_waveshapeValue))));
                          _2 = ZeroInitialiser();
                          _2[0] = static_cast<float> (amp * static_cast<float> (left));
                          _2[1] = static_cast<float> (amp * static_cast<float> (right));
                          out_value_audioOut = out_value_audioOut + _2;
                          _state.m_normalisedPhase = _state.m_normalisedPhase + _state.m_normalisedPhaseIncrement;
        }
        _ifnot_2: { out_value_audioOut = out_value_audioOut + ZeroInitialiser();
                    _state.m__resumePoint = 1;
                    _io.m__out_audioOut = out_value_audioOut;
        }
    }

    void PolyBlep___initialise (PolyBlep___State& _state) noexcept
    {
        _state.m_normalisedPhase = 0;
        _state.m_normalisedPhaseIncrement = 0;
        _state.m_waveshapeValue = 0;
        _state.m_detuneSemitonesValue = 0;
        _state.m_detuneValue = 0;
        _state.m_frequencyMultiplier = 1.0f;
    }

    //==============================================================================
    void Controller___eventIn_struct_NoteOn (Controller___State& _state, soul__note_events__NoteOn e) noexcept
    {
        NoteHandler__addNote (_state.m_noteHandler, e.m_channel, e.m_note, e.m_velocity);
    }

    void Controller___eventIn_struct_NoteOff (Controller___State& _state, soul__note_events__NoteOff e) noexcept
    {
        NoteHandler__removeNote (_state.m_noteHandler, e.m_channel, e.m_note);
    }

    void Controller___eventIn_struct_PitchBend (Controller___State& _state, soul__note_events__PitchBend e) noexcept
    {
        NoteHandler__addPitchBend (_state.m_noteHandler, e.m_channel, e.m_bendSemitones);
    }

    void Controller___eventIn_struct_Pressure (Controller___State& _state, soul__note_events__Pressure p) noexcept
    {
        NoteHandler__addPressure (_state.m_noteHandler, p.m_channel, p.m_pressure);
    }

    void Controller___eventIn_struct_Slide (Controller___State& _state, soul__note_events__Slide s) noexcept
    {
        NoteHandler__addSlide (_state.m_noteHandler, s.m_channel, s.m_slide);
    }

    void Controller__run (Controller___State& _state, Controller___IO& _io) noexcept
    {
        float out_value_note1_noteOut = {}, out_value_note1_frequencyOut = {}, out_value_note1_velocity = {}, out_value_note1_active = {}, out_value_note2_frequencyOut = {}, out_value_note1_pressure = {}, out_value_note1_slide = {}, _2 = {}, _3 = {}, _4 = {}, _5 = {}, _6 = {}, _7 = {}, _8 = {}, _9 = {}, _10
        = {}, _T2 = {};
        int32_t _resumePoint = {}, _11 = {};
        NoteHandler__NoteHandler _12 = {};
        float noteFrequency = {};
        bool _13 = {};

        out_value_note1_noteOut = 0;
        out_value_note1_frequencyOut = 0;
        out_value_note1_velocity = 0;
        out_value_note1_active = 0;
        out_value_note2_frequencyOut = 0;
        out_value_note1_pressure = 0;
        out_value_note1_slide = 0;
        _resumePoint = _state.m__resumePoint;
        if (_resumePoint == 1) goto _body_0;
        _block_0: { _state.m_multiplier = 1.0f;
                    _state.m_sampleCount = 0;
                    _12 = NoteHandler__create();
                    _state.m_noteHandler = _12;
        }
        _body_0: { _11 = _state.m_sampleCount;
                   _state.m_sampleCount = _11 + 1;
                   if (! (_state.m_sampleCount == 1000)) goto _ifnot_0;
        }
        _if_0: { _state.m_sampleCount = 0;
                 _state.m_multiplier = _state.m_multiplier + 0.001f;
                 if (! (_state.m_multiplier >= 1.01f)) goto _ifnot_0;
        }
        _if_1: { _state.m_multiplier = 1.0f; }
        _ifnot_0: { _2 = NoteHandler__getCurrentNote (_state.m_noteHandler);
                    _3 = soul__noteNumberToFrequency_2 (_2);
                    noteFrequency = static_cast<float> (_3);
                    _4 = NoteHandler__getCurrentNote (_state.m_noteHandler);
                    out_value_note1_noteOut = out_value_note1_noteOut + _4;
                    out_value_note1_frequencyOut = out_value_note1_frequencyOut + static_cast<float> (noteFrequency);
                    out_value_note2_frequencyOut = out_value_note2_frequencyOut + static_cast<float> (noteFrequency * static_cast<float> (_state.m_multiplier));
                    _5 = NoteHandler__getCurrentVelocity (_state.m_noteHandler);
                    out_value_note1_velocity = out_value_note1_velocity + _5;
                    _13 = NoteHandler__isNoteActive (_state.m_noteHandler);
                    if (! _13) goto _ternary_false_2;
        }
        _ternary_true_2: { _6 = NoteHandler__getCurrentVelocity (_state.m_noteHandler);
                           _T2 = _6;
                           goto _ternary_end_2;
        }
        _ternary_false_2: { _7 = 0;
                            _T2 = _7;
        }
        _ternary_end_2: { _8 = _T2;
                          out_value_note1_active = out_value_note1_active + _8;
                          _9 = NoteHandler__getCurrentPressure (_state.m_noteHandler);
                          out_value_note1_pressure = out_value_note1_pressure + _9;
                          _10 = NoteHandler__getCurrentSlide (_state.m_noteHandler);
                          out_value_note1_slide = out_value_note1_slide + _10;
                          _state.m__resumePoint = 1;
                          _io.m__out_note1_noteOut = out_value_note1_noteOut;
                          _io.m__out_note1_frequencyOut = out_value_note1_frequencyOut;
                          _io.m__out_note1_velocity = out_value_note1_velocity;
                          _io.m__out_note1_active = out_value_note1_active;
                          _io.m__out_note2_frequencyOut = out_value_note2_frequencyOut;
                          _io.m__out_note1_pressure = out_value_note1_pressure;
                          _io.m__out_note1_slide = out_value_note1_slide;
        }
    }

    void Controller___initialise (Controller___State& _state) noexcept
    {
        _state.m_noteHandler = ZeroInitialiser();
    }

    //==============================================================================
    NoteHandler__NoteHandler NoteHandler__create() noexcept
    {
        NoteHandler__NoteHandler r = {};

        r = ZeroInitialiser();
        r.m_currentVelocity = 0;
        r.m_currentNote = 0;
        r.m_activeNotes = 0;
        r.m_maxNotes = 16;
        return r;
    }

    void NoteHandler__addNote (NoteHandler__NoteHandler& this_, int32_t channel, float note, float velocity) noexcept
    {
        int32_t _2 = {};

        if (! (static_cast<int32_t> (this_.m_activeNotes) == 0)) goto _ifnot_0;
        _if_0: { this_.m_currentVelocity = velocity; }
        _ifnot_0: { if (! (static_cast<int32_t> (this_.m_activeNotes) < this_.m_maxNotes)) goto _ifnot_1; }
        _if_1: { this_.m_noteArray[this_.m_activeNotes].m_channel = channel;
                 this_.m_noteArray[this_.m_activeNotes].m_note = note;
                 this_.m_noteArray[this_.m_activeNotes].m_bendSemitones = 0;
                 _2 = this_.m_activeNotes;
                 this_.m_activeNotes = (_2 + 1) & 15;
        }
        _ifnot_1: { this_.m_currentNote = note;
                    this_.m_currentBendSemitones = 0;
                    this_.m_currentPressure = 0;
                    this_.m_currentSlide = 0;
        }
    }

    void NoteHandler__removeNote (NoteHandler__NoteHandler& this_, int32_t channel, float note) noexcept
    {
        int32_t readPoint = {}, writePoint = {}, _2 = {}, _3 = {};

        readPoint = 0;
        writePoint = 0;
        _loop_0: { if (! (static_cast<int32_t> (readPoint) < static_cast<int32_t> (this_.m_activeNotes))) goto _break_0; }
        _body_0: { if (! (this_.m_noteArray[readPoint].m_note != note)) goto _ifnot_2; }
        _if_2: { this_.m_noteArray[writePoint] = this_.m_noteArray[readPoint];
                 _2 = writePoint;
                 writePoint = (_2 + 1) & 15;
        }
        _ifnot_2: { _3 = readPoint;
                    readPoint = (_3 + 1) & 15;
                    goto _loop_0;
        }
        _break_0: { this_.m_activeNotes = writePoint;
                    if (! (static_cast<int32_t> (this_.m_activeNotes) > 0)) goto _ifnot_3;
        }
        _if_3: { this_.m_currentNote = this_.m_noteArray[(this_.m_activeNotes - 1) & 15].m_note;
                 this_.m_currentBendSemitones = this_.m_noteArray[(this_.m_activeNotes - 1) & 15].m_bendSemitones;
        }
        _ifnot_3: {}
    }

    void NoteHandler__addPitchBend (NoteHandler__NoteHandler& this_, int32_t channel, float bendSemitones) noexcept
    {
        int32_t i = {}, _2 = {};

        i = 0;
        _loop_1: { if (! (i < static_cast<int32_t> (this_.m_activeNotes))) goto _break_1; }
        _body_1: { if (! (this_.m_noteArray[_intrin_wrap (static_cast<int32_t> (i), 16) & 15].m_channel == channel)) goto _cont_1; }
        _if_4: { this_.m_noteArray[_intrin_wrap (static_cast<int32_t> (i), 16) & 15].m_bendSemitones = bendSemitones; }
        _cont_1: { _2 = i;
                   i = _2 + 1;
                   goto _loop_1;
        }
        _break_1: { if (! (static_cast<int32_t> (this_.m_activeNotes) > 0)) goto _ifnot_5; }
        _if_5: { this_.m_currentBendSemitones = this_.m_noteArray[(this_.m_activeNotes - 1) & 15].m_bendSemitones; }
        _ifnot_5: {}
    }

    void NoteHandler__addPressure (NoteHandler__NoteHandler& this_, int32_t channel, float pressure) noexcept
    {
        int32_t i = {}, _2 = {};

        i = 0;
        _loop_2: { if (! (i < static_cast<int32_t> (this_.m_activeNotes))) goto _break_2; }
        _body_2: { if (! (this_.m_noteArray[_intrin_wrap (static_cast<int32_t> (i), 16) & 15].m_channel == channel)) goto _cont_2; }
        _if_6: { this_.m_noteArray[_intrin_wrap (static_cast<int32_t> (i), 16) & 15].m_pressure = pressure; }
        _cont_2: { _2 = i;
                   i = _2 + 1;
                   goto _loop_2;
        }
        _break_2: { if (! (static_cast<int32_t> (this_.m_activeNotes) > 0)) goto _ifnot_7; }
        _if_7: { this_.m_currentPressure = this_.m_noteArray[(this_.m_activeNotes - 1) & 15].m_pressure; }
        _ifnot_7: {}
    }

    void NoteHandler__addSlide (NoteHandler__NoteHandler& this_, int32_t channel, float slide) noexcept
    {
        int32_t i = {}, _2 = {};

        i = 0;
        _loop_3: { if (! (i < static_cast<int32_t> (this_.m_activeNotes))) goto _break_3; }
        _body_3: { if (! (this_.m_noteArray[_intrin_wrap (static_cast<int32_t> (i), 16) & 15].m_channel == channel)) goto _cont_3; }
        _if_8: { this_.m_noteArray[_intrin_wrap (static_cast<int32_t> (i), 16) & 15].m_slide = slide; }
        _cont_3: { _2 = i;
                   i = _2 + 1;
                   goto _loop_3;
        }
        _break_3: { if (! (static_cast<int32_t> (this_.m_activeNotes) > 0)) goto _ifnot_9; }
        _if_9: { this_.m_currentSlide = this_.m_noteArray[(this_.m_activeNotes - 1) & 15].m_slide; }
        _ifnot_9: {}
    }

    float NoteHandler__getCurrentNote (NoteHandler__NoteHandler& this_) noexcept
    {
        return this_.m_currentNote + this_.m_currentBendSemitones;
    }

    float NoteHandler__getCurrentVelocity (NoteHandler__NoteHandler& this_) noexcept
    {
        return this_.m_currentVelocity;
    }

    float NoteHandler__getCurrentPressure (NoteHandler__NoteHandler& this_) noexcept
    {
        return this_.m_currentPressure;
    }

    float NoteHandler__getCurrentSlide (NoteHandler__NoteHandler& this_) noexcept
    {
        return this_.m_currentSlide;
    }

    bool NoteHandler__isNoteActive (NoteHandler__NoteHandler& this_) noexcept
    {
        return static_cast<int32_t> (this_.m_activeNotes) != 0;
    }

    //==============================================================================
    void ADSREnvelope___attack_f32 (ADSREnvelope___State& _state, float f) noexcept
    {
        float _2 = {};
        double seconds = {}, secondsToLinearStep = {};

        _2 = std::pow (f * 0.01f, 4.0f);
        seconds = static_cast<double> (0.001 + (9.999 * static_cast<double> (_2)));
        secondsToLinearStep = static_cast<double> (1.0 / static_cast<double> (static_cast<double> (static_cast<float> (static_cast<float> ((sampleRate * 1.0)))) * seconds));
        _state.m_attackRamp = static_cast<double> (secondsToLinearStep);
    }

    void ADSREnvelope___decay_f32 (ADSREnvelope___State& _state, float f) noexcept
    {
        float _2 = {};
        double seconds = {}, inverse = {}, secondsToMultiplier = {};
        double _3 = {};

        _2 = std::pow (f * 0.01f, 4.0f);
        seconds = static_cast<double> (0.001 + (9.999 * static_cast<double> (_2)));
        inverse = static_cast<double> (1.0 / static_cast<double> (static_cast<double> (static_cast<float> (static_cast<float> ((sampleRate * 1.0)))) * seconds));
        _3 = std::pow (0.0001, static_cast<double> (inverse));
        secondsToMultiplier = static_cast<double> (_3);
        _state.m_decayMultiplier = static_cast<double> (secondsToMultiplier);
    }

    void ADSREnvelope___sustain_f32 (ADSREnvelope___State& _state, float f) noexcept
    {
        _state.m_sustainLevel = static_cast<double> (f) * 0.009999999776482582;
    }

    void ADSREnvelope___release_f32 (ADSREnvelope___State& _state, float f) noexcept
    {
        float _2 = {};
        double seconds = {}, inverse = {}, secondsToMultiplier = {};
        double _3 = {};

        _2 = std::pow (f * 0.01f, 4.0f);
        seconds = static_cast<double> (0.001 + (9.999 * static_cast<double> (_2)));
        inverse = static_cast<double> (1.0 / static_cast<double> (static_cast<double> (static_cast<float> (static_cast<float> ((sampleRate * 1.0)))) * seconds));
        _3 = std::pow (0.0001, static_cast<double> (inverse));
        secondsToMultiplier = static_cast<double> (_3);
        _state.m_releaseMultiplier = static_cast<double> (secondsToMultiplier);
    }

    void ADSREnvelope__calculateTargetValue (ADSREnvelope___State& _state, float noteVelocity) noexcept
    {
        float attenuation = {};
        float _2 = {};

        attenuation = static_cast<float> ((-12.0f * _state.m_velocitySensitivity) * (0.75f - noteVelocity));
        _2 = std::pow (10.0f, static_cast<float> (attenuation / 10.0f));
        _state.m_targetValue = static_cast<double> (_2);
    }

    void ADSREnvelope__run (ADSREnvelope___State& _state, ADSREnvelope___IO& _io) noexcept
    {
        float out_value_envelopeLevel = {}, _2 = {}, _3 = {}, _4 = {}, _5 = {}, _6 = {};
        int32_t _resumePoint = {};
        double _7 = {};
        bool _8 = {}, _9 = {}, _10 = {}, _T2 = {};

        out_value_envelopeLevel = 0;
        _resumePoint = _state.m__resumePoint;
        if (_resumePoint == 1) goto _resume_point_1;
        _check_resume_point_2: { if (_resumePoint == 2) goto _loop_2; }
        _check_resume_point_3: { if (_resumePoint == 3) goto _loop_3; }
        _check_resume_point_4: { if (_resumePoint == 4) goto _body_0; }
        _block_0: { _state.m_value = 0; }
        _body_0: { _2 = _io.m__in_triggerLevel;
                   if (! (_2 > 0)) goto _ifnot_0;
        }
        _if_0: { _3 = _io.m__in_triggerLevel;
                 ADSREnvelope__calculateTargetValue (_state, _3);
        }
        _loop_1: { if (! (_state.m_value < _state.m_targetValue)) goto _break_1; }
        _body_1: { _state.m_value = _state.m_value + _state.m_attackRamp;
                   out_value_envelopeLevel = out_value_envelopeLevel + static_cast<float> (_state.m_value);
                   _state.m__resumePoint = 1;
                   goto _exit;
        }
        _resume_point_1: { _4 = _io.m__in_triggerLevel;
                           if (! (_4 <= 0)) goto _loop_1;
        }
        _break_1: { _7 = soul__intrinsics___min_specialised_2_f64_f64 (_state.m_value, _state.m_targetValue);
                    _state.m_value = _7;
        }
        _loop_2: { if (! (_state.m_value > (_state.m_targetValue * _state.m_sustainLevel))) goto _ternary_false_2; }
        _ternary_true_2: { _5 = _io.m__in_triggerLevel;
                           _8 = _5 > 0;
                           _T2 = _8;
                           goto _ternary_end_2;
        }
        _ternary_false_2: { _9 = false;
                            _T2 = _9;
        }
        _ternary_end_2: { _10 = _T2;
                          if (! _10) goto _loop_3;
        }
        _body_2: { _state.m_value = _state.m_value * _state.m_decayMultiplier;
                   out_value_envelopeLevel = out_value_envelopeLevel + static_cast<float> (_state.m_value);
                   _state.m__resumePoint = 2;
                   goto _exit;
        }
        _loop_3: { _6 = _io.m__in_triggerLevel;
                   if (! (_6 > 0)) goto _ifnot_0;
        }
        _body_3: { out_value_envelopeLevel = out_value_envelopeLevel + static_cast<float> (_state.m_value);
                   _state.m__resumePoint = 3;
                   goto _exit;
        }
        _ifnot_0: { _state.m_value = _state.m_value * _state.m_releaseMultiplier;
                    out_value_envelopeLevel = out_value_envelopeLevel + static_cast<float> (_state.m_value);
                    _state.m__resumePoint = 4;
        }
        _exit: { _io.m__out_envelopeLevel = out_value_envelopeLevel; }
    }

    void ADSREnvelope___initialise (ADSREnvelope___State& _state) noexcept
    {
        _state.m_attackRamp = 0.01;
        _state.m_decayMultiplier = 0.995;
        _state.m_sustainLevel = 1.0;
        _state.m_releaseMultiplier = 0.99991;
        _state.m_velocitySensitivity = 0;
        _state.m_targetValue = 1.0;
    }

    //==============================================================================
    void Detuner___voiceCountIn_f32 (Detuner___State& _state, float count) noexcept
    {
        _state.m_voiceCount = count;
    }

    void Detuner___detuneIn_f32 (Detuner___State& _state, float f) noexcept
    {
        _state.m_detune = f;
    }

    void Detuner__setDetuneVals (Detuner___State& _state) noexcept
    {
        float amt = {}, increment = {}, increment_2 = {};
        int32_t i = {}, _2 = {}, i_2 = {}, _3 = {};

        amt = -1.0f;
        if (! (std::fmod (_state.m_voiceCount, 2.0f) != 0)) goto _ifnot_0;
        _if_0: { increment = 2.0f / (_state.m_voiceCount + -1.0f);
                 i = 0;
        }
        _loop_0: { if (! (i < 16)) goto _ifend_0; }
        _body_0: { Detuner__setDetuneVal (_state, i, _state.m_detune, amt, increment);
                   _2 = i;
                   i = _2 + 1;
                   goto _loop_0;
        }
        _ifnot_0: { increment_2 = 2.0f / _state.m_voiceCount;
                    i_2 = 0;
        }
        _loop_1: { if (! (i_2 < 16)) goto _ifend_0; }
        _body_1: { if (! (amt == 0)) goto _ifnot_1; }
        _if_1: { amt = amt + increment_2; }
        _ifnot_1: { Detuner__setDetuneVal (_state, i_2, _state.m_detune, amt, increment_2);
                    _3 = i_2;
                    i_2 = _3 + 1;
                    goto _loop_1;
        }
        _ifend_0: {}
    }

    void Detuner__setDetuneVal (Detuner___State& _state, int32_t i, float detune, float& amt, float increment) noexcept
    {
        if (! (static_cast<float> (i) < _state.m_voiceCount)) goto _ifnot_2;
        _if_2: { _state.m_detuneVals[_intrin_wrap (static_cast<int32_t> (i), 16) & 15] = static_cast<float> (amt) * detune;
                 amt = static_cast<float> (amt) + increment;
                 return;
        }
        _ifnot_2: { _state.m_detuneVals[_intrin_wrap (static_cast<int32_t> (i), 16) & 15] = 0; }
    }

    void Detuner__run (Detuner___State& _state, Detuner___IO& _io) noexcept
    {
        float out_value_logOut = {}, out_value_activeOut1 = {}, out_value_activeOut2 = {}, out_value_activeOut3 = {}, out_value_activeOut4 = {}, out_value_activeOut5 = {}, out_value_activeOut6 = {}, out_value_activeOut7 = {}, out_value_activeOut8 = {}, out_value_activeOut9 = {}, out_value_activeOut10 = {},
        out_value_activeOut11 = {}, out_value_activeOut12 = {}, out_value_activeOut13 = {}, out_value_activeOut14 = {}, out_value_activeOut15 = {}, out_value_activeOut16 = {}, out_value_detuneOut1 = {}, out_value_detuneOut2 = {}, out_value_detuneOut3 = {}, out_value_detuneOut4 = {}, out_value_detuneOut5 = {},
         out_value_detuneOut6 = {}, out_value_detuneOut7 = {}, out_value_detuneOut8 = {}, out_value_detuneOut9 = {}, out_value_detuneOut10 = {}, out_value_detuneOut11 = {}, out_value_detuneOut12 = {}, out_value_detuneOut13 = {}, out_value_detuneOut14 = {}, out_value_detuneOut15 = {}, out_value_detuneOut16 =
        {};
        int32_t i = {}, _2 = {}, j = {}, _3 = {};

        out_value_logOut = 0;
        out_value_activeOut1 = 0;
        out_value_activeOut2 = 0;
        out_value_activeOut3 = 0;
        out_value_activeOut4 = 0;
        out_value_activeOut5 = 0;
        out_value_activeOut6 = 0;
        out_value_activeOut7 = 0;
        out_value_activeOut8 = 0;
        out_value_activeOut9 = 0;
        out_value_activeOut10 = 0;
        out_value_activeOut11 = 0;
        out_value_activeOut12 = 0;
        out_value_activeOut13 = 0;
        out_value_activeOut14 = 0;
        out_value_activeOut15 = 0;
        out_value_activeOut16 = 0;
        out_value_detuneOut1 = 0;
        out_value_detuneOut2 = 0;
        out_value_detuneOut3 = 0;
        out_value_detuneOut4 = 0;
        out_value_detuneOut5 = 0;
        out_value_detuneOut6 = 0;
        out_value_detuneOut7 = 0;
        out_value_detuneOut8 = 0;
        out_value_detuneOut9 = 0;
        out_value_detuneOut10 = 0;
        out_value_detuneOut11 = 0;
        out_value_detuneOut12 = 0;
        out_value_detuneOut13 = 0;
        out_value_detuneOut14 = 0;
        out_value_detuneOut15 = 0;
        out_value_detuneOut16 = 0;
        i = 0;
        _loop_3: { if (! (static_cast<float> (i) < _state.m_voiceCount)) goto _break_3; }
        _body_3: { _state.m_active[_intrin_wrap (static_cast<int32_t> (i), 16) & 15] = 1.0f;
                   _2 = i;
                   i = _2 + 1;
                   goto _loop_3;
        }
        _break_3: { j = static_cast<int32_t> (_state.m_voiceCount); }
        _loop_4: { if (! (j < 16)) goto _break_4; }
        _body_4: { _state.m_active[_intrin_wrap (static_cast<int32_t> (j), 16) & 15] = 0;
                   _3 = j;
                   j = _3 + 1;
                   goto _loop_4;
        }
        _break_4: { Detuner__setDetuneVals (_state);
                    out_value_detuneOut1 = out_value_detuneOut1 + _state.m_detuneVals[0];
                    out_value_detuneOut2 = out_value_detuneOut2 + _state.m_detuneVals[1];
                    out_value_detuneOut3 = out_value_detuneOut3 + _state.m_detuneVals[2];
                    out_value_detuneOut4 = out_value_detuneOut4 + _state.m_detuneVals[3];
                    out_value_detuneOut5 = out_value_detuneOut5 + _state.m_detuneVals[4];
                    out_value_detuneOut6 = out_value_detuneOut6 + _state.m_detuneVals[5];
                    out_value_detuneOut7 = out_value_detuneOut7 + _state.m_detuneVals[6];
                    out_value_detuneOut8 = out_value_detuneOut8 + _state.m_detuneVals[7];
                    out_value_detuneOut9 = out_value_detuneOut9 + _state.m_detuneVals[8];
                    out_value_detuneOut10 = out_value_detuneOut10 + _state.m_detuneVals[9];
                    out_value_detuneOut11 = out_value_detuneOut11 + _state.m_detuneVals[10];
                    out_value_detuneOut12 = out_value_detuneOut12 + _state.m_detuneVals[11];
                    out_value_detuneOut13 = out_value_detuneOut13 + _state.m_detuneVals[12];
                    out_value_detuneOut14 = out_value_detuneOut14 + _state.m_detuneVals[13];
                    out_value_detuneOut15 = out_value_detuneOut15 + _state.m_detuneVals[14];
                    out_value_detuneOut16 = out_value_detuneOut16 + _state.m_detuneVals[15];
                    out_value_activeOut1 = out_value_activeOut1 + _state.m_active[0];
                    out_value_activeOut2 = out_value_activeOut2 + _state.m_active[1];
                    out_value_activeOut3 = out_value_activeOut3 + _state.m_active[2];
                    out_value_activeOut4 = out_value_activeOut4 + _state.m_active[3];
                    out_value_activeOut5 = out_value_activeOut5 + _state.m_active[4];
                    out_value_activeOut6 = out_value_activeOut6 + _state.m_active[5];
                    out_value_activeOut7 = out_value_activeOut7 + _state.m_active[6];
                    out_value_activeOut8 = out_value_activeOut8 + _state.m_active[7];
                    out_value_activeOut9 = out_value_activeOut9 + _state.m_active[8];
                    out_value_activeOut10 = out_value_activeOut10 + _state.m_active[9];
                    out_value_activeOut11 = out_value_activeOut11 + _state.m_active[10];
                    out_value_activeOut12 = out_value_activeOut12 + _state.m_active[11];
                    out_value_activeOut13 = out_value_activeOut13 + _state.m_active[12];
                    out_value_activeOut14 = out_value_activeOut14 + _state.m_active[13];
                    out_value_activeOut15 = out_value_activeOut15 + _state.m_active[14];
                    out_value_activeOut16 = out_value_activeOut16 + _state.m_active[15];
                    _state.m__resumePoint = 1;
                    _io.m__out_logOut = out_value_logOut;
                    _io.m__out_activeOut1 = out_value_activeOut1;
                    _io.m__out_activeOut2 = out_value_activeOut2;
                    _io.m__out_activeOut3 = out_value_activeOut3;
                    _io.m__out_activeOut4 = out_value_activeOut4;
                    _io.m__out_activeOut5 = out_value_activeOut5;
                    _io.m__out_activeOut6 = out_value_activeOut6;
                    _io.m__out_activeOut7 = out_value_activeOut7;
                    _io.m__out_activeOut8 = out_value_activeOut8;
                    _io.m__out_activeOut9 = out_value_activeOut9;
                    _io.m__out_activeOut10 = out_value_activeOut10;
                    _io.m__out_activeOut11 = out_value_activeOut11;
                    _io.m__out_activeOut12 = out_value_activeOut12;
                    _io.m__out_activeOut13 = out_value_activeOut13;
                    _io.m__out_activeOut14 = out_value_activeOut14;
                    _io.m__out_activeOut15 = out_value_activeOut15;
                    _io.m__out_activeOut16 = out_value_activeOut16;
                    _io.m__out_detuneOut1 = out_value_detuneOut1;
                    _io.m__out_detuneOut2 = out_value_detuneOut2;
                    _io.m__out_detuneOut3 = out_value_detuneOut3;
                    _io.m__out_detuneOut4 = out_value_detuneOut4;
                    _io.m__out_detuneOut5 = out_value_detuneOut5;
                    _io.m__out_detuneOut6 = out_value_detuneOut6;
                    _io.m__out_detuneOut7 = out_value_detuneOut7;
                    _io.m__out_detuneOut8 = out_value_detuneOut8;
                    _io.m__out_detuneOut9 = out_value_detuneOut9;
                    _io.m__out_detuneOut10 = out_value_detuneOut10;
                    _io.m__out_detuneOut11 = out_value_detuneOut11;
                    _io.m__out_detuneOut12 = out_value_detuneOut12;
                    _io.m__out_detuneOut13 = out_value_detuneOut13;
                    _io.m__out_detuneOut14 = out_value_detuneOut14;
                    _io.m__out_detuneOut15 = out_value_detuneOut15;
                    _io.m__out_detuneOut16 = out_value_detuneOut16;
        }
    }

    void Detuner___initialise (Detuner___State& _state) noexcept
    {
        _state.m_detuneVals = ZeroInitialiser();
        _state.m_active = ZeroInitialiser();
        _state.m_voiceCount = 16.0f;
        _state.m_detune = 0;
    }

    //==============================================================================
    void Filter___cutoffIn_f32 (Filter___State& _state, float f) noexcept
    {
        _state.m_cutoff = f;
    }

    Filter__FilterTerms Filter__calcTermEntry (Filter___State& _state, float frequency, float resonance) noexcept
    {
        Filter__FilterTerms result = {};
        float r = {}, f = {}, c = {};
        float _2 = {}, _3 = {}, _4 = {};

        result = ZeroInitialiser();
        r = static_cast<float> (1.4f - (resonance * 0.0135f));
        _2 = soul__noteNumberToFrequency_2 (frequency);
        _3 = soul__intrinsics___clamp_specialised_3_f32_f32_f32 (_2, 0, 20000.0f);
        f = static_cast<float> (_3);
        _4 = std::tan ((3.1415927f * (10.0f + static_cast<float> (f))) / static_cast<float> ((sampleRate * 1.0)));
        c = static_cast<float> (1.0f / _4);
        result.m_a = 1.0f / ((1.0f + static_cast<float> (r * c)) + static_cast<float> (c * c));
        result.m_b = (2.0f * (1.0f - static_cast<float> (c * c))) * result.m_a;
        result.m_c = ((1.0f - static_cast<float> (r * c)) + static_cast<float> (c * c)) * result.m_a;
        return result;
    }

    void Filter__initialiseTerms (Filter___State& _state) noexcept
    {
        int32_t i = {}, j = {}, _2 = {}, _3 = {};
        Filter__FilterTerms _4 = {};

        i = 0;
        _loop_0: { if (! (i < 128)) goto _break_0; }
        _body_0: { j = 0; }
        _loop_1: { if (! (j <= 100)) goto _cont_0; }
        _body_1: { _4 = Filter__calcTermEntry (_state, static_cast<float> (i), static_cast<float> (j));
                   _state.m_precalcTerms.m_frequency[_intrin_wrap (static_cast<int32_t> (i), 128) & 127].m_resonance[_intrin_wrap (static_cast<int32_t> (_internal___wrapInt32 (static_cast<int32_t> (_intrin_wrap (static_cast<int32_t> (j), 101)), 101)), 101)] = _4;
                   _2 = j;
                   j = _2 + 1;
                   goto _loop_1;
        }
        _cont_0: { _3 = i;
                   i = _3 + 1;
                   goto _loop_0;
        }
        _break_0: {}
    }

    void Filter__updateFactors (Filter___State& _state, float cut, float resonance) noexcept
    {
        float _2 = {}, _3 = {};
        float cutoffInterpolation = {}, terms1_a = {}, terms1_b = {}, terms1_c = {}, terms2_a = {}, terms2_b = {}, terms2_c = {};
        int32_t resonanceEntry = {};
        int32_t cutoffEntry = {}, _4 = {};

        _2 = soul__intrinsics___clamp_specialised_3_f32_f32_f32 (cut, 0, 127.0f);
        cut = _2;
        _3 = soul__intrinsics___clamp_specialised_3_f32_f32_f32 (resonance, 0, 99.0f);
        resonance = _3;
        cutoffInterpolation = static_cast<float> (cut - static_cast<float> (static_cast<int32_t> (cut)));
        resonanceEntry = _intrin_wrap (static_cast<int32_t> (_internal___wrapInt32 (static_cast<int32_t> (static_cast<int32_t> (_intrin_wrap (static_cast<int32_t> (_internal___wrapInt32 (static_cast<int32_t> (_intrin_wrap (static_cast<int32_t> (static_cast<int32_t> (resonance)), 100)), 100)), 100))), 100)),
         100);
        cutoffEntry = _intrin_wrap (static_cast<int32_t> (static_cast<int32_t> (cut)), 128) & 127;
        terms1_a = static_cast<float> (_state.m_precalcTerms.m_frequency[cutoffEntry].m_resonance[resonanceEntry].m_a);
        terms1_b = static_cast<float> (_state.m_precalcTerms.m_frequency[cutoffEntry].m_resonance[resonanceEntry].m_b);
        terms1_c = static_cast<float> (_state.m_precalcTerms.m_frequency[cutoffEntry].m_resonance[resonanceEntry].m_c);
        _4 = cutoffEntry;
        cutoffEntry = (_4 + 1) & 127;
        terms2_a = static_cast<float> (_state.m_precalcTerms.m_frequency[cutoffEntry].m_resonance[resonanceEntry].m_a);
        terms2_b = static_cast<float> (_state.m_precalcTerms.m_frequency[cutoffEntry].m_resonance[resonanceEntry].m_b);
        terms2_c = static_cast<float> (_state.m_precalcTerms.m_frequency[cutoffEntry].m_resonance[resonanceEntry].m_c);
        _state.m_factor_a1 = static_cast<float> (terms1_a + ((terms2_a - terms1_a) * cutoffInterpolation));
        _state.m_factor_a2 = 2.0f * _state.m_factor_a1;
        _state.m_factor_b1 = static_cast<float> (terms1_b + ((terms2_b - terms1_b) * cutoffInterpolation));
        _state.m_factor_b2 = static_cast<float> (terms1_c + ((terms2_c - terms1_c) * cutoffInterpolation));
    }

    void Filter__run (Filter___State& _state, Filter___IO& _io) noexcept
    {
        Vector<float, 2> out_value_audioOut = {}, out = {}, _2 = {}, _3 = {};
        int32_t _resumePoint = {}, i = {}, _4 = {};
        float _5 = {};
        float in = {}, f = {};

        out_value_audioOut = ZeroInitialiser();
        _resumePoint = _state.m__resumePoint;
        if (_resumePoint == 1) goto _resume_point_1;
        _block_0: { Filter__initialiseTerms (_state);
                    _state.m_in_1 = 0;
                    _state.m_in_2 = 0;
                    _state.m_out_1 = 0;
                    _state.m_out_2 = 0;
        }
        _body_2: { _5 = _io.m__in_resonanceIn;
                   Filter__updateFactors (_state, _state.m_cutoff, _5);
                   _state.m_counter_3 = 64;
        }
        _loop_3: { if (! (_state.m_counter_3 > 0)) goto _body_2; }
        _body_3: { out = ZeroInitialiser();
                   i = 0;
        }
        _loop_4: { if (! (i < 2)) goto _break_4; }
        _body_4: { _2 = _io.m__in_audioIn;
                   in = static_cast<float> (_2[_intrin_wrap (static_cast<int32_t> (i), 2) & 1]);
                   f = static_cast<float> ((((_state.m_factor_a1 * static_cast<float> (in + static_cast<float> (_state.m_in_1))) + (_state.m_factor_a2 * _state.m_in_2)) - (_state.m_factor_b1 * _state.m_out_1)) - (_state.m_factor_b2 * _state.m_out_2));
                   _state.m_out_2 = _state.m_out_1;
                   _state.m_out_1 = static_cast<float> (f);
                   _state.m_in_2 = _state.m_in_1;
                   _state.m_in_1 = static_cast<float> (in);
                   out[_intrin_wrap (static_cast<int32_t> (i), 2) & 1] = static_cast<float> (f);
                   _4 = i;
                   i = _4 + 1;
                   goto _loop_4;
        }
        _break_4: { _3 = ZeroInitialiser();
                    _3[0] = (out[0] * 1.0f) * 0.2f;
                    _3[1] = (out[1] * 1.0f) * 0.2f;
                    out_value_audioOut = out_value_audioOut + _3;
                    _state.m__resumePoint = 1;
                    _io.m__out_audioOut = out_value_audioOut;
                    return;
        }
        _resume_point_1: { _state.m_counter_3 = _state.m_counter_3 - 1;
                           goto _loop_3;
        }
    }

    void Filter___initialise (Filter___State& _state) noexcept
    {
        _state.m_cutoff = 0;
        _state.m_precalcTerms = ZeroInitialiser();
        _state.m_factor_a1 = 0;
        _state.m_factor_a2 = 0;
        _state.m_factor_b1 = 0;
        _state.m_factor_b2 = 0;
    }

    //==============================================================================
    void Envelope__calculateTargetValue (Envelope___State& _state, float noteVelocity) noexcept
    {
        float attenuation = {};
        float _2 = {};

        attenuation = static_cast<float> ((-12.0f * _state.m_velocitySensitivity) * (0.75f - noteVelocity));
        _2 = std::pow (10.0f, static_cast<float> (attenuation / 10.0f));
        _state.m_targetValue = static_cast<double> (_2);
    }

    void Envelope__run (Envelope___State& _state, Envelope___IO& _io) noexcept
    {
        float out_value_envelopeLevel = {}, _2 = {}, _3 = {}, _4 = {}, _5 = {}, _6 = {};
        int32_t _resumePoint = {};
        double _7 = {};
        bool _8 = {}, _9 = {}, _10 = {}, _T2 = {};

        out_value_envelopeLevel = 0;
        _resumePoint = _state.m__resumePoint;
        if (_resumePoint == 1) goto _resume_point_1;
        _check_resume_point_2: { if (_resumePoint == 2) goto _loop_2; }
        _check_resume_point_3: { if (_resumePoint == 3) goto _loop_3; }
        _check_resume_point_4: { if (_resumePoint == 4) goto _body_0; }
        _block_0: { _state.m_value = static_cast<double> (_state.m_paramVal); }
        _body_0: { _2 = _io.m__in_triggerLevel;
                   if (! (_2 > 0)) goto _ifnot_0;
        }
        _if_0: { _3 = _io.m__in_triggerLevel;
                 Envelope__calculateTargetValue (_state, _3);
        }
        _loop_1: { if (! (_state.m_value < _state.m_targetValue)) goto _break_1; }
        _body_1: { _state.m_value = _state.m_value + _state.m_attackRamp;
                   out_value_envelopeLevel = out_value_envelopeLevel + static_cast<float> (_state.m_value);
                   _state.m__resumePoint = 1;
                   goto _exit;
        }
        _resume_point_1: { _4 = _io.m__in_triggerLevel;
                           if (! (_4 <= 0)) goto _loop_1;
        }
        _break_1: { _7 = soul__intrinsics___min_specialised_2_f64_f64 (_state.m_value, _state.m_targetValue);
                    _state.m_value = _7;
        }
        _loop_2: { if (! (_state.m_value > (_state.m_targetValue * _state.m_sustainLevel))) goto _ternary_false_2; }
        _ternary_true_2: { _5 = _io.m__in_triggerLevel;
                           _8 = _5 > 0;
                           _T2 = _8;
                           goto _ternary_end_2;
        }
        _ternary_false_2: { _9 = false;
                            _T2 = _9;
        }
        _ternary_end_2: { _10 = _T2;
                          if (! _10) goto _loop_3;
        }
        _body_2: { _state.m_value = _state.m_value * _state.m_decayMultiplier;
                   out_value_envelopeLevel = out_value_envelopeLevel + static_cast<float> (_state.m_value);
                   _state.m__resumePoint = 2;
                   goto _exit;
        }
        _loop_3: { _6 = _io.m__in_triggerLevel;
                   if (! (_6 > 0)) goto _ifnot_0;
        }
        _body_3: { out_value_envelopeLevel = out_value_envelopeLevel + static_cast<float> (_state.m_value);
                   _state.m__resumePoint = 3;
                   goto _exit;
        }
        _ifnot_0: { _state.m_value = _state.m_value * _state.m_releaseMultiplier;
                    out_value_envelopeLevel = out_value_envelopeLevel + static_cast<float> (_state.m_value);
                    _state.m__resumePoint = 4;
        }
        _exit: { _io.m__out_envelopeLevel = out_value_envelopeLevel; }
    }

    void Envelope___initialise (Envelope___State& _state) noexcept
    {
        _state.m_paramVal = 0;
        _state.m_modAmt = 0;
        _state.m_attackRamp = 0.01;
        _state.m_decayMultiplier = 0.995;
        _state.m_sustainLevel = 1.0;
        _state.m_releaseMultiplier = 0.99991;
        _state.m_velocitySensitivity = 0;
        _state.m_targetValue = 1.0;
    }

    //==============================================================================
    float PolyBlep_2__blep (PolyBlep_2___State& _state, float t2) noexcept
    {
        float t = {};

        t = t2;
        if (! (t < _state.m_normalisedPhaseIncrement)) goto _ifnot_0;
        _if_0: { t = t / _state.m_normalisedPhaseIncrement;
                 return ((t + t) - (t * t)) - 1.0f;
        }
        _ifnot_0: { if (! (t > (1.0f - _state.m_normalisedPhaseIncrement))) goto _ifnot_1; }
        _if_1: { t = (t - 1.0f) / _state.m_normalisedPhaseIncrement;
                 return ((t * t) + (t + t)) + 1.0f;
        }
        _ifnot_1: { return 0; }
    }

    void PolyBlep_2__run (PolyBlep_2___State& _state, PolyBlep_2___IO& _io) noexcept
    {
        Vector<float, 2> out_value_audioOut = {}, _2 = {};
        float _3 = {}, _4 = {}, _5 = {}, _6 = {}, normalisedPhase2 = {}, _7 = {}, _8 = {}, _9 = {}, squareValue = {}, _10 = {}, _11 = {}, _12 = {}, left = {}, right = {}, _13 = {}, _T4 = {};
        float sawValue = {}, amp = {};

        out_value_audioOut = ZeroInitialiser();
        _3 = _io.m__in_activeIn;
        if (! (_3 == 1.0f)) goto _ifnot_2;
        _if_2: { _4 = _io.m__in_detuneIn;
                 _state.m_detuneValue = _4 * 0.01f;
                 _5 = std::pow (2.0f, (_state.m_detuneSemitonesValue + _state.m_detuneValue) / 12.0f);
                 _state.m_frequencyMultiplier = _5;
                 _6 = _io.m__in_frequencyIn;
                 _state.m_normalisedPhaseIncrement = (_6 * (1.0f / static_cast<float> ((sampleRate * 1.0)))) * _state.m_frequencyMultiplier;
        }
        _loop_1: { if (! (_state.m_normalisedPhase >= 1.0f)) goto _break_1; }
        _body_1: { _state.m_normalisedPhase = _state.m_normalisedPhase - 1.0f;
                   goto _loop_1;
        }
        _break_1: { normalisedPhase2 = _state.m_normalisedPhase + 0.5f;
                    if (! (normalisedPhase2 > 1.0f)) goto _ifnot_3;
        }
        _if_3: { normalisedPhase2 = normalisedPhase2 - 1.0f; }
        _ifnot_3: { _7 = PolyBlep_2__blep (_state, _state.m_normalisedPhase);
                    sawValue = static_cast<float> (((2.0f * _state.m_normalisedPhase) - 1.0f) - _7);
                    _8 = PolyBlep_2__blep (_state, _state.m_normalisedPhase);
                    _9 = PolyBlep_2__blep (_state, normalisedPhase2);
                    squareValue = (-_8) + _9;
                    if (! (_state.m_normalisedPhase <= 0.5f)) goto _ternary_false_4;
        }
        _ternary_true_4: { _10 = -1.0f;
                           _T4 = _10;
                           goto _ternary_end_4;
        }
        _ternary_false_4: { _11 = 1.0f;
                            _T4 = _11;
        }
        _ternary_end_4: { _12 = _T4;
                          squareValue = squareValue + _12;
                          left = static_cast<float> (0.5 + static_cast<double> (_state.m_detuneValue));
                          right = static_cast<float> (0.5 - static_cast<double> (_state.m_detuneValue));
                          _13 = _io.m__in_gainIn;
                          amp = static_cast<float> (_13 * ((squareValue * _state.m_waveshapeValue) + static_cast<float> (sawValue * static_cast<float> (1.0f - _state.m_waveshapeValue))));
                          _2 = ZeroInitialiser();
                          _2[0] = static_cast<float> (amp * static_cast<float> (left));
                          _2[1] = static_cast<float> (amp * static_cast<float> (right));
                          out_value_audioOut = out_value_audioOut + _2;
                          _state.m_normalisedPhase = _state.m_normalisedPhase + _state.m_normalisedPhaseIncrement;
        }
        _ifnot_2: { out_value_audioOut = out_value_audioOut + ZeroInitialiser();
                    _state.m__resumePoint = 1;
                    _io.m__out_audioOut = out_value_audioOut;
        }
    }

    void PolyBlep_2___initialise (PolyBlep_2___State& _state) noexcept
    {
        _state.m_normalisedPhase = 0;
        _state.m_normalisedPhaseIncrement = 0;
        _state.m_waveshapeValue = 0;
        _state.m_detuneSemitonesValue = 0;
        _state.m_detuneValue = 0;
        _state.m_frequencyMultiplier = 1.0f;
    }

    //==============================================================================
    float PolyBlep_3__blep (PolyBlep_3___State& _state, float t2) noexcept
    {
        float t = {};

        t = t2;
        if (! (t < _state.m_normalisedPhaseIncrement)) goto _ifnot_0;
        _if_0: { t = t / _state.m_normalisedPhaseIncrement;
                 return ((t + t) - (t * t)) - 1.0f;
        }
        _ifnot_0: { if (! (t > (1.0f - _state.m_normalisedPhaseIncrement))) goto _ifnot_1; }
        _if_1: { t = (t - 1.0f) / _state.m_normalisedPhaseIncrement;
                 return ((t * t) + (t + t)) + 1.0f;
        }
        _ifnot_1: { return 0; }
    }

    void PolyBlep_3__run (PolyBlep_3___State& _state, PolyBlep_3___IO& _io) noexcept
    {
        Vector<float, 2> out_value_audioOut = {}, _2 = {};
        float _3 = {}, _4 = {}, _5 = {}, _6 = {}, normalisedPhase2 = {}, _7 = {}, _8 = {}, _9 = {}, squareValue = {}, _10 = {}, _11 = {}, _12 = {}, left = {}, right = {}, _13 = {}, _T4 = {};
        float sawValue = {}, amp = {};

        out_value_audioOut = ZeroInitialiser();
        _3 = _io.m__in_activeIn;
        if (! (_3 == 1.0f)) goto _ifnot_2;
        _if_2: { _4 = _io.m__in_detuneIn;
                 _state.m_detuneValue = _4 * 0.01f;
                 _5 = std::pow (2.0f, (_state.m_detuneSemitonesValue + _state.m_detuneValue) / 12.0f);
                 _state.m_frequencyMultiplier = _5;
                 _6 = _io.m__in_frequencyIn;
                 _state.m_normalisedPhaseIncrement = (_6 * (1.0f / static_cast<float> ((sampleRate * 1.0)))) * _state.m_frequencyMultiplier;
        }
        _loop_1: { if (! (_state.m_normalisedPhase >= 1.0f)) goto _break_1; }
        _body_1: { _state.m_normalisedPhase = _state.m_normalisedPhase - 1.0f;
                   goto _loop_1;
        }
        _break_1: { normalisedPhase2 = _state.m_normalisedPhase + 0.5f;
                    if (! (normalisedPhase2 > 1.0f)) goto _ifnot_3;
        }
        _if_3: { normalisedPhase2 = normalisedPhase2 - 1.0f; }
        _ifnot_3: { _7 = PolyBlep_3__blep (_state, _state.m_normalisedPhase);
                    sawValue = static_cast<float> (((2.0f * _state.m_normalisedPhase) - 1.0f) - _7);
                    _8 = PolyBlep_3__blep (_state, _state.m_normalisedPhase);
                    _9 = PolyBlep_3__blep (_state, normalisedPhase2);
                    squareValue = (-_8) + _9;
                    if (! (_state.m_normalisedPhase <= 0.5f)) goto _ternary_false_4;
        }
        _ternary_true_4: { _10 = -1.0f;
                           _T4 = _10;
                           goto _ternary_end_4;
        }
        _ternary_false_4: { _11 = 1.0f;
                            _T4 = _11;
        }
        _ternary_end_4: { _12 = _T4;
                          squareValue = squareValue + _12;
                          left = static_cast<float> (0.5 + static_cast<double> (_state.m_detuneValue));
                          right = static_cast<float> (0.5 - static_cast<double> (_state.m_detuneValue));
                          _13 = _io.m__in_gainIn;
                          amp = static_cast<float> (_13 * ((squareValue * _state.m_waveshapeValue) + static_cast<float> (sawValue * static_cast<float> (1.0f - _state.m_waveshapeValue))));
                          _2 = ZeroInitialiser();
                          _2[0] = static_cast<float> (amp * static_cast<float> (left));
                          _2[1] = static_cast<float> (amp * static_cast<float> (right));
                          out_value_audioOut = out_value_audioOut + _2;
                          _state.m_normalisedPhase = _state.m_normalisedPhase + _state.m_normalisedPhaseIncrement;
        }
        _ifnot_2: { out_value_audioOut = out_value_audioOut + ZeroInitialiser();
                    _state.m__resumePoint = 1;
                    _io.m__out_audioOut = out_value_audioOut;
        }
    }

    void PolyBlep_3___initialise (PolyBlep_3___State& _state) noexcept
    {
        _state.m_normalisedPhase = 0;
        _state.m_normalisedPhaseIncrement = 0;
        _state.m_waveshapeValue = 0;
        _state.m_detuneSemitonesValue = 0;
        _state.m_detuneValue = 0;
        _state.m_frequencyMultiplier = 1.0f;
    }

    //==============================================================================
    float PolyBlep_4__blep (PolyBlep_4___State& _state, float t2) noexcept
    {
        float t = {};

        t = t2;
        if (! (t < _state.m_normalisedPhaseIncrement)) goto _ifnot_0;
        _if_0: { t = t / _state.m_normalisedPhaseIncrement;
                 return ((t + t) - (t * t)) - 1.0f;
        }
        _ifnot_0: { if (! (t > (1.0f - _state.m_normalisedPhaseIncrement))) goto _ifnot_1; }
        _if_1: { t = (t - 1.0f) / _state.m_normalisedPhaseIncrement;
                 return ((t * t) + (t + t)) + 1.0f;
        }
        _ifnot_1: { return 0; }
    }

    void PolyBlep_4__run (PolyBlep_4___State& _state, PolyBlep_4___IO& _io) noexcept
    {
        Vector<float, 2> out_value_audioOut = {}, _2 = {};
        float _3 = {}, _4 = {}, _5 = {}, _6 = {}, normalisedPhase2 = {}, _7 = {}, _8 = {}, _9 = {}, squareValue = {}, _10 = {}, _11 = {}, _12 = {}, left = {}, right = {}, _13 = {}, _T4 = {};
        float sawValue = {}, amp = {};

        out_value_audioOut = ZeroInitialiser();
        _3 = _io.m__in_activeIn;
        if (! (_3 == 1.0f)) goto _ifnot_2;
        _if_2: { _4 = _io.m__in_detuneIn;
                 _state.m_detuneValue = _4 * 0.01f;
                 _5 = std::pow (2.0f, (_state.m_detuneSemitonesValue + _state.m_detuneValue) / 12.0f);
                 _state.m_frequencyMultiplier = _5;
                 _6 = _io.m__in_frequencyIn;
                 _state.m_normalisedPhaseIncrement = (_6 * (1.0f / static_cast<float> ((sampleRate * 1.0)))) * _state.m_frequencyMultiplier;
        }
        _loop_1: { if (! (_state.m_normalisedPhase >= 1.0f)) goto _break_1; }
        _body_1: { _state.m_normalisedPhase = _state.m_normalisedPhase - 1.0f;
                   goto _loop_1;
        }
        _break_1: { normalisedPhase2 = _state.m_normalisedPhase + 0.5f;
                    if (! (normalisedPhase2 > 1.0f)) goto _ifnot_3;
        }
        _if_3: { normalisedPhase2 = normalisedPhase2 - 1.0f; }
        _ifnot_3: { _7 = PolyBlep_4__blep (_state, _state.m_normalisedPhase);
                    sawValue = static_cast<float> (((2.0f * _state.m_normalisedPhase) - 1.0f) - _7);
                    _8 = PolyBlep_4__blep (_state, _state.m_normalisedPhase);
                    _9 = PolyBlep_4__blep (_state, normalisedPhase2);
                    squareValue = (-_8) + _9;
                    if (! (_state.m_normalisedPhase <= 0.5f)) goto _ternary_false_4;
        }
        _ternary_true_4: { _10 = -1.0f;
                           _T4 = _10;
                           goto _ternary_end_4;
        }
        _ternary_false_4: { _11 = 1.0f;
                            _T4 = _11;
        }
        _ternary_end_4: { _12 = _T4;
                          squareValue = squareValue + _12;
                          left = static_cast<float> (0.5 + static_cast<double> (_state.m_detuneValue));
                          right = static_cast<float> (0.5 - static_cast<double> (_state.m_detuneValue));
                          _13 = _io.m__in_gainIn;
                          amp = static_cast<float> (_13 * ((squareValue * _state.m_waveshapeValue) + static_cast<float> (sawValue * static_cast<float> (1.0f - _state.m_waveshapeValue))));
                          _2 = ZeroInitialiser();
                          _2[0] = static_cast<float> (amp * static_cast<float> (left));
                          _2[1] = static_cast<float> (amp * static_cast<float> (right));
                          out_value_audioOut = out_value_audioOut + _2;
                          _state.m_normalisedPhase = _state.m_normalisedPhase + _state.m_normalisedPhaseIncrement;
        }
        _ifnot_2: { out_value_audioOut = out_value_audioOut + ZeroInitialiser();
                    _state.m__resumePoint = 1;
                    _io.m__out_audioOut = out_value_audioOut;
        }
    }

    void PolyBlep_4___initialise (PolyBlep_4___State& _state) noexcept
    {
        _state.m_normalisedPhase = 0;
        _state.m_normalisedPhaseIncrement = 0;
        _state.m_waveshapeValue = 0;
        _state.m_detuneSemitonesValue = 0;
        _state.m_detuneValue = 0;
        _state.m_frequencyMultiplier = 1.0f;
    }

    //==============================================================================
    float PolyBlep_5__blep (PolyBlep_5___State& _state, float t2) noexcept
    {
        float t = {};

        t = t2;
        if (! (t < _state.m_normalisedPhaseIncrement)) goto _ifnot_0;
        _if_0: { t = t / _state.m_normalisedPhaseIncrement;
                 return ((t + t) - (t * t)) - 1.0f;
        }
        _ifnot_0: { if (! (t > (1.0f - _state.m_normalisedPhaseIncrement))) goto _ifnot_1; }
        _if_1: { t = (t - 1.0f) / _state.m_normalisedPhaseIncrement;
                 return ((t * t) + (t + t)) + 1.0f;
        }
        _ifnot_1: { return 0; }
    }

    void PolyBlep_5__run (PolyBlep_5___State& _state, PolyBlep_5___IO& _io) noexcept
    {
        Vector<float, 2> out_value_audioOut = {}, _2 = {};
        float _3 = {}, _4 = {}, _5 = {}, _6 = {}, normalisedPhase2 = {}, _7 = {}, _8 = {}, _9 = {}, squareValue = {}, _10 = {}, _11 = {}, _12 = {}, left = {}, right = {}, _13 = {}, _T4 = {};
        float sawValue = {}, amp = {};

        out_value_audioOut = ZeroInitialiser();
        _3 = _io.m__in_activeIn;
        if (! (_3 == 1.0f)) goto _ifnot_2;
        _if_2: { _4 = _io.m__in_detuneIn;
                 _state.m_detuneValue = _4 * 0.01f;
                 _5 = std::pow (2.0f, (_state.m_detuneSemitonesValue + _state.m_detuneValue) / 12.0f);
                 _state.m_frequencyMultiplier = _5;
                 _6 = _io.m__in_frequencyIn;
                 _state.m_normalisedPhaseIncrement = (_6 * (1.0f / static_cast<float> ((sampleRate * 1.0)))) * _state.m_frequencyMultiplier;
        }
        _loop_1: { if (! (_state.m_normalisedPhase >= 1.0f)) goto _break_1; }
        _body_1: { _state.m_normalisedPhase = _state.m_normalisedPhase - 1.0f;
                   goto _loop_1;
        }
        _break_1: { normalisedPhase2 = _state.m_normalisedPhase + 0.5f;
                    if (! (normalisedPhase2 > 1.0f)) goto _ifnot_3;
        }
        _if_3: { normalisedPhase2 = normalisedPhase2 - 1.0f; }
        _ifnot_3: { _7 = PolyBlep_5__blep (_state, _state.m_normalisedPhase);
                    sawValue = static_cast<float> (((2.0f * _state.m_normalisedPhase) - 1.0f) - _7);
                    _8 = PolyBlep_5__blep (_state, _state.m_normalisedPhase);
                    _9 = PolyBlep_5__blep (_state, normalisedPhase2);
                    squareValue = (-_8) + _9;
                    if (! (_state.m_normalisedPhase <= 0.5f)) goto _ternary_false_4;
        }
        _ternary_true_4: { _10 = -1.0f;
                           _T4 = _10;
                           goto _ternary_end_4;
        }
        _ternary_false_4: { _11 = 1.0f;
                            _T4 = _11;
        }
        _ternary_end_4: { _12 = _T4;
                          squareValue = squareValue + _12;
                          left = static_cast<float> (0.5 + static_cast<double> (_state.m_detuneValue));
                          right = static_cast<float> (0.5 - static_cast<double> (_state.m_detuneValue));
                          _13 = _io.m__in_gainIn;
                          amp = static_cast<float> (_13 * ((squareValue * _state.m_waveshapeValue) + static_cast<float> (sawValue * static_cast<float> (1.0f - _state.m_waveshapeValue))));
                          _2 = ZeroInitialiser();
                          _2[0] = static_cast<float> (amp * static_cast<float> (left));
                          _2[1] = static_cast<float> (amp * static_cast<float> (right));
                          out_value_audioOut = out_value_audioOut + _2;
                          _state.m_normalisedPhase = _state.m_normalisedPhase + _state.m_normalisedPhaseIncrement;
        }
        _ifnot_2: { out_value_audioOut = out_value_audioOut + ZeroInitialiser();
                    _state.m__resumePoint = 1;
                    _io.m__out_audioOut = out_value_audioOut;
        }
    }

    void PolyBlep_5___initialise (PolyBlep_5___State& _state) noexcept
    {
        _state.m_normalisedPhase = 0;
        _state.m_normalisedPhaseIncrement = 0;
        _state.m_waveshapeValue = 0;
        _state.m_detuneSemitonesValue = 0;
        _state.m_detuneValue = 0;
        _state.m_frequencyMultiplier = 1.0f;
    }

    //==============================================================================
    float PolyBlep_6__blep (PolyBlep_6___State& _state, float t2) noexcept
    {
        float t = {};

        t = t2;
        if (! (t < _state.m_normalisedPhaseIncrement)) goto _ifnot_0;
        _if_0: { t = t / _state.m_normalisedPhaseIncrement;
                 return ((t + t) - (t * t)) - 1.0f;
        }
        _ifnot_0: { if (! (t > (1.0f - _state.m_normalisedPhaseIncrement))) goto _ifnot_1; }
        _if_1: { t = (t - 1.0f) / _state.m_normalisedPhaseIncrement;
                 return ((t * t) + (t + t)) + 1.0f;
        }
        _ifnot_1: { return 0; }
    }

    void PolyBlep_6__run (PolyBlep_6___State& _state, PolyBlep_6___IO& _io) noexcept
    {
        Vector<float, 2> out_value_audioOut = {}, _2 = {};
        float _3 = {}, _4 = {}, _5 = {}, _6 = {}, normalisedPhase2 = {}, _7 = {}, _8 = {}, _9 = {}, squareValue = {}, _10 = {}, _11 = {}, _12 = {}, left = {}, right = {}, _13 = {}, _T4 = {};
        float sawValue = {}, amp = {};

        out_value_audioOut = ZeroInitialiser();
        _3 = _io.m__in_activeIn;
        if (! (_3 == 1.0f)) goto _ifnot_2;
        _if_2: { _4 = _io.m__in_detuneIn;
                 _state.m_detuneValue = _4 * 0.01f;
                 _5 = std::pow (2.0f, (_state.m_detuneSemitonesValue + _state.m_detuneValue) / 12.0f);
                 _state.m_frequencyMultiplier = _5;
                 _6 = _io.m__in_frequencyIn;
                 _state.m_normalisedPhaseIncrement = (_6 * (1.0f / static_cast<float> ((sampleRate * 1.0)))) * _state.m_frequencyMultiplier;
        }
        _loop_1: { if (! (_state.m_normalisedPhase >= 1.0f)) goto _break_1; }
        _body_1: { _state.m_normalisedPhase = _state.m_normalisedPhase - 1.0f;
                   goto _loop_1;
        }
        _break_1: { normalisedPhase2 = _state.m_normalisedPhase + 0.5f;
                    if (! (normalisedPhase2 > 1.0f)) goto _ifnot_3;
        }
        _if_3: { normalisedPhase2 = normalisedPhase2 - 1.0f; }
        _ifnot_3: { _7 = PolyBlep_6__blep (_state, _state.m_normalisedPhase);
                    sawValue = static_cast<float> (((2.0f * _state.m_normalisedPhase) - 1.0f) - _7);
                    _8 = PolyBlep_6__blep (_state, _state.m_normalisedPhase);
                    _9 = PolyBlep_6__blep (_state, normalisedPhase2);
                    squareValue = (-_8) + _9;
                    if (! (_state.m_normalisedPhase <= 0.5f)) goto _ternary_false_4;
        }
        _ternary_true_4: { _10 = -1.0f;
                           _T4 = _10;
                           goto _ternary_end_4;
        }
        _ternary_false_4: { _11 = 1.0f;
                            _T4 = _11;
        }
        _ternary_end_4: { _12 = _T4;
                          squareValue = squareValue + _12;
                          left = static_cast<float> (0.5 + static_cast<double> (_state.m_detuneValue));
                          right = static_cast<float> (0.5 - static_cast<double> (_state.m_detuneValue));
                          _13 = _io.m__in_gainIn;
                          amp = static_cast<float> (_13 * ((squareValue * _state.m_waveshapeValue) + static_cast<float> (sawValue * static_cast<float> (1.0f - _state.m_waveshapeValue))));
                          _2 = ZeroInitialiser();
                          _2[0] = static_cast<float> (amp * static_cast<float> (left));
                          _2[1] = static_cast<float> (amp * static_cast<float> (right));
                          out_value_audioOut = out_value_audioOut + _2;
                          _state.m_normalisedPhase = _state.m_normalisedPhase + _state.m_normalisedPhaseIncrement;
        }
        _ifnot_2: { out_value_audioOut = out_value_audioOut + ZeroInitialiser();
                    _state.m__resumePoint = 1;
                    _io.m__out_audioOut = out_value_audioOut;
        }
    }

    void PolyBlep_6___initialise (PolyBlep_6___State& _state) noexcept
    {
        _state.m_normalisedPhase = 0;
        _state.m_normalisedPhaseIncrement = 0;
        _state.m_waveshapeValue = 0;
        _state.m_detuneSemitonesValue = 0;
        _state.m_detuneValue = 0;
        _state.m_frequencyMultiplier = 1.0f;
    }

    //==============================================================================
    float PolyBlep_7__blep (PolyBlep_7___State& _state, float t2) noexcept
    {
        float t = {};

        t = t2;
        if (! (t < _state.m_normalisedPhaseIncrement)) goto _ifnot_0;
        _if_0: { t = t / _state.m_normalisedPhaseIncrement;
                 return ((t + t) - (t * t)) - 1.0f;
        }
        _ifnot_0: { if (! (t > (1.0f - _state.m_normalisedPhaseIncrement))) goto _ifnot_1; }
        _if_1: { t = (t - 1.0f) / _state.m_normalisedPhaseIncrement;
                 return ((t * t) + (t + t)) + 1.0f;
        }
        _ifnot_1: { return 0; }
    }

    void PolyBlep_7__run (PolyBlep_7___State& _state, PolyBlep_7___IO& _io) noexcept
    {
        Vector<float, 2> out_value_audioOut = {}, _2 = {};
        float _3 = {}, _4 = {}, _5 = {}, _6 = {}, normalisedPhase2 = {}, _7 = {}, _8 = {}, _9 = {}, squareValue = {}, _10 = {}, _11 = {}, _12 = {}, left = {}, right = {}, _13 = {}, _T4 = {};
        float sawValue = {}, amp = {};

        out_value_audioOut = ZeroInitialiser();
        _3 = _io.m__in_activeIn;
        if (! (_3 == 1.0f)) goto _ifnot_2;
        _if_2: { _4 = _io.m__in_detuneIn;
                 _state.m_detuneValue = _4 * 0.01f;
                 _5 = std::pow (2.0f, (_state.m_detuneSemitonesValue + _state.m_detuneValue) / 12.0f);
                 _state.m_frequencyMultiplier = _5;
                 _6 = _io.m__in_frequencyIn;
                 _state.m_normalisedPhaseIncrement = (_6 * (1.0f / static_cast<float> ((sampleRate * 1.0)))) * _state.m_frequencyMultiplier;
        }
        _loop_1: { if (! (_state.m_normalisedPhase >= 1.0f)) goto _break_1; }
        _body_1: { _state.m_normalisedPhase = _state.m_normalisedPhase - 1.0f;
                   goto _loop_1;
        }
        _break_1: { normalisedPhase2 = _state.m_normalisedPhase + 0.5f;
                    if (! (normalisedPhase2 > 1.0f)) goto _ifnot_3;
        }
        _if_3: { normalisedPhase2 = normalisedPhase2 - 1.0f; }
        _ifnot_3: { _7 = PolyBlep_7__blep (_state, _state.m_normalisedPhase);
                    sawValue = static_cast<float> (((2.0f * _state.m_normalisedPhase) - 1.0f) - _7);
                    _8 = PolyBlep_7__blep (_state, _state.m_normalisedPhase);
                    _9 = PolyBlep_7__blep (_state, normalisedPhase2);
                    squareValue = (-_8) + _9;
                    if (! (_state.m_normalisedPhase <= 0.5f)) goto _ternary_false_4;
        }
        _ternary_true_4: { _10 = -1.0f;
                           _T4 = _10;
                           goto _ternary_end_4;
        }
        _ternary_false_4: { _11 = 1.0f;
                            _T4 = _11;
        }
        _ternary_end_4: { _12 = _T4;
                          squareValue = squareValue + _12;
                          left = static_cast<float> (0.5 + static_cast<double> (_state.m_detuneValue));
                          right = static_cast<float> (0.5 - static_cast<double> (_state.m_detuneValue));
                          _13 = _io.m__in_gainIn;
                          amp = static_cast<float> (_13 * ((squareValue * _state.m_waveshapeValue) + static_cast<float> (sawValue * static_cast<float> (1.0f - _state.m_waveshapeValue))));
                          _2 = ZeroInitialiser();
                          _2[0] = static_cast<float> (amp * static_cast<float> (left));
                          _2[1] = static_cast<float> (amp * static_cast<float> (right));
                          out_value_audioOut = out_value_audioOut + _2;
                          _state.m_normalisedPhase = _state.m_normalisedPhase + _state.m_normalisedPhaseIncrement;
        }
        _ifnot_2: { out_value_audioOut = out_value_audioOut + ZeroInitialiser();
                    _state.m__resumePoint = 1;
                    _io.m__out_audioOut = out_value_audioOut;
        }
    }

    void PolyBlep_7___initialise (PolyBlep_7___State& _state) noexcept
    {
        _state.m_normalisedPhase = 0;
        _state.m_normalisedPhaseIncrement = 0;
        _state.m_waveshapeValue = 0;
        _state.m_detuneSemitonesValue = 0;
        _state.m_detuneValue = 0;
        _state.m_frequencyMultiplier = 1.0f;
    }

    //==============================================================================
    float PolyBlep_8__blep (PolyBlep_8___State& _state, float t2) noexcept
    {
        float t = {};

        t = t2;
        if (! (t < _state.m_normalisedPhaseIncrement)) goto _ifnot_0;
        _if_0: { t = t / _state.m_normalisedPhaseIncrement;
                 return ((t + t) - (t * t)) - 1.0f;
        }
        _ifnot_0: { if (! (t > (1.0f - _state.m_normalisedPhaseIncrement))) goto _ifnot_1; }
        _if_1: { t = (t - 1.0f) / _state.m_normalisedPhaseIncrement;
                 return ((t * t) + (t + t)) + 1.0f;
        }
        _ifnot_1: { return 0; }
    }

    void PolyBlep_8__run (PolyBlep_8___State& _state, PolyBlep_8___IO& _io) noexcept
    {
        Vector<float, 2> out_value_audioOut = {}, _2 = {};
        float _3 = {}, _4 = {}, _5 = {}, _6 = {}, normalisedPhase2 = {}, _7 = {}, _8 = {}, _9 = {}, squareValue = {}, _10 = {}, _11 = {}, _12 = {}, left = {}, right = {}, _13 = {}, _T4 = {};
        float sawValue = {}, amp = {};

        out_value_audioOut = ZeroInitialiser();
        _3 = _io.m__in_activeIn;
        if (! (_3 == 1.0f)) goto _ifnot_2;
        _if_2: { _4 = _io.m__in_detuneIn;
                 _state.m_detuneValue = _4 * 0.01f;
                 _5 = std::pow (2.0f, (_state.m_detuneSemitonesValue + _state.m_detuneValue) / 12.0f);
                 _state.m_frequencyMultiplier = _5;
                 _6 = _io.m__in_frequencyIn;
                 _state.m_normalisedPhaseIncrement = (_6 * (1.0f / static_cast<float> ((sampleRate * 1.0)))) * _state.m_frequencyMultiplier;
        }
        _loop_1: { if (! (_state.m_normalisedPhase >= 1.0f)) goto _break_1; }
        _body_1: { _state.m_normalisedPhase = _state.m_normalisedPhase - 1.0f;
                   goto _loop_1;
        }
        _break_1: { normalisedPhase2 = _state.m_normalisedPhase + 0.5f;
                    if (! (normalisedPhase2 > 1.0f)) goto _ifnot_3;
        }
        _if_3: { normalisedPhase2 = normalisedPhase2 - 1.0f; }
        _ifnot_3: { _7 = PolyBlep_8__blep (_state, _state.m_normalisedPhase);
                    sawValue = static_cast<float> (((2.0f * _state.m_normalisedPhase) - 1.0f) - _7);
                    _8 = PolyBlep_8__blep (_state, _state.m_normalisedPhase);
                    _9 = PolyBlep_8__blep (_state, normalisedPhase2);
                    squareValue = (-_8) + _9;
                    if (! (_state.m_normalisedPhase <= 0.5f)) goto _ternary_false_4;
        }
        _ternary_true_4: { _10 = -1.0f;
                           _T4 = _10;
                           goto _ternary_end_4;
        }
        _ternary_false_4: { _11 = 1.0f;
                            _T4 = _11;
        }
        _ternary_end_4: { _12 = _T4;
                          squareValue = squareValue + _12;
                          left = static_cast<float> (0.5 + static_cast<double> (_state.m_detuneValue));
                          right = static_cast<float> (0.5 - static_cast<double> (_state.m_detuneValue));
                          _13 = _io.m__in_gainIn;
                          amp = static_cast<float> (_13 * ((squareValue * _state.m_waveshapeValue) + static_cast<float> (sawValue * static_cast<float> (1.0f - _state.m_waveshapeValue))));
                          _2 = ZeroInitialiser();
                          _2[0] = static_cast<float> (amp * static_cast<float> (left));
                          _2[1] = static_cast<float> (amp * static_cast<float> (right));
                          out_value_audioOut = out_value_audioOut + _2;
                          _state.m_normalisedPhase = _state.m_normalisedPhase + _state.m_normalisedPhaseIncrement;
        }
        _ifnot_2: { out_value_audioOut = out_value_audioOut + ZeroInitialiser();
                    _state.m__resumePoint = 1;
                    _io.m__out_audioOut = out_value_audioOut;
        }
    }

    void PolyBlep_8___initialise (PolyBlep_8___State& _state) noexcept
    {
        _state.m_normalisedPhase = 0;
        _state.m_normalisedPhaseIncrement = 0;
        _state.m_waveshapeValue = 0;
        _state.m_detuneSemitonesValue = 0;
        _state.m_detuneValue = 0;
        _state.m_frequencyMultiplier = 1.0f;
    }

    //==============================================================================
    float PolyBlep_9__blep (PolyBlep_9___State& _state, float t2) noexcept
    {
        float t = {};

        t = t2;
        if (! (t < _state.m_normalisedPhaseIncrement)) goto _ifnot_0;
        _if_0: { t = t / _state.m_normalisedPhaseIncrement;
                 return ((t + t) - (t * t)) - 1.0f;
        }
        _ifnot_0: { if (! (t > (1.0f - _state.m_normalisedPhaseIncrement))) goto _ifnot_1; }
        _if_1: { t = (t - 1.0f) / _state.m_normalisedPhaseIncrement;
                 return ((t * t) + (t + t)) + 1.0f;
        }
        _ifnot_1: { return 0; }
    }

    void PolyBlep_9__run (PolyBlep_9___State& _state, PolyBlep_9___IO& _io) noexcept
    {
        Vector<float, 2> out_value_audioOut = {}, _2 = {};
        float _3 = {}, _4 = {}, _5 = {}, _6 = {}, normalisedPhase2 = {}, _7 = {}, _8 = {}, _9 = {}, squareValue = {}, _10 = {}, _11 = {}, _12 = {}, left = {}, right = {}, _13 = {}, _T4 = {};
        float sawValue = {}, amp = {};

        out_value_audioOut = ZeroInitialiser();
        _3 = _io.m__in_activeIn;
        if (! (_3 == 1.0f)) goto _ifnot_2;
        _if_2: { _4 = _io.m__in_detuneIn;
                 _state.m_detuneValue = _4 * 0.01f;
                 _5 = std::pow (2.0f, (_state.m_detuneSemitonesValue + _state.m_detuneValue) / 12.0f);
                 _state.m_frequencyMultiplier = _5;
                 _6 = _io.m__in_frequencyIn;
                 _state.m_normalisedPhaseIncrement = (_6 * (1.0f / static_cast<float> ((sampleRate * 1.0)))) * _state.m_frequencyMultiplier;
        }
        _loop_1: { if (! (_state.m_normalisedPhase >= 1.0f)) goto _break_1; }
        _body_1: { _state.m_normalisedPhase = _state.m_normalisedPhase - 1.0f;
                   goto _loop_1;
        }
        _break_1: { normalisedPhase2 = _state.m_normalisedPhase + 0.5f;
                    if (! (normalisedPhase2 > 1.0f)) goto _ifnot_3;
        }
        _if_3: { normalisedPhase2 = normalisedPhase2 - 1.0f; }
        _ifnot_3: { _7 = PolyBlep_9__blep (_state, _state.m_normalisedPhase);
                    sawValue = static_cast<float> (((2.0f * _state.m_normalisedPhase) - 1.0f) - _7);
                    _8 = PolyBlep_9__blep (_state, _state.m_normalisedPhase);
                    _9 = PolyBlep_9__blep (_state, normalisedPhase2);
                    squareValue = (-_8) + _9;
                    if (! (_state.m_normalisedPhase <= 0.5f)) goto _ternary_false_4;
        }
        _ternary_true_4: { _10 = -1.0f;
                           _T4 = _10;
                           goto _ternary_end_4;
        }
        _ternary_false_4: { _11 = 1.0f;
                            _T4 = _11;
        }
        _ternary_end_4: { _12 = _T4;
                          squareValue = squareValue + _12;
                          left = static_cast<float> (0.5 + static_cast<double> (_state.m_detuneValue));
                          right = static_cast<float> (0.5 - static_cast<double> (_state.m_detuneValue));
                          _13 = _io.m__in_gainIn;
                          amp = static_cast<float> (_13 * ((squareValue * _state.m_waveshapeValue) + static_cast<float> (sawValue * static_cast<float> (1.0f - _state.m_waveshapeValue))));
                          _2 = ZeroInitialiser();
                          _2[0] = static_cast<float> (amp * static_cast<float> (left));
                          _2[1] = static_cast<float> (amp * static_cast<float> (right));
                          out_value_audioOut = out_value_audioOut + _2;
                          _state.m_normalisedPhase = _state.m_normalisedPhase + _state.m_normalisedPhaseIncrement;
        }
        _ifnot_2: { out_value_audioOut = out_value_audioOut + ZeroInitialiser();
                    _state.m__resumePoint = 1;
                    _io.m__out_audioOut = out_value_audioOut;
        }
    }

    void PolyBlep_9___initialise (PolyBlep_9___State& _state) noexcept
    {
        _state.m_normalisedPhase = 0;
        _state.m_normalisedPhaseIncrement = 0;
        _state.m_waveshapeValue = 0;
        _state.m_detuneSemitonesValue = 0;
        _state.m_detuneValue = 0;
        _state.m_frequencyMultiplier = 1.0f;
    }

    //==============================================================================
    float PolyBlep_10__blep (PolyBlep_10___State& _state, float t2) noexcept
    {
        float t = {};

        t = t2;
        if (! (t < _state.m_normalisedPhaseIncrement)) goto _ifnot_0;
        _if_0: { t = t / _state.m_normalisedPhaseIncrement;
                 return ((t + t) - (t * t)) - 1.0f;
        }
        _ifnot_0: { if (! (t > (1.0f - _state.m_normalisedPhaseIncrement))) goto _ifnot_1; }
        _if_1: { t = (t - 1.0f) / _state.m_normalisedPhaseIncrement;
                 return ((t * t) + (t + t)) + 1.0f;
        }
        _ifnot_1: { return 0; }
    }

    void PolyBlep_10__run (PolyBlep_10___State& _state, PolyBlep_10___IO& _io) noexcept
    {
        Vector<float, 2> out_value_audioOut = {}, _2 = {};
        float _3 = {}, _4 = {}, _5 = {}, _6 = {}, normalisedPhase2 = {}, _7 = {}, _8 = {}, _9 = {}, squareValue = {}, _10 = {}, _11 = {}, _12 = {}, left = {}, right = {}, _13 = {}, _T4 = {};
        float sawValue = {}, amp = {};

        out_value_audioOut = ZeroInitialiser();
        _3 = _io.m__in_activeIn;
        if (! (_3 == 1.0f)) goto _ifnot_2;
        _if_2: { _4 = _io.m__in_detuneIn;
                 _state.m_detuneValue = _4 * 0.01f;
                 _5 = std::pow (2.0f, (_state.m_detuneSemitonesValue + _state.m_detuneValue) / 12.0f);
                 _state.m_frequencyMultiplier = _5;
                 _6 = _io.m__in_frequencyIn;
                 _state.m_normalisedPhaseIncrement = (_6 * (1.0f / static_cast<float> ((sampleRate * 1.0)))) * _state.m_frequencyMultiplier;
        }
        _loop_1: { if (! (_state.m_normalisedPhase >= 1.0f)) goto _break_1; }
        _body_1: { _state.m_normalisedPhase = _state.m_normalisedPhase - 1.0f;
                   goto _loop_1;
        }
        _break_1: { normalisedPhase2 = _state.m_normalisedPhase + 0.5f;
                    if (! (normalisedPhase2 > 1.0f)) goto _ifnot_3;
        }
        _if_3: { normalisedPhase2 = normalisedPhase2 - 1.0f; }
        _ifnot_3: { _7 = PolyBlep_10__blep (_state, _state.m_normalisedPhase);
                    sawValue = static_cast<float> (((2.0f * _state.m_normalisedPhase) - 1.0f) - _7);
                    _8 = PolyBlep_10__blep (_state, _state.m_normalisedPhase);
                    _9 = PolyBlep_10__blep (_state, normalisedPhase2);
                    squareValue = (-_8) + _9;
                    if (! (_state.m_normalisedPhase <= 0.5f)) goto _ternary_false_4;
        }
        _ternary_true_4: { _10 = -1.0f;
                           _T4 = _10;
                           goto _ternary_end_4;
        }
        _ternary_false_4: { _11 = 1.0f;
                            _T4 = _11;
        }
        _ternary_end_4: { _12 = _T4;
                          squareValue = squareValue + _12;
                          left = static_cast<float> (0.5 + static_cast<double> (_state.m_detuneValue));
                          right = static_cast<float> (0.5 - static_cast<double> (_state.m_detuneValue));
                          _13 = _io.m__in_gainIn;
                          amp = static_cast<float> (_13 * ((squareValue * _state.m_waveshapeValue) + static_cast<float> (sawValue * static_cast<float> (1.0f - _state.m_waveshapeValue))));
                          _2 = ZeroInitialiser();
                          _2[0] = static_cast<float> (amp * static_cast<float> (left));
                          _2[1] = static_cast<float> (amp * static_cast<float> (right));
                          out_value_audioOut = out_value_audioOut + _2;
                          _state.m_normalisedPhase = _state.m_normalisedPhase + _state.m_normalisedPhaseIncrement;
        }
        _ifnot_2: { out_value_audioOut = out_value_audioOut + ZeroInitialiser();
                    _state.m__resumePoint = 1;
                    _io.m__out_audioOut = out_value_audioOut;
        }
    }

    void PolyBlep_10___initialise (PolyBlep_10___State& _state) noexcept
    {
        _state.m_normalisedPhase = 0;
        _state.m_normalisedPhaseIncrement = 0;
        _state.m_waveshapeValue = 0;
        _state.m_detuneSemitonesValue = 0;
        _state.m_detuneValue = 0;
        _state.m_frequencyMultiplier = 1.0f;
    }

    //==============================================================================
    float PolyBlep_11__blep (PolyBlep_11___State& _state, float t2) noexcept
    {
        float t = {};

        t = t2;
        if (! (t < _state.m_normalisedPhaseIncrement)) goto _ifnot_0;
        _if_0: { t = t / _state.m_normalisedPhaseIncrement;
                 return ((t + t) - (t * t)) - 1.0f;
        }
        _ifnot_0: { if (! (t > (1.0f - _state.m_normalisedPhaseIncrement))) goto _ifnot_1; }
        _if_1: { t = (t - 1.0f) / _state.m_normalisedPhaseIncrement;
                 return ((t * t) + (t + t)) + 1.0f;
        }
        _ifnot_1: { return 0; }
    }

    void PolyBlep_11__run (PolyBlep_11___State& _state, PolyBlep_11___IO& _io) noexcept
    {
        Vector<float, 2> out_value_audioOut = {}, _2 = {};
        float _3 = {}, _4 = {}, _5 = {}, _6 = {}, normalisedPhase2 = {}, _7 = {}, _8 = {}, _9 = {}, squareValue = {}, _10 = {}, _11 = {}, _12 = {}, left = {}, right = {}, _13 = {}, _T4 = {};
        float sawValue = {}, amp = {};

        out_value_audioOut = ZeroInitialiser();
        _3 = _io.m__in_activeIn;
        if (! (_3 == 1.0f)) goto _ifnot_2;
        _if_2: { _4 = _io.m__in_detuneIn;
                 _state.m_detuneValue = _4 * 0.01f;
                 _5 = std::pow (2.0f, (_state.m_detuneSemitonesValue + _state.m_detuneValue) / 12.0f);
                 _state.m_frequencyMultiplier = _5;
                 _6 = _io.m__in_frequencyIn;
                 _state.m_normalisedPhaseIncrement = (_6 * (1.0f / static_cast<float> ((sampleRate * 1.0)))) * _state.m_frequencyMultiplier;
        }
        _loop_1: { if (! (_state.m_normalisedPhase >= 1.0f)) goto _break_1; }
        _body_1: { _state.m_normalisedPhase = _state.m_normalisedPhase - 1.0f;
                   goto _loop_1;
        }
        _break_1: { normalisedPhase2 = _state.m_normalisedPhase + 0.5f;
                    if (! (normalisedPhase2 > 1.0f)) goto _ifnot_3;
        }
        _if_3: { normalisedPhase2 = normalisedPhase2 - 1.0f; }
        _ifnot_3: { _7 = PolyBlep_11__blep (_state, _state.m_normalisedPhase);
                    sawValue = static_cast<float> (((2.0f * _state.m_normalisedPhase) - 1.0f) - _7);
                    _8 = PolyBlep_11__blep (_state, _state.m_normalisedPhase);
                    _9 = PolyBlep_11__blep (_state, normalisedPhase2);
                    squareValue = (-_8) + _9;
                    if (! (_state.m_normalisedPhase <= 0.5f)) goto _ternary_false_4;
        }
        _ternary_true_4: { _10 = -1.0f;
                           _T4 = _10;
                           goto _ternary_end_4;
        }
        _ternary_false_4: { _11 = 1.0f;
                            _T4 = _11;
        }
        _ternary_end_4: { _12 = _T4;
                          squareValue = squareValue + _12;
                          left = static_cast<float> (0.5 + static_cast<double> (_state.m_detuneValue));
                          right = static_cast<float> (0.5 - static_cast<double> (_state.m_detuneValue));
                          _13 = _io.m__in_gainIn;
                          amp = static_cast<float> (_13 * ((squareValue * _state.m_waveshapeValue) + static_cast<float> (sawValue * static_cast<float> (1.0f - _state.m_waveshapeValue))));
                          _2 = ZeroInitialiser();
                          _2[0] = static_cast<float> (amp * static_cast<float> (left));
                          _2[1] = static_cast<float> (amp * static_cast<float> (right));
                          out_value_audioOut = out_value_audioOut + _2;
                          _state.m_normalisedPhase = _state.m_normalisedPhase + _state.m_normalisedPhaseIncrement;
        }
        _ifnot_2: { out_value_audioOut = out_value_audioOut + ZeroInitialiser();
                    _state.m__resumePoint = 1;
                    _io.m__out_audioOut = out_value_audioOut;
        }
    }

    void PolyBlep_11___initialise (PolyBlep_11___State& _state) noexcept
    {
        _state.m_normalisedPhase = 0;
        _state.m_normalisedPhaseIncrement = 0;
        _state.m_waveshapeValue = 0;
        _state.m_detuneSemitonesValue = 0;
        _state.m_detuneValue = 0;
        _state.m_frequencyMultiplier = 1.0f;
    }

    //==============================================================================
    float PolyBlep_12__blep (PolyBlep_12___State& _state, float t2) noexcept
    {
        float t = {};

        t = t2;
        if (! (t < _state.m_normalisedPhaseIncrement)) goto _ifnot_0;
        _if_0: { t = t / _state.m_normalisedPhaseIncrement;
                 return ((t + t) - (t * t)) - 1.0f;
        }
        _ifnot_0: { if (! (t > (1.0f - _state.m_normalisedPhaseIncrement))) goto _ifnot_1; }
        _if_1: { t = (t - 1.0f) / _state.m_normalisedPhaseIncrement;
                 return ((t * t) + (t + t)) + 1.0f;
        }
        _ifnot_1: { return 0; }
    }

    void PolyBlep_12__run (PolyBlep_12___State& _state, PolyBlep_12___IO& _io) noexcept
    {
        Vector<float, 2> out_value_audioOut = {}, _2 = {};
        float _3 = {}, _4 = {}, _5 = {}, _6 = {}, normalisedPhase2 = {}, _7 = {}, _8 = {}, _9 = {}, squareValue = {}, _10 = {}, _11 = {}, _12 = {}, left = {}, right = {}, _13 = {}, _T4 = {};
        float sawValue = {}, amp = {};

        out_value_audioOut = ZeroInitialiser();
        _3 = _io.m__in_activeIn;
        if (! (_3 == 1.0f)) goto _ifnot_2;
        _if_2: { _4 = _io.m__in_detuneIn;
                 _state.m_detuneValue = _4 * 0.01f;
                 _5 = std::pow (2.0f, (_state.m_detuneSemitonesValue + _state.m_detuneValue) / 12.0f);
                 _state.m_frequencyMultiplier = _5;
                 _6 = _io.m__in_frequencyIn;
                 _state.m_normalisedPhaseIncrement = (_6 * (1.0f / static_cast<float> ((sampleRate * 1.0)))) * _state.m_frequencyMultiplier;
        }
        _loop_1: { if (! (_state.m_normalisedPhase >= 1.0f)) goto _break_1; }
        _body_1: { _state.m_normalisedPhase = _state.m_normalisedPhase - 1.0f;
                   goto _loop_1;
        }
        _break_1: { normalisedPhase2 = _state.m_normalisedPhase + 0.5f;
                    if (! (normalisedPhase2 > 1.0f)) goto _ifnot_3;
        }
        _if_3: { normalisedPhase2 = normalisedPhase2 - 1.0f; }
        _ifnot_3: { _7 = PolyBlep_12__blep (_state, _state.m_normalisedPhase);
                    sawValue = static_cast<float> (((2.0f * _state.m_normalisedPhase) - 1.0f) - _7);
                    _8 = PolyBlep_12__blep (_state, _state.m_normalisedPhase);
                    _9 = PolyBlep_12__blep (_state, normalisedPhase2);
                    squareValue = (-_8) + _9;
                    if (! (_state.m_normalisedPhase <= 0.5f)) goto _ternary_false_4;
        }
        _ternary_true_4: { _10 = -1.0f;
                           _T4 = _10;
                           goto _ternary_end_4;
        }
        _ternary_false_4: { _11 = 1.0f;
                            _T4 = _11;
        }
        _ternary_end_4: { _12 = _T4;
                          squareValue = squareValue + _12;
                          left = static_cast<float> (0.5 + static_cast<double> (_state.m_detuneValue));
                          right = static_cast<float> (0.5 - static_cast<double> (_state.m_detuneValue));
                          _13 = _io.m__in_gainIn;
                          amp = static_cast<float> (_13 * ((squareValue * _state.m_waveshapeValue) + static_cast<float> (sawValue * static_cast<float> (1.0f - _state.m_waveshapeValue))));
                          _2 = ZeroInitialiser();
                          _2[0] = static_cast<float> (amp * static_cast<float> (left));
                          _2[1] = static_cast<float> (amp * static_cast<float> (right));
                          out_value_audioOut = out_value_audioOut + _2;
                          _state.m_normalisedPhase = _state.m_normalisedPhase + _state.m_normalisedPhaseIncrement;
        }
        _ifnot_2: { out_value_audioOut = out_value_audioOut + ZeroInitialiser();
                    _state.m__resumePoint = 1;
                    _io.m__out_audioOut = out_value_audioOut;
        }
    }

    void PolyBlep_12___initialise (PolyBlep_12___State& _state) noexcept
    {
        _state.m_normalisedPhase = 0;
        _state.m_normalisedPhaseIncrement = 0;
        _state.m_waveshapeValue = 0;
        _state.m_detuneSemitonesValue = 0;
        _state.m_detuneValue = 0;
        _state.m_frequencyMultiplier = 1.0f;
    }

    //==============================================================================
    float PolyBlep_13__blep (PolyBlep_13___State& _state, float t2) noexcept
    {
        float t = {};

        t = t2;
        if (! (t < _state.m_normalisedPhaseIncrement)) goto _ifnot_0;
        _if_0: { t = t / _state.m_normalisedPhaseIncrement;
                 return ((t + t) - (t * t)) - 1.0f;
        }
        _ifnot_0: { if (! (t > (1.0f - _state.m_normalisedPhaseIncrement))) goto _ifnot_1; }
        _if_1: { t = (t - 1.0f) / _state.m_normalisedPhaseIncrement;
                 return ((t * t) + (t + t)) + 1.0f;
        }
        _ifnot_1: { return 0; }
    }

    void PolyBlep_13__run (PolyBlep_13___State& _state, PolyBlep_13___IO& _io) noexcept
    {
        Vector<float, 2> out_value_audioOut = {}, _2 = {};
        float _3 = {}, _4 = {}, _5 = {}, _6 = {}, normalisedPhase2 = {}, _7 = {}, _8 = {}, _9 = {}, squareValue = {}, _10 = {}, _11 = {}, _12 = {}, left = {}, right = {}, _13 = {}, _T4 = {};
        float sawValue = {}, amp = {};

        out_value_audioOut = ZeroInitialiser();
        _3 = _io.m__in_activeIn;
        if (! (_3 == 1.0f)) goto _ifnot_2;
        _if_2: { _4 = _io.m__in_detuneIn;
                 _state.m_detuneValue = _4 * 0.01f;
                 _5 = std::pow (2.0f, (_state.m_detuneSemitonesValue + _state.m_detuneValue) / 12.0f);
                 _state.m_frequencyMultiplier = _5;
                 _6 = _io.m__in_frequencyIn;
                 _state.m_normalisedPhaseIncrement = (_6 * (1.0f / static_cast<float> ((sampleRate * 1.0)))) * _state.m_frequencyMultiplier;
        }
        _loop_1: { if (! (_state.m_normalisedPhase >= 1.0f)) goto _break_1; }
        _body_1: { _state.m_normalisedPhase = _state.m_normalisedPhase - 1.0f;
                   goto _loop_1;
        }
        _break_1: { normalisedPhase2 = _state.m_normalisedPhase + 0.5f;
                    if (! (normalisedPhase2 > 1.0f)) goto _ifnot_3;
        }
        _if_3: { normalisedPhase2 = normalisedPhase2 - 1.0f; }
        _ifnot_3: { _7 = PolyBlep_13__blep (_state, _state.m_normalisedPhase);
                    sawValue = static_cast<float> (((2.0f * _state.m_normalisedPhase) - 1.0f) - _7);
                    _8 = PolyBlep_13__blep (_state, _state.m_normalisedPhase);
                    _9 = PolyBlep_13__blep (_state, normalisedPhase2);
                    squareValue = (-_8) + _9;
                    if (! (_state.m_normalisedPhase <= 0.5f)) goto _ternary_false_4;
        }
        _ternary_true_4: { _10 = -1.0f;
                           _T4 = _10;
                           goto _ternary_end_4;
        }
        _ternary_false_4: { _11 = 1.0f;
                            _T4 = _11;
        }
        _ternary_end_4: { _12 = _T4;
                          squareValue = squareValue + _12;
                          left = static_cast<float> (0.5 + static_cast<double> (_state.m_detuneValue));
                          right = static_cast<float> (0.5 - static_cast<double> (_state.m_detuneValue));
                          _13 = _io.m__in_gainIn;
                          amp = static_cast<float> (_13 * ((squareValue * _state.m_waveshapeValue) + static_cast<float> (sawValue * static_cast<float> (1.0f - _state.m_waveshapeValue))));
                          _2 = ZeroInitialiser();
                          _2[0] = static_cast<float> (amp * static_cast<float> (left));
                          _2[1] = static_cast<float> (amp * static_cast<float> (right));
                          out_value_audioOut = out_value_audioOut + _2;
                          _state.m_normalisedPhase = _state.m_normalisedPhase + _state.m_normalisedPhaseIncrement;
        }
        _ifnot_2: { out_value_audioOut = out_value_audioOut + ZeroInitialiser();
                    _state.m__resumePoint = 1;
                    _io.m__out_audioOut = out_value_audioOut;
        }
    }

    void PolyBlep_13___initialise (PolyBlep_13___State& _state) noexcept
    {
        _state.m_normalisedPhase = 0;
        _state.m_normalisedPhaseIncrement = 0;
        _state.m_waveshapeValue = 0;
        _state.m_detuneSemitonesValue = 0;
        _state.m_detuneValue = 0;
        _state.m_frequencyMultiplier = 1.0f;
    }

    //==============================================================================
    float PolyBlep_14__blep (PolyBlep_14___State& _state, float t2) noexcept
    {
        float t = {};

        t = t2;
        if (! (t < _state.m_normalisedPhaseIncrement)) goto _ifnot_0;
        _if_0: { t = t / _state.m_normalisedPhaseIncrement;
                 return ((t + t) - (t * t)) - 1.0f;
        }
        _ifnot_0: { if (! (t > (1.0f - _state.m_normalisedPhaseIncrement))) goto _ifnot_1; }
        _if_1: { t = (t - 1.0f) / _state.m_normalisedPhaseIncrement;
                 return ((t * t) + (t + t)) + 1.0f;
        }
        _ifnot_1: { return 0; }
    }

    void PolyBlep_14__run (PolyBlep_14___State& _state, PolyBlep_14___IO& _io) noexcept
    {
        Vector<float, 2> out_value_audioOut = {}, _2 = {};
        float _3 = {}, _4 = {}, _5 = {}, _6 = {}, normalisedPhase2 = {}, _7 = {}, _8 = {}, _9 = {}, squareValue = {}, _10 = {}, _11 = {}, _12 = {}, left = {}, right = {}, _13 = {}, _T4 = {};
        float sawValue = {}, amp = {};

        out_value_audioOut = ZeroInitialiser();
        _3 = _io.m__in_activeIn;
        if (! (_3 == 1.0f)) goto _ifnot_2;
        _if_2: { _4 = _io.m__in_detuneIn;
                 _state.m_detuneValue = _4 * 0.01f;
                 _5 = std::pow (2.0f, (_state.m_detuneSemitonesValue + _state.m_detuneValue) / 12.0f);
                 _state.m_frequencyMultiplier = _5;
                 _6 = _io.m__in_frequencyIn;
                 _state.m_normalisedPhaseIncrement = (_6 * (1.0f / static_cast<float> ((sampleRate * 1.0)))) * _state.m_frequencyMultiplier;
        }
        _loop_1: { if (! (_state.m_normalisedPhase >= 1.0f)) goto _break_1; }
        _body_1: { _state.m_normalisedPhase = _state.m_normalisedPhase - 1.0f;
                   goto _loop_1;
        }
        _break_1: { normalisedPhase2 = _state.m_normalisedPhase + 0.5f;
                    if (! (normalisedPhase2 > 1.0f)) goto _ifnot_3;
        }
        _if_3: { normalisedPhase2 = normalisedPhase2 - 1.0f; }
        _ifnot_3: { _7 = PolyBlep_14__blep (_state, _state.m_normalisedPhase);
                    sawValue = static_cast<float> (((2.0f * _state.m_normalisedPhase) - 1.0f) - _7);
                    _8 = PolyBlep_14__blep (_state, _state.m_normalisedPhase);
                    _9 = PolyBlep_14__blep (_state, normalisedPhase2);
                    squareValue = (-_8) + _9;
                    if (! (_state.m_normalisedPhase <= 0.5f)) goto _ternary_false_4;
        }
        _ternary_true_4: { _10 = -1.0f;
                           _T4 = _10;
                           goto _ternary_end_4;
        }
        _ternary_false_4: { _11 = 1.0f;
                            _T4 = _11;
        }
        _ternary_end_4: { _12 = _T4;
                          squareValue = squareValue + _12;
                          left = static_cast<float> (0.5 + static_cast<double> (_state.m_detuneValue));
                          right = static_cast<float> (0.5 - static_cast<double> (_state.m_detuneValue));
                          _13 = _io.m__in_gainIn;
                          amp = static_cast<float> (_13 * ((squareValue * _state.m_waveshapeValue) + static_cast<float> (sawValue * static_cast<float> (1.0f - _state.m_waveshapeValue))));
                          _2 = ZeroInitialiser();
                          _2[0] = static_cast<float> (amp * static_cast<float> (left));
                          _2[1] = static_cast<float> (amp * static_cast<float> (right));
                          out_value_audioOut = out_value_audioOut + _2;
                          _state.m_normalisedPhase = _state.m_normalisedPhase + _state.m_normalisedPhaseIncrement;
        }
        _ifnot_2: { out_value_audioOut = out_value_audioOut + ZeroInitialiser();
                    _state.m__resumePoint = 1;
                    _io.m__out_audioOut = out_value_audioOut;
        }
    }

    void PolyBlep_14___initialise (PolyBlep_14___State& _state) noexcept
    {
        _state.m_normalisedPhase = 0;
        _state.m_normalisedPhaseIncrement = 0;
        _state.m_waveshapeValue = 0;
        _state.m_detuneSemitonesValue = 0;
        _state.m_detuneValue = 0;
        _state.m_frequencyMultiplier = 1.0f;
    }

    //==============================================================================
    float PolyBlep_15__blep (PolyBlep_15___State& _state, float t2) noexcept
    {
        float t = {};

        t = t2;
        if (! (t < _state.m_normalisedPhaseIncrement)) goto _ifnot_0;
        _if_0: { t = t / _state.m_normalisedPhaseIncrement;
                 return ((t + t) - (t * t)) - 1.0f;
        }
        _ifnot_0: { if (! (t > (1.0f - _state.m_normalisedPhaseIncrement))) goto _ifnot_1; }
        _if_1: { t = (t - 1.0f) / _state.m_normalisedPhaseIncrement;
                 return ((t * t) + (t + t)) + 1.0f;
        }
        _ifnot_1: { return 0; }
    }

    void PolyBlep_15__run (PolyBlep_15___State& _state, PolyBlep_15___IO& _io) noexcept
    {
        Vector<float, 2> out_value_audioOut = {}, _2 = {};
        float _3 = {}, _4 = {}, _5 = {}, _6 = {}, normalisedPhase2 = {}, _7 = {}, _8 = {}, _9 = {}, squareValue = {}, _10 = {}, _11 = {}, _12 = {}, left = {}, right = {}, _13 = {}, _T4 = {};
        float sawValue = {}, amp = {};

        out_value_audioOut = ZeroInitialiser();
        _3 = _io.m__in_activeIn;
        if (! (_3 == 1.0f)) goto _ifnot_2;
        _if_2: { _4 = _io.m__in_detuneIn;
                 _state.m_detuneValue = _4 * 0.01f;
                 _5 = std::pow (2.0f, (_state.m_detuneSemitonesValue + _state.m_detuneValue) / 12.0f);
                 _state.m_frequencyMultiplier = _5;
                 _6 = _io.m__in_frequencyIn;
                 _state.m_normalisedPhaseIncrement = (_6 * (1.0f / static_cast<float> ((sampleRate * 1.0)))) * _state.m_frequencyMultiplier;
        }
        _loop_1: { if (! (_state.m_normalisedPhase >= 1.0f)) goto _break_1; }
        _body_1: { _state.m_normalisedPhase = _state.m_normalisedPhase - 1.0f;
                   goto _loop_1;
        }
        _break_1: { normalisedPhase2 = _state.m_normalisedPhase + 0.5f;
                    if (! (normalisedPhase2 > 1.0f)) goto _ifnot_3;
        }
        _if_3: { normalisedPhase2 = normalisedPhase2 - 1.0f; }
        _ifnot_3: { _7 = PolyBlep_15__blep (_state, _state.m_normalisedPhase);
                    sawValue = static_cast<float> (((2.0f * _state.m_normalisedPhase) - 1.0f) - _7);
                    _8 = PolyBlep_15__blep (_state, _state.m_normalisedPhase);
                    _9 = PolyBlep_15__blep (_state, normalisedPhase2);
                    squareValue = (-_8) + _9;
                    if (! (_state.m_normalisedPhase <= 0.5f)) goto _ternary_false_4;
        }
        _ternary_true_4: { _10 = -1.0f;
                           _T4 = _10;
                           goto _ternary_end_4;
        }
        _ternary_false_4: { _11 = 1.0f;
                            _T4 = _11;
        }
        _ternary_end_4: { _12 = _T4;
                          squareValue = squareValue + _12;
                          left = static_cast<float> (0.5 + static_cast<double> (_state.m_detuneValue));
                          right = static_cast<float> (0.5 - static_cast<double> (_state.m_detuneValue));
                          _13 = _io.m__in_gainIn;
                          amp = static_cast<float> (_13 * ((squareValue * _state.m_waveshapeValue) + static_cast<float> (sawValue * static_cast<float> (1.0f - _state.m_waveshapeValue))));
                          _2 = ZeroInitialiser();
                          _2[0] = static_cast<float> (amp * static_cast<float> (left));
                          _2[1] = static_cast<float> (amp * static_cast<float> (right));
                          out_value_audioOut = out_value_audioOut + _2;
                          _state.m_normalisedPhase = _state.m_normalisedPhase + _state.m_normalisedPhaseIncrement;
        }
        _ifnot_2: { out_value_audioOut = out_value_audioOut + ZeroInitialiser();
                    _state.m__resumePoint = 1;
                    _io.m__out_audioOut = out_value_audioOut;
        }
    }

    void PolyBlep_15___initialise (PolyBlep_15___State& _state) noexcept
    {
        _state.m_normalisedPhase = 0;
        _state.m_normalisedPhaseIncrement = 0;
        _state.m_waveshapeValue = 0;
        _state.m_detuneSemitonesValue = 0;
        _state.m_detuneValue = 0;
        _state.m_frequencyMultiplier = 1.0f;
    }

    //==============================================================================
    float PolyBlep_16__blep (PolyBlep_16___State& _state, float t2) noexcept
    {
        float t = {};

        t = t2;
        if (! (t < _state.m_normalisedPhaseIncrement)) goto _ifnot_0;
        _if_0: { t = t / _state.m_normalisedPhaseIncrement;
                 return ((t + t) - (t * t)) - 1.0f;
        }
        _ifnot_0: { if (! (t > (1.0f - _state.m_normalisedPhaseIncrement))) goto _ifnot_1; }
        _if_1: { t = (t - 1.0f) / _state.m_normalisedPhaseIncrement;
                 return ((t * t) + (t + t)) + 1.0f;
        }
        _ifnot_1: { return 0; }
    }

    void PolyBlep_16__run (PolyBlep_16___State& _state, PolyBlep_16___IO& _io) noexcept
    {
        Vector<float, 2> out_value_audioOut = {}, _2 = {};
        float _3 = {}, _4 = {}, _5 = {}, _6 = {}, normalisedPhase2 = {}, _7 = {}, _8 = {}, _9 = {}, squareValue = {}, _10 = {}, _11 = {}, _12 = {}, left = {}, right = {}, _13 = {}, _T4 = {};
        float sawValue = {}, amp = {};

        out_value_audioOut = ZeroInitialiser();
        _3 = _io.m__in_activeIn;
        if (! (_3 == 1.0f)) goto _ifnot_2;
        _if_2: { _4 = _io.m__in_detuneIn;
                 _state.m_detuneValue = _4 * 0.01f;
                 _5 = std::pow (2.0f, (_state.m_detuneSemitonesValue + _state.m_detuneValue) / 12.0f);
                 _state.m_frequencyMultiplier = _5;
                 _6 = _io.m__in_frequencyIn;
                 _state.m_normalisedPhaseIncrement = (_6 * (1.0f / static_cast<float> ((sampleRate * 1.0)))) * _state.m_frequencyMultiplier;
        }
        _loop_1: { if (! (_state.m_normalisedPhase >= 1.0f)) goto _break_1; }
        _body_1: { _state.m_normalisedPhase = _state.m_normalisedPhase - 1.0f;
                   goto _loop_1;
        }
        _break_1: { normalisedPhase2 = _state.m_normalisedPhase + 0.5f;
                    if (! (normalisedPhase2 > 1.0f)) goto _ifnot_3;
        }
        _if_3: { normalisedPhase2 = normalisedPhase2 - 1.0f; }
        _ifnot_3: { _7 = PolyBlep_16__blep (_state, _state.m_normalisedPhase);
                    sawValue = static_cast<float> (((2.0f * _state.m_normalisedPhase) - 1.0f) - _7);
                    _8 = PolyBlep_16__blep (_state, _state.m_normalisedPhase);
                    _9 = PolyBlep_16__blep (_state, normalisedPhase2);
                    squareValue = (-_8) + _9;
                    if (! (_state.m_normalisedPhase <= 0.5f)) goto _ternary_false_4;
        }
        _ternary_true_4: { _10 = -1.0f;
                           _T4 = _10;
                           goto _ternary_end_4;
        }
        _ternary_false_4: { _11 = 1.0f;
                            _T4 = _11;
        }
        _ternary_end_4: { _12 = _T4;
                          squareValue = squareValue + _12;
                          left = static_cast<float> (0.5 + static_cast<double> (_state.m_detuneValue));
                          right = static_cast<float> (0.5 - static_cast<double> (_state.m_detuneValue));
                          _13 = _io.m__in_gainIn;
                          amp = static_cast<float> (_13 * ((squareValue * _state.m_waveshapeValue) + static_cast<float> (sawValue * static_cast<float> (1.0f - _state.m_waveshapeValue))));
                          _2 = ZeroInitialiser();
                          _2[0] = static_cast<float> (amp * static_cast<float> (left));
                          _2[1] = static_cast<float> (amp * static_cast<float> (right));
                          out_value_audioOut = out_value_audioOut + _2;
                          _state.m_normalisedPhase = _state.m_normalisedPhase + _state.m_normalisedPhaseIncrement;
        }
        _ifnot_2: { out_value_audioOut = out_value_audioOut + ZeroInitialiser();
                    _state.m__resumePoint = 1;
                    _io.m__out_audioOut = out_value_audioOut;
        }
    }

    void PolyBlep_16___initialise (PolyBlep_16___State& _state) noexcept
    {
        _state.m_normalisedPhase = 0;
        _state.m_normalisedPhaseIncrement = 0;
        _state.m_waveshapeValue = 0;
        _state.m_detuneSemitonesValue = 0;
        _state.m_detuneValue = 0;
        _state.m_frequencyMultiplier = 1.0f;
    }

    //==============================================================================
    int32_t _internal___minInt32 (int32_t a, int32_t b) noexcept
    {
        if (! (a < b)) goto _moreThan;
        _lessThan: { return a; }
        _moreThan: { return b; }
    }

    int32_t _internal___wrapInt32 (int32_t n, int32_t range) noexcept
    {
        int32_t _2 = {};

        if (! (n == 0)) goto _notEquals;
        _equals: { return 0; }
        _notEquals: { _2 = n % range;
                      if (! (_2 < 0)) goto _moduloPositive;
        }
        _moduloNegative: { return _2 + range; }
        _moduloPositive: { return _2; }
    }


    #if __clang__
     #pragma clang diagnostic pop
    #elif defined(__GNUC__)
     #pragma GCC diagnostic pop
    #elif defined(_MSC_VER)
     #pragma warning (pop)
    #endif

    //==============================================================================
    // The program contains no string literals, so this function should never be called
    static constexpr const char* lookupStringLiteral (int32_t)  { return {}; }

    //==============================================================================
    _State state = {}, initialisedState;

    double sampleRate = 1.0;
    uint32_t framesToAdvance = 0;
    uint64_t totalFramesElapsed = 0;
};

