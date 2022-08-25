#include "pch.h"
#include "Decompressor.xaml.h"
#if __has_include("Decompressor.g.cpp")
#include "Decompressor.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::HuffmanCompressionApp::implementation
{
    Decompressor::Decompressor()
    {
        InitializeComponent();
    }

    int32_t Decompressor::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void Decompressor::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }
}