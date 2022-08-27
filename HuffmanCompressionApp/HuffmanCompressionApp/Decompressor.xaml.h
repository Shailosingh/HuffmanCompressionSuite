#pragma once

#include "Decompressor.g.h"

namespace winrt::HuffmanCompressionApp::implementation
{
    struct Decompressor : DecompressorT<Decompressor>
    {
        Decompressor();

        int32_t MyProperty();
        void MyProperty(int32_t value);
        void FileSelectButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void FolderSelectButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::HuffmanCompressionApp::factory_implementation
{
    struct Decompressor : DecompressorT<Decompressor, implementation::Decompressor>
    {
    };
}
