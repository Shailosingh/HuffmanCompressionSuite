#pragma once

#include "Decompressor.g.h"

namespace winrt::HuffmanCompressionApp::implementation
{
    struct Decompressor : DecompressorT<Decompressor>
    {
        //Datafields
        HWND WindowHandle;

        Decompressor();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        //Thread functions
        void DecompressionThreadManager(std::wstring inputFilePath, std::wstring outputFolderPath);

        //Dispatcher functions
        void DispatchToStatusBox(const wchar_t* statusText);
        void DispatchToProgressBar(uint64_t bitCounter, uint64_t totalBits);

        //Events
        void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);
        void FileSelectButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void FolderSelectButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void StartButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::HuffmanCompressionApp::factory_implementation
{
    struct Decompressor : DecompressorT<Decompressor, implementation::Decompressor>
    {
    };
}
