#include "pch.h"
#include "Decompressor.xaml.h"
#if __has_include("Decompressor.g.cpp")
#include "Decompressor.g.cpp"
#include "HuffmanLibrary/GeneralConstants.h"
#endif
#include "HuffmanLibrary/HuffmanDecompressor.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::HuffmanCompressionApp::implementation
{
    Decompressor::Decompressor()
    {
        //Initialize datafield (it should be truly filled in OnNavigatedTo)
        WindowHandle = NULL;

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

    //Event handlers-------------------------------------------------------------------------------------------------------------------------------------------------------------------
    void Decompressor::OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e)
    {
        //Retrieves the MainWindow's handle
        Window window = e.Parameter().as<Window>();
        auto windowNative{ window.try_as<::IWindowNative>() };
        winrt::check_bool(windowNative);
        WindowHandle = { 0 };
        windowNative->get_WindowHandle(&WindowHandle);
    }
    
    void Decompressor::FileSelectButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        //Setup file dialog
        IFileOpenDialog* fileOpener = nullptr;
        HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog,           // Class ID for the FileOpenFialog
                                    NULL,                          //  Making a non-aggregate object
                                  CLSCTX_INPROC_SERVER,         //   Context where code is run (A DLL that runs in this process)
                                        IID_PPV_ARGS(&fileOpener));  //    Interface pointer for object
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to open the file dialog!");
            return;
        }

        //Ensure that only the extension of the compressed files are allowed
        std::wstring extensionString = L"*" + COMPRESSED_EXTENSION;
        COMDLG_FILTERSPEC compressedExtension = {L"Compressed Files", extensionString.c_str()};
        hr = fileOpener->SetFileTypes(1, &compressedExtension);
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to restrict extensions of file dialog!");
            fileOpener->Release();
            return;
        }

        //Setup the title of file dialog
        hr = fileOpener->SetTitle(L"Choose a file to compress!");
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to set title of file dialog!");
            fileOpener->Release();
            return;
        }

        //Show the dialog and wait for the selected file
        //To ensure the dialog is modal, give it the HWND (https://stackoverflow.com/questions/42596788/how-can-i-open-an-modal-file-dialog-with-ifileopendialog)
        hr = fileOpener->Show(WindowHandle);
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to show the file dialog!");
            fileOpener->Release();
            return;
        }

        //Get the IShellItem (selected file from the dialog)
        IShellItem* selectedItem;
        hr = fileOpener->GetResult(&selectedItem);
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to select file!");
            fileOpener->Release();
            return;
        }

        //Get the filepath out of the selected item
        wchar_t* filePathPointer = nullptr;
        hr = selectedItem->GetDisplayName(SIGDN_FILESYSPATH, &filePathPointer);
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to extract path from selected path!");
            fileOpener->Release();
            selectedItem->Release();
            return;
        }

        //Set the filePathPointer to the FileDirectoryBox
        FileDirectoryBox().Text(filePathPointer);

        //Release the selected IShellItem and its DisplayName (filePathPointer)
        selectedItem->Release();
        CoTaskMemFree(filePathPointer);

        //Release the IFileDialog (fileOpener)
        fileOpener->Release();

        //Success message
        StatusBox().Text(L"Waiting to compress...");
    }


    void Decompressor::FolderSelectButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        //Setup file dialog
        IFileOpenDialog* folderOpener = nullptr;
        HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog,           // Class ID for the FileOpenFialog
                                    NULL,                          //  Making a non-aggregate object
                                  CLSCTX_INPROC_SERVER,         //   Context where code is run (A DLL that runs in this process)
                                        IID_PPV_ARGS(&folderOpener));//    Interface pointer for object
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to open the file dialog!");
            return;
        }

        //Get the current options of the file dialog
        DWORD optionFlags;
        hr = folderOpener->GetOptions(&optionFlags);
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to retrieve options for file dialog!");
            folderOpener->Release();
            return;
        }

        //Force the dialog to only allow selection of folders
        hr = folderOpener->SetOptions(optionFlags | FOS_PICKFOLDERS);
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to set options for file dialog!");
            folderOpener->Release();
            return;
        }

        //Setup the title of file dialog
        hr = folderOpener->SetTitle(L"Choose a folder for decompressed file to be placed in");
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to set title of file dialog!");
            folderOpener->Release();
            return;
        }

        //Show the dialog and wait for the selected file
        //To ensure the dialog is modal, give it the HWND (https://stackoverflow.com/questions/42596788/how-can-i-open-an-modal-file-dialog-with-ifileopendialog)
        hr = folderOpener->Show(WindowHandle);
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to show the file dialog!");
            folderOpener->Release();
            return;
        }

        //Get the IShellItem (selected file from the dialog)
        IShellItem* selectedItem;
        hr = folderOpener->GetResult(&selectedItem);
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to select folder!");
            folderOpener->Release();
            return;
        }

        //Get the folder path out of the selected item
        wchar_t* folderPathPointer = nullptr;
        hr = selectedItem->GetDisplayName(SIGDN_FILESYSPATH, &folderPathPointer);
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to extract path from selected path!");
            folderOpener->Release();
            selectedItem->Release();
            return;
        }

        //Set the folderPathPointer to the FolderDirectoryBox
        FolderDirectoryBox().Text(folderPathPointer);

        //Release the selected IShellItem and its DisplayName (filePathPointer)
        selectedItem->Release();
        CoTaskMemFree(folderPathPointer);

        //Release the IFileDialog (folderOpener)
        folderOpener->Release();

        //Success message
        StatusBox().Text(L"Waiting to compress...");
    }

    void Decompressor::StartButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        //Disable the window
        EnableWindow(WindowHandle, false);

        //Initialize variables
        HuffmanDecompressor decompressorObject;

        //Convert the file and folder directory boxes to strings
        std::wstring inputFilePath(FileDirectoryBox().Text());
        std::wstring outputFolderPath(FolderDirectoryBox().Text());

        //Create the thread that manages compression (not static function, so I must offer pointer to function and object)
        std::thread threadManager(&Decompressor::DecompressionThreadManager, this, inputFilePath, outputFolderPath);
        threadManager.detach();
    }

    //Thread Functions-----------------------------------------------------------------------------------------------------------------------------------------------------------------
    void Decompressor::DecompressionThreadManager(std::wstring inputFilePath, std::wstring outputFolderPath)
    {
        HuffmanDecompressor decompressorObject;

        //Reset the progress bar
        DispatchToProgressBar(0, 1);

        //Since BeginDecompression() is not a static function, I must offer a pointer to the function and its object as the first two arguments
        std::thread decompressionThread(&HuffmanDecompressor::BeginDecompression, &decompressorObject, inputFilePath, outputFolderPath);

        DispatchToStatusBox(L"Paths being validated...");
        while (!decompressorObject.IsFinished && !decompressorObject.FileAndDirectoryValidated);
        if (decompressorObject.ExitError)
        {
            decompressionThread.join();
            DispatchToStatusBox(decompressorObject.StatusMessage.c_str());
            EnableWindow(WindowHandle, true);
            return;
        }

        DispatchToStatusBox(L"Getting the file extension...");
        while (!decompressorObject.IsFinished && !decompressorObject.RecordedFileExtension);
        if (decompressorObject.ExitError)
        {
            decompressionThread.join();
            DispatchToStatusBox(decompressorObject.StatusMessage.c_str());
            EnableWindow(WindowHandle, true);
            return;
        }

        DispatchToStatusBox(L"Generating the character table...");
        while (!decompressorObject.IsFinished && !decompressorObject.CharacterTableFinished);
        if (decompressorObject.ExitError)
        {
            decompressionThread.join();
            DispatchToStatusBox(decompressorObject.StatusMessage.c_str());
            EnableWindow(WindowHandle, true);
            return;
        }

        DispatchToStatusBox(L"Constructing the huffman tree...");
        while (!decompressorObject.IsFinished && !decompressorObject.TreeConstructed);
        if (decompressorObject.ExitError)
        {
            decompressionThread.join();
            DispatchToStatusBox(decompressorObject.StatusMessage.c_str());
            EnableWindow(WindowHandle, true);
            return;
        }

        DispatchToStatusBox(L"Building decompressed file...");
        while (!decompressorObject.IsFinished)
        {
            DispatchToProgressBar(decompressorObject.BitCounter, decompressorObject.TotalBitCount);
        }

        if (decompressorObject.ExitError)
        {
            decompressionThread.join();
            DispatchToStatusBox(decompressorObject.StatusMessage.c_str());
            EnableWindow(WindowHandle, true);
            return;
        }

        decompressionThread.join();
        DispatchToStatusBox(L"Decompression successful");
        EnableWindow(WindowHandle, true);
    }

    //Dispatcher functions-------------------------------------------------------------------------------------------------------------------------------------------------------------
    void Decompressor::DispatchToStatusBox(const wchar_t* statusText)
    {
        if (this->DispatcherQueue().HasThreadAccess())
        {
            StatusBox().Text(statusText);
        }
        else
        {
            bool isQueued = this->DispatcherQueue().TryEnqueue(Microsoft::UI::Dispatching::DispatcherQueuePriority::Normal, [statusText, this]()
                {
                    StatusBox().Text(statusText);
                });
        }
    }

    void Decompressor::DispatchToProgressBar(uint64_t bitCounter, uint64_t totalBits)
    {
        //Calculate the percentage
        double percentProgress = (bitCounter * 100) / totalBits;

        if (this->DispatcherQueue().HasThreadAccess())
        {
            ProgressBar().Value(percentProgress);
        }
        else
        {
            bool isQueued = this->DispatcherQueue().TryEnqueue(Microsoft::UI::Dispatching::DispatcherQueuePriority::Normal, [percentProgress, this]()
                {
                    ProgressBar().Value(percentProgress);
                });
        }
    }
}