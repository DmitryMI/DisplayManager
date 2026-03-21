#include <gtest/gtest.h>

#include "DisplayManager/Factory.h"
#include "DisplayManager/NvApi/Configuration.h"
#include "DisplayManager/Serialization/BoostInputArchive.h"
#include "DisplayManager/Serialization/BoostOutputArchive.h"

namespace DisplayManager::NvApi
{
    class ConfigurationTest : public ::testing::Test
    {

    };

    TEST_F(ConfigurationTest, ConstructTest)
    {
        auto displayProvider = Factory::GetDisplayProvider();
        const auto& configOriginal = *dynamic_cast<const NvApi::Configuration*>(&displayProvider->GetActiveConfiguration());
        NvApi::Configuration configDeserialized;
        {
            auto outputArchive = Serialization::BoostOutputArchive("test_archive.txt");
            configOriginal.Serialize(outputArchive);
        }
        {
            auto inputArchive = Serialization::BoostInputArchive("test_archive.txt");
            configDeserialized.Deserialize(inputArchive);
        }
        bool eq = configOriginal == configDeserialized;
        EXPECT_TRUE(eq);
        EXPECT_EQ(configOriginal, configDeserialized);
    }

}