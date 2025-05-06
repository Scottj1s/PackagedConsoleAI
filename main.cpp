#include "pch.h"

#include <winrt/Microsoft.Windows.Management.Deployment.h>
#include <winrt/Microsoft.Windows.AI.h>
#include <winrt/Microsoft.Windows.AI.Search.Experimental.ApplicationContentIndex.h>
#include <iostream>

using namespace winrt;
using namespace Windows::Foundation;

using namespace Microsoft::Windows::AI::Search::Experimental::ApplicationContentIndex;

void Log(hstring const& message)
{
    std::wcout << message.c_str() << std::endl;
}

IAsyncAction CallContentIndexer()
{
    Log(L"Calling ApplicationContentIndexer.GetReadyState");
    auto readyState = ApplicationContentIndexer::GetReadyState();
    if (readyState == Microsoft::Windows::AI::AIFeatureReadyState::NotReady)
    {
        Log(L"Calling ApplicationContentIndexer.EnsureReadyAsync");
        co_await ApplicationContentIndexer::EnsureReadyAsync();
    }

    Log(L"Creating AppContentIndexer");
    auto indexer = co_await ApplicationContentIndexer::GetOrCreateIndexerAsync(L"index");

    std::map<hstring, hstring> data = {
        {L"item1", L"Dogs are loyal and affectionate animals known for their companionship, intelligence, and diverse breeds."},
        {L"item2", L"Fish are aquatic creatures that breathe through gills and come in a vast variety of shapes, sizes, and colors."},
        {L"item3", L"Here is some information about Cats: Cats are cute and fluffy. Young cats are very playful."},
        {L"item4", L"Broccoli is a nutritious green vegetable rich in vitamins, fiber, and antioxidants."},
        {L"item5", L"Computers are powerful electronic devices that process information, perform calculations, and enable communication worldwide."},
        {L"item6", L"Music is a universal language that expresses emotions, tells stories, and connects people through rhythm and melody."},
    };

    Log(L"Adding data to index");
    for (auto const& [key, value] : data)
    {
        Log(L"Adding " + key);
        auto textContent = AppManagedIndexableAppContent::CreateFromString(key, value);
        co_await indexer.AddOrUpdateAsync(textContent);
    }

    hstring queryString = L"Facts about kittens.";
    Log(L"Running Query: '" + queryString + L"'");
    auto query = indexer.CreateQuery(queryString);
    auto matches = co_await query.GetNextTextMatchesAsync(5);

    Log(L"Got results:");
    for (auto const& match : matches)
    {
        if (auto textMatch = match.try_as<AppManagedTextQueryMatch>())
        {
            auto dataMatch = data[textMatch.ContentId()];
            Log(L"Match: " + textMatch.ContentId() + L" - '" + dataMatch + L"'");
        }
    }
}

int main()
{
    init_apartment();
    CallContentIndexer().get();
}
