#pragma once

#include "Compressor.g.h"

namespace winrt::HuffmanCompressionApp::implementation
{
    struct Compressor : CompressorT<Compressor>
    {
        Compressor();

        int32_t MyProperty();
        void MyProperty(int32_t value);
        void FileSelectButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void FolderSelectButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::HuffmanCompressionApp::factory_implementation
{
    struct Compressor : CompressorT<Compressor, implementation::Compressor>
    {
    };
}
