#pragma once

#include "Compressor.g.h"

namespace winrt::HuffmanCompressionApp::implementation
{
    struct Compressor : CompressorT<Compressor>
    {
        //Datafields
        HWND WindowHandle;

        Compressor();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        //Thread functions
        void CompressionThreadManager(std::wstring inputFilePath, std::wstring outputFolderPath);

        //Dispatcher functions
        void DispatchToStatusBox(const wchar_t* statusText);
        void DispatchToProgressBar(uint64_t bitCounter, uint64_t totalBits);

        //Event handlers
        void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);
        void FileSelectButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void FolderSelectButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void StartButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::HuffmanCompressionApp::factory_implementation
{
    struct Compressor : CompressorT<Compressor, implementation::Compressor>
    {
    };
}
