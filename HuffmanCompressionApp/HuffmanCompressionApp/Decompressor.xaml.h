#pragma once

#include "Decompressor.g.h"

namespace winrt::HuffmanCompressionApp::implementation
{
    struct Decompressor : DecompressorT<Decompressor>
    {
        Decompressor();

        int32_t MyProperty();
        void MyProperty(int32_t value);
    };
}

namespace winrt::HuffmanCompressionApp::factory_implementation
{
    struct Decompressor : DecompressorT<Decompressor, implementation::Decompressor>
    {
    };
}
