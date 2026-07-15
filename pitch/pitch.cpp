#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <ostream>
#include <portaudio.h>

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 512

static void checkError(PaError error)
{
        if (error != paNoError)
        {
                std::cout << "Portaudio error: " << Pa_GetErrorText(error) << "\n";
                exit(EXIT_FAILURE);
        }
}

static inline float max(float a, float b)
{
        return a > b ? a : b;
}

static int PaCallback(const void *inputBffer, void *outputBuffer, unsigned long frames_per_buffer,
                      const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

int main(int argc, char *argv[])
{
        PaError err;
        int numDevices;
        const PaDeviceInfo *deviceInfo;
        int device;
        PaStreamParameters inputParameters;
        PaStreamParameters outputParameters;
        PaStream *stream;

        // Initialize portaudio
        err = Pa_Initialize();
        checkError(err);

        // Get number of devices
        numDevices = Pa_GetDeviceCount();
        std::cout << "Number of devices: " << numDevices << "\n";
        if (numDevices < 0)
        {
                std::cout << "Error getting device count!\n"; // Because they can be less than zero
                exit(EXIT_FAILURE);
        }
        else if (numDevices == 0)
        {
                std::cout << "No available audio devices!\n";
                exit(EXIT_SUCCESS);
        }

        // Get device info
        // for (int i = 0; i < numDevices; i++)
        // {
        //         deviceInfo = Pa_GetDeviceInfo(i);
        //         std::cout << "Device: " << i << "\nName: " << deviceInfo->name
        //                   << "\nMax input channels: " << deviceInfo->maxInputChannels
        //                   << "\nMax output channels: " << deviceInfo->maxOutputChannels
        //                   << "\nDefault sample rate: " << deviceInfo->defaultSampleRate << "\n\n";
        // }
        device = 8;

        deviceInfo = Pa_GetDeviceInfo(device);
        std::cout << "Device: " << device << "\nName: " << deviceInfo->name
                  << "\nMax input channels: " << deviceInfo->maxInputChannels
                  << "\nMax output channels: " << deviceInfo->maxOutputChannels
                  << "\nDefault sample rate: " << deviceInfo->defaultSampleRate << "\n\n";

        memset(&inputParameters, 0, sizeof(inputParameters));
        inputParameters.channelCount = 2; // Working with the left and right input
        inputParameters.device = device;  // Setting the device to pulse... because the guy in the video did it
        inputParameters.hostApiSpecificStreamInfo = NULL;
        inputParameters.sampleFormat = paFloat32;
        inputParameters.suggestedLatency = Pa_GetDeviceInfo(device)->defaultLowInputLatency;

        memset(&outputParameters, 0, sizeof(outputParameters));
        outputParameters.channelCount = 2; // Working with the left and right input
        outputParameters.device = device;  // Setting the device to pulse... because the guy in the video did it
        outputParameters.hostApiSpecificStreamInfo = NULL;
        outputParameters.sampleFormat = paFloat32;
        outputParameters.suggestedLatency = Pa_GetDeviceInfo(device)->defaultLowInputLatency;

        err = Pa_OpenStream(&stream, &inputParameters, &outputParameters, SAMPLE_RATE, FRAMES_PER_BUFFER, paNoFlag,
                            PaCallback, NULL);
        checkError(err);

        // Capture audio
        err = Pa_StartStream(stream);
        checkError(err);

        Pa_Sleep(10 * 1000); // 10 seconds

        // Stop capture
        err = Pa_StopStream(stream);
        checkError(err);

        err = Pa_CloseStream(stream);
        checkError(err);

        // De-init portaudio
        err = Pa_Terminate();
        checkError(err);

        std::cout << "Hello, portaudio!\n";
        return 0;
}

static int PaCallback(const void *inputBffer, void *outputBuffer, unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
        float *inBuffer = (float *)inputBffer;
        (void)outputBuffer;
        int displaySize = 100;
        std::cout << "\r";
        float leftVol, rightVol;

        leftVol = rightVol = 0;

        for (unsigned long i = 0; i < framesPerBuffer * 2; i += 2)
        {
                leftVol = max(leftVol, std::abs(inBuffer[i]));
                rightVol = max(rightVol, std::abs(inBuffer[i + 1]));
        }

        for (int i = 0; i < displaySize; i++)
        {
                float barProportion = i / (float)displaySize;
                if (barProportion <= leftVol && barProportion <= rightVol)
                {
                        std::cout << "█";
                }
                else if (barProportion <= leftVol)
                {
                        std::cout << "▀";
                }
                else if (barProportion <= rightVol)
                {
                        std::cout << "▄";
                }
                else
                {
                        std::cout << " ";
                }
        }
        std::cout << std::flush;

        return 0;
}
