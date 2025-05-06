#include "pch.h"

#include <winrt/Microsoft.Windows.Management.Deployment.h>
#include <winrt/Microsoft.Windows.AI.h>
#include <winrt/Microsoft.Windows.AI.Search.Experimental.ApplicationContentIndex.h>

using namespace winrt;
using namespace Windows::Foundation;

int main()
{
    init_apartment();
    auto x = winrt::Microsoft::Windows::AI::Search::Experimental::ApplicationContentIndex::ApplicationContentIndexer::GetReadyState();
    x;
}
