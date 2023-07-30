#include "ECFMP/flowmeasure/CanonicalFlowMeasureInfo.h"

namespace ECFMPTest::FlowMeasure {
    class CanonicalFlowMeasureInfoTest : public ::testing::Test
    {
    };

    TEST_F(CanonicalFlowMeasureInfoTest, ItHasACanonicalIdentifierOriginalRevision)
    {
        const auto identifier = "EGTT02A";
        const auto canonicalFlowMeasureInfo = ECFMP::FlowMeasure::CanonicalFlowMeasureInfo(identifier);
        ASSERT_EQ("EGTT02A", canonicalFlowMeasureInfo.Identifier());
    }

    TEST_F(CanonicalFlowMeasureInfoTest, ItHasACanonicalRevisionOriginalRevision)
    {
        const auto identifier = "EGTT02A";
        const auto canonicalFlowMeasureInfo = ECFMP::FlowMeasure::CanonicalFlowMeasureInfo(identifier);
        ASSERT_EQ(0, canonicalFlowMeasureInfo.Revision());
    }

    TEST_F(CanonicalFlowMeasureInfoTest, ItHasACanonicalIdentifierFirstRevision)
    {
        const auto identifier = "EGTT02A-1";
        const auto canonicalFlowMeasureInfo = ECFMP::FlowMeasure::CanonicalFlowMeasureInfo(identifier);
        ASSERT_EQ("EGTT02A", canonicalFlowMeasureInfo.Identifier());
    }

    TEST_F(CanonicalFlowMeasureInfoTest, ItHasACanonicalRevisionFirstRevision)
    {
        const auto identifier = "EGTT02A-1";
        const auto canonicalFlowMeasureInfo = ECFMP::FlowMeasure::CanonicalFlowMeasureInfo(identifier);
        ASSERT_EQ(1, canonicalFlowMeasureInfo.Revision());
    }

    TEST_F(CanonicalFlowMeasureInfoTest, ItHasACanonicalIdentifierThirteenthRevision)
    {
        const auto identifier = "EGTT02A-13";
        const auto canonicalFlowMeasureInfo = ECFMP::FlowMeasure::CanonicalFlowMeasureInfo(identifier);
        ASSERT_EQ("EGTT02A", canonicalFlowMeasureInfo.Identifier());
    }

    TEST_F(CanonicalFlowMeasureInfoTest, ItHasACanonicalRevisionThirteenthRevision)
    {
        const auto identifier = "EGTT02A-13";
        const auto canonicalFlowMeasureInfo = ECFMP::FlowMeasure::CanonicalFlowMeasureInfo(identifier);
        ASSERT_EQ(13, canonicalFlowMeasureInfo.Revision());
    }

    TEST_F(CanonicalFlowMeasureInfoTest, ItHasACanonicalIdentifierBadRevision)
    {
        const auto identifier = "EGTT02A-AB";
        const auto canonicalFlowMeasureInfo = ECFMP::FlowMeasure::CanonicalFlowMeasureInfo(identifier);
        ASSERT_EQ("EGTT02A", canonicalFlowMeasureInfo.Identifier());
    }

    TEST_F(CanonicalFlowMeasureInfoTest, ItHasACanonicalRevisionBadRevision)
    {
        const auto identifier = "EGTT02A-AB";
        const auto canonicalFlowMeasureInfo = ECFMP::FlowMeasure::CanonicalFlowMeasureInfo(identifier);
        ASSERT_EQ(0, canonicalFlowMeasureInfo.Revision());
    }

    TEST_F(CanonicalFlowMeasureInfoTest, ItIsCanonicallyAfterAnotherMeasureWithSameIdentifierAndLowerRevision)
    {
        const auto identifier = "EGTT02A-1";
        const auto canonicalFlowMeasureInfo = ECFMP::FlowMeasure::CanonicalFlowMeasureInfo(identifier);
        const auto otherIdentifier = "EGTT02A-0";
        const auto otherCanonicalFlowMeasureInfo = ECFMP::FlowMeasure::CanonicalFlowMeasureInfo(otherIdentifier);
        EXPECT_TRUE(canonicalFlowMeasureInfo.IsAfter(otherCanonicalFlowMeasureInfo));
    }

    TEST_F(CanonicalFlowMeasureInfoTest, ItIsNotCanonicallyAfterAnotherMeasureWithSameIdentifierAndHigherRevision)
    {
        const auto identifier = "EGTT02A-1";
        const auto canonicalFlowMeasureInfo = ECFMP::FlowMeasure::CanonicalFlowMeasureInfo(identifier);
        const auto otherIdentifier = "EGTT02A-2";
        const auto otherCanonicalFlowMeasureInfo = ECFMP::FlowMeasure::CanonicalFlowMeasureInfo(otherIdentifier);
        EXPECT_FALSE(canonicalFlowMeasureInfo.IsAfter(otherCanonicalFlowMeasureInfo));
    }

    TEST_F(CanonicalFlowMeasureInfoTest, ItIsNotCanonicallyAfterAnotherMeasureWithDifferentIdentifierAndLowerRevision)
    {
        const auto identifier = "EGTT02A-1";
        const auto canonicalFlowMeasureInfo = ECFMP::FlowMeasure::CanonicalFlowMeasureInfo(identifier);
        const auto otherIdentifier = "EGTT02B-0";
        const auto otherCanonicalFlowMeasureInfo = ECFMP::FlowMeasure::CanonicalFlowMeasureInfo(otherIdentifier);
        EXPECT_FALSE(canonicalFlowMeasureInfo.IsAfter(otherCanonicalFlowMeasureInfo));
    }
}// namespace ECFMPTest::FlowMeasure
